/**
 * @author      : faraz
 * @file        : mem
 * @created     : Fri 17 Dec 2021 07:27:59 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <errno.h>
#include <sys/types.h>

#include "rtos.h"
#include "picon/utils.h"
#include "picon/ioctl.h"
#include "picon/mem.h"
#include "picon/io.h"
#include "picon/log.h"
#include "picon/config.h"
#include "picon.h"

#ifndef CONFIG_TMP_DEV_SIZE
#define CONFIG_TMP_DEV_SIZE	(4)
#endif

static uint8_t	tmp_buf[CONFIG_TMP_DEV_SIZE];
static uint16_t pos  = 0;
static uint16_t size = 0;


int picon_tmp_init(uint8_t ux, void *params)
{
	UNUSED(ux);
	UNUSED(params);

	memset(tmp_buf, 0, sizeof(tmp_buf));	

	return 0;
}


const void *picon_tmp_open(const DEVICE_FILE *devf, int flags)
{
	UNUSED(devf);

	if (flags == O_TRUNC) {
		memset(tmp_buf, 0, sizeof(tmp_buf));
		size = 0;
	}

	pos = 0;

	return (const void *) devf;
}


int picon_tmp_close(const DEVICE_FILE *devf)
{
	UNUSED(devf);

	pos = 0;

	return 0;
}

int picon_tmp_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	uint16_t i=0;

	UNUSED(devf);

	if (!buf) return -EINVAL;
	if (!count) return 0;

	while (i < count && i < size) {
		buf[i++] = tmp_buf[pos];
		pos = (pos + 1) % sizeof(tmp_buf);
	}

	return (int) i;
}


int picon_tmp_write(const DEVICE_FILE *devf, const unsigned char *buf, unsigned int count)
{
	uint16_t i=0;

	UNUSED(devf);

	if (!buf) return -EINVAL;
	if (!count) return 0;

	while (i < count) {
		tmp_buf[pos] = buf[i++];
		pos = (pos + 1) % sizeof(tmp_buf);
	}

	size = (size + count) % sizeof(tmp_buf);

	return (int) count;
}



int picon_tmp_pread(const DEVICE_FILE *devf, void *buf, uint16_t count, uint32_t offset)
{
	UNUSED(devf);
	if (!buf) return -EINVAL;

	memcpy(buf, (void *) tmp_buf + MIN(offset, sizeof(tmp_buf)), MIN(count, (sizeof(tmp_buf) - offset)));

	return (int) count;
}


int picon_tmp_pwrite(const DEVICE_FILE *devf, const void *buf, uint16_t count, uint32_t offset)
{
	UNUSED(devf);
	if (!buf) return -EINVAL;

	memcpy((void *) tmp_buf + MIN(offset, sizeof(tmp_buf)), buf, MIN(count, (sizeof(tmp_buf) - offset)));

	return (int) count;
}


