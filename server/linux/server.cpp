/*
 * File: server.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 20:47:18
 * Author: 3urobeat
 * 
 * Last Modified: 05.02.2022 14:26:10
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file. 
 */


//CMake Build Tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#build-and-run
//Run command from build folder: make .. 
//Then compile from build folder with command: cmake --build .

#include "../lib/include/serial/serial.h" //include library (Tutorial used: https://youtu.be/uHw7QyL4CM8)
#include <iostream>
#include "thread"

const char port[] = "/dev/ttyUSB0";
const unsigned int baud = 19200;

const char version[] = "v0.1.0";

using namespace std;

//Entry point
int main() {
    cout << "arduino-resource-monitor Server for Linux " << version << " starting..." << endl;
    cout << "Opening port " << port << " and setting " << baud << " Baud..." << endl;

    serial::Serial connection(port, baud, serial::Timeout::simpleTimeout(3000)); //make a connection

    //Check if port is now open
    if (connection.isOpen()) {
        cout << "\nPort opened successfully!" << endl;
    } else {
        cout << "Error: Port failed to open! Exiting..." << endl;
        exit(1);
    }

    connection.flushOutput(); //clear anything that may be buffered
    
    cout << "Starting to send data..." << endl;

    //Start sending test string
    char testStr[] = "test message";

    //Send our data 3 times as it seems like some writes get lost often
    for (int i = 0; i < 3; i++) {
        connection.write(testStr); //send data using library

        this_thread::sleep_for(chrono::milliseconds(2000)); //sleep for a moment to avoid timing issues with Serial
    }
}