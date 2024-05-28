/*
 * File: configWrapper.c
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-26 11:19:03
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-27 22:55:51
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


#define configDir ".config/arduino-resource-monitor/"
#define configFile "config.toml"

#define mkdirCmd "mkdir -p " configDir


char _configDirPath[128] = "";
char _configFilePath[128] = "";
char _configContent[512] = "";

struct ConfigValues config;


/**
 * Creates config directory and initializes it with the default configuration file if it does not exist yet
 */
void _createDefaultConfig()
{
    #if autoCreateConfig == 1
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
                printf("Warn: Failed to open stream to config at '%s'! Using default settings. Error: %s\n", _configFilePath, strerror(errno));
                return;
            }

            // Write defaultConfig to config and close stream again
            fprintf(_configFilePtr, defaultConfig);

            (void) fclose(_configFilePtr);
        }
    #endif
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
    _createDefaultConfig();


    // Read config content
    logDebug("importConfigFile(): Importing and parsing config file...");


}


/**
 * Parses config struct into a file and writes it to the disk
 */
void exportConfigFile()
{

}
