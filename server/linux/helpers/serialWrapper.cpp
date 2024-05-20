/*
 * File: serialWrapper.cpp
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-20 17:02:14
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-20 17:22:31
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"

#include "serial/serial.h"


serial::Serial *_connection = nullptr;

char _connectionPort[32] = "";


void serialNewConnection(const char *port, uint32_t baudRate)
{
    _connection = new serial::Serial(port, baudRate, serial::Timeout::simpleTimeout(25));

    strncpy(_connectionPort, _connection->getPort().c_str(), sizeof(_connectionPort));
}

bool serialIsOpen()
{
    if (!_connection) return false;

    return _connection->isOpen();
}

void serialClose()
{
    if (_connection) {
        _connection->close();

        delete _connection;
        _connection = nullptr;
    }

    _connectionPort[0] = '\0';
}

void serialFlushOutput()
{
    if (!_connection) return;

    _connection->flushOutput();
}

void serialWrite(const char *data)
{
    if (!_connection) return;

    _connection->write(data);
}

void serialRead(char *dest)
{
    if (!_connection) return;

    *dest = _connection->read(1).c_str()[0];
}

char *serialGetPort()
{
    return _connectionPort;
}
