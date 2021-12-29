/**
 * @author      : faraz
 * @file        : picon
 * @created     : Sat 18 Dec 2021 05:53:59 PM EST
 */

#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "hardware/rtc.h"
#include "pico/types.h"

#include "rtos.h"
#include "picon.h"
#include "picon/utils.h"
#include "picon/printf.h"
#include "picon/console.h"
#include "picon/config.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>


typedef enum _picon_gpio_role {
	TX,	// UART
	RX,
	CTS,
	RTS,
	SDA,	// I2C
	SCL,
	SCK,	// SPI
	CS,
	MOSI,
	MISO,

	A,	// PWM
	B,	// PWM

	GPIN0, //CLOCK
	GPOUT0, 
	GPIN1,
	GPOUT1,
	GPOUT2,
	GPOUT3,

	OVCUR_DET, // USB
	VBUS_DET,
	VBUS_EN,

	ROLE_NULL,
} picon_gpio_role_t;

typedef struct _picon_gpio_def {
	enum gpio_function	func;
	int8_t			idx;
	picon_gpio_role_t	role;
} picon_gpio_def;


//#ifdef RASPBERRYPI_PICO
#define PICON_MAX_GPIO		(29)
#define PICON_MAX_GPIO_FUNC	(9)
//#end

picon_gpio_def	picon_gpios[PICON_MAX_GPIO + 1][PICON_MAX_GPIO_FUNC] = {
	{ { GPIO_FUNC_SPI, 0, MISO }, { GPIO_FUNC_UART, 0, TX }, { GPIO_FUNC_I2C, 0, SDA }, { GPIO_FUNC_PWM, 0, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 0
	
	{ { GPIO_FUNC_SPI, 0, CS }, { GPIO_FUNC_UART, 0, RX }, { GPIO_FUNC_I2C, 0, SCL }, { GPIO_FUNC_PWM, 0, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_DET }, }, // 1
	
	{ { GPIO_FUNC_SPI, 0, SCK }, { GPIO_FUNC_UART, 0, CTS }, { GPIO_FUNC_I2C, 1, SDA }, { GPIO_FUNC_PWM, 1, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 2
	
	{ { GPIO_FUNC_SPI, 0, MOSI }, { GPIO_FUNC_UART, 0, RTS }, { GPIO_FUNC_I2C, 1, SCL }, { GPIO_FUNC_PWM, 1, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 3
	
	{ { GPIO_FUNC_SPI, 0, MISO }, { GPIO_FUNC_UART, 1, TX }, { GPIO_FUNC_I2C, 0, SDA }, { GPIO_FUNC_PWM, 2, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_DET }, }, // 4
	
	{ { GPIO_FUNC_SPI, 0, CS }, { GPIO_FUNC_UART, 1, RX }, { GPIO_FUNC_I2C, 0, SCL }, { GPIO_FUNC_PWM, 2, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 5
	
	{ { GPIO_FUNC_SPI, 0, SCK }, { GPIO_FUNC_UART, 1, CTS }, { GPIO_FUNC_I2C, 1, SDA }, { GPIO_FUNC_PWM, 3, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 6
	
	{ { GPIO_FUNC_SPI, 0, MOSI }, { GPIO_FUNC_UART, 1, RTS }, { GPIO_FUNC_I2C, 1, SCL }, { GPIO_FUNC_PWM, 3, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_DET }, }, // 7

	{ { GPIO_FUNC_SPI, 1, MISO }, { GPIO_FUNC_UART, 1, TX }, { GPIO_FUNC_I2C, 0, SDA }, { GPIO_FUNC_PWM, 4, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 8

	{ { GPIO_FUNC_SPI, 1, CS }, { GPIO_FUNC_UART, 1, RX }, { GPIO_FUNC_I2C, 0, SCL }, { GPIO_FUNC_PWM, 4, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 9
	
	{ { GPIO_FUNC_SPI, 1, SCK }, { GPIO_FUNC_UART, 1, CTS }, { GPIO_FUNC_I2C, 1, SDA }, { GPIO_FUNC_PWM, 5, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_DET }, }, // 10
	
	{ { GPIO_FUNC_SPI, 1, MOSI }, { GPIO_FUNC_UART, 1, RTS }, { GPIO_FUNC_I2C, 1, SCL }, { GPIO_FUNC_PWM, 5, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 11
	
	{ { GPIO_FUNC_SPI, 1, MISO }, { GPIO_FUNC_UART, 0, TX }, { GPIO_FUNC_I2C, 0, SDA }, { GPIO_FUNC_PWM, 6, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 12
	
	{ { GPIO_FUNC_SPI, 1, CS }, { GPIO_FUNC_UART, 0, RX }, { GPIO_FUNC_I2C, 0, SCL }, { GPIO_FUNC_PWM, 6, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_DET }, }, // 13
	
	{ { GPIO_FUNC_SPI, 1, SCK }, { GPIO_FUNC_UART, 0, CTS }, { GPIO_FUNC_I2C, 1, SDA }, { GPIO_FUNC_PWM, 7, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 14
	
	{ { GPIO_FUNC_SPI, 1, MOSI }, { GPIO_FUNC_UART, 0, RTS }, { GPIO_FUNC_I2C, 1, SCL }, { GPIO_FUNC_PWM, 7, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 15
	
	{ { GPIO_FUNC_SPI, 0, MISO }, { GPIO_FUNC_UART, 0, TX }, { GPIO_FUNC_I2C, 0, SDA }, { GPIO_FUNC_PWM, 0, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_DET }, }, // 16
	
	{ { GPIO_FUNC_SPI, 0, CS }, { GPIO_FUNC_UART, 0, RX }, { GPIO_FUNC_I2C, 0, SCL }, { GPIO_FUNC_PWM, 0, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 17
	
	{ { GPIO_FUNC_SPI, 0, SCK }, { GPIO_FUNC_UART, 0, CTS }, { GPIO_FUNC_I2C, 1, SDA }, { GPIO_FUNC_PWM, 1, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 18
	
	{ { GPIO_FUNC_SPI, 0, MOSI }, { GPIO_FUNC_UART, 0, RTS }, { GPIO_FUNC_I2C, 1, SCL }, { GPIO_FUNC_PWM, 1, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_DET }, }, // 19
	
	{ { GPIO_FUNC_SPI, 0, MISO }, { GPIO_FUNC_UART, 1, TX }, { GPIO_FUNC_I2C, 0, SDA }, { GPIO_FUNC_PWM, 2, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_GPCK, -1, GPIN0 },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 20
	
	{ { GPIO_FUNC_SPI, 0, CS }, { GPIO_FUNC_UART, 1, RX }, { GPIO_FUNC_I2C, 0, SCL }, { GPIO_FUNC_PWM, 2, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_GPCK, -1, GPOUT0 },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 21
	
	{ { GPIO_FUNC_SPI, 0, SCK }, { GPIO_FUNC_UART, 1, CTS }, { GPIO_FUNC_I2C, 1, SDA }, { GPIO_FUNC_PWM, 3, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_GPCK, -1, GPIN1 },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 22
	
	{ { GPIO_FUNC_SPI, 0, MOSI }, { GPIO_FUNC_UART, 1, RTS }, { GPIO_FUNC_I2C, 1, SCL }, { GPIO_FUNC_PWM, 3, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_GPCK, -1, GPOUT1 },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 23
	
	{ { GPIO_FUNC_SPI, 1, MISO }, { GPIO_FUNC_UART, 1, TX }, { GPIO_FUNC_I2C, 0, SDA }, { GPIO_FUNC_PWM, 4, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_GPCK, -1, GPOUT2 },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 24
	
	{ { GPIO_FUNC_SPI, 1, CS }, { GPIO_FUNC_UART, 1, RX }, { GPIO_FUNC_I2C, 0, SCL }, { GPIO_FUNC_PWM, 4, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_GPCK, -1, GPOUT3 },
	  { GPIO_FUNC_USB, -1, VBUS_DET }, }, // 25
	
	{ { GPIO_FUNC_SPI, 1, SCK }, { GPIO_FUNC_UART, 1, CTS }, { GPIO_FUNC_I2C, 1, SDA }, { GPIO_FUNC_PWM, 5, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 26
	
	{ { GPIO_FUNC_SPI, 1, MOSI }, { GPIO_FUNC_UART, 1, RTS }, { GPIO_FUNC_I2C, 1, SCL }, { GPIO_FUNC_PWM, 5, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, OVCUR_DET }, }, // 27
	
	{ { GPIO_FUNC_SPI, 1, MISO }, { GPIO_FUNC_UART, 0, TX }, { GPIO_FUNC_I2C, 0, SDA }, { GPIO_FUNC_PWM, 6, A },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_DET }, }, // 28
	
	{ { GPIO_FUNC_SPI, 1, CS }, { GPIO_FUNC_UART, 0, RX }, { GPIO_FUNC_I2C, 0, SCL }, { GPIO_FUNC_PWM, 6, B },
          { GPIO_FUNC_SIO, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_PIO0, -1, ROLE_NULL }, { GPIO_FUNC_NULL, -1, ROLE_NULL },
	  { GPIO_FUNC_USB, -1, VBUS_EN }, }, // 29
};


#ifdef CONFIG_HEART_BEAT
static void picon_heart_beat_task(void *args)
{
	UNUSED(args);

	while (1) {
		gpio_xor_mask(1<<CONFIG_HEART_BEAT_PIN);
		rtos_task_delay(rtos_ms_to_ticks(CONFIG_HEART_BEAT_RATE));

#ifdef CONFIG_WDG
		// kick the watchdog
		watchdog_update();
#endif
	}
}
#endif

int picon_init()
{
	rtc_init();

#ifdef CONFIG_HEART_BEAT
	gpio_init(CONFIG_HEART_BEAT_PIN);
	gpio_set_dir(CONFIG_HEART_BEAT_PIN, GPIO_OUT);
	gpio_put(CONFIG_HEART_BEAT_PIN, 0);

	rtos_task_create(picon_heart_beat_task, "hrtbt", CONFIG_HEART_BEAT_STACK_SIZE,
		    NULL, CONFIG_HEART_BEAT_PRIORITY, NULL);
#endif

#ifdef CONFIG_WDG
	// set up the WDG
	watchdog_enable(CONFIG_WDG_PERIOD, 1);

#endif

	return 0;
}

void picon_reboot()
{
	watchdog_reboot(0, 0, 0);
}

bool picon_watchdog_caused_reboot()
{
	return watchdog_caused_reboot();
}

void picon_set_datetime(time_t time)
{
	struct tm	tm;
	datetime_t	dt;

	localtime_r(&time, &tm);

	dt.year  = tm.tm_year + 1900;
	dt.month = tm.tm_mon + 1;
	dt.day   = tm.tm_mday;
	dt.dotw  = tm.tm_wday;
	dt.hour  = tm.tm_hour;
	dt.min   = tm.tm_min;
	dt.sec   = tm.tm_sec;

	rtc_set_datetime(&dt);
}

time_t time(time_t *tloc)
{
	struct tm	tm;
	datetime_t	dt;
	time_t		t;

	rtc_get_datetime(&dt);

	tm.tm_year = dt.year - 1900;
	tm.tm_mon  = dt.month - 1;
	tm.tm_mday = dt.day;
	tm.tm_wday = dt.dotw;
	tm.tm_hour = dt.hour;
	tm.tm_min  = dt.min;
	tm.tm_sec  = dt.sec;

	t = mktime(&tm);

	if (tloc)
		*tloc = t;

	return t;
}

