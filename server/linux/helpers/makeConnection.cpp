/*
 * File: makeConnection.cpp
 * Project: arduino-resource-monitor
 * Created Date: 15.11.2023 22:31:32
 * Author: 3urobeat
 *
 * Last Modified: 19.12.2023 15:49:11
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"

#include <dirent.h>

using namespace std;


/**
 * Attempts to find and connect to Arduino over Serial
 */
serial::Serial* makeConnection()
{
    serial::Serial *_connection;

    // Get all used USB ports by iterating through /sys/class/tty/
    DIR *dp;
    struct dirent *ep;
    dp = opendir("/sys/class/tty/");

    if (dp == NULL)
    {
        printf("Error: Failed to open '/sys/class/tty/' to find all used USB ports!");
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

    printf("Found %d eligible device(s)...\n", usbPortsFound);


    // Attempt to connect to all ports
    for (int i = 0; i < usbPortsFound; i++)
    {
        char *port = usbPorts[i];

        logDebug("Attempting to connect on port '%s', timeout is set to %dms...", port, arduinoReplyTimeout);

        try
        {
            // Open a new connection with timeout set in config
            _connection = new serial::Serial(port, baud, serial::Timeout::simpleTimeout(25));

            this_thread::sleep_until(chrono::steady_clock::now() + chrono::milliseconds(2500)); // Necessary atm because Arduino resets for some reason

            if (!_connection->isOpen())
            {
                printf("Failed to connect to device '%s': Port did not open\n", port);

                _connection = nullptr;
                continue;
            }


            // Attempt to send client our header. A header starts with a +, normal data with a ~
            _connection->flushOutput(); // Clear anything that may be buffered

            char headerStr[64] = "+ResourceMonitorLinuxServer-";
            strcat(headerStr, version);
            strcat(headerStr, "#"); // strcat null terminates here because "#" is a null terminated string

            _connection->write(headerStr);


            // Attempt to listen for a response as long as port is open, buffer has enough space and we haven't run into arduinoReplyTimeout
            char buffer[64] = "";
            unsigned int offset = 0;
            auto timestamp = chrono::steady_clock::now();

            while (_connection->isOpen() && timestamp + chrono::milliseconds(arduinoReplyTimeout) > chrono::steady_clock::now() && offset < sizeof(buffer) - 1)
            {
                buffer[offset] = *_connection->read(1).c_str();

                // Do not increment iteration if nothing was read or pipeline was cleared and let next iteration overwrite char
                if (buffer[offset] == '\0' || buffer[offset] == '\n') continue;

                // Break loop if end char was received
                if (buffer[offset] == '#') break;

                offset++;
            }


            // Check the response
            if (strlen(buffer) == 0)
            {
                printf("Received no response from client!\n");
                _connection->close();
                _connection = nullptr;
                continue;
            }

            if (strstr(buffer, "+ResourceMonitorClient") == NULL)
            {
                printf("Received invalid response from client: %s", buffer);
                _connection->close();
                _connection = nullptr;
                continue;
            }

            // Compare version
            char versionStr[16] = "";

            strncpy(versionStr, buffer + strlen("+ResourceMonitorClient-"), sizeof(versionStr) - 1); // Offset buffer by header content infront of version
            versionStr[strlen(versionStr) - 1] = '\0'; // Remove last char which is the end char #

            if (strcmp(versionStr, version) != 0)
            {
                printf("Version mismatch! Client runs on %s but we are on %s!", versionStr, version);
                _connection->close();
                _connection = nullptr;
                continue;
            }


            logDebug("Received valid response from client: %s", buffer);
            break;
        }
        catch(const std::exception& e)
        {
            printf("Failed to connect to device '%s': %s", port, e.what());

            // Close connection if still open
            if (_connection && _connection->isOpen())
            {
                _connection->close();
            }

            delete _connection;
            _connection = nullptr;
        }
    }

    return _connection;
}
