/*
 * File: printMeasurements.cpp
 * Project: arduino-resource-monitor
 * Created Date: 24.01.2023 17:41:01
 * Author: 3urobeat
 *
 * Last Modified: 18.11.2023 14:46:00
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"

using namespace std;


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


// Store what we last sent to the Arduino to avoid unnecessary refreshes
namespace arduinoCache {
    char cpuLoad[dataSize] = "";
    char cpuTemp[dataSize] = "";
    char ramUsage[dataSize] = "";
    char swapUsage[dataSize] = "";
    char gpuLoad[dataSize] = "";
    char gpuTemp[dataSize] = "";
};


chrono::time_point<chrono::steady_clock> lastWriteTime; // Track time to decide if we have to send an alive ping so the Arduino doesn't display the Lost Connection screen


char sendTempStr[32];

/**
 * Sends the actual serial message
 */
void _sendSerial(char *str, measurementTypes id)
{
    memset(sendTempStr, '\0', sizeof(sendTempStr));

    // Construct string to send
    sendTempStr[0] = '~';      // Data prefix
    sendTempStr[1] = id + '0'; // Simple hack to "convert" id to char. Only supports id <= 10 ofc
    sendTempStr[2] = '-';      // Add a separator

    strncat(sendTempStr, str, sizeof(sendTempStr) - 5); // 5 because of prefix char, type char, separator, end delimiter and null byte
    strcat(sendTempStr, "#");

    // Send content (team yippee)
    connection->write(sendTempStr);

    //cout << "Sending (" << strlen(sendTempStr) << "): " << sendTempStr << endl;

    // Refresh lastWriteTime
    lastWriteTime = chrono::steady_clock::now();
}


/**
 * Send updated measurements to the Arduino
 */
void sendMeasurements()
{
    // Send what changed
    if (strcmp(measurements::cpuLoad, arduinoCache::cpuLoad) != 0) {
        _sendSerial(measurements::cpuLoad, cpuLoadID);
        strcpy(arduinoCache::cpuLoad, measurements::cpuLoad);
    }

    if (strcmp(measurements::cpuTemp, arduinoCache::cpuTemp) != 0) {
        _sendSerial(measurements::cpuTemp, cpuTempID);
        strcpy(arduinoCache::cpuTemp, measurements::cpuTemp);
    }

    if (strcmp(measurements::ramUsage, arduinoCache::ramUsage) != 0) {
        _sendSerial(measurements::ramUsage, ramUsageID);
        strcpy(arduinoCache::ramUsage, measurements::ramUsage);
    }

    if (strcmp(measurements::swapUsage, arduinoCache::swapUsage) != 0) {
        _sendSerial(measurements::swapUsage, swapUsageID);
        strcpy(arduinoCache::swapUsage, measurements::swapUsage);
    }

    if (strcmp(measurements::gpuLoad, arduinoCache::gpuLoad) != 0) {
        _sendSerial(measurements::gpuLoad, gpuLoadID);
        strcpy(arduinoCache::gpuLoad, measurements::gpuLoad);
    }

    if (strcmp(measurements::gpuTemp, arduinoCache::gpuTemp) != 0) {
        _sendSerial(measurements::gpuTemp, gpuTempID);
        strcpy(arduinoCache::gpuTemp, measurements::gpuTemp);
    }


    // Send alive ping if nothing was written in the last 5 secs to prevent Connection Lost screen from showing
    if (chrono::steady_clock::now() - lastWriteTime > chrono::milliseconds(5000)) {
        //cout << "Sending alive ping!" << endl;

        _sendSerial("", pingID);
    }
}
