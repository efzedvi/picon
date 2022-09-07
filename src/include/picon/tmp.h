/**
 * @author      : faraz
 * @file        : tmp
 * @created     : Tue 06 Sep 2022 09:04:51 PM EDT
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TMP_H
#define TMP_H

#include "picon/ioctl.h"
#include "picon/dev.h"


int picon_tmp_init(uint8_t ux, void *params);

const void *picon_tmp_open(const DEVICE_FILE *devf, int flags);
int picon_tmp_close(const DEVICE_FILE *devf);
int picon_tmp_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count);
int picon_tmp_write(const DEVICE_FILE *devf, const unsigned char *buf, unsigned int count);
int picon_tmp_pread(const DEVICE_FILE *devf, void *buf, uint16_t count, uint32_t offset);
int picon_tmp_pwrite(const DEVICE_FILE *devf, const void *buf, uint16_t count, uint32_t offset);

#define TMP_DEV \
&(DEVICE)\
{ .init  = picon_tmp_init,\
  .open  = picon_tmp_open,\
  .close = picon_tmp_close,\
  .read  = picon_tmp_read,\
  .write = picon_tmp_write,\
  .pread = picon_tmp_pread,\
  .pwrite= picon_tmp_pwrite,\
  .ioctl = NULL,\
  .lseek = NULL,\
  .sendto   = NULL,\
  .recvfrom = NULL,\
  .fsync = NULL,\
  .flags = DEV_FLAG_SERIALIZE | DEV_FLAG_MEMORY \
}

#endif /* end of include guard TMP_H */
