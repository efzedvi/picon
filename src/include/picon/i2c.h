/**
 * @author      : faraz
 * @file        : i2c.h
 * @created     : Wed 05 Jan 2022 06:16:46 PM EST
 */

#ifndef I2C_H
#define I2C_H

#include "picon/dev.h"

#define PICON_I2C_STANDARD_100K		(100000)
#define PICON_I2C_FAST_400K		(400000)

typedef struct _uart_cfg_t {
	uint32_t	speed;
	int8_t		scl;
	int8_t		sda;
} i2c_cfg_t;


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
