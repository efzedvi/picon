/**
 * @author      : faraz
 * @file        : shell_i2c
 * @created     : Wed 05 Jan 2022 10:47:37 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SHELL_I2C_H
#define SHELL_I2C_H

#define SHELL_I2C_CMD_STACK_SIZE  (320)

int shell_i2c(int argc, char *argv[]);

#define SHELL_I2C_COMMAND \
	{ CMD(i2c), SHELL_I2C_CMD_STACK_SIZE, "Command to scan and xfer data to/from I2C devices", 0 },

#endif /* end of include guard SHELL_I2C_H */
