/*
 * File: getMeasurements.cpp
 * Project: arduino-resource-monitor
 * Created Date: 24.01.2023 17:40:48
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-19 17:49:33
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"

#include <dirent.h>
#include <errno.h>


// Stores all current measurements
namespace measurements {
    char cpuLoad[dataSize] = "/"; // Init with '/' because it takes 2 measurements to display
    char cpuTemp[dataSize] = "";
    char ramUsage[dataSize] = "";
    char swapUsage[dataSize] = "";
    char gpuLoad[dataSize] = "";
    char gpuTemp[dataSize] = "";
};


// Command responses sometimes have a trailing line break, this function removes them
void _removeLineBreak(char *str)
{
    if (str[strlen(str) - 1] == '\n') {
        char buf[16] = "";
        strncat(buf, str, strlen(str) - 1);
        strcpy(str, buf);
    }
}


// Function to run command and get output
void getStdoutFromCommand(char *dest, const char *cmd) // https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/  (converted from string to char arrs by me)
{
    FILE * stream;

    const int max_buffer = 256;
    char buffer[max_buffer];

    char tempCmd[256];
    strncpy(tempCmd, cmd, 250);
    strcat(tempCmd, " 2>&1");

    stream = popen(cmd, "r");

    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) strcat(dest, buffer);

        _removeLineBreak(dest);
        pclose(stream);
    }
}


// Persistent data for _getSensorFileContent()
FILE *sensorFileP         = nullptr;
char *sensorContentBuffer = NULL; // TODO: Change to fixed size

/**
 * Reads the content of a file at path until encountering delim, copies it into dest and returns amount of bytes read (includes delim character)
 */
ssize_t _getSensorFileContent(char *dest, int size, const char *path, const char delim = '\0')
{
    // Read path
    errno = 0;
    sensorFileP = fopen(path, "r");

    if (!sensorFileP)
    {
        printf("Error: Failed to read sensor '%s'! Error: %s", path, strerror(errno));
        return 0;
    }

    size_t contentLen  = 0;
    ssize_t bytes_read = getdelim(&sensorContentBuffer, &contentLen, delim, sensorFileP); // Read only the first line as it contains the aggregate of all threads - https://stackoverflow.com/a/174743

    (void) fclose(sensorFileP);

    // Move null terminator in contentBuffer by 1 byte if the last char is a newline (this was always the case in my testing)
    if (sensorContentBuffer[bytes_read - 1] == '\n') sensorContentBuffer[bytes_read - 1] = '\0';


    // Copy data into dest
    strncpy(dest, sensorContentBuffer, size);

    logDebug("_getSensorFileContent(): Read '%s' from '%s'", sensorContentBuffer, path);


    // Clean up
    //free(procStatContentBuffer);  // This should *theoretically* not be needed here, as we always read content of roughly the same length and getdelim() should reuse the allocated memory
    //procStatContentBuffer = NULL;

    return bytes_read;
}


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
    ssize_t bytes_read = _getSensorFileContent(getCpuLoadBuffer, sizeof(getCpuLoadBuffer), "/proc/stat", '\n');

    getCpuLoadBuffer[bytes_read - 1] = ' '; // Replace delimiter (included in result from getdelim()) with a space, so that the last value will be processed in the loop below


    // Split procStatContentBuffer at spaces at calc values (rowTitle, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice - see 1.8 @ https://www.kernel.org/doc/Documentation/filesystems/proc.txt)
    long nonIdle  = 0;
    long total    = 0;
    int   matches = 0;                         // Zero-Based counter of matches, starting with 'user'
    char *valueStartPtr = getCpuLoadBuffer + 5; // Point to the start of the first element

    for (int i = 5; i < bytes_read; i++) // Start after row title 'cpu  '           // TODO: This would be cooler (and maybe faster) using pointers instead of accessing array index
    {                                    // Behold: A custom implementation for splitting a char array at spaces. I tried dealing with strtok_r() and decided to do it myself.
        if (getCpuLoadBuffer[i] == ' ')
        {
            getCpuLoadBuffer[i] = '\0'; // Replace this space with a null-byte. This makes atoi() read from the first char of the current value up until this space (now null-byte)

            // Add this value to nonIdle (if not column idle or iowait) and always add to total
            if (matches != 3 && matches != 4) nonIdle += atoi(valueStartPtr);
            total += atoi(valueStartPtr);

            valueStartPtr = &getCpuLoadBuffer[i + 1]; // +1 to get "over" space and point to the first char of the next value
            matches++;
        }
    }


    // Calculate cpu load using new and previous measurement (if one exists)
    if (lastCpuRawNonIdle > 0 && lastCpuRawTotal)
    {
        float cpuLoad = ((lastCpuRawNonIdle - nonIdle) * 100.0) / (lastCpuRawTotal - total);

        gcvt(round(cpuLoad), 3, measurements::cpuLoad); // Round float, restrict to 3 digits (0-100%) and write into cpuLoad
    }

    // Update lastCpuRawMeasurement with raw data
    lastCpuRawNonIdle = nonIdle;
    lastCpuRawTotal   = total;
}


/**
 * Retreives measurements by running commands
 */
void getMeasurements()
{
    char buffer[16] = "";

    logDebug("Updating sensor values...");


    // Get CPU load & temp
    _getCpuLoad();

    _getSensorFileContent(buffer, sizeof(buffer), sensorPaths::cpuTemp);
    gcvt(atoi(buffer) / 1000, 3, measurements::cpuTemp); // Buffer to int, divide by 1000 (sensors report 50°C as 50000), round, restrict to 3 digits (0-100°C) and write into cpuTemp


    // Get RAM and Swap usage
    char ramUsageTemp[dataSize] = "";
    getStdoutFromCommand(ramUsageTemp, "free -m | awk -v c=\\'used\\' 'NR==1 {for (i=1; i<=NF; i++) if ($i==c) break}''{print $(i-4)}' | head -2 | tail -1 | awk '{print $1/1000}'"); // awk converts MB to GB here

    for (int i = 0; i < 4; i++) { // Use a stupid for loop to avoid doing dtoa() stuff
        measurements::ramUsage[i] = ramUsageTemp[i];

        // If dot was reached, add one decimal and break loop
        if (ramUsageTemp[i] == '.') {
            measurements::ramUsage[i + 1] = ramUsageTemp[i + 1];
            measurements::ramUsage[i + 2] = '\0';
            break;
        }
    }


    char swapUsageTemp[dataSize] = "";
    getStdoutFromCommand(swapUsageTemp, "free -m | awk -v c=\\'used\\' 'NR==1 {for (i=1; i<=NF; i++) if ($i==c) break}''{print $(i-4)}' | tail -1 | awk '{print $1/1000}'"); // awk converts MB to GB here

    for (int i = 0; i < 4; i++) { // Use a stupid for loop to avoid doing dtoa() stuff
        measurements::swapUsage[i] = swapUsageTemp[i];

        // If dot was reached, add one decimal and break loop
        if (swapUsageTemp[i] == '.') {
            measurements::swapUsage[i + 1] = swapUsageTemp[i + 1];
            measurements::swapUsage[i + 2] = '\0';
            break;
        }
    }


    // Get GPU load & temp
    _getSensorFileContent(measurements::gpuLoad, sizeof(dataSize), sensorPaths::gpuLoad); // Sensor 'gpu_busy_percent' returns value straight up like it is

    _getSensorFileContent(buffer, sizeof(buffer), sensorPaths::gpuTemp);
    gcvt(atoi(buffer) / 1000, 3, measurements::gpuTemp); // Buffer to int, divide by 1000 (sensors report 50°C as 50000), round, restrict to 3 digits (0-100°C) and write into cpuTemp

    // Old readout using external binaries. Maybe we need it again in the future? I can't test nvidia & intel or older amd gpus at the moment
    /* #if gpuType == 0
        getStdoutFromCommand(measurements::gpuLoad, "nvidia-settings -q GPUUtilization -t | awk -F '[,= ]' '{ print $2 }'"); // awk cuts response down to only the graphics parameter

        getStdoutFromCommand(measurements::gpuTemp, "nvidia-settings -q GPUCoreTemp -t");
    #elif gpuType == 1
        getStdoutFromCommand(measurements::gpuLoad, gpuUtilCmd);
        gcvt(round(atof(measurements::gpuLoad)), 3, measurements::gpuLoad); // Convert to float, floor, restrict digits to max 3 and convert float back to char arr

        getStdoutFromCommand(measurements::gpuTemp, gpuTempCmd);
        gcvt(round(atof(measurements::gpuTemp)), 3, measurements::gpuTemp); // Convert to float, floor, restrict digits to max 3 and convert float back to char arr
    #else
        strcpy(measurements::gpuLoad, "/");
        strcpy(measurements::gpuTemp, "/");
    #endif */
}
