/*
 * File: getMeasurements.c
 * Project: arduino-resource-monitor
 * Created Date: 2023-01-24 17:40:48
 * Author: 3urobeat
 *
 * Last Modified: 2025-12-16 21:15:19
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "sensors.h"


// Stores all current measurements
struct MeasurementTypes measurements;


// Persistent data for _getCpuLoad()
long lastCpuRawNonIdle = 0;
long lastCpuRawTotal   = 0;

char getCpuLoadBuffer[64] = "";

/**
 * Calculates the current CPU utilization
 */
void _getCpuLoad()
{
    // Read '/proc/stat'
    getFileContent(getCpuLoadBuffer, sizeof(getCpuLoadBuffer), "/proc/stat", '\n');

    size_t bytes_read = strlen(getCpuLoadBuffer);

    getCpuLoadBuffer[bytes_read]     = ' ';  // Replace delimiter (included in result from getdelim()) with a space, so that the last value will be processed in the loop below
    getCpuLoadBuffer[bytes_read + 1] = '\0'; // Make sure a null byte exists. This should already be the case but maybe I'm stupid


    // Split procStatContentBuffer at spaces at calc values (rowTitle, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice - see 1.8 @ https://www.kernel.org/doc/Documentation/filesystems/proc.txt)
    long nonIdle  = 0;
    long total    = 0;

    int  matches  = 0; // Zero-Based counter of matches, starting with 'user'

    char *valuePtr      = getCpuLoadBuffer + 5; // Point to the start of the first element, skip title column
    char *valueStartPtr = valuePtr;

    while (*valuePtr) // Behold: A custom implementation for splitting a char array at spaces. I tried dealing with strtok_r() and decided to do it myself.
    {
        if (*valuePtr == ' ')
        {
            *valuePtr = '\0'; // Replace this space with a null-byte. This makes atoi() read from the first char of the current value up until this space (now null-byte)

            // Add this value to nonIdle (if not column idle or iowait) and always add to total
            if (matches != 3 && matches != 4) nonIdle += atoi(valueStartPtr);
            total += atoi(valueStartPtr);

            valueStartPtr = valuePtr + 1; // +1 to get "over" space and point to the first char of the next value
            matches++;
        }

        valuePtr++;
    }


    // Calculate cpu load using new and previous measurement (if one exists)
    if (lastCpuRawNonIdle > 0 && lastCpuRawTotal > 0)
    {
        float cpuLoad = ((lastCpuRawNonIdle - nonIdle) * 100.0) / (lastCpuRawTotal - total);
        cpuLoad = abs(round(cpuLoad));          // Round and absolute to remove minus if result is 0.000

        gcvt(cpuLoad, 3, measurements.cpuLoad); // Restrict to 3 digits (0-100%) and write into cpuLoad
    }

    // Update lastCpuRawMeasurement with raw data
    lastCpuRawNonIdle = nonIdle;
    lastCpuRawTotal   = total;
}


// Persistent data for _getMemSwapUsage()
char getMemSwapUsageBuffer[2048] = "";

/**
 * Processes output from '/proc/meminfo' to get mem & swap usage
 */
void _getMemSwapUsage()
{
    // Read '/proc/meminfo'
    getFileContentFull(getMemSwapUsageBuffer, sizeof(getMemSwapUsageBuffer), "/proc/meminfo");


    // Iterate through rows and find MemTotal, MemAvailable, SwapTotal & SwapFree
    int memTotal, memAvailable, swapTotal, swapFree;
    char *lineStartPtr = getMemSwapUsageBuffer;
    char *bufferPtr    = getMemSwapUsageBuffer;

    while(*bufferPtr)
    {
        if (*bufferPtr == '\n' || *bufferPtr == '\0')
        {
            *(bufferPtr - 3) = '\0'; // Place nullbyte in front of 'kB', present at the end of this line. This makes the following functions read from the first char of this line up until the end of the number listed in this line.

            strtok(lineStartPtr, " "); // Cut line into pieces, separated by spaces. After this call, lineStartPtr points to the beginning of the first element, aka the line title.

            // See if this line starts with one of the interesting values
            if (strStartsWith("MemTotal", lineStartPtr)) {
                lineStartPtr = strtok(NULL, " "); // Gets the second match, which is our number
                memTotal = atoi(lineStartPtr);
            }
            else if (strStartsWith("MemAvailable", lineStartPtr))
            {
                lineStartPtr = strtok(NULL, " ");
                memAvailable = atoi(lineStartPtr);
            }
            else if (strStartsWith("SwapTotal", lineStartPtr))
            {
                lineStartPtr = strtok(NULL, " ");
                swapTotal = atoi(lineStartPtr);
            }
            else if (strStartsWith("SwapFree", lineStartPtr))
            {
                lineStartPtr = strtok(NULL, " ");
                swapFree = atoi(lineStartPtr);
            }

            lineStartPtr = bufferPtr + 1; // +1 to get "over" newline and point to the first char of the next line
        }

        bufferPtr++;
    }


    // Calculate used RAM & Swap, convert to GB, format and write into measurements
    float mem  = (memTotal - memAvailable) / 1000000.0;
    float swap = (swapTotal - swapFree)    / 1000000.0;

    floatToFixedLengthStr(measurements.ramUsage, mem);

    if (swapTotal > 0) // Is Swap enabled?
    {
        if (swap < 0.01) // Do not try to convert values below 10 MB (0.01 GB)
        {
            strcpy(measurements.swapUsage, "0.00");
        }
        else
        {
            floatToFixedLengthStr(measurements.swapUsage, swap);
        }
    }
}


// Persistent data for getMeasurements()
char buffer[16] = "";

/**
 * Retreives new data and updates measurements
 */
void getMeasurements()
{
    logDebug("Updating sensor values...");


    // Get CPU load & temp
    _getCpuLoad();

    if (sensorPaths.cpuTemp[0] != '\0') // Check if a sensor was found before attempting to use it
    {
        getFileContentFull(buffer, sizeof(buffer), sensorPaths.cpuTemp);
        gcvt(atoi(buffer) / 1000, 3, measurements.cpuTemp); // Buffer to int, divide by 1000 (sensors report 50°C as 50000), round, restrict to 3 digits (0-100°C) and write into cpuTemp
    }


    // Get RAM and Swap usage
    _getMemSwapUsage();


    // Get GPU load & temp
    if (config.gpuType == NVIDIA) // TODO: I do not know yet if Nvidia GPUs can be read through the fs, therefore we're still using the old method here
    {
        getCmdStdout(measurements.gpuLoad, sizeof(measurements.gpuLoad), "nvidia-settings -q GPUUtilization -t | awk -F '[,= ]' '{ print $2 }'"); // awk cuts response down to only the graphics parameter

        getCmdStdout(measurements.gpuTemp, sizeof(measurements.gpuTemp), "nvidia-settings -q GPUCoreTemp -t");
    }
    else
    {
        if (sensorPaths.gpuLoad[0] != '\0') // Check if a sensor was found before attempting to use it
        {
            getFileContentFull(measurements.gpuLoad, sizeof(dataSize), sensorPaths.gpuLoad); // Sensor 'gpu_busy_percent' returns value straight up like it is
        }

        if (sensorPaths.gpuTemp[0] != '\0') // Check if a sensor was found before attempting to use it
        {
            getFileContentFull(buffer, sizeof(buffer), sensorPaths.gpuTemp);
            gcvt(atoi(buffer) / 1000, 3, measurements.gpuTemp); // Buffer to int, divide by 1000 (sensors report 50°C as 50000), round, restrict to 3 digits (0-100°C) and write into cpuTemp
        }
    }
}
