/*
 * File: main.h
 * Project: arduino-resource-monitor
 * Created Date: 2022-02-05 12:22:33
 * Author: 3urobeat
 *
 * Last Modified: 2025-12-16 20:43:23
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once


#include <Arduino.h>
#include <stdbool.h> // Datatype bool in C

#include "helpers/helpers.h"


// Config variables
#define maxcol 20
#define maxrow 4


// Do not modify
#define serialHeader "+ResourceMonitorClient"
#define version "v0.8.0"

#define baud 9600
#define serialEOL "#\n"


// C++ functions need a prefix when viewed from a C++ compiler in order to allow C files to call them
#ifdef __cplusplus
    extern "C" void setup_c();
    extern "C" void loop_c();
    extern "C" void serialEvent_c();
#else
    extern void setup_c();
    extern void loop_c();
    extern void serialEvent_c();
#endif
