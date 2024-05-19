/*
 * File: getSensors.cpp
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-18 13:48:34
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-19 17:29:28
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"

#include <dirent.h>
#include <errno.h>


// Stores filesystem paths for all sensors we've found
namespace sensorPaths {
    // cpuLoad does not need a sensor, we read from '/proc/stat'
    char cpuTemp[pathSize] = "";
    char ramUsage[pathSize] = "";
    char swapUsage[pathSize] = "";
    char gpuLoad[pathSize] = "";
    char gpuTemp[pathSize] = "";
};


/**
 * C string startsWith implementation - https://stackoverflow.com/a/4770992
 */
bool _strStartsWith(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}


/**
 * Checks if a directory exists. Outputs error to stdout if directory could not be opened
 */
bool _fileExists(const char *path)
{
    errno = 0;
    FILE* sensorFileP = fopen(path, "r");

    if (sensorFileP)
    {
        (void) fclose(sensorFileP);
        return true;
    }
    else
    {
        printf("Error: Failed to open sensor '%s'! Ignoring it. Error: %s\n", path, strerror(errno));
        return false;
    }
}


/**
 * Checks for known sensor names and populates sensorPaths
 */
void _processSensorName(const char *sensorPath, const char *sensorName)
{
    // Check if path is too long to store and ignore it
    if (strlen(sensorPath) > pathSize - 16)
    {
        logDebug("Warn: Path of sensor name '%s (%s)' is too long (%i chars) to store! Ignoring it...\n", sensorName, sensorPath, strlen(sensorPath));
    }




    // CPU Temp: Check if sensor matches a known name
    if (_strStartsWith(sensorName, "k10temp") || _strStartsWith(sensorName, "coretemp")) // AMD || Intel CPU
    {
        if (strlen(sensorPaths::cpuTemp) == 0) // Check if user already configured this sensor
        {
            strcpy(sensorPaths::cpuTemp, sensorPath);
            strcat(sensorPaths::cpuTemp, "/temp1_input");

            // Attempt to open to check if it exists. Log success message or reset sensor path on failure
            bool sensorPathExists = _fileExists(sensorPaths::cpuTemp);

            if (sensorPathExists) printf("Found CPU Temperature sensor '%s' at '%s'!\n", sensorPaths::cpuTemp, sensorName);
                else strcpy(sensorPaths::cpuTemp, "");
        }
    }

    // GPU Temp: Check if sensor matches a known name
    if (_strStartsWith(sensorName, "amdgpu") || _strStartsWith(sensorName, "nvidia")) // AMD || Nvidia GPU // TODO: I don't know how nvidia sensors are called
    {
        if (strlen(sensorPaths::gpuLoad) == 0) // Check if user already configured this sensor
        {
            strcpy(sensorPaths::gpuLoad, sensorPath);
            strcat(sensorPaths::gpuLoad, "/device/gpu_busy_percent"); // TODO: Does this exist for NVIDIA cards?

            // Attempt to open to check if it exists. Log success message or reset sensor path on failure
            bool sensorPathExists = _fileExists(sensorPaths::gpuLoad);

            if (sensorPathExists) printf("Found GPU Load sensor '%s' at '%s'!\n", sensorPaths::gpuLoad, sensorName);
                else strcpy(sensorPaths::gpuLoad, "");
        }

        if (strlen(sensorPaths::gpuTemp) == 0) // Check if user already configured this sensor
        {
            strcpy(sensorPaths::gpuTemp, sensorPath);
            strcat(sensorPaths::gpuTemp, "/temp1_input");

            // Attempt to open to check if it exists. Log success message or reset sensor path on failure
            bool sensorPathExists = _fileExists(sensorPaths::gpuTemp);

            if (sensorPathExists) printf("Found GPU Temperature sensor '%s' at '%s'!\n", sensorPaths::gpuTemp, sensorName);
                else strcpy(sensorPaths::gpuTemp, "");
        }
    }
}


/**
 * Attempts to find CPU & GPU sensors in '/sys/class/hwmon'
 */
void _findCpuGpuSensors()
{
    const char *hwmonDirStr = "/sys/class/hwmon/";


    // Get all dirs in '/sys/class/hwmon/'
    DIR *hwmonDirP;
    struct dirent *ep;
    hwmonDirP = opendir(hwmonDirStr);

    if (hwmonDirP == NULL) printf("Error: Failed to open '/sys/class/hwmon/' to probe all sensors!");


    // Collect all valid 'hwmon*' directories and check their 'name' files
    FILE *hwmonNameFileP = nullptr;
    char pathBuffer[128] = "";

    while ((ep = readdir(hwmonDirP)) != NULL)
    {
        if (strncmp(ep->d_name, "hwmon", 5) == 0)
        {
            // Construct path
            strcpy(pathBuffer, hwmonDirStr);     // Reset content to base string
            strncat(pathBuffer, ep->d_name, 16); // Append directory name, limit to 16 chars
            strcat(pathBuffer, "/name\0");       // Append '/name', the file we are interested in

            logDebug("Constructed hwmon name path '%s'. Attempting to open...", pathBuffer);


            // Attempt to open path and read its content
            errno = 0;
            hwmonNameFileP = fopen(pathBuffer, "r");

            char *contentBuffer = NULL; // TODO: Is this cool? I'd rather use reusable buffer with a fixed size (I think)
            size_t contentLen   = 0;
            ssize_t bytes_read  = -1;

            if (hwmonNameFileP)
            {
                logDebug("File opened, attempting to read...");

                // Read content into contentBuffer, close file again when done
                bytes_read = getdelim(&contentBuffer, &contentLen, '\0', hwmonNameFileP); // https://stackoverflow.com/a/174743

                fclose(hwmonNameFileP);
            }
            else
            {
                logDebug("Failed to open/read file! Error: %s", strerror(errno));
            }


            // If something was read, check if sensor has an interesting name
            if (bytes_read != -1)
            {
                // Move null terminator in contentBuffer by 1 byte if the last char is a newline (this was always the case in my testing)
                if (contentBuffer[bytes_read - 1] == '\n') contentBuffer[bytes_read - 1] = '\0';

                // Move null terminator in pathBuffer to before '/name' to cut it off and pass _processSensorName() the "raw" path
                pathBuffer[strlen(pathBuffer) - 5] = '\0';


                // Let _processSensorName() take a look at it
                logDebug("Found sensor at '%s' with name '%s'! Checking if we care about it...", pathBuffer, contentBuffer);

                _processSensorName(pathBuffer, contentBuffer);
            }
            else
            {
                logDebug("Error: Sensor '%s' name has no content!", ep->d_name);
            }
        }
    }

    (void) closedir(hwmonDirP);
}


/**
 * Attempts to discover sensor paths and populates variables in sensorPaths. Returns boolean if every sensor was found and execution can continue.
 */
bool getSensors()
{
    printf("Attempting to discover hardware sensors...\n");

    // Find CPU & GPU sensors
    _findCpuGpuSensors();


    // Find RAM & Swap


    // Disable GPU if not found, this can happen on older iGPUs and therefore should not prevent application from starting
    if (strlen(sensorPaths::gpuLoad) == 0) strcpy(sensorPaths::gpuLoad, "/");
    if (strlen(sensorPaths::gpuTemp) == 0) strcpy(sensorPaths::gpuTemp, "/");

    // Check if a sensor path is empty and return false
    if (strlen(sensorPaths::cpuTemp)   == 0 ||
        strlen(sensorPaths::ramUsage)  == 0 ||
        strlen(sensorPaths::swapUsage) == 0)
    {
        return false;
    }

    return true; // Success
}
