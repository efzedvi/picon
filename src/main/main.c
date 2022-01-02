/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "picon.h"
#include "picon/dev.h"          
#include "picon/io.h"           
#include "picon/console.h"      
#include "picon/ioctl.h"        
#include "picon/uart.h"
#include "picon/usb_serial.h"
#include "picon/mem.h"
#include "shell/shell.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "rtos.h"

const uart_cfg_t picon_uart_cfg = {
		.baud = PICON_DEFAULT_BAUD_RATE,
		.data_bits = PICON_DEFAULT_DATA_BITS,
		.stop_bits = PICON_DEFAULT_STOP_BITS,
		.parity    = PICON_DEFAULT_PARITY,
		.rx  = PICON_DEFAULT_UART_RX_PIN,
		.tx  = PICON_DEFAULT_UART_TX_PIN,
		.cts = -1,
		.rts = -1,
		};

static const DEVICE_FILE *dt[] = {
	DEV_ENTRY("/dev/usbcdc",  USB_SERIAL_DEV, 0, NULL)
	DEV_ENTRY("/dev/mem", MEM_DEV, 0, NULL)
        DEV_ENTRY("/dev/uart0", UART_DEV, 0, (void *) &picon_uart_cfg)
        NULL
};


int main()
{
	picon_init();
	io_init();
        dev_init(dt);

        CONSOLE_CREATE("/dev/uart0", "ttyS0");
        CONSOLE_CREATE("/dev/usbcdc", "ttyCDC");

	rtos_task_start_scheduler();

	for(;;);
}

