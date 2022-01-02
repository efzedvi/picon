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

#include "picon/config.h"

int picon_init(void);
void picon_reboot(void);
bool picon_watchdog_caused_reboot(void);
void picon_set_datetime(time_t time);

#endif /* end of include guard BOARD_H */

