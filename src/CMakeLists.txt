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

add_library(pico_common INTERFACE)
target_sources(pico_common INTERFACE
        main.c)
target_include_directories(pico_common INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        )
target_link_libraries(pico_common INTERFACE
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap5
        pico_stdlib
        pico_multicore)

add_executable(pico)

target_link_libraries(pico pico_common)
pico_add_extra_outputs(pico)
pico_enable_stdio_usb(pico 1)

#target_compile_definitions(on_core_one PRIVATE
#        mainRUN_FREE_RTOS_ON_CORE=1
#        PICO_STACK_SIZE=0x1000
#)

