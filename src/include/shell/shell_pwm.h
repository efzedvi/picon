/**
 * @author      : faraz
 * @file        : shell_pwm
 * @created     : Sun 08 May 2022 06:34:34 PM EDT
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SHELL_PWM_H
#define SHELL_PWM_H

#include <picon/config.h>

#define SHELL_PWM_CMD_STACK_SIZE  (256)

int shell_pwm(int argc, char *argv[]);

#ifdef CONFIG_PWM
#define SHELL_PWM_COMMAND \
	{ CMD(pwm), SHELL_PWM_CMD_STACK_SIZE, "PWM command", 0 },
#else
#define SHELL_PWM_COMMAND
#endif

#endif /* end of include guard SHELL_PWM_H */

