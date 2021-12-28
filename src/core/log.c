/**
 * @author      : faraz
 * @file        : log
 * @created     : Fri 17 Dec 2021 07:26:37 PM EST
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "rtos.h"
#include "picon/log.h"
#include "picon/printf.h"


typedef struct
{
  uint8_t       file_id;
  uint16_t      line;
  char          data[LOG_DATA_SIZE];
} __attribute((__packed__)) LOG_REC;

#define LOG_REC_SIZE            sizeof(LOG_REC)
#define LOG_DEFAULT_SIZE	(256)


rtos_semaphore_handle_t log_mtx = NULL;
uint8_t			log_enabled = 0;
LOG_REC			*log_buf = NULL;
uint16_t			log_idx = 0;
uint16_t		log_size = 0;
uint16_t			log_count = 0;

void log_init(uint16_t size)
{
	size = (!size) ? LOG_DEFAULT_SIZE : size;

	if (!log_mtx)
		log_mtx = rtos_semaphore_create_mutex();

	log_clear();
	if (log_buf) return;

	log_buf = (LOG_REC*) rtos_port_malloc(size * LOG_REC_SIZE);

	log_size = (log_buf) ? size : 0;
}

void log_shutdown()
{
	if (!log_buf) return;

	rtos_port_free(log_buf);
	log_buf = NULL;
	log_size = 0;
	log_clear();
}

void log_capture(uint8_t onoff)
{
	if (log_buf && log_size)
		log_enabled = onoff;
	else
		log_enabled = 0;
}

void log_record(uint8_t from_isr, LOG_SRC_FILE file, uint16_t line, const char* fmt, ...)
{
	LOG_REC		rec;
	va_list 	args;
	char		*s;

	if (!log_enabled || !log_buf) return;

	rec.file_id = file;
	rec.line    = line;
	s	    = rec.data;

	va_start(args, fmt);
	vprint(&s, fmt, args);
	va_end(args);

	// no scheduler? then don't bother
	if (rtos_task_get_scheduler_state() == RTOS_TASK_SCHEDULER_RUNNING) {
		if (from_isr) {
			rtos_semaphore_take_from_isr(log_mtx, NULL);
		} else {
			rtos_semaphore_take(log_mtx, RTOS_PORT_MAX_DELAY);
		}
	}

	log_buf[ log_idx ] = rec;

	log_idx = (log_idx + 1) % log_size;
	log_count = (++log_count < log_size) ? log_count : log_size;

	if (rtos_task_get_scheduler_state() == RTOS_TASK_SCHEDULER_RUNNING) {
		if (from_isr) {
			rtos_semaphore_give_from_isr(log_mtx, NULL);
		} else {
			rtos_semaphore_give(log_mtx);
		}
	}
}

void log_display(uint8_t num)
{
	LOG_REC		rec;
	uint8_t		i,j = 0;

	if (!num || num > log_count) num = log_count;

	if (log_idx > 0)
		i = log_idx - 1;
	else
		i = 0;

	if (log_count < log_size)
		i = 0;

	while(num--)  {
		rec = log_buf[i];
		printf("%03d: %03d:%04d: %s\n", j++, rec.file_id, rec.line, rec.data);
		i=(i + 1) % log_size;
	}
}

void log_clear(void)
{
	log_idx = 0;
	log_count = 0;
}

void log_status(uint16_t *size, uint8_t *capturing)
{
	if (!capturing || !size) return;

	*size = log_size;
	*capturing = log_enabled;
}

