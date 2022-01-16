/**
 * @author      : faraz
 * @file        : ioctl.h
 * @created     : Fri 17 Dec 2021 07:12:05 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PICON_IOCTL_H
#define PICON_IOCTL_H

#include "utils.h"

typedef void (*user_isr_func)(void);

typedef enum {
	PICON_IOC_GET_DEV_FLAGS,

	//UART
	PICON_IOC_UART_SET_BPS,

	//GPIO

	//ADC DAC

	// to handle control-c
	PICON_IOC_TTY_SET_INT,
	PICON_IOC_TTY_GET_INT,

	// SPI

	//I2C
	PICON_IOC_I2C_XFER,

	//Timer

	//ASK

	//Flash
	PICON_IOC_STORAGE_READ,
	PICON_IOC_STORAGE_ERASE,
	PICON_IOC_STORAGE_WRITE,
	PICON_IOC_STORAGE_ERASE_WRITE,
	PICON_IOC_STORAGE_GET_SIZE,
	PICON_IOC_STORAGE_NUM_SECTORS,

	PICON_IOC_MAX,
} PICON_IOCTL_REQ;


// I2C flags
#define PICON_I2C_M_RD		(1 << 0)

#define PICON_I2C_IOCTL_MAX_MSGS	(4)

typedef enum {
	PICON_I2C_SPEED_SM_100K,
	PICON_I2C_SPEED_FM_400K,
	PICON_I2C_SPEED_FMP_1M,
	PICON_I2C_SPEED_UNKNOWN,
} PICON_I2C_SPEED;

typedef struct _picon_i2c_msg {
	uint16_t	flags;
	uint16_t	len;
	uint8_t		*buf;
} PICON_I2C_MSG;

typedef struct _picon_ioctl_i2c {
	uint8_t		addr;
	PICON_I2C_MSG	*msgs;
	uint8_t		nmsgs;
} PICON_IOCTL_I2C;

typedef struct _picon_ioctl_storage {
	uint8_t		*buf;
	uint32_t	offset;
	uint16_t	count;		// how many bytes to read/write
	uint16_t	xfer_count;	// how many bytes read/written
} PICON_IOCTL_STORAGE;

#endif /* end of include guard GPIO_H */
