/*
 * File: server.h
 * Project: arduino-resource-monitor
 * Created Date: 2023-01-24 17:56:00
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-27 22:50:26
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once


// Include library headers
#include <stdio.h>   // printf, fopen, gcvt, getdelim...
#include <stdint.h>  // uint32_t, ...
#include <stdlib.h>  // exit, atoi, ...
#include <stdbool.h> // Datatype bool in C
#include <string.h>  // strcat, strcpy, ..
#include <errno.h>   // Error codes
#include <unistd.h>  // sleep, usleep
#include <time.h>    // clock
#include <math.h>    // round


// Include project headers
#include "comm/comm.h"
#include "data/data.h"
#include "helpers/helpers.h"
#include "sensors/sensors.h"


// Configuration variables
#define arduinoReplyTimeout 5000         // How long to wait for an answer from the USB port in ms before giving up
#define connectionRetryTimeout 5000      // How long to wait between attempts to connect again in ms after all USB ports have failed
#define connectionRetryAmount 10         // How often to retry finding a connection
#define connectionRetryMultiplier 0.5    // retry * connectionRetryTimeout * connectionRetryMultiplier

#define checkInterval 1000               // 1 second is lowest value possibe as mpstat takes a second to collect data
#define gpuType 0                        // 0 for automatic discovery (AMD), 1 for Nvidia (nvidia-settings will be used)

#define autoCreateConfig 1               // Should the config be automatically created if it doesn't exist?

#define defaultConfig "lastSeenVersion = " version \
                        "\n\n[timeouts]" \
                        "\narduinoReplyTimeout = 5000" \
                        "\nconnectionRetryTimeout = 5000" \
                        "\nconnectionRetryAmount = 10" \
                        "\nconnectionRetryMultiplier = 0.5" \
                        "\n\n[sensors]" \
                        "\ngpuType = AMD" \
                        "\ncpuTempSensor = " \
                        "\ngpuLoadSensor = " \
                        "\ngpuTempSensor = " \
                        "\ncheckInterval = 1000" \
                        "\n"


// Debug Settings: Force debug and clientLessMode if flag is set, otherwise use your setting
#ifdef BUILD_RELEASE            // Do not modify
    #define printDebug 0
    #define clientLessMode 0
#elif BUILD_RELEASE_CLIENT_LESS // Do not modify
    #define printDebug 0
    #define clientLessMode 1
#else                           // You can modify this
    #define printDebug 0                  // Prints (a lot of) additional information during runtime, useful for debugging.
    #define clientLessMode 0              // Disables connecting & sending data to the Arduino Client and only logs results to stdout. Useful during development
#endif


// Do not modify
#define displayCols 20
#define displayRows 4

#define baud 9600

#define version "v0.7.0"


// Functions implemented in server.c that should be accessible
extern void connect();
extern void reconnect();


// Logs debug messages if enabled
#if printDebug
    #define logDebug(...) \
        printf("[DEBUG] "); \
        printf(__VA_ARGS__, NULL); \
        printf("\n");
#else
    #define logDebug(...);
#endif
