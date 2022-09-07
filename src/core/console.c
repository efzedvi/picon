/**
 * @author      : faraz
 * @file        : console.c
 * @created     : Fri 17 Dec 2021 07:23:56 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "picon/utils.h"
#include "picon/console.h"
#include "picon/io.h"
#include "picon/dev.h"
#include "picon/log.h"
#include "rtos.h"

#define CONSOLE_MAX		(5)

#define GETCHAR_DELAY		(40)

void *malloc(size_t size)
{
	CONSOLE_INFO	*console_info;
	int i;
	void *ptr = NULL;

	console_info = TASK_GET_LOCAL_STORAGE();

	if (!console_info) {
		ptr = rtos_port_malloc(size);
		return ptr;
	}

	for(i=0; i<CONSOLE_MAX_ALLOCS; i++) {
		if (console_info->allocs[i] == NULL) {
			ptr = rtos_port_malloc(size);
			if (!ptr) return NULL;

			console_info->allocs[i] = ptr; // remember our memory allocations
			return ptr;
		}
	}

	return NULL;
}

void free(void *ptr)
{
	CONSOLE_INFO	*console_info;
	int i;

	if (!ptr) return;

	console_info = TASK_GET_LOCAL_STORAGE();

	if (!console_info) {
		rtos_port_free(ptr);
		return;
	}

	for(i=0; i<CONSOLE_MAX_ALLOCS; i++) {
		if (console_info->allocs[i]==ptr) {
			rtos_port_free(ptr);
			console_info->allocs[i] = NULL;
			return;
		}
	}
}

void picon_free_all(CONSOLE_INFO *console_info)
{
	int i;

	if (!console_info) return;

	for(i=0; i<CONSOLE_MAX_ALLOCS; i++) {
		if (console_info->allocs[i] != NULL) {
			rtos_port_free(console_info->allocs[i]);
			console_info->allocs[i] = NULL;
		}
	}
}


int console_create(const char *dev, const char *name, rtos_task_function_t console_task, uint16_t stack_size, uint8_t priority, void *args)
{
	int			i, console_fd = -1;
	rtos_base_type_t 	rv = 0;
	rtos_task_handle_t	task_handle = NULL;
	CONSOLE_INFO	*console_info;

	if (!dev) return -ENOMEM;

	console_fd = open(dev, PICON_IO_NONBLOCK | O_RDWR);
	if (console_fd < 0) return -EINVAL;

	console_info = (CONSOLE_INFO *) rtos_port_malloc(sizeof(CONSOLE_INFO));
	if (console_info == NULL) return -ENOMEM;

	console_info->stdfd[STDIN_FILENO]  = console_fd;
	console_info->stdfd[STDOUT_FILENO] = console_fd;
	console_info->stdfd[STDERR_FILENO] = console_fd;

	for(i=0; i<CONSOLE_MAX_ALLOCS; i++) {
		console_info->allocs[i] = NULL;
	}

	rv = rtos_task_create(console_task, name, stack_size, args, priority, &task_handle);
	if (rv != RTOS_PASS || task_handle == NULL) {
		rtos_port_free(console_info);
		return -ENOMEM;
	}

	rtos_task_set_thread_local_storage_pointer(task_handle, 0, (void *) console_info);

	return 0;
}


int console_create_child(const char *name, rtos_task_function_t console_task, uint16_t stack_size, uint8_t priority, void *args,
			rtos_task_handle_t *task_handle, CONSOLE_INFO *console_info)
{
	int			i;
	rtos_base_type_t 	rv = 0;
	CONSOLE_INFO		*parent_info;

	if (!task_handle || !console_info) return -EINVAL;

	parent_info = TASK_GET_LOCAL_STORAGE();
	if (!parent_info) return -EINVAL;

	console_info->stdfd[STDIN_FILENO]  = parent_info->stdfd[STDIN_FILENO];
	console_info->stdfd[STDOUT_FILENO] = parent_info->stdfd[STDOUT_FILENO];
	console_info->stdfd[STDERR_FILENO] = parent_info->stdfd[STDERR_FILENO];

	for(i=0; i<CONSOLE_MAX_ALLOCS; i++) {
		console_info->allocs[i] = NULL;
	}

	rv = rtos_task_create(console_task, name, stack_size, args, priority, task_handle);
	if (rv != RTOS_PASS || *task_handle == NULL) {
		return -ENOMEM;
	}

	rtos_task_set_thread_local_storage_pointer(*task_handle, 0, (void *) console_info);

	return 0;
}

int putchar(int ch)
{
	unsigned char c = (char) ch;
	CONSOLE_INFO  *console_info;
	int stdout_fd = -1;

	console_info = TASK_GET_LOCAL_STORAGE();

	if (!console_info || console_info->stdfd[STDOUT_FILENO] < 0) return -EINVAL;

	stdout_fd = console_info->stdfd[STDOUT_FILENO];

	if (c == '\n') {
		c = '\r';
		write(stdout_fd, &c, 1);
		c = '\n';
	}

	return write(stdout_fd, &c, 1);
}

int getchar(void)
{
	char ch = -1;
	CONSOLE_INFO  *console_info;
	int stdin_fd;

	console_info = TASK_GET_LOCAL_STORAGE();

	if (!console_info || console_info->stdfd[STDOUT_FILENO] < 0) return -EINVAL;

	stdin_fd = console_info->stdfd[STDIN_FILENO];

	if (read(stdin_fd, &ch, 1) == 1) {
		return (int) ch;
	}

	return -1;
}


/*********************************************************************
 * A very simple line editing routine. It supports:
 *
 *	^U	Kill line
 *	^A	Begin line
 *	^B	Backup character
 *	^F	Forward character
 *	^E	End line
 *	^H	Backspace
 *	^I	Insert a blank
 *	^D or rubout
 *		Delete char
 *
 * The returned line is NOT terminated with '\n' like fgets(), but 
 * rather works like the old gets().
 *
 * Returns the number of characters returned in buf.
 *
 *********************************************************************/

#include "picon/printf.h"


int getline(char *buf, unsigned short bufsiz, char *hist[], unsigned char hist_size)
{
	int ch = 0, esc=0;
	unsigned short bufx = 0, buflen = 0, hx=0;
	int i=0;

	if ( bufsiz <= 1 ) return -1;

	--bufsiz;		// Leave room for nul byte

	while ( ch != '\n' ) {

		ch = getchar();

		if (ch <= 0) {
			rtos_task_delay(rtos_ms_to_ticks(GETCHAR_DELAY));
			continue;
		}
		if (ch > 127)
			ch &= 0x7F; // HACK: Pico sometimes sets the 8th bit for some reasons!

		if ( ch == 27) { // esc
			esc = 1;
			continue;
		}
		if ( ch == '[' && esc==1) {
			esc = 2;
			continue;
		}
		if (esc == 2) {
			switch (ch) {
				case 'D': // LEFT arrow
					ch = CONTROL('B');
					break;
				case 'A': // UP arrow
					if (hist && hist_size && hx < hist_size) {
						for(i=bufx; i>0; i--)
							putchar('\b');
						for(i=0; i<buflen; i++)
							putchar(' ');
						for(i=buflen; i>0; i--)
							putchar('\b');

						strncpy(buf, hist[hx], bufsiz);
						buflen = strlen(buf);
						bufx = buflen;
						hx++;

						for(i=0; i<buflen; i++)
							putchar(buf[i]);
					}
					continue;
					break;
				case 'C': // RIGHT arrow
					ch = CONTROL('F');
					break;
				case 'B': // DOWN arrow
					if (!hist || !hist_size) continue;
					for(i=bufx; i>0; i--)
						putchar('\b');
					for(i=0; i<buflen; i++)
						putchar(' ');
					for(i=buflen; i>0; i--)
						putchar('\b');

					if (hx > 1) {
						strncpy(buf, hist[--hx], bufsiz);
						buflen = strlen(buf);
						bufx = buflen;

						for(i=0; i<buflen; i++)
							putchar(buf[i]);
					} else {
						memset(buf, 0, bufsiz);
						bufx = buflen = 0;
						hx = 0;
					}
					continue;
					break;
			}
		}
		esc = 0;

		switch ( ch ) {
			case CONTROL('U'):	// Kill line
				for ( ; bufx > 0; --bufx )
					putchar('\b');

				for ( ; bufx < buflen; ++bufx )
					putchar(' ');

				buflen = 0;
				// Fall thru
			case CONTROL('A'):	// Begin line
				for ( ; bufx > 0; --bufx )
					putchar('\b');
				break;
			case CONTROL('B'):	// Backward char
				if ( bufx > 0 ) {
					--bufx;
					putchar('\b');
				}
				break;
			case CONTROL('F'):	// Forward char
				if ( bufx < bufsiz && bufx < buflen )
					putchar(buf[bufx++]);
				break;
			case CONTROL('E'):	// End line
				for ( ; bufx < buflen; ++bufx )
					putchar(buf[bufx]);
				break;
			case CONTROL('H'):	// Backspace char
			case 0x7F:		// Rubout
				if ( bufx <= 0 )
					break;
				--bufx;
				putchar('\b');
				// Fall thru
			case CONTROL('D'):	// Delete char
				if ( bufx < buflen ) {
					memmove(buf+bufx,buf+bufx+1,buflen-bufx-1);
					--buflen;
					for ( unsigned x=bufx; x<buflen; ++x )
						putchar(buf[x]);
					putchar(' ');
					for ( unsigned x=buflen+1; x>bufx; --x )
						putchar('\b');
				}
				break;
			case CONTROL('I'):	// Insert characters (TAB)
				if ( bufx < buflen && (buflen + 1) < bufsiz ) {
					memmove(buf+bufx+1,buf+bufx,buflen-bufx);
					buf[bufx] = ' ';
					++buflen;
					putchar(' ');
					for ( unsigned x=bufx+1; x<buflen; ++x )
						putchar(buf[x]);
					for ( unsigned x=bufx; x<buflen; ++x )
						putchar('\b');
				}
				break;
			case '\r':
			case '\n':		// End line
				ch = '\n';
				break;
			default:		// Overtype
				if ( bufx >= bufsiz ) {
					putchar(0x07);	// Bell
					continue;	// No room left
				}
				/*
				buf[bufx++] = ch;
				putchar(ch);

				if ( bufx > buflen )
					buflen = bufx;
				*/

				if (bufx == buflen) {
					buf[bufx++] = ch;
					putchar(ch);

					if ( bufx > buflen )
						buflen = bufx;

				} else if ( bufx<buflen && buflen + 1 < bufsiz ) {
					memmove(buf+bufx+1,buf+bufx,buflen-bufx);
					buf[bufx] = ch;
					++buflen;
					putchar(ch);
					for ( unsigned short x=bufx+1; x<buflen; ++x )
						putchar(buf[x]);
					for ( unsigned short x=buflen; x>bufx+1; x-- )
						putchar('\b');
					bufx++;
				}
		}

		if ( bufx > buflen )
			buflen = bufx;
	}

	buf[buflen] = 0;
	putchar('\n');

	if (hist_size && buflen) { // save history
		for(i=hist_size-1; i>0; i--)
			strncpy(hist[i], hist[i-1], bufsiz);
		strncpy(hist[0], buf, bufsiz);
	}

	return bufx;
}


int puts(const char *s)
{
	//if (!s) return -1;

	for(; *s; s++)
		putchar(*s);

	putchar('\n');

	return 0;
}

// End getline
