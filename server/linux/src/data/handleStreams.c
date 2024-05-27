/*
 * File: handleStreams.c
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-22 17:57:28
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-26 14:34:01
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "data.h"


/**
 * Reads a FILE stream until encountering delim or EOF and writes into dest
 */
void _readStream(char *dest, uint16_t size, FILE *stream, const char delim)
{
    int   charCode;
    char *destCurrentPtr = dest;
    char *destEndPtr     = dest + size - 1;

    while ((charCode = getc(stream)) != EOF && destCurrentPtr < destEndPtr) // Read until reaching end-of-file or running out of space
    {
        *destCurrentPtr = (char) charCode;
        destCurrentPtr++;

        if (charCode == delim) break; // Stop reading if we reached the desired delimiter
    }

    // Append a null-byte to properly terminate the string and close stream
    *destCurrentPtr = '\0';


    // Move null terminator in contentBuffer by 1 byte if the last char is a newline (this was always the case in my testing)
    int bytesRead = strlen(dest);

    if (dest[bytesRead - 1] == '\n') dest[bytesRead - 1] = '\0';
}


// Persistent data for getCmdStdout()
char cmdBuffer[256] = "";

/**
 * Runs a shell command and writes its output to dest. Command must be <=250 chars long.
 */
void getCmdStdout(char *dest, int size, const char *cmd)
{
    // Append "2>&1" to command to redirect stderr to stdout
    strncpy(cmdBuffer, cmd, 250);
    strcat(cmdBuffer, " 2>&1");

    // Open stream to file
    errno = 0;
    FILE *streamPtr = popen(cmd, "r");

    if (!streamPtr)
    {
        printf("Error: Failed to run command '%s'! Error: %s\n", cmd, strerror(errno));
        return;
    }

    // Read stream byte-per-byte
    _readStream(dest, size, streamPtr, '\0');

    // Close stream
    (void) pclose(streamPtr);

    // Debug log result and return size
    logDebug("getCmdStdout(): Read '%s' from '%s'", dest, cmd);
}


/**
 * Reads the content of a file until encountering delim or EOF and writes into dest
 */
void getFileContent(char *dest, int size, const char *path, const char delim)
{
    // Open stream to file
    errno = 0;
    FILE *streamPtr = fopen(path, "r");

    if (!streamPtr)
    {
        printf("Error: Failed to read sensor '%s'! Error: %s\n", path, strerror(errno));
        return;
    }

    // Read stream byte-per-byte
    _readStream(dest, size, streamPtr, delim);

    // Close stream
    (void) fclose(streamPtr);

    // Debug log result and return size
    logDebug("getFileContent(): Read '%s' from '%s'", dest, path);
}
