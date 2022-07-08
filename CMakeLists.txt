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
	src/core/env.c
	src/drivers/uart.c
	src/drivers/i2c.c
	src/drivers/usb_serial.c
	src/drivers/mem.c
	src/drivers/flash.c
	src/shell/shell.c
	src/shell/shell_log.c
	src/shell/shell_gpio.c
	src/shell/shell_i2c.c
	src/shell/shell_env.c
	src/shell/shell_pwm.c
        )
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
	hardware_flash
	hardware_gpio
	hardware_i2c
	hardware_irq
	hardware_pwm
	hardware_regs
	hardware_rtc
	hardware_sync
	hardware_uart
	hardware_watchdog
	#pico_cyw43_arch_lwip_sys_freertos
	#pico_cyw43_arch_none
        pico_multicore)

if (PICO_CYW43_SUPPORTED) # set by PICO_BOARD=pico_w
	set(CONFIG_PICO_W 1)
endif()

add_executable(picon src/main/main.c)

#pico_enable_stdio_usb(picon 0)
#pico_enable_stdio_uart(picon 1)


target_link_libraries(picon picon_common)
pico_add_extra_outputs(picon)

