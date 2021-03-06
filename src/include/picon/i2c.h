/**
 * @author      : faraz
 * @file        : i2c.h
 * @created     : Wed 05 Jan 2022 06:16:46 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef I2C_H
#define I2C_H

#include "picon/dev.h"

#define PICON_I2C_STANDARD_100K		(100000)
#define PICON_I2C_FAST_400K		(400000)

typedef struct _i2c_cfg_t {
	uint32_t	speed;
	int8_t		scl;
	int8_t		sda;
} i2c_cfg_t;

// Addresses of the form 000 0xxx or 111 1xxx are reserved. No slave should
// have these addresses.
#define I2C_RESERVED_ADDR(addr) 	(((addr) & 0x78) == 0 || ((addr) & 0x78) == 0x78)

int picon_i2c_init(uint8_t ux, void *params);

const void *picon_i2c_open(const DEVICE_FILE *devf, int flags);
int picon_i2c_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data);
int picon_i2c_close(const DEVICE_FILE *devf);


#define I2C_DEV \
&(DEVICE)\
{ .init  = picon_i2c_init,\
  .open  = picon_i2c_open,\
  .close = picon_i2c_close,\
  .read  = NULL,\
  .write = NULL,\
  .pread = NULL,\
  .pwrite= NULL,\
  .ioctl = picon_i2c_ioctl,\
  .lseek = NULL,\
  .sendto   = NULL,\
  .recvfrom = NULL,\
  .fsync    = NULL,\
  .flags = DEV_FLAG_SERIALIZE \
}


#endif /* end of include guard I2C_H */
