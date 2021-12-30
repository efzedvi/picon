# Note that this file can be used as a top level CMakeLists.txt 


#set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

set(ENV{FREERTOS_KERNEL_PATH} "../freertos/")
# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

project(picon C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_library(picon_common INTERFACE)
target_sources(picon_common INTERFACE
        src/core/dev.c
        src/core/io.c
        src/core/console.c
        src/core/utils.c
        src/core/printf-stdarg.c
        src/core/log.c
        src/core/picon.c
	src/drivers/uart.c
	src/drivers/usb_serial.c
	src/drivers/mem.c
	src/shell/shell.c
	src/shell/shell_log.c
	src/shell/shell_gpio.c
        src/main/main.c)
target_include_directories(picon_common INTERFACE
	src/include
        ${CMAKE_CURRENT_LIST_DIR}
        )
target_link_libraries(picon_common INTERFACE
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
	pico_platform
        pico_runtime
        pico_time
        pico_stdlib_headers
	pico_unique_id
	pico_bootrom
	pico_malloc
        tinyusb_device_unmarked
#        pico_stdio_usb
	hardware_regs
	hardware_gpio
	hardware_uart
	hardware_irq
	hardware_watchdog
	hardware_rtc
        pico_multicore)

add_executable(picon)

#pico_enable_stdio_usb(picon 0)
#pico_enable_stdio_uart(picon 1)


target_link_libraries(picon picon_common)
pico_add_extra_outputs(picon)

