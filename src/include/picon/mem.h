/**
 * @author      : faraz
 * @file        : mem
 * @created     : Fri 17 Dec 2021 07:19:23 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MEM_H
#define MEM_H

#include "picon/ioctl.h"
#include "picon/dev.h"


int picon_mem_init(uint8_t ux, void *params);

const void *picon_mem_open(const DEVICE_FILE *devf, int flags);
int picon_mem_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data);
int picon_mem_close(const DEVICE_FILE *devf);
int picon_mem_pread(const DEVICE_FILE *devf, void *buf, uint16_t count, uint32_t offset);
int picon_mem_pwrite(const DEVICE_FILE *devf, const void *buf, uint16_t count, uint32_t offset);

#define MEM_DEV \
&(DEVICE)\
{ .init  = picon_mem_init,\
  .open  = picon_mem_open,\
  .close = picon_mem_close,\
  .read  = NULL,\
  .write = NULL,\
  .pread = picon_mem_pread,\
  .pwrite= picon_mem_pwrite,\
  .ioctl = picon_mem_ioctl,\
  .lseek = NULL,\
  .sendto   = NULL,\
  .recvfrom = NULL,\
  .fsync = NULL,\
  .flags = DEV_FLAG_SERIALIZE | DEV_FLAG_MEMORY \
}


#endif /* end of include guard MEM_H */

