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
#include "picon.h"


int picon_mem_init(uint8_t ux, void *params)
{
	UNUSED(ux);
	UNUSED(params);

	return 0;
}


const void *picon_mem_open(const DEVICE_FILE *devf, int flags)
{
	UNUSED(devf);
	UNUSED(flags);

	return (const void *) devf;
}


int picon_mem_close(const DEVICE_FILE *devf)
{
	UNUSED(devf);

	return 0;
}


int picon_mem_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data)
{
	UNUSED(devf);
	UNUSED(request);
	UNUSED(data);

	return 0;
}

int picon_mem_pread(const DEVICE_FILE *devf, void *buf, uint16_t count, uint32_t offset)
{
	UNUSED(devf);
	if (!buf) return -EINVAL;

	memcpy(buf, (void *) offset, count);

	return (int) count;
}


int picon_mem_pwrite(const DEVICE_FILE *devf, const void *buf, uint16_t count, uint32_t offset)
{
	UNUSED(devf);
	if (!buf) return -EINVAL;

	memcpy((void *) offset, buf, count);

	return (int) count;
}


