/**
 * @author      : faraz
 * @file        : picon
 * @created     : Sat 18 Dec 2021 06:17:07 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PICON_H 
#define PICON_H

#include <time.h>
#include <stdbool.h>
#include <time.h>

#include "hardware/uart.h"

#include "picon/config.h"

#ifdef CONFIG_DBG_CONSOLE_ENABLED

void dbg_msg(char *msg);

#define DBG_UART	uart0
#define DBG_UART_INIT()		\
{\
	uart_init(DBG_UART, 115200);\
	gpio_init(1);\
	gpio_set_dir(1, GPIO_IN);\
	gpio_set_function(1, GPIO_FUNC_UART);\
	gpio_pull_up(1);\
	gpio_init(0);\
	gpio_set_dir(0, GPIO_OUT);\
	gpio_set_function(0, GPIO_FUNC_UART);\
	gpio_set_pulls(0, true, true);\
	uart_set_hw_flow(DBG_UART, 0, 0);\
	uart_set_format(DBG_UART, 8, 1, UART_PARITY_NONE);\
}

#define DBG_MSG(s)	dbg_msg((s))

#else
#define DBG_UART_INIT()
#define DBG_MSG(s)
#endif


int picon_init(void);
void picon_reboot(void);
bool picon_watchdog_caused_reboot(void);
void picon_set_datetime(time_t time);

#endif /* end of include guard BOARD_H */

