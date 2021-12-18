/**
 * @author      : faraz (faraz@opti780)
 * @file        : printf
 * @created     : Friday Feb 14, 2020 15:40:19 EST
 */

#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>

int printf(const char *format, ...);
int sprintf(char *out, const char *format, ...);
int snprintf( char *buf, unsigned int count, const char *format, ... );
int vprint( char **out, const char *format, va_list args);

#endif /* end of include guard PRINTF_H */
