 /**
 * @author      : faraz
 * @file        : uart.c
 * @created     : Fri 17 Dec 2021 07:28:36 PM EST
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "semphr.h"

#include "rtos.h"
#include "picon/io.h"
#include "picon/dev.h"
#include "picon/ioctl.h"
#include "picon/utils.h"
#include "picon/log.h"

#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"


#define UART_QUEUE_SIZE		(80)
#define UART_MAX		(2)
#define MIN_WAIT_TIME		(40)

#define PICON_DEFAULT_BAUD_RATE 	115200
#define PICON_DEFAULT_DATA_BITS 	8
#define PICON_DEFAULT_PARITY    	UART_PARITY_NONE
#define PICON_DEFAULT_STOP_BITS 	1

#define PICON_DEFAULT_UART_TX_PIN	0
#define PICON_DEFAULT_UART_RX_PIN	1


typedef struct _uart_t {
	uart_inst_t*		uart;		// UART pointer 
	int8_t			rx_pin;		// rx gpio pin
	int8_t			tx_pin;		// rx gpio pin
} uart_t;

// UART receive queues
rtos_queue_handle_t uartq[UART_MAX];

// to handle control-c
rtos_semaphore_handle_t  uart_sems[UART_MAX];

static uart_t uarts[UART_MAX] = {
	{ .uart = uart0,
	  .rx_pin = PICON_DEFAULT_UART_RX_PIN, 
	  .tx_pin = PICON_DEFAULT_UART_TX_PIN,
	},
	{ .uart = uart1,
	  .rx_pin = -1,
	  .tx_pin = -1,
	},

};

uint8_t uart_flags[UART_MAX];

// ISR to receive data
static void picon_uart_common_rx_isr(uint8_t ux)
{
	const uart_t *uartp = uarts+ux;		// Access UART's buffer
	uart_inst_t  *uart = uarts[ux].uart;	// Lookup UART address
	uint8_t 	ch;			// Read data byte
	rtos_base_type_t	higher_priority_task_woken;
	uint16_t must_yield=0;
	int      uart_irq;

	if ( ux >=UART_MAX || !uartp || !(uartq[ux]) )
		return;					// Not open for ISR receiving!

	uart_irq = ( uart == uart0) ? UART0_IRQ : UART1_IRQ;

	irq_set_enabled(uart_irq, false); // in SMP mode ISRs can run concurrently

	while (uart_is_readable(uart)) {
		ch = (uint8_t) uart_get_hw(uart)->dr;	// Read data ... don't use uart_getc(uart)

		// This is a lame way to handle control-c, but we keep it for now until I think
		// of a better and more ellegant way
		if (uart_sems[ux] != NULL  && ch == CONTROL('C')) {
			rtos_semaphore_give_from_isr(uart_sems[ux], &higher_priority_task_woken);
		} else {
			// Save data if the buffer is not full
			rtos_queue_send_from_isr(uartq[ux], &ch, &higher_priority_task_woken);
		}

		if (higher_priority_task_woken == RTOS_TRUE) {
			must_yield = 1;
		}
	}
	irq_set_enabled(uart_irq, true);

	rtos_port_yield_from_isr(must_yield);
}

void picon_uart0_rx_isr(void)
{
	picon_uart_common_rx_isr(0);
}

void picon_uart1_rx_isr(void)
{
	picon_uart_common_rx_isr(1);
}

int picon_uart_init(uint8_t ux, void *params)
{
	const uart_t	*uartp = uarts+ux;		// Access UART's buffer
	uart_inst_t  	*uart = uarts[ux].uart;		// Lookup UART address
	uint32_t bps = 115200;

	if (params) {
		bps = (uint32_t) params;
	}

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;			// Invalid UART ref

	if (uartq[ux])
		return -EINVAL;			// already initialized

	uart_sems[ux] = NULL;

	if (uartp->rx_pin < 0 && uartp->tx_pin < 0)
		return -EINVAL;			// Invalid pins

	uart_init(uart, PICON_DEFAULT_BAUD_RATE);

	if (uartp->rx_pin >= 0)
		gpio_set_function(uartp->rx_pin, GPIO_FUNC_UART);

	if (uartp->tx_pin >= 0)
		gpio_set_function(uartp->tx_pin, GPIO_FUNC_UART);

	// Set our data format
	// 8N1
	uart_set_format(uart, PICON_DEFAULT_DATA_BITS, PICON_DEFAULT_STOP_BITS, PICON_DEFAULT_PARITY);

	// Set UART flow control CTS/RTS, we don't want these, so turn them off
	uart_set_hw_flow(uart, false, false);

	// Turn off FIFO's - we want to do this character by character
	uart_set_fifo_enabled(uart, false);

	if (uartp->rx_pin >= 0) {
		// Set up a RX interrupt
		// We need to set up the handler first
		// Select correct interrupt for the UART we are using
		int uart_irq;

		// And set up and enable the interrupt handlers
		if (uart == uart0) {
			uart_irq = UART0_IRQ;
			irq_set_exclusive_handler(uart_irq, picon_uart0_rx_isr);
		} else {
			uart_irq = UART1_IRQ;
			irq_set_exclusive_handler(uart_irq, picon_uart1_rx_isr);
		}
		irq_set_enabled(uart_irq, true);
	}

	return 0;
}

const void *picon_uart_open(const DEVICE_FILE *devf, int flags)
{
	unsigned char 	ux;
	const uart_t	*uartp;
	uart_inst_t     *uart;

	if (!devf) return NULL;

	ux = devf->minor;
	uartp = uarts+ux;		// Access UART's record 

	if ( ux >= UART_MAX || !uartp )
		return NULL;		// Invalid UART ref

	uart = uarts[ux].uart;		// Lookup UART address

	if (uartq[ux])
		return (const void *) devf;	// already initialized

	uartq[ux] = rtos_queue_create(UART_QUEUE_SIZE, sizeof(char));
	if (!uartq[ux]) return NULL;

	uart_flags[ux] = flags;

	if (uartp->rx_pin >= 0) {
		// Now enable the UART to send interrupts - RX only
		uart_set_irq_enables(uart, true, false);
	}

	return (const void *) devf;
}

int picon_uart_close(const DEVICE_FILE *devf)
{
	unsigned char 	ux;
	const uart_t	*uartp;
	uart_inst_t     *uart;

	if (!devf) return -EINVAL;

	ux = devf->minor;
	uartp = uarts+ux;		// Access UART's buffer
	uart = uarts[ux].uart;		// Lookup UART address

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;			// Invalid UART ref

	if (!uartq[ux])
		return -EINVAL;	// not opened?

	// stop receiving interrupts
	uart_set_irq_enables(uart, false ,false);

	rtos_queue_delete(uartq[ux]);
	uartq[ux] = NULL;
	uart_sems[ux] = NULL;
	uart_flags[ux] = 0;

	return 0;
}

int picon_uart_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data)
{
	unsigned char 	ux;
	const uart_t	*uartp;
	uart_inst_t     *uart;
	int rc=0;

	if (!devf) return -1;

	ux = devf->minor;
	uartp = uarts+ux;		// Access UART's buffer
	uart = uarts[ux].uart;		// Lookup UART address

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;		// Invalid UART ref

	if (!uartq[ux])
		return -EINVAL;	// not opened


	uart_set_irq_enables(uart, false, false);

        switch (request) {
		case PICON_IOC_UART_SET_BPS:
			uart_set_baudrate(uart, (unsigned int) data);
			break;

		case PICON_IOC_TTY_SET_INT:
			uart_sems[ux] = (rtos_semaphore_handle_t) data;
			break;

		case PICON_IOC_TTY_GET_INT:
			if (!data) rc = -EINVAL;
			*((rtos_semaphore_handle_t *) data) = uart_sems[ux];
			break;
		default:
			rc = -EINVAL;
			break;
	}

	uart_set_irq_enables(uart, true, false);

	return rc;
}

int picon_uart_write(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	unsigned char 	ux;
	const uart_t	*uartp;
	uart_inst_t     *uart;
	uint32_t	n;

	if (!devf) return -1;

	ux = devf->minor;
	uartp = uarts+ux;		// Access UART's buffer
	uart = uarts[ux].uart;		// Lookup UART address

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;		// Invalid UART ref

	for (n=0; n < count; n++ ) {
		while ( ! uart_is_writable(uart) )
			rtos_task_yield();	

		// No CRLF support
		uart_putc_raw(uart, buf[n]);
	}

	return n;
}


int picon_uart_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	unsigned char 	ux;
	const uart_t	*uartp;
	uint32_t	n;
	rtos_tick_type_t	wait_time = RTOS_PORT_MAX_DELAY;

	if (!devf) return -1;

	ux = devf->minor;
	uartp = uarts+ux;		// Access UART's buffer

	if (uart_flags[ux] & PICON_IO_NONBLOCK)
		wait_time = MIN_WAIT_TIME;

	if ( ux >=UART_MAX || !uartp || !(uartq[ux]) )
		return -1;			// Invalid UART ref or not opened

	for (n=0; n < count; n++ ) {
		if (rtos_queue_receive(uartq[ux], buf++, wait_time) != RTOS_PASS)
			break;
	}

	return n;
}
