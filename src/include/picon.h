/**
 * @author      : faraz
 * @file        : picon
 * @created     : Sat 18 Dec 2021 06:17:07 PM EST
 */

#ifndef PICON_H 
#define PICON_H

#include <time.h>
#include <stdbool.h>

#include "picon/config.h"

int picon_init(void);
void picon_reboot(void);
bool picon_watchdog_caused_reboot(void);

#endif /* end of include guard BOARD_H */

