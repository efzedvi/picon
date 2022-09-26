/**
 * @author      : faraz
 * @file        : shell_log
 * @created     : Sun 08 May 2022 06:37:25 PM EDT
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

#include "pico/types.h"
#include "hardware/pwm.h"
#include "hardware/platform_defs.h"
#include "hardware/gpio.h"

#include "picon/config.h"
#include "picon/console.h"
#include "picon/printf.h"
#include "picon/io.h"
#include "picon/ioctl.h"
#include "picon/utils.h"
#include "picon.h"

#include "shell/shell.h"
#include "shell/shell_pwm.h"

static void shell_pwm_stop(uint8_t gpio)
{
	uint		slice_num;

	slice_num = pwm_gpio_to_slice_num(gpio);

	pwm_set_enabled(slice_num, true);
	gpio_set_function(gpio, GPIO_FUNC_NULL);
}

// period is the same as wrap, aka TOP
// prescaler is the divider
static void shell_pwm_start(uint8_t gpio, uint8_t prescaler, uint16_t period, float duty_cycle)
{
	uint		slice;
	uint16_t	level;

	level = (uint16_t) ((period * 1.0f) * (duty_cycle / 100.0f));

	gpio_set_function(gpio, GPIO_FUNC_PWM);

	slice = pwm_gpio_to_slice_num(gpio);

	pwm_set_clkdiv_int_frac(slice, prescaler, 0.0f);

	pwm_set_wrap(slice, MAX(period-1, 1));
	pwm_set_chan_level(slice, pwm_gpio_to_channel(gpio), level);

	pwm_set_enabled(slice, true);
}


static void shell_pwm_usage(void)
{
	printf("pwm -g <gpio> [ -c stop | -p <prescaler> -t <period> -d <duty_cycle> -c start]\n");
}

int shell_pwm(int argc, char *argv[])
{
	const char      *opt_string = "p:t:d:g:c:h";
	PICON_OPT       opt;
	int             next_option, tmp;
        uint8_t         prescaler=1, slice, channel=0;
	uint16_t	period=0xffff; //wrap aka. TOP
        float           duty_cycle=1.0f;
        int8_t          gpio = -1, cmd = -1;
	int 		rv = 0;
	enum gpio_function	func;

	if (argc < 3 || argc > 11) {
		shell_pwm_usage();
		return -1;
	}

	opt_init(&opt, opt_string);

	do {
		next_option = getopt(&opt, argc, argv);

		switch (next_option) {
			case 'p':
				tmp = atoi(opt.optarg);
				if (tmp < 1 || tmp > UINT8_MAX) {
					printf("Invalid prescaler, must be [1-%d]\n", UINT8_MAX);
					return -2;
				}
				prescaler = (uint8_t) tmp;
				break;
			case 't':
				tmp = atoi(opt.optarg);
				if (tmp < 1 || tmp > UINT16_MAX) {
					printf("Invalid period/wrap, must be [1-%d]\n", UINT16_MAX);
					return -2;
				}
				period = (uint16_t) tmp;
				break;
			case 'd':
				duty_cycle = stof(opt.optarg);
				if (duty_cycle <= 0.0f || duty_cycle > 100.0f) {
					printf("Invalid duty cycle\n");
					return -2;
				}
				break;
			case 'g':
				tmp = atoi(opt.optarg);
				if (tmp < 0 || tmp >= NUM_BANK0_GPIOS) {
					printf("Invalid gpio\n");
					return -2;
				}
				gpio = (int8_t) tmp;
				func = gpio_get_function(gpio);
				if (func != GPIO_FUNC_PWM && func != GPIO_FUNC_SIO && func != GPIO_FUNC_XIP && func != GPIO_FUNC_NULL) {
					printf("gpio is not free!\n");
					return -2;
				}
				break;
			case 'c':
				if (strcmp(opt.optarg, "start") == 0)
					cmd = 1;
				else if (strcmp(opt.optarg, "stop") == 0)
					cmd = 0;
				else {
					printf("Invalid cmd: '%s' (must be 'start' or 'stop')\n", opt.optarg);
					return -2;
				}
				break;
			case 'h':
				shell_pwm_usage();
				return -1;
			case -1:
				break;
			case '?':
				printf("Invalid opiton\n\n");
				shell_pwm_usage();
				return -1;
			case ':':
				printf("Missing argument\n\n");
			default:
				shell_pwm_usage();
				return -1;
		}

	} while (next_option != -1);

	if (cmd == -1) {
		printf("no command specified (must be 'start' or 'stop')\n");
		return -3;
	}

	if (gpio == -1) {
		printf("no gpio specified\n");
		return -3;
	}

	if (cmd == 0) {
		shell_pwm_stop(gpio);
	} else {
		shell_pwm_start(gpio, prescaler, period, duty_cycle);
	}

	return rv;
}



