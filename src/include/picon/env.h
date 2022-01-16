/**
 * @author      : faraz
 * @file        : env
 * @created     : Friday Jan 28, 2022 22:53:54 EST
 */
#ifndef ENV_H
#define ENV_H

#include <stdint.h>

#define ENV_KEY_SIZE	(16)
#define ENV_VALUE_SIZE	(64)

void env_init(void);

int env_set(const char *key, const char *value);

int env_get(const char *key, char *buf, uint8_t size);

uint8_t env_count(void);

int env_get_by_id(uint8_t id, char *key, uint8_t key_size, char *value, uint8_t value_size);

void env_zap(void);

#endif /* end of include guard ENV_H */
