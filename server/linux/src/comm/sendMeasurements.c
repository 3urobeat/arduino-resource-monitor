/*
 * File: printMeasurements.c
 * Project: arduino-resource-monitor
 * Created Date: 2023-01-24 17:41:01
 * Author: 3urobeat
 *
 * Last Modified: 2025-12-16 20:48:25
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "comm.h"


// Specifies the IDs (min 0, max 9) which data messages are prefixed with to indicate their type
typedef enum {
    pingID      = 0, // Empty data message only used by server for preventing connection loss screen
    cpuLoadID   = 1,
    cpuTempID   = 2,
    ramUsageID  = 3,
    swapUsageID = 4,
    gpuLoadID   = 5,
    gpuTempID   = 6
} MeasurementPrefixIDsType;


// Create storage for caching what we last sent to the Arduino to avoid unnecessary refreshes
struct MeasurementTypes arduinoCache;

// Track time to decide if we have to send an alive ping so the Arduino doesn't display the 'Lost Connection!' screen
clock_t lastWriteTime = 0;


char sendTempStr[32];

/**
 * Sends the actual serial message
 */
void _sendSerial(const char *str, MeasurementPrefixIDsType id)
{
    memset(sendTempStr, '\0', sizeof(sendTempStr));

    // Construct string to send
    sendTempStr[0] = '~';      // Data prefix
    sendTempStr[1] = id + '0'; // Simple hack to "convert" id to char. Only supports id <= 10 ofc
    sendTempStr[2] = '-';      // Add a separator

    strncat(sendTempStr, str, sizeof(sendTempStr) - 5); // 5 because of prefix char, type char, separator, end delimiter and null byte
    strcat(sendTempStr, "#");

    // Delay sending message if necessary to give Arduino time to process previous message, cutie is a little sloow
    const clock_t lastWriteDiff = clock() - lastWriteTime;

    if (lastWriteDiff < 500)
    {
        logDebug("_sendSerial: Delaying sending message by %ims to give Arduino processing time", (500 - lastWriteDiff))
        usleep((500 - lastWriteDiff) * 1000);
    }

    // Send content (team yippee)
    if (!serialWrite(sendTempStr, strlen(sendTempStr)))
    {
        reconnect();
        return;
    }

    logDebug("Sending (%d): %s", strlen(sendTempStr), sendTempStr)

    // Refresh lastWriteTime
    lastWriteTime = clock();
}


/**
 * Send updated measurements to the Arduino
 */
void sendMeasurements()
{
    // Send what changed
    if (strcmp(measurements.cpuLoad, arduinoCache.cpuLoad) != 0) {
        _sendSerial(measurements.cpuLoad, cpuLoadID);
        strcpy(arduinoCache.cpuLoad, measurements.cpuLoad);
    }

    if (strcmp(measurements.cpuTemp, arduinoCache.cpuTemp) != 0) {
        _sendSerial(measurements.cpuTemp, cpuTempID);
        strcpy(arduinoCache.cpuTemp, measurements.cpuTemp);
    }

    if (strcmp(measurements.ramUsage, arduinoCache.ramUsage) != 0) {
        _sendSerial(measurements.ramUsage, ramUsageID);
        strcpy(arduinoCache.ramUsage, measurements.ramUsage);
    }

    if (strcmp(measurements.swapUsage, arduinoCache.swapUsage) != 0) {
        _sendSerial(measurements.swapUsage, swapUsageID);
        strcpy(arduinoCache.swapUsage, measurements.swapUsage);
    }

    if (strcmp(measurements.gpuLoad, arduinoCache.gpuLoad) != 0) {
        _sendSerial(measurements.gpuLoad, gpuLoadID);
        strcpy(arduinoCache.gpuLoad, measurements.gpuLoad);
    }

    if (strcmp(measurements.gpuTemp, arduinoCache.gpuTemp) != 0) {
        _sendSerial(measurements.gpuTemp, gpuTempID);
        strcpy(arduinoCache.gpuTemp, measurements.gpuTemp);
    }


    // Send alive ping if nothing was written in the last 5 secs to prevent Connection Lost screen from showing
    if (clock() - lastWriteTime > 5000) {
        logDebug("Sending alive ping!");

        _sendSerial("", pingID);
    }
}


/**
 * Logs the current measurements to stdout instead of sending them to the Client
 */
void logMeasurements()
{
    printf("CPU: %s%% %s°C\n", measurements.cpuLoad, measurements.cpuTemp);
    printf("RAM: %sGB %sGB\n", measurements.ramUsage, measurements.swapUsage);
    printf("GPU: %s%% %s°C\n", measurements.gpuLoad, measurements.gpuTemp);
}


/**
 * Resets the arduinoCache struct. This causes sendMeasurements() to resend every measurement.
 */
void resetCache()
{
    logDebug("Resetting arduinoCache...");

    memset(arduinoCache.cpuLoad,   0, dataSize);
    memset(arduinoCache.cpuTemp,   0, dataSize);
    memset(arduinoCache.ramUsage,  0, dataSize);
    memset(arduinoCache.swapUsage, 0, dataSize);
    memset(arduinoCache.gpuLoad,   0, dataSize);
    memset(arduinoCache.gpuTemp,   0, dataSize);
}
