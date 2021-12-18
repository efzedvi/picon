/**
 * @author      : faraz
 * @file        : dev.c
 * @created     : Fri 17 Dec 2021 07:24:14 PM EST
 */

#include "picon/dev.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

static unsigned char dev_num = 0;

static const DEVICE_FILE  **dev_files;

void dev_init(const DEVICE_FILE **df)
{
	unsigned char i;

	dev_files = df;
        for(i=0; dev_files[i] && i<INT8_MAX; i++) {
		dev_files[i]->dev->init(dev_files[i]->minor, dev_files[i]->init_params);
	}
	dev_num = i;
}

unsigned char dev_count(void)
{
        return dev_num;
}

const char *dev_get_name(unsigned char idx)
{
	if (idx >= dev_num) return NULL;
	if (!dev_files[idx]) return NULL;

	return (const char *) dev_files[idx]->name;
}


const DEVICE_FILE *dev_by_idx(unsigned char idx)
{
	if (idx >= dev_num) return NULL;
	if (!dev_files[idx]) return NULL;

	return dev_files[idx];
}

const DEVICE_FILE *dev_by_name(const DEV_NAME name)
{
	unsigned char i;
	const DEVICE_FILE *found=NULL;

        for(i=0; dev_files[i] && i<INT8_MAX; i++) {
		if (strncmp(dev_files[i]->name, name, DEV_NAME_SIZE)==0) {
			found = dev_files[i];
			break;
		}
	}

	return found;
}

