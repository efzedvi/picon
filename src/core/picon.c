/**
 * @author      : faraz
 * @file        : picon
 * @created     : Sat 18 Dec 2021 05:53:59 PM EST
 */

#include "picon/picon.h"
#include "rtos.h"
#include "picon/utils.h"
#include "picon/printf.h"
#include "picon/console.h"
#include "picon/config.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#ifdef CONFIG_HEART_BEAT
static void picon_heart_beat_task(void *args)
{
	UNUSED(args);

	while (1) {
		RTOS_TASK_DELAY(RTOS_MS_TO_TICKS(CONFIG_HEART_BEAT_RATE));

		gpio_toggle(GPIOC,GPIO13);

#ifdef CONFIG_WDG
		// kick the watchdog
#endif
	}
}
#endif

int picon_board_init()
{

#ifdef CONFIG_HEART_BEAT

	rtos_task_create(board_heart_beat_task, "hrtbt", CONFIG_HEART_BEAT_STACK_SIZE,
		    NULL, CONFIG_HEART_BEAT_PRIORITY, NULL);

#endif

#ifdef CONFIG_WDG
	// set up the WDG
#endif

	return 0;
}

void picon_board_reset()
{
}

