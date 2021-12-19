/**
 * @author      : faraz (faraz)
 * @file        : config
 * @created     : Fri 17 Dec 2021 07:07:26 PM EST
 */

#ifndef CONFIG_H
#define CONFIG_H

#define XSTR(x) #x
#define STR(x) XSTR(x)

#define PICON_VERSION_MAJOR	0
#define PICON_VERSION_MINOR	4
#define PICON_VERSION_PATCH	0

#define PICON_VERSION	STR(PICON_VERSION_MAJOR) "." \
			STR(PICON_VERSION_MINOR) "." \
			STR(PICON_VERSION_PATCH)

#define PICON_BUILD_DATE		__DATE__
#define PICON_BUILD_TIME		__TIME__


// configuration macros
#define CONFIG_HEART_BEAT		(1)
#define CONFIG_WDG			(1)
#define CONFIG_WDG_PERIOD		(5000)
#define CONFIG_HEART_BEAT_RATE		(500)
#define CONFIG_HEART_BEAT_PRIORITY	(2)
#define CONFIG_HEART_BEAT_STACK_SIZE	(48)

#define CONFIG_SHELL_CMD_PRIORITY	(2)

#define CONFIG_SHELL_EXPR		(1)

#define CONFIG_LOGGING			(1)

#endif /* end of include guard CONFIG_H */