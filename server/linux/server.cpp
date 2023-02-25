/*
 * File: server.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 20:47:18
 * Author: 3urobeat
 * 
 * Last Modified: 25.02.2023 17:25:47
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
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
char cpuTempCmd[128];                        // For constructing the cpuTempCmd once on start
char gpuUtilCmd[128];                        // AMD GPU only
char gpuTempCmd[128];                        // AMD GPU only

char fullStr[displayRows][displayCols + 2];  // For constructing what is going to be sent to the Arduino now
char lcdCache[displayRows][displayCols + 5]; // Save what has been sent previously to avoid sending identical stuff multiple times

serial::Serial connection(port, baud, serial::Timeout::simpleTimeout(3000)); // Make a connection



// Entry point
int main()
{
    cout << "arduino-resource-monitor by 3urobeat" << endl;
    cout << "Server for Linux " << version << " starting...\n" << endl;
    cout << "Opening port " << port << " and setting " << baud << " Baud..." << endl;

    // Check if port is now open
    if (connection.isOpen()) {
        cout << "Port opened successfully!" << endl;
    } else {
        cout << "Error: Port failed to open! Exiting..." << endl;
        exit(1);
    }

    // Wait a moment after establishing connection before starting to write
    auto x = chrono::steady_clock::now() + chrono::milliseconds(1000);
    this_thread::sleep_until(x);

    connection.flushOutput(); // Clear anything that may be buffered


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


    // Start getting and sending sensor data
    cout << "\nStarting to send data..." << endl;

    // Run intervalEvent() every checkInterval ms
    while (true) {
        // Get current measurements
        getMeasurements();

        // Send current measurements
        sendMeasurements();
        
        // Delay for checkInterval ms
        auto x = chrono::steady_clock::now() + chrono::milliseconds(checkInterval);
        this_thread::sleep_until(x);
    }
}