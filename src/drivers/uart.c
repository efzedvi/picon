 /**
 * @author      : faraz
 * @file        : uart.c
 * @created     : Fri 17 Dec 2021 07:28:36 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
#include "picon/uart.h"
#include "picon/log.h"

#include "hardware/platform_defs.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "pico/platform.h"


#define UART_QUEUE_SIZE		(128)
#define UART_MAX		NUM_UARTS
#define MIN_WAIT_TIME		(40)

typedef struct _uart_t {
	uart_inst_t*		uart;		// UART pointer
	uart_cfg_t		cfg;
} uart_t;

// UART receive queues
rtos_queue_handle_t uartq_rx[UART_MAX];
#ifdef CONFIG_UART_TX_QUEUE
rtos_queue_handle_t uartq_tx[UART_MAX];
#endif

// to handle control-c
rtos_semaphore_handle_t  uart_sems[UART_MAX];

static uart_t uarts[UART_MAX] = {
	{ .uart = uart0,
	  .cfg = {
		.baud = PICON_DEFAULT_BAUD_RATE,
		.data_bits = PICON_DEFAULT_DATA_BITS,
		.stop_bits = PICON_DEFAULT_STOP_BITS,
		.parity    = PICON_DEFAULT_PARITY,
		.rx  = PICON_DEFAULT_UART_RX_PIN,
		.tx  = PICON_DEFAULT_UART_TX_PIN,
		.cts = -1,
		.rts = -1,
	  },
	},
	{ .uart = uart1,
	  .cfg = {
		.baud = 0,
		.data_bits = -1,
		.stop_bits = -1,
		.parity    = -1,
		.rx  = -1,
		.tx  = -1,
		.cts = -1,
		.rts = -1,
	  },
	},

};

uint8_t uart_flags[UART_MAX];

// ISR to receive data, __time_critical_func causes to not be in flash (i.e is placed in RAM)
static void __time_critical_func(picon_uart_common_isr)(uint8_t ux)
{
	const uart_t *uartp = uarts+ux;		// Access UART's buffer
	uart_inst_t  *uart = uarts[ux].uart;	// Lookup UART address
	uint8_t 	ch;			// Read data byte
	rtos_base_type_t	higher_priority_task_woken;
	rtos_base_type_t	rv;
	uint16_t must_yield=0;


	if ( ux >=UART_MAX || !uartp)
		return;				// Not open for ISR receiving!

	if (uartq_rx[ux] && uart_is_readable(uart)) {
		// Read data
		//ch = (uint8_t) uart_getc(uart);
		ch = (uint8_t) uart_get_hw(uart)->dr;

		// This is a lame way to handle control-c, but we keep it for now until I think
		// of a better and more ellegant way
		if (uart_sems[ux] != NULL  && ch == CONTROL('C')) {
			rtos_semaphore_give_from_isr(uart_sems[ux], &higher_priority_task_woken);
		} else {
			// Save data if the buffer is not full
			rtos_queue_send_from_isr(uartq_rx[ux], &ch, &higher_priority_task_woken);
		}

		if (higher_priority_task_woken == RTOS_TRUE) {
			must_yield = 1;
		}
	}

#ifdef CONFIG_UART_TX_QUEUE
	if (uartq_tx[ux]) {
		if (uart_is_writable(uart) && rtos_queue_messages_waiting_from_isr(uartq_tx[ux])) {
			rv = rtos_queue_receive_from_isr(uartq_tx[ux], &ch, &higher_priority_task_woken);

			if (rv == RTOS_TRUE) {
				uart_get_hw(uart)->dr = ch;
				//uart_putc_raw(uart, ch);

				if (higher_priority_task_woken == RTOS_TRUE) {
					must_yield = 1;
				}
			}
		}

		// whether or not we want to receive TX interrupts depends on availability of items in the TX quieue
		uart_set_irq_enables(uart, uartq_rx[ux]!=NULL, rtos_queue_messages_waiting_from_isr(uartq_tx[ux]));
	}
#endif

	rtos_port_yield_from_isr(must_yield);
}


void picon_uart0_isr(void)
{
	picon_uart_common_isr(0);
}

void picon_uart1_isr(void)
{
	picon_uart_common_isr(1);
}

int picon_uart_init(uint8_t ux, void *params)
{
	uart_t		*uartp = uarts+ux;		// Access UART's buffer
	uart_inst_t  	*uart = uarts[ux].uart;		// Lookup UART address
	int uart_irq;
	uart_cfg_t	*cfg;

	if (params) {
		cfg = (uart_cfg_t *) params;
		uartp->cfg = *cfg;
	}

	if (uartp->cfg.baud < 1200)
		uartp->cfg.baud = PICON_DEFAULT_BAUD_RATE;

	if (uartp->cfg.data_bits < 0)
		uartp->cfg.data_bits = PICON_DEFAULT_DATA_BITS;

	if (uartp->cfg.stop_bits < 0)
		uartp->cfg.stop_bits = PICON_DEFAULT_STOP_BITS;

	if (uartp->cfg.parity < 0)
		uartp->cfg.parity = PICON_DEFAULT_PARITY;


	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;			// Invalid UART ref

	if (uartq_rx[ux])
		return -EINVAL;			// already initialized

#ifdef CONFIG_UART_TX_QUEUE
	if (uartq_tx[ux])
		return -EINVAL;			// already initialized
#endif

	uart_sems[ux] = NULL;

	if (uartp->cfg.rx < 0 && uartp->cfg.tx < 0)
		return -EINVAL;			// Invalid cfg

	uart_init(uart, uartp->cfg.baud);

	if (uartp->cfg.rx >= 0) {
		uartp->cfg.rx &= 0x1f;
		gpio_init(uartp->cfg.rx);
		gpio_set_dir(uartp->cfg.rx, GPIO_IN);
		gpio_set_function(uartp->cfg.rx, GPIO_FUNC_UART);
		gpio_pull_up(uartp->cfg.rx); // input pull up mode
	}

	if (uartp->cfg.tx >= 0) {
		uartp->cfg.tx &= 0x1f;
		gpio_init(uartp->cfg.tx);
		gpio_set_dir(uartp->cfg.tx, GPIO_OUT);
		gpio_set_function(uartp->cfg.tx, GPIO_FUNC_UART);
		gpio_set_pulls(uartp->cfg.tx, true, true);	// pushpull
	}

	if (uartp->cfg.cts >= 0) {
		uartp->cfg.cts  &= 0x1f;
		gpio_set_function(uartp->cfg.cts, GPIO_FUNC_UART);
	}

	if (uartp->cfg.rts >= 0) {
		uartp->cfg.rts  &= 0x1f;
		gpio_set_function(uartp->cfg.cts, GPIO_FUNC_UART);
	}

	uart_irq = (uart == uart0) ? UART0_IRQ : UART1_IRQ;

	// set hw flow control
	uart_set_hw_flow(uart, uartp->cfg.cts >= 0, uartp->cfg.rts >= 0);

	// Set our data format
	uart_set_format(uart, uartp->cfg.data_bits, uartp->cfg.stop_bits, uartp->cfg.parity);

	// Turn off FIFO's - we want to do this character by character
	uart_set_fifo_enabled(uart, false);

	if (uartp->cfg.rx >= 0 || uartp->cfg.tx >= 0) {
		if (ux == 0) {
			irq_set_exclusive_handler(uart_irq, picon_uart0_isr);
		} else {
			irq_set_exclusive_handler(uart_irq, picon_uart1_isr);
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

	if (uartq_rx[ux])
		return (const void *) devf;	// already initialized

#ifdef CONFIG_UART_TX_QUEUE
	if (uartq_tx[ux])
		return (const void *) devf;	// already initialized
#endif

	uart_flags[ux] = flags;

	uart_set_irq_enables(uart, false, false);
	if (uartp->cfg.rx >= 0) {
		uartq_rx[ux] = rtos_queue_create(UART_QUEUE_SIZE, sizeof(char));
		if (!uartq_rx[ux]) goto failure;
	}

#ifdef CONFIG_UART_TX_QUEUE
	if (uartp->cfg.tx >= 0) {
		uartq_tx[ux] = rtos_queue_create(UART_QUEUE_SIZE, sizeof(char));
		if (!uartq_tx[ux]) goto failure;
	}
#endif
	// Now enable the UART to send interrupts
        // since we just opened the dvice there is no items in the TX queue hence
	// we don't want the ISR to be flooding us for being available to send
	uart_set_irq_enables(uart, uartq_rx[ux]!=NULL, false);

	return (const void *) devf;

failure:
	if (uartq_rx[ux])
		rtos_queue_delete(uartq_rx[ux]);

	uartq_rx[ux] = NULL;

#ifdef CONFIG_UART_TX_QUEUE
	if (uartq_tx[ux])
		rtos_queue_delete(uartq_tx[ux]);

	uartq_tx[ux] = NULL;
#endif

	return NULL;
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

#ifdef CONFIG_UART_TX_QUEUE
	if (!uartq_rx[ux] && !uartq_tx[ux])
		return -EINVAL;	// not opened?

	if (uartq_tx[ux])
		rtos_queue_delete(uartq_tx[ux]);

	uartq_tx[ux] = NULL;
#endif

	// stop receiving interrupts
	uart_set_irq_enables(uart, false ,false);

	if (uartq_rx[ux])
		rtos_queue_delete(uartq_rx[ux]);

	uartq_rx[ux] = NULL;
	uart_sems[ux] = NULL;
	uart_flags[ux] = 0;

	return 0;
}

int picon_uart_fsync(const DEVICE_FILE *devf)
{
	unsigned char 	ux;
	const uart_t	*uartp;
	uart_inst_t     *uart;

	if (!devf) return -EINVAL;

#ifdef CONFIG_UART_TX_QUEUE
	ux = devf->minor;
	uartp = uarts+ux;		// Access UART's buffer
	uart = uarts[ux].uart;		// Lookup UART address

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;			// Invalid UART ref

	if (!uartq_tx[ux])
		return 0; // nothing to be done

	// stop receiving interrupts
	uart_set_irq_enables(uart, false ,false);

	// block until all TX data is transmitted
	while (rtos_queue_messages_waiting(uartq_tx[ux]))
		rtos_task_yield();
#endif

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

#ifdef CONFIG_UART_TX_QUEUE
	if (!uartq_rx[ux] && !uartq_tx[ux])
		return -EINVAL;	// not opened
#endif

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

#ifdef CONFIG_UART_TX_QUEUE
	uart_set_irq_enables(uart, uartq_rx[ux]!=NULL, uartq_tx[ux]!=NULL && rtos_queue_messages_waiting(uartq_tx[ux]) );
#else
	uart_set_irq_enables(uart, uartq_rx[ux]!=NULL, false);
#endif

	return rc;
}

int picon_uart_write(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	unsigned char 	ux;
	const uart_t	*uartp;
	uart_inst_t     *uart;
	uint32_t	n;
	rtos_base_type_t        rv;

	if (!devf) return -1;

	ux = devf->minor;
	uartp = uarts+ux;		// Access UART's buffer
	uart = uarts[ux].uart;		// Lookup UART address

	if ( ux >=UART_MAX || !uartp)
		return -EINVAL;		// Invalid UART ref

#ifdef CONFIG_UART_TX_QUEUE
	if (!uartq_tx[ux] )
		return -EINVAL;

	for (n=0; n < count; n++ ) {
		rv = rtos_queue_send(uartq_tx[ux], buf+n, RTOS_PORT_MAX_DELAY);
		if (rv != RTOS_TRUE)
			break;
	}

	uart_set_irq_enables(uart, uartq_rx[ux]!=NULL, rtos_queue_messages_waiting(uartq_tx[ux]) );
#else
	for (n=0; n < count; n++ ) {
		uart_putc_raw(uart, buf[n]);
	}
#endif

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

	if ( ux >=UART_MAX || !uartp || !(uartq_rx[ux]) )
		return -1;			// Invalid UART ref or not opened

	for (n=0; n < count; n++ ) {
		if (rtos_queue_receive(uartq_rx[ux], buf++, wait_time) != RTOS_PASS)
			break;
	}

	return n;
}
