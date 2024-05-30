/*
 * File: data.h
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-26 14:00:50
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-30 23:36:43
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include "../server.h"


// Default configuration data. This is used to create config.toml or when config.toml does not specify a value
#define defaultConfig   "[general]" \
                        "\nlastSeenVersion = \"" version "\"" \
                        "\n\n[timeouts]" \
                        "\narduinoReplyTimeout = 5000" \
                        "\nconnectionRetryTimeout = 5000" \
                        "\nconnectionRetryAmount = 10" \
                        "\nconnectionRetryMultiplier = 0.5" \
                        "\n\n[sensors]" \
                        "\ngpuType = \"amd\"" \
                        "\ncpuTempSensorPath = \"\"" \
                        "\ngpuLoadSensorPath = \"\"" \
                        "\ngpuTempSensorPath = \"\"" \
                        "\ncheckInterval = 1000" \
                        "\n"


// Stores currently imported config
struct ConfigValues {
    // General
    char lastSeenVersion[16];

    // Timeouts
    int arduinoReplyTimeout;       // How long to wait for an answer from the USB port in ms before giving up
    int connectionRetryTimeout;    // How long to wait between attempts to connect again in ms after all USB ports have failed
    int connectionRetryAmount;     // How often to retry finding a connection
    float connectionRetryMultiplier;    // retry * connectionRetryTimeout * connectionRetryMultiplier

    // Sensors
    uint8_t gpuType;                    // 0 for automatic discovery (AMD), 1 for Nvidia (nvidia-settings will be used)
    char cpuTempSensorPath[128];
    char gpuLoadSensorPath[128];
    char gpuTempSensorPath[128];
    int checkInterval;             // 1 second is lowest value possibe as mpstat takes a second to collect data
};

extern struct ConfigValues config;


// Functions to export
extern void importConfigFile();
extern void exportConfigFile();

extern void getCmdStdout(char *dest, int size, const char *cmd);
extern void getFileContent(char *dest, int size, const char *path, const char delim);
#define getFileContentFull(dest, size, path) getFileContent(dest, size, path, '\0') // Overload to omit delimiter and read till null byte
