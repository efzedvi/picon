/**
 * @author      : faraz
 * @file        : zero (driver for /dev/zero) 
 * @created     : Tue 06 Sep 2022 10:50:39 AM EDT
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ZERO_H
#define ZERO_H

#include "picon/ioctl.h"
#include "picon/dev.h"


int picon_zero_init(uint8_t ux, void *params);

const void *picon_zero_open(const DEVICE_FILE *devf, int flags);
int picon_zero_close(const DEVICE_FILE *devf);
int picon_zero_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count);
int picon_zero_write(const DEVICE_FILE *devf, const unsigned char *buf, unsigned int count);

#define ZERO_DEV \
&(DEVICE)\
{ .init  = picon_zero_init,\
  .open  = picon_zero_open,\
  .close = picon_zero_close,\
  .read  = picon_zero_read,\
  .write = picon_zero_write,\
  .pread = NULL,\
  .pwrite= NULL,\
  .ioctl = NULL,\
  .lseek = NULL,\
  .sendto   = NULL,\
  .recvfrom = NULL,\
  .fsync = NULL,\
  .flags = DEV_FLAG_SERIALIZE \
}


#endif /* end of include guard MEM_H */

