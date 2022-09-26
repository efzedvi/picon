/**
 * @author      : faraz
 * @file        : shell_adc
 * @created     : Sunday Sep 25, 2022 19:15:37 EDT
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef SHELL_ADC_H
#define SHELL_ADC_H

#include <picon/config.h>

#define SHELL_ADC_CMD_STACK_SIZE  (256)

int shell_adc(int argc, char *argv[]);

#ifdef CONFIG_ADC
#define SHELL_ADC_COMMAND \
	{ CMD(adc), SHELL_ADC_CMD_STACK_SIZE, "ADC command", 0 },
#else
#define SHELL_ADC_COMMAND
#endif

#endif /* end of include guard SHELL_ADC_H */
