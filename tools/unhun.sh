#!/bin/bash


./unhun.pl  ../src/include/FreeRTOSConfig.h  ../freertos/include/FreeRTOS.h ../freertos/portable/ThirdParty/GCC/rpi_pico/portmacro.h `ls ../freertos/include/*.h | grep -v S | grep -v proto | sort`

