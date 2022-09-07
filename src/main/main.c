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
#include "picon/i2c.h"
#include "picon/usb_serial.h"
#include "picon/mem.h"
#include "picon/null.h"
#include "picon/zero.h"
#include "picon/tmp.h"
#include "picon/log.h"
#include "picon/env.h"
#include "picon/flash.h"
#include "picon/ateeprom.h"
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
	.tx  = PICON_DEFAULT_UART_TX_PIN, //gpio 0
	.rx  = PICON_DEFAULT_UART_RX_PIN, //gpio 1
	.cts = -1,
	.rts = -1,
	};

const i2c_cfg_t picon_i2c_cfg = {
	.speed = PICON_I2C_STANDARD_100K,
	.sda = PICO_DEFAULT_I2C_SDA_PIN,	//gpio 4
	.scl = PICO_DEFAULT_I2C_SCL_PIN		//gpio 5
	};

const PICON_ATEEPROM_CFG eeprom_config = {
	.address	= 0x50,
	.type		= ATEEPROM_24C08,
	.device_name	= "/dev/i2c0"
};


static const DEVICE_FILE *dt[] = {
	DEV_ENTRY("/dev/usbcdc",  USB_SERIAL_DEV, 0, NULL)
	DEV_ENTRY("/dev/null", NULL_DEV, 0, NULL)
	DEV_ENTRY("/dev/zero", ZERO_DEV, 0, NULL)
	DEV_ENTRY("/dev/tmp", TMP_DEV, 0, NULL)
	DEV_ENTRY("/dev/mem", MEM_DEV, 0, NULL)
	DEV_ENTRY("/dev/uart0", UART_DEV, 0, (void *) &picon_uart_cfg)
	DEV_ENTRY("/dev/i2c0", I2C_DEV, 0, (void*) (&picon_i2c_cfg))
	DEV_ENTRY("/dev/flash", FLASH_DEV, 0, NULL)
	//DEV_ENTRY("/dev/nvm", ATEEPROM_DEV, 0, (void *) &eeprom_config)  // uncomment to use the above AT24C08 device
        NULL
};


int main()
{
	picon_init();
	io_init();
        dev_init(dt);
	env_init();

        CONSOLE_CREATE("/dev/uart0", "ttyS0");
        CONSOLE_CREATE("/dev/usbcdc", "ttyCDC");

	rtos_task_start_scheduler();

	for(;;);
}

