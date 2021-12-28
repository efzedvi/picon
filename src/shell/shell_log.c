/**
 * @author      : faraz
 * @file        : shell_log
 * @created     : Fri 24 Dec 2021 06:52:08 PM EST
 */

#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "picon/config.h"
#include "picon/console.h"
#include "picon/printf.h"
#include "picon/io.h"
#include "picon/ioctl.h"
#include "picon/utils.h"
#include "picon.h"

#include "picon/log.h"
#include "shell/shell.h"
#include "shell/shell_log.h"

static void shell_log_usage(void)
{
	printf("log < on | off | start | stop | clear | dump >\n");
}

int shell_log(int argc, char *argv[])
{
	int 		rv = 0;
	uint16_t	size=0;
	uint8_t 	capturing=0;

	if (argc < 2) {
		log_status(&size, &capturing);
		printf("size=%d enabled=%d\n", size, capturing);
		return 0;
	}

	if (strcmp(argv[1], "on") == 0) {
		log_init(0); // TODO perhaps take an arg and pass it here?
	} else if (strcmp(argv[1], "off") == 0) {
		log_shutdown();
	} else if (strcmp(argv[1], "start") == 0) {
		log_capture(1); // start capturing
	} else if (strcmp(argv[1], "stop") == 0) {
		log_capture(0); // stop capturing
	} else if (strcmp(argv[1], "clear") == 0) {
		log_clear();
	} else if (strcmp(argv[1], "dump") == 0 || strcmp(argv[1], "d") == 0) {
		log_display(0); // TODO perhaps take an arg and pass it here?
	} else if (strcmp(argv[1], "help") == 0) {
		shell_log_usage();
	} else {
		shell_log_usage();
		rv = -1;
	}

	return rv;
}



