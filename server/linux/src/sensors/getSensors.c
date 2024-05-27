/*
 * File: getSensors.c
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-18 13:48:34
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-26 22:37:37
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "sensors.h"

#include <dirent.h>
#include <errno.h>


// Stores filesystem paths for all sensors we've found
struct SensorTypes sensorPaths;


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


// Persistent data for _processSensorName()
bool cpuTempAutoDiscovered = false; // Used for printing warning when multiple CPU/GPUs were auto-discovered
bool gpuLoadAutoDiscovered = false;
bool gpuTempAutoDiscovered = false;


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


    // HwMon CPU Temp: Check if sensor matches a known name
    if (strStartsWith("k10temp", sensorName)         // AMD
        || strStartsWith("coretemp", sensorName)     // Intel
        || strStartsWith("cpu_thermal", sensorName)) // Raspberry Pi
    {
        if (strlen(sensorPaths.cpuTemp) == 0) // Check if user already configured this sensor
        {
            strcpy(sensorPaths.cpuTemp, sensorPath);
            strcat(sensorPaths.cpuTemp, "/temp1_input");

            // Attempt to open to check if it exists. Log success message or reset sensor path on failure
            bool sensorPathExists = _fileExists(sensorPaths.cpuTemp);

            if (sensorPathExists)
            {
                printf("Found CPU Temperature sensor '%s' at '%s'!\n", sensorPaths.cpuTemp, sensorName);
                cpuTempAutoDiscovered = true;
            }
            else strcpy(sensorPaths.cpuTemp, "");
        }
        else
        {
            if (cpuTempAutoDiscovered) printf("Warning: Your system has multiple CPU hwmon's! If you have multiple CPUs and the wrong chip's temperature sensor has been chosen, please configure it manually.\n");
        }
    }

    // ThermalZone CPU Temp: Check if sensor matches a known name
    if (strStartsWith("CPU-therm", sensorName)) // Nvidia Jetson Nano
    {
        if (strlen(sensorPaths.cpuTemp) == 0) // Check if user already configured this sensor
        {
            strcpy(sensorPaths.cpuTemp, sensorPath);
            strcat(sensorPaths.cpuTemp, "/temp");

            // Attempt to open to check if it exists. Log success message or reset sensor path on failure
            bool sensorPathExists = _fileExists(sensorPaths.cpuTemp);

            if (sensorPathExists)
            {
                printf("Found CPU Temperature sensor '%s' at '%s'!\n", sensorPaths.cpuTemp, sensorName);
                cpuTempAutoDiscovered = true;
            }
            else strcpy(sensorPaths.cpuTemp, "");
        }
        else
        {
            if (cpuTempAutoDiscovered) printf("Warning: Your system has multiple CPU hwmon's! If you have multiple CPUs and the wrong chip's temperature sensor has been chosen, please configure it manually.\n");
        }
    }

    // HwMon GPU Temp: Check if sensor matches a known name
    if (strStartsWith("amdgpu", sensorName) && gpuType == 0) // AMD || Nvidia GPU // TODO: I don't know how nvidia sensors are called
    {
        if (strlen(sensorPaths.gpuLoad) == 0) // Check if user already configured this sensor
        {
            strcpy(sensorPaths.gpuLoad, sensorPath);
            strcat(sensorPaths.gpuLoad, "/device/gpu_busy_percent"); // TODO: Does this exist for NVIDIA cards?

            // Attempt to open to check if it exists. Log success message or reset sensor path on failure
            bool sensorPathExists = _fileExists(sensorPaths.gpuLoad);

            if (sensorPathExists)
            {
                printf("Found GPU Load sensor '%s' at '%s'!\n", sensorPaths.gpuLoad, sensorName);
                gpuLoadAutoDiscovered = true;
            } else strcpy(sensorPaths.gpuLoad, "");
        }
        else
        {
            if (gpuLoadAutoDiscovered) printf("Warning: Your system has multiple GPU hwmon's! If you have multiple GPUs and the wrong card's load sensor has been chosen, please configure it manually.\n");
        }

        if (strlen(sensorPaths.gpuTemp) == 0) // Check if user already configured this sensor
        {
            strcpy(sensorPaths.gpuTemp, sensorPath);
            strcat(sensorPaths.gpuTemp, "/temp1_input");

            // Attempt to open to check if it exists. Log success message or reset sensor path on failure
            bool sensorPathExists = _fileExists(sensorPaths.gpuTemp);

            if (sensorPathExists)
            {
                printf("Found GPU Temperature sensor '%s' at '%s'!\n", sensorPaths.gpuTemp, sensorName);
                gpuTempAutoDiscovered = true;
            } else strcpy(sensorPaths.gpuTemp, "");
        }
        else
        {
            if (gpuTempAutoDiscovered) printf("Warning: Your system has multiple GPU hwmon's! If you have multiple GPUs and the wrong card's load sensor has been chosen, please configure it manually.\n");
        }
    }

    // ThermalZone GPU Temp: Check if sensor matches a known name
    if (strStartsWith("GPU-therm", sensorName)) // Nvidia Jetson Nano
    {
        if (strlen(sensorPaths.gpuTemp) == 0) // Check if user already configured this sensor
        {
            strcpy(sensorPaths.gpuTemp, sensorPath);
            strcat(sensorPaths.gpuTemp, "/temp");

            // Attempt to open to check if it exists. Log success message or reset sensor path on failure
            bool sensorPathExists = _fileExists(sensorPaths.gpuTemp);

            if (sensorPathExists)
            {
                printf("Found GPU Temperature sensor '%s' at '%s'!\n", sensorPaths.gpuTemp, sensorName);
                gpuTempAutoDiscovered = true;
            } else strcpy(sensorPaths.gpuTemp, "");
        }
        else
        {
            if (gpuTempAutoDiscovered) printf("Warning: Your system has multiple GPU hwmon's! If you have multiple GPUs and the wrong card's load sensor has been chosen, please configure it manually.\n");
        }
    }
}


/**
 * Attempts to find relevant sensors in '/sys/class/hwmon'
 */
void _findHwmonSensors()
{
    const char *hwmonDirStr = "/sys/class/hwmon/";


    // Get all dirs in '/sys/class/hwmon/'
    DIR *hwmonDirP;
    struct dirent *ep;
    hwmonDirP = opendir(hwmonDirStr);

    if (hwmonDirP == NULL) printf("Error: Failed to open '/sys/class/hwmon/' to probe all sensors!\n");


    // Collect all valid 'hwmon*' directories and check their 'name' files
    FILE *hwmonNameFileP = NULL;
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

            free(contentBuffer);
        }
    }

    (void) closedir(hwmonDirP);
}


/**
 * Attempts to find relevant sensors in '/sys/devices/virtual/thermal/'
 */
void _findThermalZoneSensors()
{
    const char *thermalDirStr = "/sys/devices/virtual/thermal/";


    // Get all dirs in '/sys/devices/virtual/thermal/'
    DIR *thermalDirP;
    struct dirent *ep;
    thermalDirP = opendir(thermalDirStr);

    if (thermalDirP == NULL) printf("Error: Failed to open '/sys/devices/virtual/thermal/' to probe all sensors!\n");


    // Collect all valid 'thermal_zone*' directories and check their 'name' files
    FILE *thermalNameFileP = NULL;
    char pathBuffer[128] = "";

    while ((ep = readdir(thermalDirP)) != NULL)
    {
        if (strncmp(ep->d_name, "thermal_zone", 12) == 0)
        {
            // Construct path
            strcpy(pathBuffer, thermalDirStr);   // Reset content to base string
            strncat(pathBuffer, ep->d_name, 16); // Append directory name, limit to 16 chars
            strcat(pathBuffer, "/type\0");       // Append '/type', the file we are interested in

            logDebug("Constructed thermal_zone type path '%s'. Attempting to open...", pathBuffer);


            // Attempt to open path and read its content
            errno = 0;
            thermalNameFileP = fopen(pathBuffer, "r");

            char *contentBuffer = NULL; // TODO: Is this cool? I'd rather use reusable buffer with a fixed size (I think)
            size_t contentLen   = 0;
            ssize_t bytes_read  = -1;

            if (thermalNameFileP)
            {
                logDebug("File opened, attempting to read...");

                // Read content into contentBuffer, close file again when done
                bytes_read = getdelim(&contentBuffer, &contentLen, '\0', thermalNameFileP); // https://stackoverflow.com/a/174743

                fclose(thermalNameFileP);
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

            free(contentBuffer);
        }
    }

    (void) closedir(thermalDirP);
}


/**
 * Attempts to discover sensor paths and populates variables in sensorPaths
 */
void getSensors()
{
    printf("Attempting to discover hardware sensors...\n");

    // Find CPU & GPU sensors
    _findHwmonSensors();

    // Couldn't find all sensors in hwmon? Check thermal_zone next, some ARM devices (like the Nvidia Jetson Nano) use that instead
    if (strlen(sensorPaths.cpuTemp) == 0 || strlen(sensorPaths.gpuLoad) == 0 || strlen(sensorPaths.gpuTemp) == 0)
    {
        logDebug("Didn't find all sensors in hwmon directory, searching thermal_zones next...");
        _findThermalZoneSensors();
    }


    // Log warnings for missing sensors and write '/' into respective measurements variable
    if (strlen(sensorPaths.cpuTemp) == 0)
    {
        printf("Warn: I could not automatically find any 'CPU Temperature' sensor! If you have one, please configure it manually.\n");
        strcpy(measurements.cpuTemp, "/");
    }

    if (strlen(sensorPaths.gpuLoad) == 0 && gpuType == 0)
    {
        printf("Warn: I could not automatically find any 'GPU Load' sensor! If you have one, please configure it manually.\n");
        strcpy(measurements.gpuLoad, "/");
    }

    if (strlen(sensorPaths.gpuTemp) == 0 && gpuType == 0)
    {
        printf("Warn: I could not automatically find any 'GPU Temperature' sensor! If you have one, please configure it manually.\n");
        strcpy(measurements.gpuTemp, "/");
    }

    printf("\n");
}
