/**
 * @author      : faraz
 * @file        : shell_env
 * @created     : Sat 05 Feb 2022 07:02:25 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SHELL_ENV_H
#define SHELL_ENV_H

#include <picon/config.h>

#define SHELL_ENV_CMD_STACK_SIZE  (256)

int shell_env(int argc, char *argv[]);

#ifdef PICON_ENV
#define SHELL_ENV_COMMAND \
	{ CMD(env), SHELL_ENV_CMD_STACK_SIZE, "sets/gets/lists environment variables", 0 },
#else
#define SHELL_ENV_COMMAND
#endif

#endif /* end of include guard SHELL_ENV_H */

