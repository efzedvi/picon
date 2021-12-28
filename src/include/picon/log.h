/**
 * @author      : faraz (faraz@d620)
 * @file        : log.h
 * @created     : Fri 17 Dec 2021 07:17:23 PM EST
 */
#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include "picon/config.h"
#include "picon/printf.h"

typedef enum {
	ADC_C,
	CONSOLE_C,
	DEV_C,
	I2C_C,
	IO_C,
	LOG_C,
	MAIN_C,
	SHELL_C,
	SHELL_I2C_C,
	UART_C,
	USB_SERIAL_C,
	UTILS_C,

	MAX_SOURCE_FILE,
} LOG_SRC_FILE;

#define LOG_DATA_SIZE           (13)

#ifdef PICON_LOGGING
#define LOG(file, log_fmt, log_arg...)\
	do{\
		log_record(0, file, __LINE__, log_fmt, ##log_arg);\
	} while (0)

#define LOG_FROM_ISR(file, log_fmt, log_arg...)\
	do{\
		log_record(1, file, __LINE__, log_fmt, ##log_arg);\
	} while (0)
#else       
#define LOG(file, log_fmt, log_arg...)
#define LOG_FROM_ISR(file, log_fmt, log_arg...)
#endif

void log_init(uint16_t size);
void log_shutdown(void);
void log_status(uint16_t *size, uint8_t *capturing);
void log_capture(uint8_t onoff);
void log_record(uint8_t from_isr, LOG_SRC_FILE file, uint16_t line, const char* fmt, ...);
void log_display(uint8_t num);
void log_clear(void);

#endif /* end of include guard LOG_H */
