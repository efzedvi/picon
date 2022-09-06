/**
 * @author      : faraz (faraz@d620)
 * @file        : ateeprom
 * @created     : Tue 06 Sep 2022 01:15:12 AM EDT
 */

#include <string.h>
#include <errno.h>
#include <sys/types.h>

#include "rtos.h"
#include "picon/utils.h"
#include "picon/ioctl.h"
#include "picon/ateeprom.h"
#include "picon/io.h"
#include "picon/log.h"
#include "picon.h"


typedef struct {
	uint16_t	size;
	uint8_t		page_size;
	uint8_t		addr_len;
} eeprom_info_t;

const eeprom_info_t eeprom_info[] = {
	{ .size = 256,  .page_size = 8,  .addr_len = 1 },	// ATEEPROM_24C02,
	{ .size = 512,  .page_size = 16, .addr_len = 1 },	// ATEEPROM_24C04,
	{ .size = 1024, .page_size = 16, .addr_len = 1 },	// ATEEPROM_24C08,
	{ .size = 2048, .page_size = 16, .addr_len = 1 },	// ATEEPROM_24C16,
	{ .size = 4096, .page_size = 32, .addr_len = 1 },	// ATEEPROM_24C32,
	{ .size = 8192, .page_size = 32, .addr_len = 1 },	// ATEEPROM_24C64,
};


int picon_ateeprom_init(uint8_t ux, void *params)
{
	UNUSED(ux);
	UNUSED(params);

	return 0;
}


const void *picon_ateeprom_open(const DEVICE_FILE *devf, int flags)
{
	UNUSED(devf);
	UNUSED(flags);

	return (const void *) devf;
}


int picon_ateeprom_close(const DEVICE_FILE *devf)
{
	UNUSED(devf);

	return 0;
}


int picon_ateeprom_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data)
{
	UNUSED(devf);
	UNUSED(request);
	UNUSED(data);

	return 0;
}

int picon_ateeprom_pread(const DEVICE_FILE *devf, void *buf, uint16_t count, uint32_t offset)
{
	int 			fd=-1, nread=0;
	PICON_ATEEPROM_CFG	*cfg;
	PICON_I2C_MSG    	msgs[2];
	PICON_IOCTL_I2C  	ioctl_msg;
	uint16_t		to_xfer, off_temp;
	uint8_t			chunk_size=0;


	if (!buf || !devf || !devf->init_params) return -EINVAL;

	if (count == 0) return 0;

	cfg = (PICON_ATEEPROM_CFG *) devf->init_params;

	to_xfer = MIN(count, (eeprom_info[cfg->type].size - offset));

	if (to_xfer == 0) return 0;

	if (offset >= eeprom_info[cfg->type].size) return -EIO;

	fd = open(cfg->device_name, 0);
	if (fd < 0) return -EIO;

	while (to_xfer > 0) {

		chunk_size = MIN(to_xfer, eeprom_info[cfg->type].page_size);

		// descriptor to identify the offset
		msgs[0].flags= 0; // write
		msgs[0].len  = eeprom_info[cfg->type].addr_len; // length for the offset

		if (eeprom_info[cfg->type].addr_len > 1) {
			off_temp = ( (offset & 0xff) << 8 ) | ( offset >> 8);
			msgs[0].buf  = (uint8_t *) &off_temp;
		} else {
			msgs[0].buf  = (uint8_t *) &offset;
		}

		msgs[1].flags = PICON_I2C_M_RD;
		msgs[1].len   = chunk_size;
		msgs[1].buf   = buf+nread;

		if (eeprom_info[cfg->type].addr_len == 1)
			ioctl_msg.addr = cfg->address + (offset / 256);
		else
			ioctl_msg.addr = cfg->address;

		ioctl_msg.nmsgs = 2;
		ioctl_msg.msgs = msgs;

		if (ioctl(fd, PICON_IOC_I2C_XFER, &ioctl_msg) < 0) break;

		nread   += chunk_size;
		to_xfer -= chunk_size;
		offset  += chunk_size;
	}

	close(fd);

	return nread;
}


int picon_ateeprom_pwrite(const DEVICE_FILE *devf, const void *buf, uint16_t count, uint32_t offset)
{
	int 			fd=-1, nwritten=0;
	PICON_ATEEPROM_CFG	*cfg;
	PICON_I2C_MSG    	msgs[1];
	PICON_IOCTL_I2C  	ioctl_msg;
	uint16_t		to_xfer;
	uint8_t			chunk_size=0, addr_len = 0;
	uint8_t			xfer_buf[32+2]; // maximum supported page size + maximum offset length


	if (!buf || !devf || !devf->init_params) return -EINVAL;

	if (count == 0) return 0;

	cfg = (PICON_ATEEPROM_CFG *) devf->init_params;

	to_xfer = MIN(count, (eeprom_info[cfg->type].size - offset));

	if (to_xfer == 0) return 0;

	if (offset >= eeprom_info[cfg->type].size) return -EIO;

	fd = open(cfg->device_name, 0);
	if (fd < 0) return -EIO;

	addr_len = eeprom_info[cfg->type].addr_len;

	while (to_xfer > 0) {

		chunk_size = MIN(to_xfer, eeprom_info[cfg->type].page_size);

		if (addr_len > 1) {
			xfer_buf[0] = offset >> 8;
			xfer_buf[1] = offset & 0xff;
		} else {
			xfer_buf[0] = offset & 0xff;
		}

		memcpy(xfer_buf+addr_len, buf+nwritten, chunk_size);

		// descriptor to identify the offset
		msgs[0].flags= 0; // write
		msgs[0].len  = chunk_size + addr_len;
		msgs[0].buf  = xfer_buf;

		if (addr_len == 1)
			ioctl_msg.addr = cfg->address + (offset / 256);
		else
			ioctl_msg.addr = cfg->address;

		ioctl_msg.nmsgs = 1;
		ioctl_msg.msgs = msgs;

		if (ioctl(fd, PICON_IOC_I2C_XFER, &ioctl_msg) < 0) break;

		nwritten += chunk_size;
		to_xfer  -= chunk_size;
		offset   += chunk_size;
	}

	close(fd);

	return nwritten;
}


