#!/bin/bash

CUBE_ROOT="/home/liwaszkiewicz/workspace/others/STM32Cube_FW_F4_V1.4.0"

cmake -DCMAKE_BUILD_TYPE='Debug' \
-DDEVICE='STM32F407xx' \
-DCUBE_ROOT=$CUBE_ROOT \
-DSTARTUP_CODE="$CUBE_ROOT/Projects/STM32F4-Discovery/Templates/TrueSTUDIO/startup_stm32f407xx.s" \
-DLINKER_SCRIPT="$CUBE_ROOT/Projects/STM32F4-Discovery/Templates/TrueSTUDIO/STM32F4-Discovery/STM32F407VG_FLASH.ld" \
.
