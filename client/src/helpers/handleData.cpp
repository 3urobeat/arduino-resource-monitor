/*
 * File: handleData.cpp
 * Project: arduino-resource-monitor
 * Created Date: 17.11.2023 17:48:54
 * Author: 3urobeat
 *
 * Last Modified: 18.11.2023 15:30:51
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


// Specifies the IDs which data messages are prefixed with to indicate their type
enum measurementTypes {
    pingID      = 0, // Empty data message only used by server for preventing connection loss screen
    cpuLoadID   = 1,
    cpuTempID   = 2,
    ramUsageID  = 3,
    swapUsageID = 4,
    gpuLoadID   = 5,
    gpuTempID   = 6
};


// Stores all current measurements
namespace measurementsCache {
    char cpuLoad[dataSize] = "";
    char cpuTemp[dataSize] = "";
    char ramUsage[dataSize] = "";
    char swapUsage[dataSize] = "";
    char gpuLoad[dataSize] = "";
    char gpuTemp[dataSize] = "";
};


// Handles incoming measurement data messages and updates measurementsCache accordingly
void handleDataInput(char *str) {

    // Get measurement type by converting char to int: https://stackoverflow.com/a/868508
    int typeChar = str[1] - '0';

    char unit[4] = "";


    // Write into the correct register
    char *registerP = NULL; // Point to register so we can dedup the code below

    switch (typeChar) {
        case cpuLoadID:
            registerP = measurementsCache::cpuLoad;
            strcpy(unit, "%");
            break;
        case cpuTempID:
            registerP = measurementsCache::cpuTemp;
            strcpy(unit, "°C");
            break;
        case ramUsageID:
            registerP = measurementsCache::ramUsage;
            strcpy(unit, "GB");
            break;
        case swapUsageID:
            registerP = measurementsCache::swapUsage;
            strcpy(unit, "GB");
            break;
        case gpuLoadID:
            registerP = measurementsCache::gpuLoad;
            strcpy(unit, "%");
            break;
        case gpuTempID:
            registerP = measurementsCache::gpuTemp;
            strcpy(unit, "°C");
            break;
        default:
            return; // Unsupported type
            break;
    }

    // Copy into the correct register, offset by 3 to skip control char, separator and type id. Limit by 16 - unit size to prevent overflow.
    strncpy(registerP, str + 3, dataSize - sizeof(unit));
    strcat(registerP, unit);


    // Update connection loss check vars
    timeSinceLastSignal = 0; // Reset time since last signal
    displayingSplashScreen = false;

}
