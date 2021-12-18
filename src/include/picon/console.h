/**
 * @author      : faraz 
 * @file        : console
 * @created     : Fri 17 Dec 2021 07:09:59 PM EST
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#include "rtos.h"

#define STDIN_FILENO	(0)
#define STDOUT_FILENO	(1)
#define STDERR_FILENO	(2)

#define CONSOLE_MAX_FD		(4)
#define CONSOLE_MAX_ALLOCS	(4)

typedef struct _console_info {
	int8_t	stdfd[CONSOLE_MAX_FD];
	void	*allocs[CONSOLE_MAX_ALLOCS];
} CONSOLE_INFO;


#define TASK_GET_LOCAL_STORAGE()	(CONSOLE_INFO *)rtos_task_get_thread_local_storage_pointer(NULL, 0)

int console_create(const char *dev, const char *name, rtos_task_function_t console_task, uint16_t stack_size, uint8_t priority, void *args);

int console_create_child(const char *name, rtos_task_function_t console_task, uint16_t stack_size, uint8_t priority, void *args,
			rtos_task_handle_t *task_handle, CONSOLE_INFO *console_info);

//void *malloc(size_t size);
//void free(void *ptr);
void picon_free_all(CONSOLE_INFO *console_info);

int putchar(int ch);
int getchar(void);
int getline(char *buf, unsigned short bufsiz, char *hist[], unsigned char hist_size);

int puts(const char *s);

#endif /* end of include guard CONSOLE_H */
