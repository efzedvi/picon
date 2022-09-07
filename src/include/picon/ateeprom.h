/**
 * @author      : faraz (faraz@d620)
 * @file        : ateeprom
 * @created     : Tue 06 Sep 2022 01:02:44 AM EDT
 */

#ifndef ATEEPROM_H
#define ATEEPROM_H

#include <stdint.h>

#include "picon/ioctl.h"
#include "picon/dev.h"

typedef enum {
	ATEEPROM_24C02,
	ATEEPROM_24C04,
	ATEEPROM_24C08,
	ATEEPROM_24C16,
	ATEEPROM_24C32,
	ATEEPROM_24C64,
} ATEEPROM_TYPE;

typedef struct {
        uint8_t         address;
        ATEEPROM_TYPE	type;
	DEV_NAME	device_name;
} PICON_ATEEPROM_CFG;

int picon_ateeprom_init(uint8_t ux, void *params);

const void *picon_ateeprom_open(const DEVICE_FILE *devf, int flags);
int picon_ateeprom_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data);
int picon_ateeprom_close(const DEVICE_FILE *devf);
int picon_ateeprom_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count);
int picon_ateeprom_write(const DEVICE_FILE *devf, const unsigned char *buf, unsigned int count);
int picon_ateeprom_pread(const DEVICE_FILE *devf, void *buf, uint16_t count, uint32_t offset);
int picon_ateeprom_pwrite(const DEVICE_FILE *devf, const void *buf, uint16_t count, uint32_t offset);

#define ATEEPROM_DEV \
&(DEVICE)\
{ .init  = picon_ateeprom_init,\
  .open  = picon_ateeprom_open,\
  .close = picon_ateeprom_close,\
  .read  = picon_ateeprom_read,\
  .write = picon_ateeprom_write,\
  .pread = picon_ateeprom_pread,\
  .pwrite= picon_ateeprom_pwrite,\
  .ioctl = picon_ateeprom_ioctl,\
  .lseek = NULL,\
  .sendto   = NULL,\
  .recvfrom = NULL,\
  .flags = DEV_FLAG_SERIALIZE | DEV_FLAG_MEMORY \
}


#endif /* end of include guard ATEEPROM_H */

