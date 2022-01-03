
# Picon = Pi Pico + Console

Picon provides a command shell to Pi Pico which allows user to access most of its peripherals through shell commands.

Picon is built using Pico SDK, and FreeRTOS with SMP support

## Build

See [Getting Started with the Raspberry Pi Pico](https://rptl.io/pico-get-started) for information on how to setup the Pico SDK and your
hardware. Once it's done, simply follow these commands:

	$ git clone https://github/efzedvi/picon --recursive
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

More commands and features will be added in future.


