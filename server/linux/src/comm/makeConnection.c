/*
 * File: makeConnection.c
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-15 22:31:32
 * Author: 3urobeat
 *
 * Last Modified: 2025-12-16 17:45:39
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "comm.h"

#include <dirent.h>


/**
 * Helper to read serial message into buffer
 */
void _readSerialIntoBuffer(char *dest, uint32_t size, uint32_t timeout)
{
    // Get current time
    struct timespec timeStruct;
    clock_gettime(CLOCK_REALTIME, &timeStruct);
    time_t timestamp = timeStruct.tv_sec * 1000L;


    // Read response as long as port is open, we haven't run into timeout and buffer has enough space
    uint32_t offset = 0;

    while (serialIsOpen()
            && timestamp + timeout > (timeStruct.tv_sec * 1000L)
            && offset < size - 1)
    {
        // Refresh time struct
        clock_gettime(CLOCK_REALTIME, &timeStruct);
        logDebug("_readSerialIntoBuffer: Remaining time for this message: %ldms", (timestamp + timeout) - (timeStruct.tv_sec * 1000L));

        // Attempt to read
        if (!serialRead(dest + offset)) break;

        // Do not increment iteration if nothing was read or pipeline was cleared and let next iteration overwrite char
        if (*(dest + offset) == '\0' || *(dest + offset) == '\n') continue;

        // Break loop if end char was received
        if (*(dest + offset) == serialEOL) break;

        offset++;
    }
}


/**
 * Attempts to find and connect to Arduino over Serial
 */
void makeConnection()
{
    // Get all used USB ports by iterating through /sys/class/tty/
    DIR *dp;
    struct dirent *ep;
    dp = opendir("/sys/class/tty/");

    if (dp == NULL)
    {
        printf("\033[91mError:\033[0m Failed to open '/sys/class/tty/' to find all used USB ports!\n");
    }


    // Collect all valid ttyUSB* files
    char usbPorts[32][16] = {};
    int usbPortsFound = 0;

    while ((ep = readdir(dp)) != NULL)
    {
        // Ignore port if not an USB port
        if (strstr(ep->d_name, "ttyUSB") == NULL) continue;

        char port[16] = "/dev/";
        strncat(port, ep->d_name, sizeof(port) - 1);

        strncpy(usbPorts[usbPortsFound], port, sizeof(port) - 1);
        usbPortsFound++;
    }

    (void) closedir(dp);

    if (usbPortsFound == 0)
    {
        printf("\033[91mError:\033[0m Found no devices! Exiting...\n");
        exit(1);
    }

    printf("Found %d eligible device(s)!\n", usbPortsFound);


    // Attempt to connect to all ports
    for (int i = 0; i < usbPortsFound; i++)
    {
        char *port = usbPorts[i];

        printf("Attempting to connect on port '%s', timeout is set to %dms...\n", port, config.arduinoReplyTimeout);


        // Open a new connection with timeout set in config
        if (!serialNewConnection(port, baud)) continue;

        usleep(2500000); // Wait 2.5 seconds because the Arduino likes to reset for some reason

        if (!serialIsOpen())
        {
            printf("\033[91mError:\033[0m Failed to connect to device '%s': Port did not open\n", port);
            continue;
        }

        logDebug("Opened connection to device '%s'! Attempting to handshake...", port);


        // Attempt to send client our header. A header starts with a +, normal data with a ~
        serialFlushOutput(); // Clear anything that may be buffered

        char headerStr[64] = "+ResourceMonitorLinuxServer-";
        strcat(headerStr, version);
        strcat(headerStr, "#"); // strcat null terminates here because "#" is a null terminated string

        if (!serialWrite(headerStr, strlen(headerStr)))
        {
            serialClose();
            continue;
        }

        logDebug("Sent header '%s' to device '%s'! Listening for response...", headerStr, port);


        // Attempt to listen for a response
        char buffer[64] = "";

        _readSerialIntoBuffer(buffer, sizeof buffer, config.arduinoReplyTimeout);


        // Check the response
        if (strlen(buffer) == 0)
        {
            printf("\033[91mError:\033[0m Received no response from client!\n");
            serialClose();
            continue;
        }

        if (strstr(buffer, "+ResourceMonitorClient") == NULL)
        {
            printf("\033[91mError:\033[0m Received invalid response from client: %s\n", buffer);
            serialClose();
            continue;
        }

        // Compare version
        char versionStr[16] = "";

        strncpy(versionStr, buffer + strlen("+ResourceMonitorClient-"), sizeof(versionStr) - 1); // Offset buffer by header content infront of version
        versionStr[strlen(versionStr) - 1] = '\0'; // Remove last char which is the end char #

        if (strcmp(versionStr, version) != 0)
        {
            printf("\033[91mError:\033[0m Version mismatch! Client runs on %s but we are on %s!\n", versionStr, version);
            serialClose();
            continue;
        }


        logDebug("Received valid response from client: %s", buffer);
        break;
    }
}
