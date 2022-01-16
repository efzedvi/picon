/**
 * @author      : faraz
 * @file        : flash
 * @created     : Sun 16 Jan 2022 05:19:01 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>

#include "rtos.h"
#include "picon/utils.h"
#include "picon/ioctl.h"
#include "picon/flash.h"
#include "picon/io.h"
#include "picon/log.h"
#include "picon.h"

#include "hardware/flash.h"
#include "hardware/sync.h"

#define	FLASH_TASK_STACK_SIZE	(256)
#define	FLASH_TASK_PRIORITY	(12)

#define PICON_FLASH_REQ_READ		(0)
#define PICON_FLASH_REQ_ERASE		(1)
#define PICON_FLASH_REQ_WRITE		(2)
#define PICON_FLASH_REQ_ERASE_WRITE	(3)

#define PICON_STORAGE_FLASH_BASE	(PICO_FLASH_SIZE_BYTES - PICON_STORAGE_SIZE)
#define PICON_STORAGE_MEM_BASE		(XIP_NOCACHE_NOALLOC_BASE + PICON_STORAGE_FLASH_BASE)

rtos_task_handle_t	flash_task_handle = NULL;
rtos_semaphore_handle_t	flash_sem = NULL;

typedef struct _flash_req {
	uint8_t			code; // 0 : read, 1: write
	PICON_IOCTL_STORAGE	ioc;
} picon_flash_req_t;

picon_flash_req_t	req;

volatile uint32_t	core1_locked 		 = 0;
rtos_task_handle_t	core1_locker_task_handle = NULL;

static void __no_inline_not_in_flash_func(picon_core1_locker_task)(void *args)
{
	uint32_t	interrupt_status;

	while (1) {
		rtos_task_notify_take(RTOS_TRUE, RTOS_PORT_MAX_DELAY);

		rtos_task_suspend_all();
		interrupt_status = save_and_disable_interrupts();
		rtos_task_enter_critical();

		while (core1_locked)
			tight_loop_contents();

		rtos_task_exit_critical();
		restore_interrupts(interrupt_status);
		rtos_task_resume_all();
	}
}


static void __no_inline_not_in_flash_func(picon_flash_task)(void *args)
{
	uint32_t	interrupt_status;

	UNUSED(args);

	while (1) {
		rtos_task_notify_take(RTOS_TRUE, RTOS_PORT_MAX_DELAY);

		if (req.ioc.offset % FLASH_SECTOR_SIZE == 0) {

			core1_locked = 1;
			rtos_task_notify_give(core1_locker_task_handle);

			rtos_task_suspend_all();
			interrupt_status = save_and_disable_interrupts();
			rtos_task_enter_critical();

			switch (req.code) {
				case PICON_FLASH_REQ_READ:
					memcpy(req.ioc.buf,
						(uint8_t*)(PICON_STORAGE_MEM_BASE + req.ioc.offset),
						req.ioc.count);
					break;

				case PICON_FLASH_REQ_ERASE:
					flash_range_erase(PICON_STORAGE_FLASH_BASE + req.ioc.offset, req.ioc.count);
					break;

				case PICON_FLASH_REQ_WRITE:
					flash_range_program(PICON_STORAGE_FLASH_BASE + req.ioc.offset, req.ioc.buf, req.ioc.count);
					break;

				case PICON_FLASH_REQ_ERASE_WRITE:
					flash_range_erase(PICON_STORAGE_FLASH_BASE + req.ioc.offset, req.ioc.count);
					flash_range_program(PICON_STORAGE_FLASH_BASE + req.ioc.offset, req.ioc.buf, req.ioc.count);
					break;

				default:
					req.ioc.xfer_count = 0;
					break;
			}

			req.ioc.xfer_count = req.ioc.count;

			core1_locked = 0;

			rtos_task_exit_critical();
			restore_interrupts(interrupt_status);
			rtos_task_resume_all();

		} else {
			req.ioc.xfer_count = 0; 
		}

		rtos_semaphore_give(flash_sem);
	}
}

int picon_flash_init(uint8_t ux, void *params)
{
	rtos_base_type_t        rv = 0;

	UNUSED(ux);
	UNUSED(params);

	if (flash_task_handle != NULL && flash_sem != NULL)
		return -EEXIST; // already initialized;

	flash_sem = rtos_semaphore_create_binary();
	if (!flash_sem) return -ENOMEM;

	rv = rtos_task_create(picon_flash_task, "flash", FLASH_TASK_STACK_SIZE,
			NULL, FLASH_TASK_PRIORITY, &flash_task_handle);

	if (rv != RTOS_PASS || flash_task_handle == NULL) {
		rtos_semaphore_delete(flash_sem);
		flash_sem = NULL;
		return -ENOMEM;
	}

	// The intention is to have all the ISRs in core 0, and having
	// the flash task on core 0 make it easier to disable interrupts
	// that could mess things up while we access the flash
	rtos_task_core_affinity_set(flash_task_handle, 1 << 0);

	// start the core1 locker task
	rv = rtos_task_create(picon_core1_locker_task, "c1lck", 64,
			NULL, RTOS_CONFIG_MAX_PRIORITIES - 1, &core1_locker_task_handle);

	if (rv != RTOS_PASS || core1_locker_task_handle == NULL) {
		rtos_semaphore_delete(flash_sem);
		flash_sem = NULL;
		return -ENOMEM;
	}

	core1_locked = 0;

	return 0;
}


const void *picon_flash_open(const DEVICE_FILE *devf, int flags)
{
	UNUSED(devf);
	UNUSED(flags);

	return (const void *) devf;
}


int picon_flash_close(const DEVICE_FILE *devf)
{
	UNUSED(devf);

	return 0;
}


int picon_flash_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data)
{
	PICON_IOCTL_STORAGE	*ioc;

	UNUSED(devf);

	if (!data) return -EINVAL;

	memset(&req, 0, sizeof(req));

	switch (request) {
		case PICON_IOC_STORAGE_READ:
			req.code = PICON_FLASH_REQ_READ;
			break;

		case PICON_IOC_STORAGE_ERASE:
			req.code = PICON_FLASH_REQ_ERASE;
			break;

		case PICON_IOC_STORAGE_WRITE:
			req.code = PICON_FLASH_REQ_ERASE_WRITE;
			break;

		case PICON_IOC_STORAGE_ERASE_WRITE:
			req.code = PICON_FLASH_REQ_ERASE_WRITE;
			break;

		case PICON_IOC_STORAGE_GET_SIZE:
			*(uint32_t *) data = PICON_STORAGE_SIZE;
			return 0;

		case PICON_IOC_STORAGE_NUM_SECTORS:
			*(uint32_t *) data = PICON_NUM_SECTORS;
			return 0;

		default:
			return -EINVAL;
	}

	memcpy(&req.ioc, (PICON_IOCTL_STORAGE *) data, sizeof(PICON_IOCTL_STORAGE));

	rtos_task_notify_give(flash_task_handle);

	rtos_semaphore_take(flash_sem, RTOS_PORT_MAX_DELAY);

	if ( req.ioc.xfer_count != req.ioc.count ) {
		return -EIO;
	}

	return 0;
}

int picon_flash_pread(const DEVICE_FILE *devf, void *buf, uint16_t count, uint32_t offset)
{
	UNUSED(devf);

	if (!buf || offset > PICON_STORAGE_SIZE) return -EINVAL;

	memset(&req, 0, sizeof(req));

	req.code 	= PICON_FLASH_REQ_READ;
	req.ioc.buf  	= (uint8_t *) buf;
	req.ioc.offset 	= offset;
	req.ioc.count	= count;

	rtos_task_notify_give(flash_task_handle);

	rtos_semaphore_take(flash_sem, RTOS_PORT_MAX_DELAY);

	return (int) req.ioc.xfer_count;
}


int picon_flash_pwrite(const DEVICE_FILE *devf, const void *buf, uint16_t count, uint32_t offset)
{
	UNUSED(devf);

	if (!buf || (offset % FLASH_SECTOR_SIZE) || (count % FLASH_SECTOR_SIZE) || offset > PICON_STORAGE_SIZE) return -EINVAL;

	memset(&req, 0, sizeof(req));

	req.code 	= PICON_FLASH_REQ_ERASE_WRITE;
	req.ioc.buf  	= (uint8_t *) buf;
	req.ioc.offset 	= offset;
	req.ioc.count	= count;

	rtos_task_notify_give(flash_task_handle);

	rtos_semaphore_take(flash_sem, RTOS_PORT_MAX_DELAY);

	return (int) req.ioc.xfer_count;
}


