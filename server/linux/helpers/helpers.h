/*
 * File: helpers.h
 * Project: arduino-resource-monitor
 * Created Date: 24.01.2023 17:14:44
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-20 18:29:05
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

struct SensorTypes {
    char cpuTemp[pathSize];
    char gpuLoad[pathSize];
    char gpuTemp[pathSize];
};

extern struct SensorTypes sensorPaths;


// Stores all current measurements
#define dataSize 8

struct MeasurementTypes {
    char cpuLoad[dataSize];
    char cpuTemp[dataSize];
    char ramUsage[dataSize];
    char swapUsage[dataSize];
    char gpuLoad[dataSize];
    char gpuTemp[dataSize];
};

extern struct MeasurementTypes measurements;


// Functions defined in helpers
extern void getMeasurements();

extern void getSensors();

extern void makeConnection();

extern bool strStartsWith(const char *searchFor, const char *searchInStr);

extern void sendMeasurements();
extern void logMeasurements();
extern void resetCache();


// Functions defined in C++ wrapper for Serial library
extern "C" bool serialNewConnection(const char *port, uint32_t baudRate);
extern "C" bool serialIsOpen();
extern "C" void serialClose();
extern "C" void serialFlushOutput();
extern "C" bool serialWrite(const char *data); // Returns bool if write succeeded/failed
extern "C" bool serialRead(char *dest);        // Returns bool if read succeeded/failed
extern "C" char *serialGetPort();
