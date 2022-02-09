/**
 * @author      : faraz
 * @file        : io.c
 * @created     : Fri 17 Dec 2021 07:24:39 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "picon/dev.h"
#include "picon/io.h"
#include "picon/ioctl.h"
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "rtos.h"
#include "semphr.h"

#include "picon/log.h"

#define MAX_OPEN_FILES	(10)

typedef struct _open_file {
	const DEVICE_FILE	*devf;  //TODO: maybe keep an index to devs instead?
	volatile unsigned char	use_count;
	rtos_semaphore_handle_t	mtx;
} OPEN_FILE;


static rtos_semaphore_handle_t	iomtx;

static OPEN_FILE open_files[MAX_OPEN_FILES];

void io_init(void)
{
	short int i=0;

	for(i=0; i<MAX_OPEN_FILES; i++) {
		open_files[i].devf = NULL;
		open_files[i].mtx = NULL;
	}

	iomtx = rtos_semaphore_create_mutex();
	if (!iomtx) {
		//bad bad bad
		for(;;);
	}
}

int open(const char *name, int flags)
{
	const DEVICE_FILE 	*devf;
	unsigned char 	i=0;
	char		found = 0;
	int		rc = -1;

	devf = dev_by_name(name);
	if (!devf) return -ENXIO;

	rtos_semaphore_take(iomtx, RTOS_PORT_MAX_DELAY);

	for(i=0; i<MAX_OPEN_FILES; i++) {
		if (open_files[i].devf != NULL &&
		    strncmp(open_files[i].devf->name, name, DEV_NAME_SIZE) == 0) {
			// already opened, just return the index/fd
			open_files[i].use_count++;
			rc = i;
			goto leave; //not a fan of goto but it saves code space here
		}
	}

	for(i=0; i < MAX_OPEN_FILES; i++) {
		if (open_files[i].devf == NULL) {
			if (devf->dev->flags & DEV_FLAG_SERIALIZE) {
				open_files[i].mtx = rtos_semaphore_create_mutex();
				if (!open_files[i].mtx) {
					rc = -ENOMEM;
					goto leave;
				}
			} else {
				open_files[i].mtx = NULL;
			}

			if (!devf->dev->open || devf->dev->open(devf, flags) == NULL) {
				if (open_files[i].mtx) {
					rtos_semaphore_delete(open_files[i].mtx);
				}

				rc = -EINVAL;
				goto leave;
			}

			found = 1;
			rc = i;
			open_files[i].devf = devf;
			open_files[i].use_count = 1;

			break;
		}
	}

	if (!found) {
		rc = -ENOMEM; // TODO: find a replacement for ENOMEM
	}

leave:

	rtos_semaphore_give(iomtx);
	return rc;

}

int read(int fd, const void *buf, unsigned int count)
{
	DEVICE *dev;
	int rc;

	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	dev = open_files[fd].devf->dev;

	if (dev->read) {
		rc = dev->read(open_files[fd].devf, (unsigned char *) buf, count);
	} else {
		rc = -EINVAL;
	}

	if (open_files[fd].mtx)
		rtos_semaphore_give(open_files[fd].mtx);

	return rc;
}

int write(int fd, const void *buf, unsigned int count)
{
	DEVICE *dev;
	int rc;

	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	dev = open_files[fd].devf->dev;

	if (dev->write) {
		rc = dev->write(open_files[fd].devf, (unsigned char *) buf, count);
	} else {
		rc = -EINVAL;
	}

	if (open_files[fd].mtx)
		rtos_semaphore_give(open_files[fd].mtx);

	return rc;
}


int pread(int fd, void *buf, uint16_t count, uint32_t offset)
{
	DEVICE *dev;
	int rc;


	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	dev = open_files[fd].devf->dev;

	if (dev->pread && dev->flags & (DEV_FLAG_MEMORY | DEV_FLAG_BLOCKDEV) ) {
		rc = dev->pread(open_files[fd].devf, (unsigned char *) buf, count, offset);
	} else {
		rc = -EINVAL;
	}

	if (open_files[fd].mtx)
		rtos_semaphore_give(open_files[fd].mtx);

	return rc;
}


int pwrite(int fd, const void *buf, uint16_t count, uint32_t offset)
{
	DEVICE *dev;
	int rc;

	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	dev = open_files[fd].devf->dev;

	if (dev->pread && dev->flags & (DEV_FLAG_MEMORY | DEV_FLAG_BLOCKDEV) ) {
		rc = dev->pwrite(open_files[fd].devf, (unsigned char *) buf, count, offset);
	} else {
		rc = -EINVAL;
	}

	if (open_files[fd].mtx)
		rtos_semaphore_give(open_files[fd].mtx);

	return rc;
}

int lseek(int fd, uint32_t offset, int whence)
{
	DEVICE *dev;
	int rc;

	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	dev = open_files[fd].devf->dev;

	if (dev->lseek) {
		rc = dev->lseek(open_files[fd].devf, offset, whence);
	} else {
		rc = -EINVAL;
	}

	if (open_files[fd].mtx)
		rtos_semaphore_give(open_files[fd].mtx);

	return rc;
}

int ioctl(int fd, unsigned int request, void *data)
{
	DEVICE *dev;
	int rc=0;

	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	dev = open_files[fd].devf->dev;

	if (request == PICON_IOC_GET_DEV_FLAGS) {
		if (data)
			*( (uint8_t *)data ) = dev->flags;
		else
			rc = -EINVAL;
	} else {
		if (dev->ioctl) {
			rc = dev->ioctl(open_files[fd].devf, request, data);
		} else {
			rc = -EINVAL;
		}
	}

	if (open_files[fd].mtx)
		rtos_semaphore_give(open_files[fd].mtx);

	return rc;
}


int sendto(int fd, const void *buf, size_t len, int flags,
		   const void *dest_addr, uint8_t addrlen)
{
	DEVICE *dev;
	int rc;

	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	dev = open_files[fd].devf->dev;

	if (dev->sendto && dev->flags & DEV_FLAG_SOCKET) {
		rc = dev->sendto(open_files[fd].devf, buf, len, flags, dest_addr, addrlen);
	} else {
		rc = -EINVAL;
	}

	if (open_files[fd].mtx)
		rtos_semaphore_give(open_files[fd].mtx);

	return rc;
}

int recvfrom(int fd, void *buf, size_t len, int flags,
		     void *src_addr, uint8_t *addrlen)
{
	DEVICE *dev;
	int rc;

	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	dev = open_files[fd].devf->dev;

	if (dev->recvfrom && dev->flags & DEV_FLAG_SOCKET) {
		rc = dev->recvfrom(open_files[fd].devf, buf, len, flags, src_addr, addrlen);
	} else {
		rc = -EINVAL;
	}

	if (open_files[fd].mtx)
		rtos_semaphore_give(open_files[fd].mtx);

	return rc;
}


int close(int fd)
{
	DEVICE *dev;
	int rc = 0;

	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (--open_files[fd].use_count)  return 0;

	rtos_semaphore_take(iomtx, RTOS_PORT_MAX_DELAY);

	dev = open_files[fd].devf->dev;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	if (dev->close) {
		rc = dev->close(open_files[fd].devf);
	}

	open_files[fd].devf = NULL;

	if (open_files[fd].mtx)
		rtos_semaphore_delete(open_files[fd].mtx);

	rtos_semaphore_give(iomtx);

	return rc;
}

int fsync(int fd)
{
	DEVICE *dev;
	int rc = 0;

	if (fd < 0 || fd >= MAX_OPEN_FILES) return -EBADF;
	if (open_files[fd].devf == NULL) return -EINVAL;

	if (--open_files[fd].use_count)  return 0;

	dev = open_files[fd].devf->dev;

	if (open_files[fd].mtx)
		rtos_semaphore_take(open_files[fd].mtx, RTOS_PORT_MAX_DELAY);

	if (dev->fsync) {
		rc = dev->fsync(open_files[fd].devf);
	}

	if (open_files[fd].mtx)
		rtos_semaphore_give(open_files[fd].mtx);

	return rc;
}


