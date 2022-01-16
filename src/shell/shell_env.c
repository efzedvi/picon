/**
 * @author      : faraz
 * @file        : shell_env
 * @created     : Sat 05 Feb 2022 06:37:20 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "picon/config.h"
#include "picon/console.h"
#include "picon/printf.h"
#include "picon/env.h"
#include "picon/utils.h"
#include "picon.h"

#include "shell/shell_env.h"

static void shell_env_usage(void)
{
	printf("env [-h ] | [ name [ = [value] ]\n");
}

static void shell_env_print(char *key, char *value)
{
	if (key && value)
		printf("%s=%s\n", key, value);
}

int shell_env(int argc, char **argv)
{
	uint8_t		i, num_envs;
	char		key[ENV_KEY_SIZE + 1];
	char		value[ENV_VALUE_SIZE + 1];

	memset(key, 0, sizeof(key));
	memset(value, 0, sizeof(value));

	switch (argc) {
		case 1: // List all the envs 
			num_envs = env_count();
			for(i=0; i<num_envs; i++) {
				if (env_get_by_id(i, key, ENV_KEY_SIZE, value, ENV_VALUE_SIZE) == 0)
					shell_env_print(key, value);
			}
			break;
		case 2:
			if (strcmp(argv[1], "-h") == 0) {
				shell_env_usage();
				return 0;
			}
			if (env_get(argv[1], value, ENV_VALUE_SIZE) != 0) {
				printf("'%s' not found\n", argv[1]);
				return 2;
			}
			shell_env_print(argv[1], value);
			break;
		case 3:
			if (strcmp(argv[2], "=") != 0) {
				shell_env_usage();
				return 1;
			}

			if (env_set(argv[1], NULL) != 0) {
				printf("Cannot unset variable\n");
			}

			break;
		case 4:
			if (strcmp(argv[2], "=") != 0) {
				shell_env_usage();
				return 1;
			}

			if (env_set(argv[1], argv[3]) != 0) {
				printf("Cannot set variable\n");
			}
			break;
		default:
			shell_env_usage();
			return 1;
	}

	return 0;
}
