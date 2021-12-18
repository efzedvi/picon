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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "rtos.h"
#include "picon/utils.h"
#include "picon/ioctl.h"
#include "picon/io.h"
#include "picon/log.h"

#include "uart.h"


#define UART_QUEUE_SIZE		(64)
#define UART_MAX		(3)
#define MIN_WAIT_TIME		(20)

static volatile unsigned char initialized = 0;  // True when UART configured

typedef struct _uart_t {
	uint32_t		usart;		// USART address
	uint32_t		rcc;		// RCC address
	uint8_t			irq;		// IRQ number
	uint32_t		rx_port;	// rx gpio PORT
	uint32_t		tx_port;	// rx gpio PORT
	uint16_t		rx_pin;		// rx gpio pin
	uint16_t		tx_pin;		// rx gpio pin
} uart_t;


// to handle control-c
rtos_semaphore_handle_t  uart_sems[UART_MAX];

static const uart_t uarts[UART_MAX] = {
	{ .usart = USART1, .rcc = RCC_USART1, .irq = NVIC_USART1_IRQ,
	  .rx_port = GPIO_BANK_USART1_RX, .rx_pin = GPIO_USART1_RX, 
	  .tx_port = GPIO_BANK_USART1_TX, .tx_pin = GPIO_USART1_TX,
	},
	{ .usart = USART2, .rcc = RCC_USART2, .irq = NVIC_USART2_IRQ,
	  .rx_port = GPIO_BANK_USART2_RX, .rx_pin = GPIO_USART2_RX, 
	  .tx_port = GPIO_BANK_USART2_TX, .tx_pin = GPIO_USART2_TX,
	},
	{ .usart = USART3, .rcc = RCC_USART3, .irq = NVIC_USART3_IRQ,
	  .rx_port = GPIO_BANK_USART3_RX, .rx_pin = GPIO_USART3_RX, 
	  .tx_port = GPIO_BANK_USART3_TX, .tx_pin = GPIO_USART3_TX,
	} 
};

// UART receive queues
rtos_queue_handle_t uartq[UART_MAX];

uint8_t uart_flags[UART_MAX];

// ISR to receive data
static void picon_uart_common_isr(unsigned int ux)
{
	const uart_t *uartp = uarts+ux;			// Access USART's buffer
	uint32_t uart = uarts[ux].usart;		// Lookup USART address
	uint16_t 	ch;				// Read data byte
	rtos_base_type_t	higher_priority_task_woken;
	uint16_t must_yield=0;

	if ( ux >=UART_MAX || !uartp || !(uartq[ux]) )
		return;					// Not open for ISR receiving!

	while ( USART_SR(uart) & USART_SR_RXNE ) {	// Read status
		ch = usart_recv(uart);			// Read data

		// This is a lame way to handle control-c, but we keep it for now until I think
		// of a better and more ellegant way
		if (uart_sems[ux] != NULL  && ch == CONTROL('C')) {
			RTOS_SEMAPHORE_GIVE_FROM_ISR(uart_sems[ux], &higher_priority_task_woken);
		} else {
			// Save data if the buffer is not full
			RTOS_QUEUE_SEND_FROM_ISR(uartq[ux], &ch, &higher_priority_task_woken);
		}

		if (higher_priority_task_woken == RTOS_TRUE) {
			must_yield = 1;
		}
	}

	RTOS_PORT_YIELD_FROM_ISR(must_yield);
}

void usart1_isr(void)
{
	picon_uart_common_isr(0);
}

void usart2_isr(void)
{
	picon_uart_common_isr(1);
}

void usart3_isr(void)
{
	picon_uart_common_isr(2);
}


int picon_uart_init(uint8_t ux, void *params)
{
	const uart_t *uartp = uarts+ux;		// Access USART's buffer
	uint32_t uart = uarts[ux].usart;	// Lookup USART address
	uint32_t bps = 115200;

	if (params) {
		bps = (uint32_t) params;
	}

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;			// Invalid UART ref

	if (uartq[ux])
		return -EINVAL;			// already initialized

	usart_disable_rx_interrupt(uart);

	// GPIO clocks must have allready been enabled in board initialization

	// setup the gpios
	gpio_set_mode(uartp->rx_port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, uartp->rx_pin);
	gpio_set(uartp->rx_port, uartp->rx_pin); // input pull up mode
	gpio_set_mode(uartp->tx_port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		      uartp->tx_pin);

	gpio_port_config_lock(uartp->rx_port, uartp->rx_pin);
	gpio_port_config_lock(uartp->tx_port, uartp->tx_pin);

	rcc_periph_clock_enable(uartp->rcc);
	usart_set_baudrate(uart, bps);
	usart_set_databits(uart, 8);
	usart_set_parity(uart, USART_PARITY_NONE);
	usart_set_stopbits(uart, USART_STOPBITS_1);
	usart_set_mode(uart, USART_MODE_TX_RX);
	usart_set_flow_control(uart, USART_FLOWCONTROL_NONE);

	nvic_disable_irq(uartp->irq);
	usart_disable(uart);

	uart_sems[ux] = NULL;

	return 0;
}

const void *picon_uart_open(const DEVICE_FILE *devf, int flags)
{
	unsigned char ux;
	const uart_t *uartp;
	uint32_t uart;

	if (!devf) return NULL;

	ux = devf->minor;
	uartp = uarts+ux;		// Access USART's record 

	if ( ux >= UART_MAX || !uartp )
		return NULL;		// Invalid UART ref

	uart = uarts[ux].usart;		// Lookup USART address

	if (uartq[ux])
		return (const void *) devf;	// already initialized

	usart_disable_rx_interrupt(uart);

	uartq[ux] = RTOS_QUEUE_CREATE(UART_QUEUE_SIZE, sizeof(char));
	if (!uartq[ux]) return NULL;

	// we corrently ignore the flags for uart devices
	usart_set_mode(uart, USART_MODE_TX_RX);

	nvic_enable_irq(uartp->irq);
	usart_enable(uart);
	usart_enable_rx_interrupt(uart);

	uart_flags[ux] = flags;

	return (const void *) devf;
}

int picon_uart_close(const DEVICE_FILE *devf)
{
	unsigned char ux;
	const uart_t *uartp;
	uint32_t uart;

	if (!devf) return -EINVAL;

	ux = devf->minor;
	uartp = uarts+ux;		// Access USART's buffer
	uart = uarts[ux].usart;		// Lookup USART address

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;			// Invalid UART ref

	if (!uartq[ux])
		return -EINVAL;	// not opened?

	usart_disable_rx_interrupt(uart);

	nvic_disable_irq(uartp->irq);
	usart_disable(uart);

	rtos_queue_delete(uartq[ux]);
	uartq[ux] = NULL;
	uart_sems[ux] = NULL;
	uart_flags[ux] = 0;

	return 0;
}

int picon_uart_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data)
{
	unsigned char ux;
	const uart_t *uartp;
	uint32_t uart;
	int rc=0;

	if (!devf) return -1;

	ux = devf->minor;
	uartp = uarts+ux;		// Access USART's buffer
	uart = uarts[ux].usart;		// Lookup USART address

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;		// Invalid UART ref

	if (!uartq[ux])
		return -EINVAL;	// not opened

	usart_disable_rx_interrupt(uart);

        switch (request) {
		case PICON_IOC_UART_SET_BPS:
			usart_set_baudrate(uart, (unsigned int) data);
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

	usart_enable_rx_interrupt(uart);

	return rc;
}

int picon_uart_write(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	unsigned char ux;
	const uart_t *uartp;
	uint32_t uart, n;

	if (!devf) return -1;

	ux = devf->minor;
	uartp = uarts+ux;		// Access USART's buffer
	uart = uarts[ux].usart;		// Lookup USART address

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;		// Invalid UART ref

	for (n=0; n < count; n++ ) {
		while ( (USART_SR(uart) & USART_SR_TXE) == 0 )
			RTOS_TASK_YIELD();	

		usart_send_blocking(uart, buf[n]);
	}

	return n;
}


int picon_uart_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	unsigned char ux;
	const uart_t *uartp;
	uint32_t n;
	rtos_tick_type_t	wait_time = RTOS_PORT_MAX_DELAY;

	if (!devf) return -1;

	ux = devf->minor;
	uartp = uarts+ux;		// Access USART's buffer

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
