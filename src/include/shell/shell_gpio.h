/**
 * @author      : faraz
 * @file        : shell_gpio
 * @created     : Wed 29 Dec 2021 11:45:41 AM EST
 */

#ifndef SHELL_GPIO_H
#define SHELL_GPIO_H

#define SHELL_GPIO_CMD_STACK_SIZE	(256)

int shell_gpio(int argc, char *argv[]);

#define SHELL_GPIO_COMMAND \
	{ CMD(gpio), SHELL_GPIO_CMD_STACK_SIZE, 0 },

#endif /* end of include guard SHELL_GPIO_H */
