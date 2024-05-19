/*
 * File: helpers.h
 * Project: arduino-resource-monitor
 * Created Date: 24.01.2023 17:14:44
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-19 18:23:25
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include "server.h"


// Stores filesystem paths for all sensors we've found
#define pathSize 128

namespace sensorPaths {
    // cpuLoad does not need a sensor, we read from '/proc/stat'
    extern char cpuTemp[pathSize];
    extern char ramUsage[pathSize];
    extern char swapUsage[pathSize];
    extern char gpuLoad[pathSize];
    extern char gpuTemp[pathSize];
};


// Stores all current measurements
#define dataSize 8

namespace measurements {
    extern char cpuLoad[dataSize];
    extern char cpuTemp[dataSize];
    extern char ramUsage[dataSize];
    extern char swapUsage[dataSize];
    extern char gpuLoad[dataSize];
    extern char gpuTemp[dataSize];
};


// Functions defined in helpers
extern void getMeasurements();

extern bool getSensors();

extern serial::Serial* makeConnection();

extern bool strStartsWith(const char *pre, const char *str);

extern void sendMeasurements();
extern void logMeasurements();
extern void resetCache();
