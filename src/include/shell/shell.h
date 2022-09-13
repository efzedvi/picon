/**
 * @author      : faraz
 * @file        : shell
 * @created     : Fri 17 Dec 2021 07:20:34 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SHELL_H
#define SHELL_H

#include "picon/config.h"

#define SHELL_MIN_CMD_STACK_SIZE	(256)
#define SHELL_DM_CMD_STACK_SIZE		(280)

#define	SHELL_CMD_PROPERTY_CANCELABLE		(1<<0)


typedef int (*SHELL_CMD_FUNC)(int argc, char **argv);

typedef struct _shell_cmd {
	const char 	*cmd;
	SHELL_CMD_FUNC	func;
	uint16_t	stack_size;
	const char	*desc;
	uint8_t		property;
} SHELL_COMMAND;


extern const SHELL_COMMAND  shell_commands[];


int shell_parse_data(char *str, uint16_t len, uint8_t *buf, size_t size);

void shell_task(void *args);

int shell_clear(int argc, char **argv);
int shell_date(int argc, char **argv);
int shell_echo(int argc, char **argv);
int shell_free(int argc, char **argv);
int shell_lsdev(int argc, char **argv);
int shell_mem(int argc, char **argv);
int shell_ps(int argc, char **argv);
int shell_reboot(int argc, char **argv);
int shell_uname(int argc, char **argv);
int shell_uptime(int argc, char **argv);

int shell_primes(int argc, char **argv);

#ifndef CMD
#define CMD(NAME)  #NAME, shell_ ## NAME
#endif

#ifdef CONFIG_SHELL_EXPR
int shell_expr(int argc, char **argv);
#define SHELL_EXPR_CMD_DEF	{ CMD(expr), SHELL_MIN_CMD_STACK_SIZE, "A very simple calculator", 0 },
#else
#define SHELL_EXPR_CMD_DEF
#endif


#define SHELL_BASIC_COMMANDS \
	{ CMD(clear),	SHELL_MIN_CMD_STACK_SIZE, "Clears the terminal screen", 0 },\
	{ CMD(date),	SHELL_MIN_CMD_STACK_SIZE, "Displays and sets date and time", 0 },\
	{ CMD(free),	SHELL_MIN_CMD_STACK_SIZE, "Shows amount of unused heap memory", 0 },\
	{ CMD(lsdev),	SHELL_MIN_CMD_STACK_SIZE, "Lists device files", 0 },\
	{ CMD(mem),	SHELL_DM_CMD_STACK_SIZE,  "Peeks/Pokes into memory devices", 0 },\
	{ CMD(ps),	SHELL_MIN_CMD_STACK_SIZE, "Lists current tasks", 0 },\
	{ CMD(reboot),	SHELL_MIN_CMD_STACK_SIZE, "Reboots the system or goes into BOOTSEL mode", 0 },\
	{ CMD(uname),	SHELL_MIN_CMD_STACK_SIZE, "Unit name ; displayes build info", 0 },\
	{ CMD(uptime),	SHELL_MIN_CMD_STACK_SIZE, "Shows uptime", 0 },\
	{ CMD(echo),	SHELL_MIN_CMD_STACK_SIZE, "Echos its arguments on console", 0 },\
        SHELL_EXPR_CMD_DEF \
	{ CMD(primes),	SHELL_MIN_CMD_STACK_SIZE, "Test command ; displays prime numbers for testing the terminal", SHELL_CMD_PROPERTY_CANCELABLE },
//	{ CMD(primes),	SHELL_MIN_CMD_STACK_SIZE, "Test command ; displays prime numbers for testing the terminal", 0 },

#endif /* end of include guard SHELL_H */
