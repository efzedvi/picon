/**
 * @author      : faraz
 * @file        : zero 
 * @created     : Tue 06 Sep 2022 11:11:08 AM EDT
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
#include "picon/zero.h"
#include "picon/io.h"
#include "picon/log.h"
#include "picon.h"


int picon_zero_init(uint8_t ux, void *params)
{
	UNUSED(ux);
	UNUSED(params);

	return 0;
}


const void *picon_zero_open(const DEVICE_FILE *devf, int flags)
{
	UNUSED(devf);
	UNUSED(flags);

	return (const void *) devf;
}


int picon_zero_close(const DEVICE_FILE *devf)
{
	UNUSED(devf);

	return 0;
}

int picon_zero_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	UNUSED(devf);
	if (!buf) return -EINVAL;

	memset(buf, 0, count);

	return (int) count;
}


int picon_zero_write(const DEVICE_FILE *devf, const unsigned char *buf, unsigned int count)
{
	UNUSED(devf);
	if (!buf) return -EINVAL;

	return (int) count;
}


