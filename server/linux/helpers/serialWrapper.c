/*
 * File: serialWrapper.c
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-20 17:02:14
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-22 19:23:39
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"

#include <serial.h>


serial_t *_connection;

char _connectionPort[32] = "";


bool serialNewConnection(const char *port, uint32_t baudRate)
{
    // Allocate new serial handle if none exists
    if (_connection != NULL) return false;

    _connection = serial_new();

    // Attempt to open port, check if succeeded
    int openSuccess = serial_open(_connection, port, baudRate);

    if (openSuccess < 0)
    {
        printf("Failed to open connection to device '%s'! Error: %s\n", port, serial_errmsg(_connection));
        serialClose();
        return false;
    }

    strncpy(_connectionPort, port, sizeof(_connectionPort));

    return true;
}

bool serialIsOpen()
{
    if (!_connection) return false;

    return (serial_fd(_connection) >= 0);
}

void serialClose()
{
    if (_connection) {
        serial_close(_connection);
        serial_free(_connection);
    }

    _connectionPort[0] = '\0';
    _connection = NULL;
}

void serialFlushOutput()
{
    if (!_connection) return;

    serial_flush(_connection);
}

bool serialWrite(const char *data, size_t size)
{
    if (!_connection) return false;

    int writeSuccess = serial_write(_connection, data, size);

    if (writeSuccess < 0)
    {
        printf("Failed to write to device! Error: %s\n", serial_errmsg(_connection));
        return false;
    }

    return true;
}

bool serialRead(char *dest)
{
    if (!_connection) return false;

    int readSuccess = serial_read(_connection, dest, 1, 2000); // Timeout after 2sec

    if (readSuccess < 0)
    {
        printf("Failed to read from device! Error: %s\n", serial_errmsg(_connection));
        return false;
    }

    return true;
}

char *serialGetPort()
{
    return _connectionPort;
}
