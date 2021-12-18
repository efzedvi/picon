/**
 * @author      : faraz
 * @file        : dev.h
 * @created     : Fri 17 Dec 2021 07:11:06 PM EST
 */
#ifndef DEV_H
#define DEV_H

#include <stdint.h>
#include <stddef.h>

#define	DEV_NAME_SIZE	(12)

#define DEV_FLAG_SERIALIZE	(1<<0)
#define DEV_FLAG_SOCKET         (1<<1)
#define DEV_FLAG_MEMORY		(1<<2)
#define DEV_FLAG_NONBLOCK	(1<<3)

typedef char            DEV_NAME[DEV_NAME_SIZE+1];

typedef struct _DEV DEVICE;

typedef struct _DEV_ENTRY {
	DEV_NAME	name;
	DEVICE		*dev;
	unsigned char	minor;
	void		*init_params;
} DEVICE_FILE;

#define DEV_ENTRY(nm,ops,m,p) &(DEVICE_FILE) {.name=(nm), .dev=ops, .minor=(m), .init_params=(p)},

typedef struct _DEV {
	int		(*init)(uint8_t ux, void *params);
	const void*	(*open)(const DEVICE_FILE *devf, int flags);
	int		(*close)(const DEVICE_FILE *devf);
	int		(*read)(const DEVICE_FILE *devf, unsigned char *buf, size_t len);
	int		(*write)(const DEVICE_FILE *devf, unsigned char *buf, size_t len);
	int 		(*pread)(const DEVICE_FILE *devf, void *buf, uint16_t count, uint32_t offset);
	int		(*pwrite)(const DEVICE_FILE *devf, const void *buf, uint16_t count, uint32_t offset);
	int		(*ioctl)(const DEVICE_FILE *devf, unsigned int req, void *thing);
	int		(*lseek)(const DEVICE_FILE *devf, unsigned int offset, int whence);
	int		(*sendto)(const DEVICE_FILE *devf, const void *buf, size_t len, int flags,
				  const void *dest_addr, uint8_t addrlen);
	int		(*recvfrom)(const DEVICE_FILE *devf, void *buf, size_t len, int flags,
				    void *src_addr, uint8_t *addrlen);

	uint8_t		flags;
} __attribute__((packed)) DEVICE;

void dev_init(const DEVICE_FILE **df);
unsigned char dev_count(void);
const char *dev_get_name(unsigned char idx);
const DEVICE_FILE *dev_by_idx(unsigned char idx);
const DEVICE_FILE *dev_by_name(const DEV_NAME name);

#endif /* end of include guard DEV_H */
