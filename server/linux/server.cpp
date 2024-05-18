/*
 * File: server.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 20:47:18
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-18 12:31:13
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


// CMake Build Tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#build-and-run
// Run command from build folder: cmake ..
// Then compile from build folder with command: cmake --build .

// I was unable to tell CMake to include other directories and wasted over an hour on it - so fuck it, I'm going to put everything in this file, idc about chaos right now.


#include "server.h"

using namespace std;


// Runtime vars
char cpuTempCmd[128]; // For constructing the cpuTempCmd once on start
char gpuUtilCmd[128]; // AMD GPU only
char gpuTempCmd[128]; // AMD GPU only

int connectionRetry = 0;


serial::Serial *connection; // Make a connection


// Entry point
int main()
{
    // Set title and print welcome messages
    printf("\033]0;arduino-resource-monitor Server for Linux %s by 3urobeat\007", version);

    printf("arduino-resource-monitor by 3urobeat\n");
    printf("Server for Linux %s starting...\n\n", version);


    // Validate settings
    if (checkInterval < 1000)
    {
        printf("Error: Setting checkInterval is too low! Please set it to at least 1000!\n");
        exit(1);
    }


    // Construct commands
    strcpy(cpuTempCmd, "sensors -u ");
    strcat(cpuTempCmd, cpuTempSensor);
    strcat(cpuTempCmd, " | grep temp1_input | sed 's/  temp1_input: //'"); // Get temp1_input line and remove everything except the temp value

    strcpy(gpuUtilCmd, "radeontop -b ");
    strcat(gpuUtilCmd, gpuBus);
    strcat(gpuUtilCmd, " -l 1 -d- | cut -d',' -f 2 | sed 's/ gpu //' | tail -1"); // Get readout from radeontop, split at comma, get 2 element (which is the gpu readout), replace " gpu " and get the last line

    strcpy(gpuTempCmd, "sensors -u ");
    strcat(gpuTempCmd, gpuTempSensor);
    strcat(gpuTempCmd, " | grep temp1_input | sed 's/  temp1_input: //'");


    // Begin
    connect();
}


// Handles refreshing measurements and sending them to the Arduino
void dataLoop()
{
    printf("\nStarting to send data...\n");

    // Run intervalEvent() every checkInterval ms as long as connection is not nullptr
    while (connection)
    {
        // Get current measurements
        getMeasurements();

        // Send current measurements
        sendMeasurements();

        // Delay for checkInterval ms
        this_thread::sleep_until(chrono::steady_clock::now() + chrono::milliseconds(checkInterval));
    }
}


// Attempts to find & connect to device and starts to measure & send data
void connect()
{
    connectionRetry++;

    // Find and establish connection to Arduino
    printf("Searching for Arduino...\n");

    connection = makeConnection();

    if (connection == NULL)
    {
        if (connectionRetry > connectionRetryAmount)
        {
            printf("Couldn't connect after %d attempts! Exiting...\n", connectionRetry);
            exit(1);
        }

        int delay = (int) (connectionRetryTimeout * connectionRetryMultiplier) * (connectionRetry + 1);

        printf("Couldn't connect! Attempting again in %dms (attempt %d/%d)...\n", delay, connectionRetry + 1, connectionRetryAmount);

        this_thread::sleep_until(chrono::steady_clock::now() + chrono::milliseconds(delay));
        connect();

        return;
    }

    resetCache();


    // Start getting and sending sensor data
    printf("Successfully connected to Arduino on port '%s'!\n", connection->getPort().c_str());

    dataLoop();
}


// Closes connection to the device and attempts to reconnect
void reconnect()
{
    printf("\nAttempting to reconnect in 5 seconds...\n");

    // Close connection if still open
    if (connection->isOpen())
    {
        connection->close();
    }

    delete connection;
    connection = nullptr;

    this_thread::sleep_until(chrono::steady_clock::now() + chrono::milliseconds(5000));

    // Reset connection tries
    connectionRetry = 0;

    connect();
}
