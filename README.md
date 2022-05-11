
# Picon = Pi Pico + Console

Picon provides a command shell to Pi Pico which allows user to access most of its peripherals through shell commands.

Picon is built using Pico SDK, and FreeRTOS with SMP support

## Build

See [Getting Started with the Raspberry Pi Pico](https://rptl.io/pico-get-started) for information on how to setup the Pico SDK and your
hardware. Once it's done, simply follow these commands:

	$ git clone https://github.com/efzedvi/picon --recursive
	$ cd picon
	$ mkdir build
	$ cd build
	$ cmake ..
	$ make

At this point the image file ***picon.uf2*** is ready be installed on the Pico.

## Consoles

With this current build image two independent consoles are available, one runs on the USB port (available on linux using /dev/ttyACM0), and the other one on the UART port (/dev/ttyUSB0).

Both are configured as 115200 baudrate, 8N1, with no HW or SW flow control.

Enter '?' or 'help' to get a list of available commands.

Current console commands at the moment are:

	clear        : Clears the terminal screen
	date         : Displays and sets date and time
	free         : Shows amount of unused heap memory
	lsdev        : Lists device files
	mem          : Peeks/Pokes into memory devices
	ps           : Lists current tasks
	reboot       : Reboots the system or goes into BOOTSEL mode
	uname        : Unit name ; displayes build info
	uptime       : Shows uptime
	expr         : A very simple calculator
	primes       : Test command ; displays prime numbers for testing the terminal
	log          : Log command used for debugging
	gpio         : GPIO handling command
	i2c          : Command to scan and xfer data to/from I2C devices
	env          : sets/gets/lists environment variables
	pwm          : PWM command

More commands and features will be added in future.


