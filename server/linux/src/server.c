/*
 * File: server.c
 * Project: arduino-resource-monitor
 * Created Date: 2022-02-04 20:47:18
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-31 10:31:27
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "server.h"


int connectionRetry = 0;


// Entry point
int main()
{
    // Set title and print welcome messages
    printf("\033]0;arduino-resource-monitor Server for Linux %s by 3urobeat\007", version);

    printf("arduino-resource-monitor by 3urobeat\n");
    printf("Server for Linux %s starting...\n\n", version);


    // Import config and validate settings
    importConfigFile();
    printf("\n");

    if (config.checkInterval < 1000)
    {
        printf("Error: Setting checkInterval is too low! Please set it to at least 1000!\n");
        exit(1);
    }


    // Attempt to find sensors. Terminate if sensors are missing and user intervention is required
    getSensors();

    // Begin
    connect();
}


// Handles refreshing measurements and sending them to the Arduino
void dataLoop()
{
    printf("\nStarting to send data...\n");

    strcpy(measurements.cpuLoad, "/"); // Init with '/' because it takes 2 measurements to display

    #if !clientLessMode
        while (serialIsOpen) // Run intervalEvent() every checkInterval ms as long as connection is not NULL
    #else
        while(true) // Run forever until process is manually terminated
    #endif
    {
        // Get current measurements
        getMeasurements();

        // Send measurements to Client under normal operation, log to stdout when operating in clientLessMode
        #if !clientLessMode
            sendMeasurements();
        #else
            logMeasurements();
        #endif

        // Delay for checkInterval ms
        usleep(config.checkInterval * 1000);
    }
}


// Attempts to find & connect to device and starts to measure & send data
void connect()
{
    connectionRetry++;

    // Find and establish connection to Arduino
    #if !clientLessMode
        printf("Searching for Arduino...\n");

        makeConnection();

        if (!serialIsOpen())
        {
            if (connectionRetry > config.connectionRetryAmount)
            {
                printf("Couldn't connect after %d attempts! Exiting...\n", connectionRetry);
                exit(1);
            }

            int delay = (int) (config.connectionRetryTimeout * config.connectionRetryMultiplier) * (connectionRetry + 1); // Milliseconds

            printf("Couldn't connect! Attempting again in %dms (attempt %d/%d)...\n", delay, connectionRetry + 1, config.connectionRetryAmount);

            usleep(delay * 1000);
            connect();

            return;
        }

        printf("Successfully connected to Arduino on port '%s'!\n", serialGetPort());
    #else
        printf("Skipped searching for Arduino because clientLessMode is enabled!\n");
    #endif


    // Start getting and sending sensor data
    resetCache();

    dataLoop();
}


// Closes connection to the device and attempts to reconnect
void reconnect()
{
    printf("\nAttempting to reconnect in 5 seconds...\n");

    // Close connection if still open
    serialClose();

    sleep(5);

    // Reset connection tries
    connectionRetry = 0;

    connect();
}
