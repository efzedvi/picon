/**
 * @author      : faraz
 * @file        : picon
 * @created     : Sat 18 Dec 2021 05:53:59 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "hardware/rtc.h"
#include "hardware/adc.h"
#include "pico/types.h"
#include "pico/binary_info.h"

#include "rtos.h"
#include "picon.h"
#include "picon/utils.h"
#include "picon/printf.h"
#include "picon/console.h"
#include "picon/config.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#ifdef CONFIG_PICO_W
// NOTE: There seem to be a serious bug in the way CYW43 is supported by PICO-SDK for freertos
//       We wait for now until we find a workaround for it
//#include "pico/cyw43_arch.h"
#endif

#ifdef CONFIG_HEARTBEAT
static void picon_heart_beat_task(void *args)
{
	UNUSED(args);
#ifdef CONFIG_PICO_W
	int  pin_level = 1;
#endif

#ifdef CONFIG_PICO_W
	// NOTE: There seem to be a serious bug in the way CYW43 is supported by PICO-SDK for freertos
	//       We wait for now until we find a workaround for it
	//if (cyw43_arch_init() !=0) {
		//TODO: log , set a flag, or something
	//}
#endif

	while (1) {
#ifdef CONFIG_HEARTBEAT_PIN
		gpio_xor_mask(1<<CONFIG_HEARTBEAT_PIN);
#else
	#ifdef CONFIG_PICO_W
		// NOTE: There seem to be a serious bug in the way CYW43 is supported by PICO-SDK for freertos
		//       We wait for now until we find a workaround for it
		//cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, pin_level);
		pin_level ^= 1;
	#endif
#endif
		rtos_task_delay(rtos_ms_to_ticks(CONFIG_HEARTBEAT_RATE));

#ifdef CONFIG_WDG
		// kick the watchdog
		watchdog_update();
#endif
	}
}
#endif

int picon_init()
{
	rtos_task_handle_t hrtbt_task_handle;

	rtc_init();

	DBG_UART_INIT();

	bi_decl(bi_program_description("Picon (Pico Console) program. Author: Faraz Vahabzadeh (faraz@fzv.ca)"));
	bi_decl(bi_program_version_string(PICON_VERSION));
	bi_decl(bi_program_url("https://github.com/efzedvi/picon.git"));

#ifdef CONFIG_HEARTBEAT
	#ifndef CONFIG_PICO_W
	gpio_init(CONFIG_HEARTBEAT_PIN);
	gpio_set_dir(CONFIG_HEARTBEAT_PIN, GPIO_OUT);
	gpio_put(CONFIG_HEARTBEAT_PIN, 0);
	#endif
	rtos_task_create(picon_heart_beat_task, "hrtbt", CONFIG_HEARTBEAT_STACK_SIZE,
		    NULL, CONFIG_HEARTBEAT_PRIORITY, &hrtbt_task_handle);

	#if defined(CONFIG_PICO_W) && configUSE_CORE_AFFINITY && configNUM_CORES > 1
		// PICO_W is weird
		rtos_task_core_affinity_set(hrtbt_task_handle, 1 << 0);
	#endif
#endif

#ifdef CONFIG_ADC
	adc_init();
#endif

#ifdef CONFIG_WDG
	// set up the WDG
	watchdog_enable(CONFIG_WDG_PERIOD, 1);
#endif

	return 0;
}

void picon_reboot()
{
	watchdog_reboot(0, 0, 0);
}

bool picon_watchdog_caused_reboot()
{
	return watchdog_caused_reboot();
}

void picon_set_datetime(time_t time)
{
	struct tm	tm;
	datetime_t	dt;

	localtime_r(&time, &tm);

	dt.year  = tm.tm_year + 1900;
	dt.month = tm.tm_mon + 1;
	dt.day   = tm.tm_mday;
	dt.dotw  = tm.tm_wday;
	dt.hour  = tm.tm_hour;
	dt.min   = tm.tm_min;
	dt.sec   = tm.tm_sec;

	rtc_set_datetime(&dt);
}

time_t time(time_t *tloc)
{
	struct tm	tm;
	datetime_t	dt;
	time_t		t;

	rtc_get_datetime(&dt);

	tm.tm_year = dt.year - 1900;
	tm.tm_mon  = dt.month - 1;
	tm.tm_mday = dt.day;
	tm.tm_wday = dt.dotw;
	tm.tm_hour = dt.hour;
	tm.tm_min  = dt.min;
	tm.tm_sec  = dt.sec;

	t = mktime(&tm);

	if (tloc)
		*tloc = t;

	return t;
}

#ifdef CONFIG_DBG_CONSOLE_ENABLED
void dbg_msg(char *msg)
{
	if (!msg) return;

	while (*msg) {
		if (*msg == '\n')
			uart_putc_raw(DBG_UART, '\r');
		uart_putc_raw(DBG_UART, *msg++);
	}
}
#endif
