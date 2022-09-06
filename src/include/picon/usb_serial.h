/**
 * @author      : faraz
 * @file        : usb_serial.h
 * @created     : Sun 26 Dec 2021 06:14:20 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef USB_SERIAL_H
#define USB_SERIAL_H

#include "picon/dev.h"

int  picon_usb_serial_init(uint8_t ux, void *params);
int  picon_usb_serial_ready(void);
void picon_usb_serial_yield(void);

int picon_usb_serial_read(const DEVICE_FILE *devf, unsigned char *buf, unsigned int count);
int picon_usb_serial_write(const DEVICE_FILE *devf, const unsigned char *buf, unsigned int count);
int picon_usb_serial_ioctl(const DEVICE_FILE *devf, unsigned int request, void *data);

const void *picon_usb_serial_open(const DEVICE_FILE *devf, int flags);
int picon_usb_serial_close(const DEVICE_FILE *devf);

#define USB_SERIAL_DEV \
&(DEVICE) \
{ .init  = picon_usb_serial_init, \
  .open  = picon_usb_serial_open,\
  .close = picon_usb_serial_close,\
  .read  = picon_usb_serial_read,\
  .write = picon_usb_serial_write,\
  .pread = NULL,\
  .pwrite= NULL,\
  .ioctl = picon_usb_serial_ioctl,\
  .lseek = NULL,\
  .sendto   = NULL,\
  .recvfrom = NULL,\
  .fsync = NULL,\
  .flags = DEV_FLAG_SERIALIZE \
}


#endif /* end of include guard USB_SERIAL_H */
