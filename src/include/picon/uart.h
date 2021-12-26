/**
 * @author      : faraz
 * @file        : uart.h
 * @created     : Tue 21 Dec 2021 05:07:49 PM EST
 */
#ifndef UART_H
#define UART_H

#include "picon/dev.h"

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

