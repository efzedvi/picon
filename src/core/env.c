/**
 * @author      : faraz
 * @file        : env
 * @created     : Friday Jan 28, 2022 22:52:51 EST
 */

#include <stdint.h>
#include <string.h>

#include "picon/flash.h"
#include "picon/ioctl.h"
#include "picon/log.h"
#include "picon/io.h"
#include "picon/utils.h"
#include "picon.h"
#include "picon/env.h"

#include "hardware/flash.h"

// NOTE: This is a very simplistic implementation of system paramters
//       This module does not make efficient use of memory or flash space.
//       I need to refactor this later.
// TODO: refactor this module

#define ENV_ROW_SIZE	(ENV_KEY_SIZE + ENV_VALUE_SIZE)

#define ENV_HEADER_SIZE		(FLASH_SECTOR_SIZE % ENV_ROW_SIZE)
#define ENV_NUM_ROWS		(FLASH_SECTOR_SIZE / ENV_ROW_SIZE)

// env sector is the last sector
#define ENV_SECTOR	(PICON_NUM_SECTORS - 1)

#define ENV_SIGNATURE	0x467a5623

#define FLASH_DEVICE	"/dev/flash"

typedef char	env_key_t[ENV_KEY_SIZE];
typedef char	env_value_t[ENV_VALUE_SIZE];

typedef struct _env_raw {
	env_key_t	key;
	env_value_t	value;
} env_row_t;

typedef struct _env_block {
	uint8_t		header[ENV_HEADER_SIZE];
	env_row_t	row[ENV_NUM_ROWS];
} env_block_t;


static rtos_semaphore_handle_t  env_mtx;
static env_block_t		env_block;
static uint8_t			env_cnt;

static int fd = -1;

static int env_save(void)
{
	int	rc=0;
	PICON_IOCTL_STORAGE storage;

	if (fd < 0) return -1;

	storage.buf 	= (uint8_t *) &env_block;
	storage.offset	= ENV_SECTOR * FLASH_SECTOR_SIZE;
	storage.count   = FLASH_SECTOR_SIZE;
	storage.xfer_count = 0;

	//rc = ioctl(fd, PICON_IOC_STORAGE_ERASE_WRITE, (void *) &storage);

	rc = pwrite(fd, &env_block, FLASH_SECTOR_SIZE, ENV_SECTOR * FLASH_SECTOR_SIZE);
	if (rc != FLASH_SECTOR_SIZE) return -2;

	return 0;
}

static void _env_zap(uint8_t save)
{
	uint32_t	sig;

	memset(&env_block, 0, sizeof(env_block));
	sig = ENV_SIGNATURE;
	memcpy(env_block.header, &sig, sizeof(sig));
	if (save)
		env_save();
}


static int env_load(void)
{
	int		rc = 0;
	PICON_IOCTL_STORAGE storage;

	if (fd < 0) return -1;

	// if it's loaded already then just return
	if ( *(uint32_t *) env_block.header == ENV_SIGNATURE ) return 0;

	storage.buf 	= (uint8_t *) &env_block;
	storage.offset	= ENV_SECTOR * FLASH_SECTOR_SIZE;
	storage.count   = FLASH_SECTOR_SIZE;
	storage.xfer_count = 0;

	//rc = ioctl(fd, PICON_IOC_STORAGE_READ, (void *) &storage);
	//if (rc < 0) return rc;
	rc = pread(fd, &env_block, FLASH_SECTOR_SIZE, ENV_SECTOR * FLASH_SECTOR_SIZE);
	if (rc != FLASH_SECTOR_SIZE) return -2;

	if ( *(uint32_t *) env_block.header != ENV_SIGNATURE ) {
		_env_zap(0);
	}

	return 0;
}

void env_init(void)
{
	memset(&env_block, 0, sizeof(env_block));

	fd = open(FLASH_DEVICE, 0);
	if (fd < 0) return;

	env_mtx = rtos_semaphore_create_mutex();
}

void env_zap(void)
{
	rtos_semaphore_take(env_mtx, RTOS_PORT_MAX_DELAY);

	_env_zap(1);

	rtos_semaphore_give(env_mtx);
}

void env_erase(void)
{
	int	rc=0;
	PICON_IOCTL_STORAGE storage;

	if (fd < 0) return;

	storage.buf 	= NULL;
	storage.offset	= ENV_SECTOR * FLASH_SECTOR_SIZE;
	storage.count   = FLASH_SECTOR_SIZE;
	storage.xfer_count = 0;

	ioctl(fd, PICON_IOC_STORAGE_ERASE, (void *) &storage);
}

int env_set(const char *key, const char *value)
{
	uint8_t	i, found=0, changed=0;
	int	rc=0;

	if (!key) return -1;

	rtos_semaphore_take(env_mtx, RTOS_PORT_MAX_DELAY);

	env_load();

	for(i=0; i<ENV_NUM_ROWS; i++) {

		if (env_block.row[i].key[0]) {
			if (strncmp(env_block.row[i].key, key, sizeof(env_key_t)) == 0) {
				// found the key
				if (value) {
					if (strncmp(env_block.row[i].value, value, sizeof(env_value_t)) != 0) {
						// change the value
						strncpy(env_block.row[i].value, value, sizeof(env_value_t));
						changed = 1;
					}
				} else {
					// value is NULL delete the key
					if (i < ENV_NUM_ROWS - 1) {
						memcpy(env_block.row+i, env_block.row+i+1, (ENV_NUM_ROWS-i-1) * sizeof(env_row_t));
					}
					// clear the last row
					memset(env_block.row+ENV_NUM_ROWS-1, 0, sizeof(env_row_t));
					changed = 1;
				}
				found = 1;
				break;
			}
		} else {
			break;
		}
	}

	if (!found) {
		if (i<ENV_NUM_ROWS) {
			// key not found, add it
			strncpy(env_block.row[i].key, key, sizeof(env_key_t));
			strncpy(env_block.row[i].value, value, sizeof(env_value_t));
			changed = 1;
		} else {
			rc = -2;
		}
	}

	if (changed)
		rc = env_save();

	rtos_semaphore_give(env_mtx);

	return rc;
}

int env_get(const char *key, char *buf, uint8_t size)
{
	int	i, rc=-2;

	if (!key || !buf) return -1;

	rtos_semaphore_take(env_mtx, RTOS_PORT_MAX_DELAY);

	env_load();

	for(i=0; i<ENV_NUM_ROWS; i++) {
		if (strncmp(env_block.row[i].key, key, sizeof(env_key_t)) == 0) {
			// found the key
			strncpy(buf, env_block.row[i].value, MIN(sizeof(env_value_t), size) );
			rc = 0;
			break;
		}
	}

	rtos_semaphore_give(env_mtx);

	return rc;
}

uint8_t env_count()
{
	int	i;
	uint8_t	count=0;

	rtos_semaphore_take(env_mtx, RTOS_PORT_MAX_DELAY);

	env_load();

	for(i=0; i<ENV_NUM_ROWS; i++) {
		if (env_block.row[i].key[0])
			count++;
		else
			break;
	}

	rtos_semaphore_give(env_mtx);

	return count;
}

int env_get_by_id(uint8_t id, char *key, uint8_t key_size, char *value, uint8_t value_size)
{
	int	rc=-2;

	if (!key || !value || id >= ENV_NUM_ROWS) return -1;

	rtos_semaphore_take(env_mtx, RTOS_PORT_MAX_DELAY);

	env_load();

	if (env_block.row[id].key[0]) {
		strncpy(key, env_block.row[id].key, MIN(sizeof(env_key_t), key_size) );
		strncpy(value, env_block.row[id].value, MIN(sizeof(env_value_t), value_size) );
		rc = 0;
	}

	rtos_semaphore_give(env_mtx);

	return rc;
}


