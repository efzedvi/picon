/**
 * @author      : faraz
 * @file        : flash
 * @created     : Sun 16 Jan 2022 05:19:57 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FLASH_H
#define FLASH_H

#include "picon/ioctl.h"
#include "picon/dev.h"

// The "+ FLASH_SECTOR_SIZE" is reserved for system parameters, the disk size is only 256 * 1024
#define PICON_STORAGE_SIZE	(256 * 1024 + FLASH_SECTOR_SIZE)
#define PICON_NUM_SECTORS	(PICON_STORAGE_SIZE / FLASH_SECTOR_SIZE)

int picon_flash_init(uint8_t ux, void *params);

const void *picon_flash_open(const DEVICE_FILE *devf, int flags);
int picon_flash_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data);
int picon_flash_close(const DEVICE_FILE *devf);
int picon_flash_pread(const DEVICE_FILE *devf, void *buf, uint16_t count, uint32_t offset);
int picon_flash_pwrite(const DEVICE_FILE *devf, const void *buf, uint16_t count, uint32_t offset);

#define FLASH_DEV \
&(DEVICE)\
{ .init  = picon_flash_init,\
  .open  = picon_flash_open,\
  .close = picon_flash_close,\
  .read  = NULL,\
  .write = NULL,\
  .pread = picon_flash_pread,\
  .pwrite= picon_flash_pwrite,\
  .ioctl = picon_flash_ioctl,\
  .lseek = NULL,\
  .sendto   = NULL,\
  .recvfrom = NULL,\
  .fsync = NULL,\
  .flags = DEV_FLAG_SERIALIZE | DEV_FLAG_BLOCKDEV \
}


#endif /* end of include guard FLASH_H */

