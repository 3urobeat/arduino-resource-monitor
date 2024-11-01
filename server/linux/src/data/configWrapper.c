/*
 * File: configWrapper.c
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-26 11:19:03
 * Author: 3urobeat
 *
 * Last Modified: 2024-10-02 22:27:54
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "data.h"

#include <sys/stat.h>
#include <toml.c>


#define configDir ".config/arduino-resource-monitor/"
#define configFile "config.toml"

#define mkdirCmd "mkdir -p " configDir


char _configDirPath[128] = "";
char _configFilePath[128] = "";
char _configContent[1024] = "";

struct ConfigValues config;


/**
 * Creates config directory and initializes it with the default configuration file if it does not exist yet
 */
void _createDefaultConfig()
{
    // Attempt to create directory, will fail if it already exists. Explicitly checking with stat() beforehand is pretty useless
    mkdir(_configDirPath, 0700);

    // Check if file exists, create and fill with default content if it doesn't
    struct stat st = {0};

    if (stat(_configFilePath, &st) == -1)
    {
        logDebug("_createDefaultConfig(): Config file does not exist yet, creating it...");

        // Attempt to open
        errno = 0;
        FILE *_configFilePtr = fopen(_configFilePath, "w");

        if (!_configFilePtr)
        {
            printf("\033[33mWarn:\033[0m Failed to open stream to config at '%s'! Using default settings. Error: %s\n", _configFilePath, strerror(errno));
            return;
        }

        // Write defaultConfig to config and close stream again
        fprintf(_configFilePtr, defaultConfig);

        (void) fclose(_configFilePtr);
    }
}


void _parseStringConfigEntry(const toml_table_t *arr, const char *key, char *dest, size_t size)
{
    toml_datum_t value = toml_string_in(arr, key);

    if (value.ok) strncpy(dest, value.u.s, size);
        else printf("\033[91mError:\033[0m Config key '%s' is not ok! Ignoring config key...\n", key);

    FREE(value.u.s);
}

void _parseIntConfigEntry(const toml_table_t *arr, const char *key, int *dest)
{
    toml_datum_t value = toml_int_in(arr, key);

    if (value.ok) *dest = value.u.i;
        else printf("\033[91mError:\033[0m Config key '%s' is not ok! Ignoring config key...\n", key);
}

void _parseFloatConfigEntry(const toml_table_t *arr, const char *key, float *dest)
{
    toml_datum_t value = toml_double_in(arr, key);

    if (value.ok) *dest = (float) value.u.d;
        else printf("\033[91mError:\033[0m Config key '%s' is not ok! Ignoring config key...\n", key);
}


/**
 * Parses configContent and fills config struct
 */
void _parseConfig()
{
    char errbuf[200];

    // Load configContent, abort if it is invalid
    toml_table_t* conf = toml_parse(_configContent, errbuf, sizeof(errbuf));

    if (!conf) {
        printf("\033[91mError:\033[0m Failed to parse config! Error: %s\n", errbuf);
        exit(1);
    }


    // Traverse the 'general' table
    toml_table_t* general = toml_table_in(conf, "general");

    _parseStringConfigEntry(general, "createdWithVersion", config.createdWithVersion, sizeof(config.createdWithVersion));


    // Traverse the 'timeouts' table
    toml_table_t* timeouts = toml_table_in(conf, "timeouts");

    _parseIntConfigEntry(timeouts, "arduinoReplyTimeout", &config.arduinoReplyTimeout);
    _parseIntConfigEntry(timeouts, "connectionRetryTimeout", &config.connectionRetryTimeout);
    _parseIntConfigEntry(timeouts, "connectionRetryAmount", &config.connectionRetryAmount);
    _parseFloatConfigEntry(timeouts, "connectionRetryMultiplier", &config.connectionRetryMultiplier);



    // Traverse the 'sensors' table
    toml_table_t* sensors = toml_table_in(conf, "sensors");

    char temp[16] = "";
    _parseStringConfigEntry(sensors, "gpuType", temp, sizeof(temp));

    if (strcmp(temp, "amd") == 0)    config.gpuType = AMD;
    if (strcmp(temp, "nvidia") == 0) config.gpuType = NVIDIA;

    _parseStringConfigEntry(sensors, "cpuTempSensorPath", config.cpuTempSensorPath, sizeof(config.cpuTempSensorPath));
    _parseStringConfigEntry(sensors, "gpuLoadSensorPath", config.gpuLoadSensorPath, sizeof(config.gpuLoadSensorPath));
    _parseStringConfigEntry(sensors, "gpuTempSensorPath", config.gpuTempSensorPath, sizeof(config.gpuTempSensorPath));
    _parseIntConfigEntry(sensors, "checkInterval", &config.checkInterval);


    // Free memory
    toml_free(conf);
}


/**
 * Imports config file from the disk and parses it into config struct
 */
void importConfigFile()
{
    // Construct path (I hate this - is there really no better way of handling this?)
    char *HOME = getenv("HOME");

    strncpy(_configDirPath, HOME, 32);
    strcat(_configDirPath, "/");
    strcat(_configDirPath, configDir);

    strcpy(_configFilePath, _configDirPath);
    strcat(_configFilePath, configFile);


    // Check if we need to create the directory
    #if autoCreateConfig == 1
        _createDefaultConfig();
    #endif


    // Fill configContent with defaultConfig and parse it into config struct. This creates a baseline which the user config can overwrite
    printf("Loading default configuration...\n");

    strcpy(_configContent, defaultConfig);

    _parseConfig();


    // Ignore reading config file if none exists
    struct stat st = {0};

    if (stat(_configFilePath, &st) == -1)
    {
        printf("No config found at '%s', continuing with default settings...\n", _configFilePath);
        return;
    }

    // Read config content
    printf("Loading user configuration from '%s'...\n", _configFilePath);

    getFileContentFull(_configContent, sizeof(_configContent), _configFilePath);

    // Abort if read failed and continue with default setting
    if (strlen(_configContent) == 0) return;

    // Parse result if something was read
    _parseConfig();


    // Write sensorPath into sensor struct if defined
    if (strlen(config.cpuTempSensorPath) > 0)
    {
        printf("Found pre-configured cpuTemp sensor path in config: '%s'\n", config.cpuTempSensorPath);
        strncpy(sensorPaths.cpuTemp, config.cpuTempSensorPath, sizeof(sensorPaths.cpuTemp));
    }

    if (strlen(config.gpuLoadSensorPath) > 0)
    {
        printf("Found pre-configured gpuLoad sensor path in config: '%s'\n", config.gpuLoadSensorPath);
        strncpy(sensorPaths.gpuLoad, config.gpuLoadSensorPath, sizeof(sensorPaths.gpuLoad));
    }

    if (strlen(config.gpuTempSensorPath) > 0)
    {
        printf("Found pre-configured gpuTemp sensor path in config: '%s'\n", config.gpuTempSensorPath);
        strncpy(sensorPaths.gpuTemp, config.gpuTempSensorPath, sizeof(sensorPaths.gpuTemp));
    }
}


/**
 * Parses config struct into a file and writes it to the disk
 */
void exportConfigFile()
{

}
