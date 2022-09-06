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

#ifndef NULL_H
#define NULL_H

#include "picon/ioctl.h"
#include "picon/dev.h"


int picon_null_init(uint8_t ux, void *params);

const void *picon_null_open(const DEVICE_FILE *devf, int flags);
int picon_null_close(const DEVICE_FILE *devf);
int picon_null_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count);
int picon_null_write(const DEVICE_FILE *devf, const unsigned char *buf, unsigned int count);

#define NULL_DEV \
&(DEVICE)\
{ .init  = picon_null_init,\
  .open  = picon_null_open,\
  .close = picon_null_close,\
  .read  = picon_null_read,\
  .write = picon_null_write,\
  .pread  = NULL,\
  .write = NULL,\
  .ioctl = NULL,\
  .lseek = NULL,\
  .sendto   = NULL,\
  .recvfrom = NULL,\
  .fsync = NULL,\
  .flags = DEV_FLAG_SERIALIZE \
}


#endif /* end of include guard NULL_H */

