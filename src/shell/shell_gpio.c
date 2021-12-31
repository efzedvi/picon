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
#include "picon/console.h"
#include "picon/printf.h"
#include "picon/io.h"
#include "picon/ioctl.h"
#include "picon/utils.h"

#include "shell/shell.h"
#include "shell/shell_gpio.h"
#include "hardware/platform_defs.h"
#include "hardware/gpio.h"

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

const char *role_to_str[21] = {
	"TX",
	"RX",
	"CTS",
	"RTS",
	"SDA",
	"SCL",
	"SCK",
	"CS",
	"MOSI",
	"MISO",
	"A",
	"B",
	"GPIN0",
	"GPOUT0",
	"GPIN1",
	"GPOUT1",
	"GPOUT2",
	"GPOUT3",
	"OVCUR_DET",
	"VBUS_DET",
	"VBUS_EN"
};



const char *dir_to_str[2] = {
	"input",
	"output",
};

const char *speed_to_str[2] = {
	"slow",
	"fast",
};

const char *func_to_str[9] = {
	//"XIP",
	"SPI",
	"UART",
	"I2C",
	"PWM",
	"SIO",
	"PIO0",
	"PIO1",
	"GPCK",
	"USB",
};

const uint8_t strength_to_num[4] = { 2, 4, 8, 12 };

typedef enum _shell_gpio_cmd_t {
	SHELL_GPIO_READ,
	SHELL_GPIO_WRITE,
	SHELL_GPIO_TOGGLE,
	SHELL_GPIO_CONFIG,
	SHELL_GPIO_LIST,

	SHELL_GPIO_UNKNOWN,
} shell_gpio_cmd_t;


static void func_role_to_str(uint8_t gpio, uint8_t func, char *buf, uint8_t buf_size)
{
	if (!buf || !buf_size || gpio >= PICON_MAX_GPIO) return;

	if (func > 8 || func==0) {
		strncpy(buf, " ", buf_size);
		return;
	}

	func -= 1;

	if (picon_gpios[gpio][func].idx > 0) {
		snprintf(buf, buf_size, "%s%d-%s", func_to_str[func], picon_gpios[gpio][func].idx, role_to_str[picon_gpios[gpio][func].role]);
	} else if (picon_gpios[gpio][func].role != ROLE_NULL) {
		snprintf(buf, buf_size, "%s-%s", func_to_str[func], role_to_str[picon_gpios[gpio][func].role]);
	} else {
		snprintf(buf, buf_size, "%s", func_to_str[func]);
	}
}



static void shell_gpio_usage(void)
{
	printf( "Options are:\n"
		"-l\n"
		"-p <gpio>\n"
		"-p <gpio> -w <0|1>\n"
		"-p <gpio> -t\n"
		"-p <gpio> -c <i|o>\n"
		"	   -u -d -s <s|f> -a <2|4|8|12>\n"
		);
}

void display_gpios(uint8_t start, uint8_t end)
{
	int			i;
	char			role[16];
	enum gpio_function	func;
	uint8_t			pull_up=0, pull_down=0, dir_out=0, value=0;
	enum gpio_slew_rate	speed = GPIO_SLEW_RATE_SLOW;
	enum gpio_drive_strength	strength=GPIO_DRIVE_STRENGTH_4MA;

	printf("GPIO   Direction   Value   Pull   Speed   Strength  Function\n");
	printf("---------------------------------------------------------------\n");
	for(i=start; i < PICON_MAX_GPIO && i <= end ; i++) {
		dir_out = gpio_is_dir_out(i); // note GPIO_OUT is 1/true and GPIO_IN is 0/false
		value = gpio_get(i);
		pull_up = gpio_is_pulled_up(i);
		pull_down = gpio_is_pulled_down(i);
		speed = gpio_get_slew_rate(i);
		strength = gpio_get_drive_strength(i);
		func = gpio_get_function(i);

		func_role_to_str(i, func, role, sizeof(role));

		printf(" %2d      %-6s      %d      %c%c    %s     %2d mA    %s\n", i,
			(func == GPIO_FUNC_SIO || (int) func == 0x1F) ? dir_to_str[dir_out] : "n/a",
			value,
			pull_up ? 'U' : ' ', pull_down ? 'D' : ' ',
			speed_to_str[speed], strength_to_num[strength],
			role
			);
	}
}

int shell_gpio(int argc, char *argv[])
{
	const char 	*opt_string = "p:w:tc:uds:a:lh";
	PICON_OPT  	opt;
	int	   	next_option, tmp;
	int8_t	  	gpio=-1;
	shell_gpio_cmd_t	cmd=SHELL_GPIO_UNKNOWN;
	uint8_t			pull_up=0, pull_down=0, dir_out=0, value=0;
	enum gpio_slew_rate	speed = GPIO_SLEW_RATE_SLOW;
	enum gpio_drive_strength	strength=GPIO_DRIVE_STRENGTH_4MA;


	if (argc >= 2) {
		opt_init(&opt, opt_string);

		do {
			next_option = getopt(&opt, argc, argv);

			switch (next_option) {
				case 'l':
					cmd = SHELL_GPIO_LIST;
					break;

				case 'p':
					gpio = atoi(opt.optarg);
					if (gpio < 0 || gpio >= PICON_MAX_GPIO) {
						printf("Invalid gpio\n");
						return -2;
					}
					break;
				case 'w':
					value = atoi(opt.optarg) ? 1 : 0;
					cmd = SHELL_GPIO_WRITE;
					break;
				case 't':
					cmd = SHELL_GPIO_TOGGLE;
					break;
				case 'c':
					if (strcmp(opt.optarg, "i") !=0 && strcmp(opt.optarg, "o") !=0) {
						printf("Invalid pin direction, must be either 'i', or 'o'\n");
						return -2;
					}
					dir_out = (opt.optarg[0] == 'o');
					cmd = SHELL_GPIO_CONFIG;
					break;
				case 'u':
					pull_up = 1;
					break;
				case 'd':
					pull_down = 1;
					break;
				case 's':
					if ( strcmp(opt.optarg, "s") == 0 ) {
						speed = GPIO_SLEW_RATE_SLOW;
					} else if ( strcmp(opt.optarg, "f") == 0) {
						speed = GPIO_SLEW_RATE_FAST;
					} else {
						printf("Invalid pin speed, must be either 's', or 'f'\n");
						return -2;
					}
					break;
				case 'a':
					tmp = atoi(opt.optarg);
					switch (tmp) {
						case 2:
							strength = GPIO_DRIVE_STRENGTH_2MA;
							break;
						case 4:
							strength = GPIO_DRIVE_STRENGTH_4MA;
							break;
						case 8:
							strength = GPIO_DRIVE_STRENGTH_8MA;
							break;
						case 12:
							strength = GPIO_DRIVE_STRENGTH_12MA;
							break;
						default:
							printf("Invalid pin strengh, must be either 2, 4, 8, or 12 mA\n");
							return -2;
					}
					break;
				case 'h':
					shell_gpio_usage();
					return -1;
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
	} else {
		cmd = SHELL_GPIO_LIST;
	}

	if (gpio < 0 && cmd != SHELL_GPIO_LIST) {
		printf("gpio pin not specified\n");
		return -2;
	}

	if (cmd == SHELL_GPIO_UNKNOWN)
		cmd = SHELL_GPIO_READ;

	switch (cmd) {
		case SHELL_GPIO_READ:
			display_gpios(gpio, gpio);
			break;

		case SHELL_GPIO_WRITE:
			gpio_put(gpio, value);
			break;

		case SHELL_GPIO_TOGGLE:
			gpio_xor_mask(1 << gpio);
			break;

		case SHELL_GPIO_CONFIG:
			gpio_init(gpio);
			gpio_set_dir(gpio, dir_out);
			gpio_set_pulls(gpio, pull_up, pull_down);
			gpio_set_slew_rate(gpio, speed);
			gpio_set_drive_strength(gpio, strength);
			break;

		case SHELL_GPIO_LIST:
			display_gpios(0, PICON_MAX_GPIO);
			break;

		default:
			printf("Invalid command\n\n");
			break;
	}


	return 0;
}



