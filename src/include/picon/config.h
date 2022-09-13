/**
 * @author      : faraz (faraz)
 * @file        : config
 * @created     : Fri 17 Dec 2021 07:07:26 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CONFIG_H
#define CONFIG_H

#define XSTR(x) #x
#define STR(x) XSTR(x)

#define PICON_VERSION_MAJOR	0
#define PICON_VERSION_MINOR	7
#define PICON_VERSION_PATCH	0

#define PICON_VERSION	STR(PICON_VERSION_MAJOR) "." \
			STR(PICON_VERSION_MINOR) "." \
			STR(PICON_VERSION_PATCH)

#define PICON_BUILD_DATE		__DATE__
#define PICON_BUILD_TIME		__TIME__

// debug console uart
#define CONFIG_DBG_CONSOLE_ENABLED

// configuration macros
#define CONFIG_HEARTBEAT		(1)
#define CONFIG_WDG			(1)
#define CONFIG_WDG_PERIOD		(2500)
#define CONFIG_HEARTBEAT_RATE		(500)
#define CONFIG_HEARTBEAT_PRIORITY	(9)
#define CONFIG_HEARTBEAT_STACK_SIZE	(48)
#define CONFIG_TMP_DEV_SIZE		(128)

#ifdef PICO_DEFAULT_LED_PIN
#define CONFIG_HEARTBEAT_PIN		PICO_DEFAULT_LED_PIN
#endif

// define below macro to make UART driver use TX queue
#define CONFIG_UART_TX_QUEUE

#define CONFIG_SHELL_CMD_PRIORITY	(8)

#define PICON_ENV			(1)
#define PICON_LOGGING			(1)
#define CONFIG_SHELL_EXPR		(1)
#define CONFIG_LOGGING			(1)
#define CONFIG_ENV			(1)
#define CONFIG_PWM			(1)

#endif /* end of include guard CONFIG_H */
