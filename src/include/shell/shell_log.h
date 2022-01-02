/**
 * @author      : faraz
 * @file        : shell_log
 * @created     : Fri 24 Dec 2021 06:49:57 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SHELL_LOG_H
#define SHELL_LOG_H

#include <picon/config.h>

#define SHELL_LOG_CMD_STACK_SIZE  (256)

int shell_log(int argc, char *argv[]);

#ifdef PICON_LOGGING
#define SHELL_LOG_COMMAND \
	{ CMD(log), SHELL_LOG_CMD_STACK_SIZE, "Log command used for debugging", 0 },
#else
#define SHELL_LOG_COMMAND
#endif

#endif /* end of include guard SHELL_LOG_H */

