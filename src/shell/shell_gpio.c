/**
 * @author      : faraz
 * @file        : shell_gpio.c
 * @created     : Wed 29 Dec 2021 11:47:51 AM EST
 */

#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "picon/config.h"
#include "picon/shell.h"
#include "picon/console.h"
#include "picon/printf.h"
#include "picon/io.h"
#include "picon/ioctl.h"
#include "picon/utils.h"
#include "board.h"
#include "gpio.h"

#include "picon/shell_gpio.h"
#include "hardware/platform_defs.h"

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
#define PICON_MAX_GPIO		NUM_BANK0_GPIOS
#define PICON_MAX_GPIO_FUNC	(9)
//#end

picon_gpio_def	picon_gpios[PICON_MAX_GPIO][PICON_MAX_GPIO_FUNC] = {
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




const char *dir_to_str[4] = {
	"input",
	"output-slow",
	"output-normal",
	"output-fast",
};

const char *mode_to_str[8] = {
	"input-analog",
	"input-float",
	"input-pullup",
	"input-pulldown",
	"output-pushpull",
	"output-opendrain",
	"output-alt-pp",
	"output-alt-od",
};

static void gpio_user_isr(void)
{
	if (isr_test_port && isr_test_pin)
		gpio_toggle(isr_test_port, isr_test_pin);
}


static void shell_gpio_usage(void)
{
	printf( "Options are:\n"
		"-r <gpio>\n"
		"-w <gpio> [val=0]\n"
		"-t <gpio>\n"
		"-c <gpio> -d <dir> -m <mode>\n"
		"-g <gpio>\n"
		"-l <port>\n"
		"-i <gpio> -x <trigger> -u <gpio|null>\n\n"
		"dir  : input | output-slow | output-normal | output-fast\n"
		"mode : input-analog | input-float | input-pullup | input-pulldown\n"
		"       output-pushpull | output-opendrain | output-alt-pp | output-alt-od\n"
		"trigger : rising | falling | any : default is any\n");
}

int shell_parse_gpio(char *str, PICON_GPIO *gpio)
{
	char	port_ch;
	int	tmp;

	if (!str || !gpio) return -1;

	port_ch = str[0];
	if (!isalpha(port_ch)) {
		printf("Invalid GPIO port '%s'\n\n", str);
		return -1;
	}
	tmp = toupper(port_ch) - 'A';
	if (tmp >= PICON_PORT_MAX) {
		printf("Out of range GPIO port\n\n");
		return -1;
	}
	gpio->port = (PICON_GPIO_PORT) tmp;

	if (str[1] == '\0') {
		gpio->pin = PICON_PIN_ALL;
	} else {
		tmp = atoi(str + 1);
		if (tmp > 15) {
			printf("Out of range GPIO pin\n\n");
			return -1;
		}
		if (tmp < 0) tmp = 0;
		gpio->pin = PICON_PIN(tmp % 16);
	}

	return 0;
}

int shell_gpio(int argc, char *argv[])
{
	const char *opt_string = "r:w:t:c:g:l:i:d:m:u:x:";
	PICON_OPT  opt;
	int	   next_option;
	SHELL_GPIO_CMDS	cmd = SHELL_GPIO_UNKNOWN;
	char		dir_dir=0, mode_dir=0;
	int		rv, fd=-1;
	uint16_t 	i;
	PICON_IOCTL_GPIO_CONFIG		gpio_cfg;
	PICON_IOCTL_GPIO_INTERRUPT	gpio_intr;
	PICON_GPIO			gpio, test_gpio;

	gpio.port = PICON_PORT_MAX;
	gpio.pin = PICON_PIN_ALL;
	gpio.value = 0;

	gpio_cfg.mode = PICON_GPIO_MODE_MAX;
	gpio_cfg.dir_speed = PICON_GPIO_DIR_MAX;

	gpio_intr.trigger = PICON_GPIO_TRIGGER_ANY;
	gpio_intr.func	= NULL;

	if (argc < 2) {
		shell_gpio_usage();
		return -1;
	}

	opt_init(&opt, opt_string);

	do {
		next_option = getopt(&opt, argc, argv);

		switch (next_option) {

			case 'r':
				cmd = SHELL_GPIO_READ;
				rv = shell_parse_gpio(opt.optarg, &gpio);
				if (rv < 0) return rv;
				break;
			case 'w':
				cmd = SHELL_GPIO_WRITE;
				rv = shell_parse_gpio(opt.optarg, &gpio);
				if (rv < 0) return rv;
				break;
			case 't':
				cmd = SHELL_GPIO_TOGGLE;
				rv = shell_parse_gpio(opt.optarg, &gpio);
				if (rv < 0) return rv;
				break;
			case 'c':
				cmd = SHELL_GPIO_CONFIG;
				rv = shell_parse_gpio(opt.optarg, &gpio);
				if (rv < 0) return rv;
				break;
			case 'g':
				cmd = SHELL_GPIO_CHECK;
				rv = shell_parse_gpio(opt.optarg, &gpio);
				if (rv < 0) return rv;
				break;
			case 'l':
				cmd = SHELL_GPIO_LIST;
				rv = shell_parse_gpio(opt.optarg, &gpio);
				if (rv < 0) return rv;
				break;
			case 'd':
				if (strcmp(opt.optarg, "input") == 0) {
					gpio_cfg.dir_speed = PICON_GPIO_DIR_INPUT;
					dir_dir = 1;
				} else if (strcmp(opt.optarg, "output-slow") == 0) {
					gpio_cfg.dir_speed  = PICON_GPIO_DIR_OUPUT_SLOW;
					dir_dir = 0;
				} else if (strcmp(opt.optarg, "output-normal") == 0) {
					gpio_cfg.dir_speed  = PICON_GPIO_DIR_OUPUT_NORMAL;
					dir_dir = 0;
				} else if (strcmp(opt.optarg, "output-fast") == 0) {
					gpio_cfg.dir_speed  = PICON_GPIO_DIR_OUPUT_FAST;
					dir_dir = 0;
				} else {
					printf("Invalid direction\n\n");
					shell_gpio_usage();
					return -1;
				}

				break;

			case 'm':
				if (strcmp(opt.optarg, "input-analog") == 0) {
					gpio_cfg.mode = PICON_GPIO_MODE_INPUT_ANALOG;
					mode_dir = 1;
				} else if (strcmp(opt.optarg, "input-float") == 0) {
					gpio_cfg.mode = PICON_GPIO_MODE_INPUT_FLOAT;
					mode_dir = 1;
				} else if (strcmp(opt.optarg, "input-pullup") == 0) {
					gpio_cfg.mode = PICON_GPIO_MODE_INPUT_PULLUP;
					mode_dir = 1;
				} else if (strcmp(opt.optarg, "input-pulldown") == 0) {
					gpio_cfg.mode = PICON_GPIO_MODE_INPUT_PULLDOWN;
					mode_dir = 1;
				} else if (strcmp(opt.optarg, "output-pushpull") == 0) {
					gpio_cfg.mode = PICON_GPIO_MODE_OUTPUT_PUSHPULL;
					mode_dir = 0;
				} else if (strcmp(opt.optarg, "output-opendrain") == 0) {
					gpio_cfg.mode = PICON_GPIO_MODE_OUTPUT_OPENDRAIN;
					mode_dir = 0;
				} else if (strcmp(opt.optarg, "output-alt-pp") == 0) {
					gpio_cfg.mode = PICON_GPIO_MODE_OUTPUT_ALTFN_PUSHPULL;
					mode_dir = 0;
				} else if (strcmp(opt.optarg, "output-alt-od") == 0) {
					gpio_cfg.mode = PICON_GPIO_MODE_OUTPUT_ALTFN_OPENDRAIN;
					mode_dir = 0;
				} else {
					printf("Invalid gpio_cfg.mode\n\n");
					shell_gpio_usage();
					return -1;
				}
				break;

			case 'i':
				cmd = SHELL_GPIO_INTR;
				rv = shell_parse_gpio(opt.optarg, &gpio);
				if (rv < 0) return rv;
				break;

			case 'x':
				if (strcmp(opt.optarg, "rising") == 0) {
					gpio_intr.trigger = PICON_GPIO_TRIGGER_RISING;
				} else if (strcmp(opt.optarg, "falling") == 0) {
					gpio_intr.trigger = PICON_GPIO_TRIGGER_RISING;
				} else if (strcmp(opt.optarg, "any") == 0) {
					gpio_intr.trigger = PICON_GPIO_TRIGGER_ANY;
				} else {
					printf("Invalid trigger\n\n");
					shell_gpio_usage();
					return -1;
				}
				break;

			case 'u':
				if (strcmp(opt.optarg, "null")==0) {
					gpio_intr.func = NULL;
					isr_test_port = 0;
					isr_test_pin  = 0;
				} else {
					rv = shell_parse_gpio(opt.optarg, &test_gpio);
					if (rv < 0) return rv;

					isr_test_port = gpio_ll_port(test_gpio.port);
					isr_test_pin  = test_gpio.pin;
					gpio_intr.func = gpio_user_isr;

				}
				break;

			case -1:
				break;

			case '?':
				printf("Invalid opiton\n\n");
				shell_gpio_usage();
				return -1;
			case ':':
				printf("Missing argument\n\n");
			default:
				shell_gpio_usage();
				return -1;
		};
	
	} while (next_option != -1); 

	if (dir_dir != mode_dir) {
		printf("Direction and Mode don't match\n\n");
		return -2;
	}

	if (gpio.port == PICON_PORT_MAX || gpio.port > PICON_MAX_GPIO_PORT) {
		printf("No valid GPIO port specified\n\n");
		return -3;
	}

	fd = open(GPIO_DEV_NAME, 0);
	if (fd < 0) {
		printf("Failed to open %s\n\n", GPIO_DEV_NAME);
		return -4;
	}

	rv = 0;
	switch (cmd) {
		case SHELL_GPIO_READ:
			rv = ioctl(fd, PICON_IOC_GPIO_GET, &gpio);
			if (rv < 0) break;
			if (gpio.pin == PICON_PIN_ALL)
				printf("%x\n", gpio.value);
			else
				printf("%x\n", gpio.value ? 1 : 0);
			break;

		case SHELL_GPIO_WRITE:
			if (opt.optind < argc)
				gpio.value = (uint16_t) strtol(argv[opt.optind], NULL, 16);

			rv = ioctl(fd, PICON_IOC_GPIO_SET, &gpio);
			break;

		case SHELL_GPIO_TOGGLE:
			rv = ioctl(fd, PICON_IOC_GPIO_TOGGLE, &gpio);
			break;

		case SHELL_GPIO_CONFIG:

			if (gpio_cfg.mode == PICON_GPIO_MODE_MAX) {
				printf("Inalid mode\n\n");
				close(fd);
				return -5;
			}

			if (gpio_cfg.dir_speed == PICON_GPIO_DIR_MAX) {
				printf("Inalid GPIO direction\n\n");
				close(fd);
				return -5;
			}

			gpio_cfg.gpio = gpio;
			rv = ioctl(fd, PICON_IOC_GPIO_SET_CONFIG, &gpio_cfg);
			break;

		case SHELL_GPIO_CHECK:
			gpio_cfg.gpio = gpio;
			rv = ioctl(fd, PICON_IOC_GPIO_GET_CONFIG, &gpio_cfg);
			if (rv < 0) break;

			printf("%s %s\n", dir_to_str[gpio_cfg.dir_speed],
					  mode_to_str[gpio_cfg.mode]);
			break;

		case SHELL_GPIO_LIST:
			gpio_cfg.gpio = gpio;
			for(i=0; i<sizeof(uint16_t) * 8; i++) {
				gpio_cfg.gpio.pin = BIT(i);
							
				rv = ioctl(fd, PICON_IOC_GPIO_GET_CONFIG, &gpio_cfg);
				if (rv < 0) break;

				printf("%c%d\t%d\t%s\n", 'A'+gpio.port, i,
					gpio_cfg.gpio.value ? 1 : 0,
					mode_to_str[gpio_cfg.mode]);
			}
			break;

		case SHELL_GPIO_INTR:

			if (isr_test_pin) {
				gpio_cfg.gpio = test_gpio;
				gpio_cfg.dir_speed = PICON_GPIO_DIR_OUPUT_FAST;
				gpio_cfg.mode      = PICON_GPIO_MODE_OUTPUT_PUSHPULL;
				rv = ioctl(fd, PICON_IOC_GPIO_SET_CONFIG, &gpio_cfg);
				if (rv < 0) {
					printf("Failed configuring gpio\n");
					break;
				}
			}

			gpio_intr.gpio = gpio;
			rv = ioctl(fd, PICON_IOC_GPIO_INTERRUPT, &gpio_intr);
			break;

		default:
			printf("Invalid command\n\n");
			break;
	}

	if (rv < 0) {
		printf("ioctl failed rv=%d\n\n", rv);
	}

	close(fd);
	return rv;
}



