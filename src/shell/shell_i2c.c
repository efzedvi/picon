/**
 * @author      : faraz
 * @file        : shell_i2c.c
 * @created     : Wed 05 Jan 2022 10:48:25 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "picon/config.h"
#include "picon/console.h"
#include "picon/printf.h"
#include "picon/io.h"
#include "picon/ioctl.h"
#include "picon/utils.h"
#include "picon.h"

#include "shell/shell.h"
#include "shell/shell_i2c.h"

// if this size is to be increased make sure there is enough stack
// space for the shell, this buffer is allocated on the stack
#define  MAX_XFER_SIZE		(128)

static void shell_i2c_usage(void)
{
	printf(
		"Usage: <device> [ scan | <addr> <msg>]\n"
		"msg: DESC [DATA] [DESC [DATA]]...\n"
		"DESC: {r|w}LENGTH, DATA : hex[,hex...]\n"
	      );
}

static int parse_desc(char *str, PICON_I2C_MSG *msg)
{
	char	*ptr=str, *end;

	if (!str || !msg) return -1;

	switch (*ptr) {
		case 'r':
			msg->flags = PICON_I2C_M_RD;
			break;
		case 'w':
			msg->flags = 0;
			break;
		default:
			return -2;
	};

	ptr++;
	if (!*ptr) return -3;

	msg->len = (uint16_t) strtol(ptr, &end, 10);

	if (*end != '\0') return -6;

	return 0;
}

#define GET_DESC 	(0)
#define GET_DATA	(1)

static int shell_i2c_xfer(int fd, int argc, char *argv[])
{
	int		rc;
	PICON_I2C_MSG	msgs[PICON_I2C_IOCTL_MAX_MSGS];
	PICON_IOCTL_I2C	ioctl_msg;
	uint8_t		buf[MAX_XFER_SIZE];
	uint16_t	i, j,offset=0;
	uint8_t		mode, n, addr=0;


	addr = (uint8_t) strtol(argv[2], NULL, 16);
	if (!addr) {
		printf("Invalid address\n");
		return -5;
	}

	mode = GET_DESC;
	n = 0;
	for(i=3; i<argc; i++) {

		if (n >= PICON_I2C_IOCTL_MAX_MSGS) {
			printf("too many messages\n");
			return -5;
		}

		if (mode == GET_DESC) {
			rc = parse_desc(argv[i], &msgs[n]);
			if (rc < 0) {
				printf("message descriptor error\n");
				return rc;
			}

			msgs[n].buf = NULL;
			if (offset + msgs[n].len > sizeof(buf)) {
				printf("transfer size too big\n");
				return -5;
			}


			if (msgs[n].flags == PICON_I2C_M_RD) {
				memset(buf+offset, 0, msgs[n].len);
				msgs[n].buf = buf+offset;

				offset += msgs[n].len;
				n++;
			} else {
				// we need data for writing
				mode = GET_DATA;
			}
		} else {
			rc = shell_parse_data(argv[i], msgs[n].len, buf+offset, sizeof(buf)-offset);
			if (rc < 0) {
				printf("data format error\n");
				return rc;
			}
			msgs[n].buf = buf+offset;
			offset += msgs[n].len;
			mode = GET_DESC;
			n++;
		}
	}

	ioctl_msg.addr = addr;
	ioctl_msg.nmsgs = n;
	ioctl_msg.msgs = msgs;

	rc = ioctl(fd, PICON_IOC_I2C_XFER, &ioctl_msg);
	if (rc == -EINVAL) {
		printf("ioctl failed\n");
		return -10;
	}

	if (rc == -ECONNREFUSED || rc == -ETIMEDOUT) {
		printf("Connection failed\n");
		return -11;
	}

	for(i=0; i < n; i++) {

		if (msgs[i].flags != PICON_I2C_M_RD) continue;

		for(j=0; j < msgs[i].len; j++) {
			if (j % 16 == 0) printf("%d,%02x:", i, j);
			printf(" %02x", msgs[i].buf[j]);
			if (j % 16 == 15) printf("\n");
		}
		printf("\n");
	}

	return 0;
}

static int shell_i2c_scan(int fd)
{
	int		rc;
	uint8_t		addr, byte=0;
	PICON_I2C_MSG	msg;
	PICON_IOCTL_I2C	ioctl_msg;

	for(addr=0; addr < 128; addr++) {

		if (addr % 16 == 0)
			printf("%02x: ", addr);

		// skip over reserved addresses
		if (addr < 0x03 || addr > 0x77) {
			printf(" NA");
			continue;
		}

		if ( (addr >= 0x30 && addr <= 0x37) ||
		     (addr >= 0x50 && addr <= 0x5F)) {
			// try to read
			msg.flags = PICON_I2C_M_RD;
		} else {
			// try to write
			msg.flags = 0;
		}

		msg.len   = 1;
		msg.buf   = &byte;

		ioctl_msg.addr = addr;
		ioctl_msg.nmsgs = 1;
		ioctl_msg.msgs = &msg;

		rc = ioctl(fd, PICON_IOC_I2C_XFER, &ioctl_msg);
		if (rc == -EINVAL) {
			printf("ioctl failed\n");
			return -1;
		}

		if (rc < 0) {
			printf(" --");
		} else {
			printf(" %02x", addr);
		}

		if (addr % 16 == 15) printf("\n");
	}

	printf("\n");

	return 0;
}


int shell_i2c(int argc, char *argv[])
{
	int		fd, rc;

	if (argc < 3) {
		shell_i2c_usage();
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Failed to open %s\n", argv[1]);
		return -2;
	}

	if (argc > 3) {
		rc = shell_i2c_xfer(fd, argc, argv);
	} else if (strcmp(argv[2], "scan") == 0) {
		rc = shell_i2c_scan(fd);
	} else {
		shell_i2c_usage();
		rc = -1;
	}

	close(fd);

	return rc;
}



