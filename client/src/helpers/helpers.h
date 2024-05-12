/*
 * File: helpers.h
 * Project: arduino-resource-monitor
 * Created Date: 17.11.2023 17:18:28
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-12 22:11:38
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include "main.h"


#define dataSize 16 // Intentionally larger than in the server code because we need to store the units here as well

// Stores all current measurements
namespace measurementsCache {
    extern char cpuLoad[dataSize];   // in %
    extern char cpuTemp[dataSize];   // in °C
    extern char ramUsage[dataSize];  // in GB
    extern char swapUsage[dataSize]; // in GB
    extern char gpuLoad[dataSize];   // in %
    extern char gpuTemp[dataSize];   // in °C
};


void handleBacklight();
void setupBacklight();

void handleDataInput(char *str);

void printCurrentDataToDisplay();

void handleConnectionHandshake(char *serverHandshake);
