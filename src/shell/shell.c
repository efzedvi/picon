/**
 * @author      : faraz (faraz)
 * @file        : shell
 * @created     : Fri 17 Dec 2021 07:29:12 PM EST
 */
/*
 * Copyright (c) 2022 Faraz V faraz@fzv.ca.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "pico/unique_id.h"
#include "pico/version.h"
#include "pico/bootrom.h"
#include "pico.h"
#include "hardware/address_mapped.h"
#include "hardware/regs/tbman.h"
#include "hardware/regs/sysinfo.h"

#include "picon.h"

#include "picon/config.h"
#include "picon/console.h"
#include "picon/printf.h"
#include "picon/dev.h"
#include "picon/io.h"
#include "picon/ioctl.h"
#include "picon/utils.h"
#include "picon/log.h"
#include "shell/shell.h"
#include "shell/shell_gpio.h"
#include "shell/shell_i2c.h"
#include "shell/shell_env.h"
//#include "shell/shell_adc.h"
//#include "shell/shell_dac.h"
//#include "shell/shell_timer.h"
//#include "shell/shell_spi.h"
//#include "shell/shell_rf.h"
#include "shell/shell_log.h"
#include "shell/shell_pwm.h"
#include "shell/shell_adc.h"

#define SHELL_PROMPT		"# "
#define SHELL_MAX_LINE  	(80)
#define SHELL_MAX_ARG_NUM 	(32)
#define SHELL_HIST_SIZE		(5)

const SHELL_COMMAND  shell_commands[] = {
	SHELL_BASIC_COMMANDS
	SHELL_LOG_COMMAND
	SHELL_GPIO_COMMAND
	SHELL_I2C_COMMAND
	SHELL_ENV_COMMAND
	SHELL_PWM_COMMAND
	SHELL_ADC_COMMAND
//	//SHELL_DAC_COMMAND
//	SHELL_BOARD_REGDUMP
//	SHELL_TIMER_COMMAND
//	SHELL_SPI_COMMAND
//	SHELL_RF_COMMAND
	{ NULL, 	NULL, 0, 0 },
};


typedef struct _shell_cmd_arg {
	SHELL_CMD_FUNC		func;
	int			argc;
	char			**argv;
	int			rc;
	rtos_semaphore_handle_t	sem;
	CONSOLE_INFO		*console_info;
} SHELL_CMD_ARG;

static void shell_tokenize(char *line, int *argc, char **argv, char **inf, char **outf)
{
	uint16_t non_space_found = 0;
	size_t   count;
	char     quote='\0';

	*argc = 0;
	argv[0] = line;

	if (!line || !argc || !argv || !inf || !outf) return;

	*inf = *outf = NULL;

	count = strlen(line);

	while ((count > 0) && (*argc < SHELL_MAX_ARG_NUM)) {
		if (*line == '"' || *line == '\'') {
			if (quote == 0) {
				quote = *line;
			} else if (quote == *line) {
				quote = '\0';
				*line = '\0';
			}
		} else if (isspace((int)*line) && !quote) {
			non_space_found = 0;
			*line = '\0';
		} else if (!non_space_found) {
			non_space_found = 1;            
			argv[(*argc)++] = line;         
		}

		line++;
		count--;
	}

	int ac=0;

	for(int i=0; i<*argc; i++) {
		if (strcmp(argv[i], "<") == 0)  {
			if (!ac) ac = i;
			if ((i < *argc - 1) && !*inf) {
				*inf = argv[++i];
			}

		}

		if (strcmp(argv[i], ">") == 0)  {
			if (!ac) ac = i;
			if ((i < *argc - 1) && !*outf) {
				*outf = argv[++i];
			}
		}
	}

	if (ac) *argc = ac;
}

int shell_clear(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	putchar('\f');

	return 0;
}

int shell_uname(int argc, char **argv)
{
	printf("Picon Version: %s\nFreeRTOS version: %s\nPICO SDK: %s\nbuilt: %s, %s\n",
		PICON_VERSION, RTOS_TSK_KERNEL_VERSION_NUMBER, PICO_SDK_VERSION_STRING,
		PICON_BUILD_DATE, PICON_BUILD_TIME);

	if (argc > 1 && strcmp(argv[1], "-a")==0) {
		char id[PICO_UNIQUE_BOARD_ID_SIZE_BYTES*2+1];
		id[PICO_UNIQUE_BOARD_ID_SIZE_BYTES] = '\0';
		pico_get_unique_board_id_string(id, sizeof(id));
		printf("NOR Flash ID: %s\n", id);
		printf("Chip ID: %x\n", *((io_ro_32*)(SYSINFO_BASE + SYSINFO_CHIP_ID_OFFSET)));
	}

	return 0;
}

int shell_uptime(int argc, char **argv)
{
	uint32_t now;
	uint16_t days;
	uint8_t  hours, minutes, seconds;

	UNUSED(argc);
	UNUSED(argv);

	// get uptime in seconds
	now = rtos_task_get_tick_count() / RTOS_CONFIG_TICK_RATE_HZ;

	#define SECS_HOUR	(3600)
	#define SECS_DAY	(SECS_HOUR * 24)

	days = now / SECS_DAY;
	hours = (now % SECS_DAY) / SECS_HOUR;
	minutes = (now % SECS_HOUR) / 60;
	seconds = now % 60;

	printf("%d days, %d hours %d minutes, %d seconds\n", days, hours, minutes, seconds);

	return 0;
}


int shell_free(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	printf("Free Heap bytes: %d\n", rtos_port_get_free_heap_size());

	return 0;
}

int shell_reboot(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	if (argc <= 1) {
		printf("\n");
		picon_reboot();
		// we won't return
	}

	if (strcmp(argv[1], "-s") == 0 ) {
		printf("Last reboot: %s\n", picon_watchdog_caused_reboot() ? "watchdog" : "reboot");
	} else if (strcmp(argv[1], "-b") == 0 ) {
		reset_usb_boot(0, 0);
	} else {
		printf(	"reboot [-s | -b]\n"
			"	-s : prints the cause of the last reboot\n"
			"	-b : boots into BOOTSEL mode. i.e USB mass storage device\n"
			);
	}

	return 0;
}


int shell_ps(int argc, char **argv)
{
	if (argc > 1 && strcmp(argv[1], "-x") == 0) {
		// not using rtos_task_list
		rtos_task_status_t 	  *task_array;
		volatile rtos_base_type_t num_tasks;
		int 	 i;
		const    char task_state_desc[5] = {
			'R', 'X', 'B', 'S', 'D'
		};

		num_tasks = rtos_task_get_number_of_tasks();

		task_array = (rtos_task_status_t *) rtos_port_malloc(num_tasks *
								     sizeof(rtos_task_status_t));
		if (!task_array) {
			printf("Failed to allocate memory\n");
			return -1;
		}

		rtos_task_get_system_state(task_array, num_tasks, NULL);

		printf("Task            State   Prio    Stack_Base  StackPtr  Num\n");
		printf("----------------------------------------------------------\n");
		for(i=0; i<num_tasks; i++) {
			printf("%-16s  %c      %2d      %x     %4d    %2d\n",
				task_array[i].pcTaskName,
				task_state_desc[(int) task_array[i].eCurrentState],
				task_array[i].uxCurrentPriority,
				task_array[i].pxStackBase,
				task_array[i].usStackHighWaterMark,
				task_array[i].xTaskNumber
			);
		}
		rtos_port_free(task_array);
	} else {
		char *buf;

		buf = (char *) rtos_port_malloc(1024);
		if (!buf) {
			printf("Failed to allocate memory\n");
			return -1;
		}

		rtos_task_list(buf);
		printf("Task           State   Prio    Stack   Num\n");
		printf("------------------------------------------\n");
		printf("%s\n", buf);
		rtos_port_free(buf);
	}

	return 0;
}

static void shell_mem_usage(void)
{
	printf("mem [-d <device>] -a address -t <type = 1,2,4> [-l length=1] [<value> to write]\n");
}


int shell_mem(int argc, char **argv)
{
	const char 	*opt_string = "d:a:t:l:h";
	PICON_OPT  	opt;
        int 	   	next_option, i;
	uint8_t		type=4, value_specified=0, is_dev=0, dev_flags=0;
	uint16_t   	len=1;
	uint32_t   	address=0, value=0, tmp, result;
	char		buf[17];
	DEV_NAME	mem_device;
	int		fd=-1, rc;

	if (argc < 2) {
		shell_mem_usage();
		return -1;
	}

	opt_init(&opt, opt_string);
	memset(mem_device, 0, sizeof(mem_device));

	do {
		next_option = getopt(&opt, argc, argv);

		switch (next_option) {
			case 'd':
				strncpy(mem_device, opt.optarg, DEV_NAME_SIZE);
				is_dev = 1;
				break;
			case 'a':
				address = strtol(opt.optarg, NULL, 16);
				break;
			case 't':
				tmp = strtol(opt.optarg, NULL, 10);
				if (tmp != 1 && tmp != 2 && tmp != 4) {
					printf("Type can be 1,2 or 4\n");
					shell_mem_usage();
					return -1;
				}
				type = (uint8_t) tmp;
				break;
			case 'l':
				tmp = strtol(opt.optarg, NULL, 10);
				len = (uint16_t) (tmp & 0xFFFF);
				break;
			case 'h':
				shell_mem_usage();
				return 0;
			case -1:
				break;
			case '?':
				printf("Invalid opiton\n\n");
				shell_mem_usage();
				return -1;
			case ':':
				printf("Missing argument\n\n");
				shell_mem_usage();
				return -1;
			default:
				shell_mem_usage();
				return -1;
		};
	} while (next_option != -1);

	if (opt.optind < argc) {
		value = (uint32_t) strtol(argv[opt.optind], NULL, 16);
		value_specified = 1;
	}

	if (is_dev) {
		fd = open(mem_device, O_RDWR);
		if (fd < 0) {
			printf("Failed reading %s\n", mem_device);
			return -2;
		}
		// verifty that the device is a memory device
		ioctl(fd, PICON_IOC_GET_DEV_FLAGS, &dev_flags);
		// I think we can afford not checking the return value here
		if ((dev_flags & DEV_FLAG_MEMORY) == 0) {
			printf("Not a memory device\n");
			close(fd);
			return -3;
		}
	} else {
		// if not a dev then we can only access the addressable mapped memory in 32bit mode
		type = 4;
		len += (4 - len % 4); // make len multiple of 4;
	}

	if (value_specified) {	// write
		if (fd>0) {
			rc = pwrite(fd, (void *) &value, type, address);
			if (rc != type)
				printf("failed writing %d bytes to %s, rc=%d\n", type, mem_device, rc);
		} else {
			switch (type) {
				case 1:
					*(volatile uint8_t*)address = (uint8_t) value;
					break;
				case 2:
					*(volatile uint16_t*)address = (uint16_t) value;
					break;
				case 4:
					*(volatile uint32_t*)address = (uint32_t) value;
					break;
				default:
					break;
			}
		}
	} else { // read
		for(i=0; i<len; i+=type) {

			if (len != 1 && (i % 16 == 0) ) {
				printf("%08X  ", (unsigned int) address + i );
			}

			if (fd>0)
				pread(fd, &result, type, address + i);
			else
				result = *((volatile uint32_t*)(address + i));

			switch (type) {
				case 1:
					result &= 0xff;
					printf("%02X", (uint8_t) result);

					if (result < ' ' || result > 0x7f)
						buf[i % 16] = '.';
					else
						buf[i % 16] = (char) result;
					buf[i % 16 + 1] = '\0';
					break;
				case 2:
					printf("%04X", (uint16_t) (result & 0xffff));
					break;
				case 4:
					printf("%08X", (unsigned int) result);
					break;
				default:
					break;
			}

			if ((i+type) % 16 == 0) {
				if (type ==1 && len != 1)
					printf(" %s", buf);
				printf("\n");
			} else
				printf(" ");
		}
		printf("\n");
	}

	if (fd>0) close(fd);

	return 0;
}


static void shell_command_runner_task(void *args)
{
	SHELL_CMD_ARG	*cmd;

	if (!args) rtos_task_delete(NULL);

	cmd = (SHELL_CMD_ARG *) args;

	cmd->console_info = TASK_GET_LOCAL_STORAGE();

	if (!cmd->func) {
		cmd->rc = -1;
		rtos_task_delete(NULL);
	}

	cmd->rc = cmd->func(cmd->argc, cmd->argv);

	rtos_semaphore_give(cmd->sem);

	rtos_task_suspend(NULL);
}


void shell_task(void *args)
{
	const SHELL_COMMAND	*commands = (SHELL_COMMAND *) args;
	char		line[SHELL_MAX_LINE+1], hist_line[SHELL_HIST_SIZE][SHELL_MAX_LINE+1];
	char		*hist[SHELL_HIST_SIZE];
	uint16_t	i, n;
	char		*argv[SHELL_MAX_ARG_NUM];
	char 		found=0;
	int		console_fd=-1, rc, argc = 0;
	rtos_semaphore_handle_t	sem;
	rtos_task_handle_t 	child_handle;
	CONSOLE_INFO 		*console_info, child_console_info;
	SHELL_CMD_ARG	cmd;
	char		*inf = NULL, *outf = NULL;
	int8_t		fds[3];
	int8_t		new_stdin = -1, new_stdout = -1;

	sem = rtos_semaphore_create_binary();
	console_info = TASK_GET_LOCAL_STORAGE();
	if (console_info) console_fd = console_info->stdfd[STDIN_FILENO];

	memset(hist_line, 0, sizeof(hist_line));
	for(i=0; i<SHELL_HIST_SIZE; i++)
		hist[i] = hist_line[i];

	// print banner
	printf("Welcome to Picon version %s, type '?' or 'help' for list of commands.\n\n", PICON_VERSION);

	while (1) {
		printf(SHELL_PROMPT);
		n = getline(line, SHELL_MAX_LINE+1, hist, SHELL_HIST_SIZE);
		if (n <= 0) continue;
		chomp(line);

		if (line[0] == '\0') continue;

		inf  = NULL;
		outf = NULL;

		shell_tokenize(line, &argc, argv, &inf, &outf);
		if (!argv[0] || argv[0][0] == 0 || argv[0][0] == '#') continue;

		if (argv[0][0] == '?' || strcmp(argv[0], "help") == 0) {
			for(i=0; commands[i].cmd; i++) {
				printf("%-12s", commands[i].cmd);
				if (commands[i].desc) printf(" : %s", commands[i].desc);
				printf("\n");
			}

			continue;
		}

		found = 0;
		for(i=0; commands[i].cmd; i++) {
			if (strcmp(argv[0], commands[i].cmd) == 0) {
				found = 1;

				// backup the std fds
				fds[STDIN_FILENO]  = console_info->stdfd[STDIN_FILENO];
				fds[STDOUT_FILENO] = console_info->stdfd[STDOUT_FILENO];

				if (outf) {
					new_stdout = open(outf, O_WRONLY);
					if (new_stdout >= 0) {
						console_info->stdfd[STDOUT_FILENO] = new_stdout;
					} else {
						printf("failed opening %s for writing\n", outf);
						break;
					}
				}

				new_stdin = new_stdout = -1;
				if (inf) {
					new_stdout = open(outf, O_RDONLY);
					if (new_stdin >= 0) {
						console_info->stdfd[STDIN_FILENO] = new_stdin;
					} else {
						printf("failed opening %s for reading\n", inf);
						break;
					}
				}

				if (sem && (commands[i].property & SHELL_CMD_PROPERTY_CANCELABLE) &&
				    console_fd >=0 ) {

					cmd.func = commands[i].func;
					cmd.argc = argc;
					cmd.argv = argv;
					cmd.sem  = sem;

					rc = ioctl(console_fd, PICON_IOC_TTY_SET_INT, sem);
					if (rc < 0) {
						//ioctl failed, so we fall back to traditional method
						commands[i].func(argc, argv);
					} else {
						console_create_child(commands[i].cmd, shell_command_runner_task,
								  commands[i].stack_size, CONFIG_SHELL_CMD_PRIORITY,
								  (void *) &cmd, &child_handle, &child_console_info);

						if (!child_handle) {
							printf("Failed running %s\n", commands[i].cmd);
							break;
						}
						rtos_semaphore_take(sem, RTOS_PORT_MAX_DELAY);
						ioctl(console_fd, PICON_IOC_TTY_SET_INT, NULL);
						rtos_task_suspend(child_handle);

						// if there are any allocated memory free them up
						picon_free_all(&child_console_info);

						rtos_task_delete(child_handle);
					}
					// backup the std fds
					fds[STDIN_FILENO]  = console_info->stdfd[STDIN_FILENO];
					fds[STDOUT_FILENO] = console_info->stdfd[STDOUT_FILENO];

				} else {
					commands[i].func(argc, argv);
				}

				// restore the std fds
				console_info->stdfd[STDIN_FILENO]  = fds[STDIN_FILENO];
				console_info->stdfd[STDOUT_FILENO] = fds[STDOUT_FILENO];
				console_info->stdfd[STDERR_FILENO] = fds[STDERR_FILENO];

				if (new_stdin >= 0) close(new_stdin);
				if (new_stdout >= 0) close(new_stdout);

				break;
			}
		}

		if (!found) printf("Invalid command: %s\n", argv[0]);
	}
}

int shell_primes(int argc, char **argv)
{
	int		i, j, num=0;
	unsigned char	prime=0;
	uint32_t	start, end, delta;

	if (argc > 1) {
		num = abs(atoi(argv[1]));
	} else {
		printf("primes <num>\n");
		return -1;
	}

	start = rtos_task_get_tick_count();

	for(i=3; i<=num; i++) {
		if (i % 2 == 0) continue;

		prime = 1;

		for(j=3; j<i/2; j+=2) {
			if (i % j == 0) {
				prime = 0;
				break;
			}
		}
		if (prime) printf("%d\n", i);
	}

	end = rtos_task_get_tick_count();
	delta = diff_ticks(start, end);

	printf("\nelapsed time: %u ms\n", (unsigned int) (delta * 1000 / RTOS_CONFIG_TICK_RATE_HZ));

	return 0;
}


int shell_date(int argc, char **argv)
{
	time_t t = time(NULL);
	struct tm tm;

	if (argc > 1) {

		if (strcmp(argv[1], "-t") == 0) {
			printf("%u\n", (unsigned int) t);
			return 0;
		}

		if (argc == 3 && strcmp(argv[1], "-s") == 0) {
			t = atoi(argv[2]);
			picon_set_datetime(t);
			rtos_task_delay(rtos_ms_to_ticks(500));
			t = time(NULL);
		} else {
			printf( "date               : displayes date and time \n"
				"date -s <unixtime> : to set the time\n"
				"date -t            : to get epoch time\n");
			return -1;
		}
	}

	localtime_r((const time_t *) &t, &tm);

	printf("%d/%02d/%02d-%02d:%02d:%02d\n",
		tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec);

	return 0;
}

int shell_lsdev(int argc, char **argv)
{
	uint8_t		i=0;
	const DEVICE_FILE	*dev;

	UNUSED(argc);
	UNUSED(argv);

	for(i=0; i < dev_count(); i++) {
		dev = dev_by_idx(i);
		if (!dev) return -1;

		printf("%s\n", dev->name);
	}

	return 0;
}

int shell_parse_data(char *str, uint16_t len, uint8_t *buf, size_t size)
{
	uint16_t  	i;
	uint8_t		data;
	char 		*ptr;

	if (!str || !buf || !size | !len) return -1;

	if (*str == '+') {
		str++;
		data = strtol(str, NULL, 16);
		for(i=0; i<len && i<size; i++)
			buf[i] = data++;
	} else if (*str == '-') {
		str++;
		data = (uint8_t) strtol(str, NULL, 16);
		for(i=0; i<len && i<size; i++)
			buf[i] = data--;
	} else if (*str == '=') {
		str++;
		data = (uint8_t) strtol(str, NULL, 16);
		for(i=0; i<len && i<size; i++)
			buf[i] = data;
	} else {
		for(i=0; i<len && i<size; i++, str=ptr) {

			if (!str || !*str) return -2;

			buf[i] = (uint8_t) strtol(str, &ptr, 16);

			if (ptr && *ptr) ptr++;
		}
	}

	return 0;
}


static void shell_expr_usage(void)
{
	printf( "expr <operand>\n"
		"expr <operand1> <op> <operand2>\n"
		"	<op> : + - * / & | ^ << >>\n"
	      );
}

static uint32_t shell_expr_parse_operand(char *str)
{
	uint32_t	val=0;
        uint8_t		complement=0;

	if (!str || !str[0]) return 0;

	if (str[0] == '~') {
		complement = 1;
		str++;
		if (!str[0]) return 0;
	}

	if (str[0] == '0' && toupper(str[1]) == 'X') {
		val = strtol(str, NULL, 16);
	} else {
		val = strtol(str, NULL, 0);
	}

	if (complement) val = ~ val;

	return val;
}

int shell_expr(int argc, char *argv[])
{
	uint32_t	a, b;

	if (argc < 2 || argc > 4) {
		shell_expr_usage();
		return 1;
	}

	a = shell_expr_parse_operand(argv[1]);

	if (argc > 3) {
		b = shell_expr_parse_operand(argv[3]);

		if (strcmp(argv[2], "+")==0) {
			a = a + b;
		} else if (strcmp(argv[2], "-")==0) {
			a = a - b;
		} else if (strcmp(argv[2], "*")==0) {
			a = a * b;
		} else if (strcmp(argv[2], "/")==0) {
			if (!b) {
				printf("division by zero\n");
				return 3;
			}
			a = a / b;
		} else if (strcmp(argv[2], "&")==0) {
			a = a & b;
		} else if (strcmp(argv[2], "|")==0) {
			a = a | b;
		} else if (strcmp(argv[2], "^")==0) {
			a = a ^ b;
		} else if (strcmp(argv[2], "<<")==0) {
			a = a << b;
		} else if (strcmp(argv[2], ">>")==0) {
			a = a >> b;
		} else {
			printf("Invalid operator\n");
			shell_expr_usage();
			return 2;
		}
	}

	printf("%u 0x%x\n", (unsigned int) a, (unsigned int) a);

	return 0;
}

int shell_echo(int argc, char *argv[])
{
	uint8_t		new_line = 1;
	char		*space = "";

	for(int i=1; i<argc; i++) {
		if (strcmp(argv[i], "-n") == 0)
			new_line = 0;
		else {
			printf("%s%s", space, argv[i]);
			space = " ";
		}
	}

	if (new_line)
		printf("\n");
}
