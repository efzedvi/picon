/**
 * @author      : faraz
 * @file        : io
 * @created     : Fri 17 Dec 2021 07:11:20 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IO_H
#define IO_H

#include <stdint.h>


#ifndef O_RDONLY
#define O_RDONLY	1
#endif

#ifndef O_WRONLY
#define O_WRONLY	2
#endif

#ifndef O_RDWR
#define O_RDWR		3
#endif

#define PICON_IO_NONBLOCK	4

void io_init(void);

int open(const char *name, int flags);
int read(int fd, const void *buf, unsigned int count);
int write(int fd, const void *buf, unsigned int count);
int pread(int fd, void *buf, uint16_t count, uint32_t offset);
int pwrite(int fd, const void *buf, uint16_t count, uint32_t offset);
int lseek(int fd, uint32_t offset, int whence);	
int ioctl(int fd, unsigned int request, void *data);

int sendto(int fd, const void *buf, size_t len, int flags,
		   const void *dest_addr, uint8_t addrlen);

int recvfrom(int fd, void *buf, size_t len, int flags,
		     void *src_addr, uint8_t *addrlen);

int fsync(int fd);

int close(int fd);

#endif /* end of include guard IO_H */
