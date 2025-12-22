/*
 * File: makeConnection.c
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-15 22:31:32
 * Author: 3urobeat
 *
 * Last Modified: 2025-12-22 15:37:03
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
 * Helper to read serial message into buffer. Returns success.
 */
bool _readSerialIntoBuffer(char *dest, uint32_t size, uint32_t timeout)
{
    // Get current time
    struct timespec timeStruct;
    clock_gettime(CLOCK_REALTIME, &timeStruct);
    const time_t timestamp = timeStruct.tv_sec * 1000L;


    // Read response as long as port is open, we haven't run into timeout and buffer has enough space
    uint32_t offset = 0;

    while (serialIsOpen()
            && timestamp + timeout > (timeStruct.tv_sec * 1000L)
            && offset < size - 1)
    {
        logDebug("_readSerialIntoBuffer: Remaining time for this message: %ldms", (timestamp + timeout) - (timeStruct.tv_sec * 1000L));

        // Attempt to read
        if (!serialRead(dest + offset, timeout)) break;

        // Refresh time struct
        clock_gettime(CLOCK_REALTIME, &timeStruct);

        // Do not increment iteration if nothing was read or pipeline was cleared and let next iteration overwrite char
        if (*(dest + offset) == '\0' || *(dest + offset) == '\n') continue;

        // Break loop if end char was received
        if (*(dest + offset) == serialEOL) break;

        offset++;
    }


    // Basic success checks
    if (strlen(dest) == 0) // No response
    {
        return false;
    }

    if (strstr(dest, serialClientHeader) == NULL) // Response with invalid header
    {
        printf("\033[91mError:\033[0m Received invalid response from client: %s\n", dest);
        return false;
    }

    return true;
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

        bool readSuccess = _readSerialIntoBuffer(buffer, sizeof buffer, config.arduinoReplyTimeout);

        if (!readSuccess)
        {
            printf("\033[91mError:\033[0m Received no or invalid response from client: %s\n", buffer);
            serialClose();
            continue;
        }

        // Check whether the received message is of type initial handshake
        if (*(buffer + strlen(serialClientHeader)) != '-')
        {
            printf("\033[91mError:\033[0m Received response from client of invalid type: %s\n", buffer);
            serialClose();
            continue;
        }


        // Compare version
        char versionStr[16] = "";

        strncpy(versionStr, buffer + strlen(serialClientHeader) + 1, sizeof(versionStr) - 1); // Offset buffer by header content infront of message content
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


/**
 * Checks whether client sent an interrupt message and handle it
 */
void checkForClientInterrupt()
{
    // Attempt to read into buffer
    char buffer[64] = "";

    logDebug("checkForClientInterrupt: Checking for interrupt...");
    bool readSuccess = _readSerialIntoBuffer(buffer, sizeof buffer, 500);

    if (!readSuccess)
    {
        return;
    }

    if (*(buffer + strlen(serialClientHeader)) != '*')
    {
        printf("\033[91mError:\033[0m Received interrupt message from client of invalid type: %s\n", buffer);
        return;
    }


    // Interpret the response
    logDebug("checkForClientInterrupt: Received message: %s", buffer);

    char interruptStr[16] = "";

    strncpy(interruptStr, buffer + strlen(serialClientHeader) + 1, sizeof(interruptStr) - 1); // Offset buffer by header content infront of message content
    interruptStr[strlen(interruptStr) - 1] = '\0'; // Remove last char which is the end char #

    if (strcmp(interruptStr, "DEVICE_RESET") == 0) // TODO: Switch to numbered message type enum system? Like the arduino does for comparing measurement type
    {
        printf("Received 'RESET' interrupt message from Arduino, clearing local measurement cache...\n");
        resetCache();
        return;
    }
}
