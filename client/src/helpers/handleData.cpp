/*
 * File: handleData.cpp
 * Project: steam-comment-service-bot
 * Created Date: 17.11.2023 17:48:54
 * Author: 3urobeat
 *
 * Last Modified: 18.11.2023 13:29:27
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
    char cpuLoad[8] = "";
    char cpuTemp[8] = "";
    char ramUsage[16] = "";
    char swapUsage[16] = "";
    char gpuLoad[8] = "";
    char gpuTemp[8] = "";
};


// Handles incoming measurement data messages and updates measurementsCache accordingly
void handleDataInput(char *str) {

    // Get measurement type by converting char to int: https://stackoverflow.com/a/868508
    int typeChar = str[1] - '0';

    // Specify limit for removing trailing char
    uint8_t sizeLim = strlen(str) - 1;


    // Write into the correct register
    char *registerP = nullptr; // Point to register so we only need to

    switch (typeChar) {
        case cpuLoadID:
            registerP = measurementsCache::cpuLoad;
            break;
        case cpuTempID:
            registerP = measurementsCache::cpuTemp;
            break;
        case ramUsageID:
            registerP = measurementsCache::ramUsage;
            break;
        case swapUsageID:
            registerP = measurementsCache::swapUsage;
            break;
        case gpuLoadID:
            registerP = measurementsCache::gpuLoad;
            break;
        case gpuTempID:
            registerP = measurementsCache::gpuTemp;
            break;
        default:
            lcd.centerPrint("Unsupported type!", 0, true);
            return; // Unsupported type
            break;
    }

    if (sizeLim > sizeof(*registerP) - 1) sizeLim = sizeof(*registerP) - 1; // Check if input is longer than the register's size and prevent overflow

    strncpy(registerP, str + 2, sizeLim); // Copy into the correct register, offset by 2 to skip control char and type id



    // Update connection loss check vars
    timeSinceLastSignal = 0; // Reset time since last signal
    displayingSplashScreen = false;

}
