/*
 * File: server.h
 * Project: arduino-resource-monitor
 * Created Date: 24.01.2023 17:56:00
 * Author: 3urobeat
 *
 * Last Modified: 16.11.2023 17:01:29
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once


#include "serial/serial.h" // Include library (Tutorial used: https://youtu.be/uHw7QyL4CM8)
#include <iostream>
#include <chrono>
#include <math.h>
#include <thread>

#include "helpers/helpers.h"


// Configuration variables
#define displayCols 20
#define displayRows 4

#define port "/dev/ttyUSB0"
#define baud 9600

#define checkInterval 1000               // 1 second is lowest value possibe as mpstat takes a second to collect data
#define cpuTempSensor "k10temp-pci-00c3" // sensors entry for your CPU temp
#define gpuType 1                        // Set to 0 for NVIDIA or to 1 for AMD
#define gpuBus "3"                       // If you are using an AMD GPU and you have multiple GPUs you need to specify the bus ID of the GPU to display.
#define gpuTempSensor "amdgpu-pci-0300"  // sensors entry for your AMD GPU temp (ignore if you have an NVIDIA card)


#define version "v0.5.0"


// Extern vars from server.cpp
extern char cpuTempCmd[128];
extern char gpuUtilCmd[128];
extern char gpuTempCmd[128];

extern char fullStr[displayRows][displayCols + 2];
extern char lcdCache[displayRows][displayCols + 5];

extern serial::Serial *connection;
