#!/bin/bash
# Copyright (c) 2022 Faraz V faraz@fzv.ca.
#
# SPDX-License-Identifier: BSD-3-Clause

./unhun.pl  ../src/include/FreeRTOSConfig.h  ../freertos/include/FreeRTOS.h ../freertos/portable/ThirdParty/GCC/rpi_pico/portmacro.h `ls ../freertos/include/*.h | grep -v S | grep -v proto | sort`

