/**
 * @author      : faraz
 * @file        : uart.h
 * @created     : Tue 21 Dec 2021 05:07:49 PM EST
 */
#ifndef UART_H
#define UART_H

#include "picon/dev.h"
#include "hardware/uart.h"

typedef struct _uart_cfg_t {
	uint32_t		baud;
	int8_t			data_bits;
	int8_t			stop_bits;
	int8_t			parity;
	int8_t			rx;
	int8_t			tx;
	int8_t			cts;
	int8_t			rts;
} uart_cfg_t;

#define PICON_DEFAULT_BAUD_RATE 	115200
#define PICON_DEFAULT_DATA_BITS 	8
#define PICON_DEFAULT_PARITY    	UART_PARITY_NONE
#define PICON_DEFAULT_STOP_BITS 	1

#define PICON_DEFAULT_UART_TX_PIN	0
#define PICON_DEFAULT_UART_RX_PIN	1


int picon_uart_init(uint8_t ux, void *params);

const void *picon_uart_open(const DEVICE_FILE *devf, int flags);
int picon_uart_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count);
int picon_uart_write(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count);
int picon_uart_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data);
int picon_uart_close(const DEVICE_FILE *devf);
int picon_uart_fsync(const DEVICE_FILE *devf);

#define UART_DEV \
&(DEVICE)\
{ .init  = picon_uart_init,\
  .open  = picon_uart_open,\
  .close = picon_uart_close,\
  .read  = picon_uart_read,\
  .write = picon_uart_write,\
  .pread = NULL,\
  .pwrite= NULL,\
  .ioctl = picon_uart_ioctl,\
  .lseek = NULL,\
  .sendto   = NULL,\
  .recvfrom = NULL,\
  .fsync= picon_uart_fsync,\
  .flags = DEV_FLAG_SERIALIZE \
}

#endif /* end of include guard UART_H */

