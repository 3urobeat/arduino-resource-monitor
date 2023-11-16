/*
 * File: makeConnection.cpp
 * Project: arduino-resource-monitor
 * Created Date: 15.11.2023 22:31:32
 * Author: 3urobeat
 *
 * Last Modified: 16.11.2023 17:05:09
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"

using namespace std;


/**
 * Attempts to find and connect to Arduino over Serial
 */
serial::Serial* makeConnection() {

    // Attempt to connect
    serial::Serial *_connection = new serial::Serial(port, baud, serial::Timeout::simpleTimeout(3000));


    // Check if port is now open
    if (_connection->isOpen()) {
        cout << "Port opened successfully!" << endl;
    } else {
        cout << "Error: Port failed to open! Exiting..." << endl;
        exit(1);
    }


    // Wait a moment after establishing connection before starting to write
    auto x = chrono::steady_clock::now() + chrono::milliseconds(1000);
    this_thread::sleep_until(x);

    _connection->flushOutput(); // Clear anything that may be buffered


    return _connection;

}
