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
#include "picon/null.h"
#include "picon/io.h"
#include "picon/log.h"
#include "picon.h"


int picon_null_init(uint8_t ux, void *params)
{
	UNUSED(ux);
	UNUSED(params);

	return 0;
}


const void *picon_null_open(const DEVICE_FILE *devf, int flags)
{
	UNUSED(devf);
	UNUSED(flags);

	return (const void *) devf;
}


int picon_null_close(const DEVICE_FILE *devf)
{
	UNUSED(devf);

	return 0;
}

int picon_null_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count)
{
	UNUSED(devf);
	UNUSED(count);

	if (!buf) return -EINVAL;

	return 0;
}


int picon_null_write(const DEVICE_FILE *devf, const unsigned char *buf, unsigned int count)
{
	UNUSED(devf);

	if (!buf) return -EINVAL;

	return (int) count;
}

