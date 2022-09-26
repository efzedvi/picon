/**
 * @author      : faraz
 * @file        : shell_adc
 * @created     : Sun 25 Sep 2022 07:21:00 PM EDT
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
#include "pico/float.h"
#include "pico/double.h"
#include "hardware/adc.h"
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
#include "shell/shell_adc.h"

// Pico's ADC pins start from 26, and last one is 29
// ADC channel #4 reads the internal temperature

#define ADC_MAX		(NUM_ADC_CHANNELS - 1)
#define ADC_PIN(n) 	(26 + (n) % ADC_MAX) 
#define ADC_VREF	(3.3)           
#define ADC_RANGE	(1 << 12)    
#define ADC_CONVERT	(ADC_VREF / (ADC_RANGE - 1))
#define MAX_COUNT	(100)

static int shell_adc_read(uint8_t adc_channel, uint8_t count)
{
	uint32_t	sum = 0;
	uint16_t	raw_data = 0, avg;


	if (adc_channel >= NUM_ADC_CHANNELS || count > MAX_COUNT)
		return -1;

	if (adc_channel == 4) // if internal temperature sensor channel
		adc_set_temp_sensor_enabled(true);
	else
		adc_gpio_init( ADC_PIN(adc_channel) );

	adc_select_input( adc_channel );

	if (count == 0) count = 1;

	for(int i = 0; i < count; i++) {
		// NOTE: this has busy wait loop in it, perhaps a rewrite with yealing is in the future
		raw_data = adc_read();
		rtos_task_delay(1);
		sum += raw_data;
	}

	avg = sum / count;

	printf("raw=%d, float=%.2f\n", avg, avg * ADC_CONVERT);

	if (adc_channel == 4) { // internal temperature sensor channel
		float adc = (float) avg  * (float) ADC_CONVERT;
		float temp = 27.0f - (adc - 0.706f) / 0.001721f;

		printf("Internal MCU Temperature : %3.2f\n", temp);
	}

	return 0;
}

static void shell_adc_usage(void)
{
	printf("adc -a <n=0,1,2,3,4> [-c <count>]\n");
	printf("\tChannels 0-3 map to GPIOs 26-29\n");
}

int shell_adc(int argc, char *argv[])
{
	const char      *opt_string = "a:c:h";
	PICON_OPT       opt;
	int             next_option, tmp;
	uint8_t		adc_channel=0, count=0;

	if (argc < 3 || argc > 5 || argc == 4 ) {
		shell_adc_usage();
		return -1;
	}

	opt_init(&opt, opt_string);

	do {
		next_option = getopt(&opt, argc, argv);

		switch (next_option) {
			case 'a':
				tmp = atoi(opt.optarg);
				if (tmp < 0 || tmp >= NUM_ADC_CHANNELS) {
					printf("Invalid ADC channel %d, must be [0-%d]\n", tmp, NUM_ADC_CHANNELS);
					return -2;
				}
				adc_channel = (uint8_t) tmp;
				break;
			case 'c':
				tmp = atoi(opt.optarg);
				if (tmp < 1 || tmp > MAX_COUNT) {
					printf("Invalid count %d, must be [1-%d]\n", tmp, MAX_COUNT);
					return -2;
				}
				count = (uint16_t) tmp;
				break;
			case 'h':
				shell_adc_usage();
				return -1;
			case -1:
				break;
			case '?':
				printf("Invalid opiton\n\n");
				shell_adc_usage();
				return -1;
			case ':':
				printf("Missing argument\n\n");
			default:
				shell_adc_usage();
				return -1;
		}

	} while (next_option != -1);

	shell_adc_read(adc_channel, count);

	return 0;
}



