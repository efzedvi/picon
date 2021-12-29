/**
 * @author      : faraz
 * @file        : picon
 * @created     : Sat 18 Dec 2021 05:53:59 PM EST
 */

#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "hardware/rtc.h"
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


#ifdef CONFIG_HEART_BEAT
static void picon_heart_beat_task(void *args)
{
	UNUSED(args);

	while (1) {
		gpio_xor_mask(1<<CONFIG_HEART_BEAT_PIN);
		rtos_task_delay(rtos_ms_to_ticks(CONFIG_HEART_BEAT_RATE));

#ifdef CONFIG_WDG
		// kick the watchdog
		watchdog_update();
#endif
	}
}
#endif

int picon_init()
{
	rtc_init();

	bi_decl(bi_program_description("Picon (Pico Console) program. Author: Faraz Vahabzadeh (faraz@fzv.ca)"));
	bi_decl(bi_program_version_string(PICON_VERSION));
	bi_decl(bi_program_url("<github url here>")); //TODO


#ifdef CONFIG_HEART_BEAT
	gpio_init(CONFIG_HEART_BEAT_PIN);
	gpio_set_dir(CONFIG_HEART_BEAT_PIN, GPIO_OUT);
	gpio_put(CONFIG_HEART_BEAT_PIN, 0);

	rtos_task_create(picon_heart_beat_task, "hrtbt", CONFIG_HEART_BEAT_STACK_SIZE,
		    NULL, CONFIG_HEART_BEAT_PRIORITY, NULL);
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

