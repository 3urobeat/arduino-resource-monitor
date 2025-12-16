/*
 * File: comm.h
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-26 14:01:12
 * Author: 3urobeat
 *
 * Last Modified: 2025-12-16 17:32:44
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include "../server.h"


// Functions to export
extern void makeConnection();
extern void checkForClientInterrupt();

extern void sendMeasurements();
extern void logMeasurements();
extern void resetCache();

extern bool serialNewConnection(const char *port, uint32_t baudRate);
extern bool serialIsOpen();
extern void serialClose();
extern void serialFlushOutput();
extern bool serialWrite(const char *data, size_t size); // Returns bool if write succeeded/failed
extern bool serialRead(char *dest); // Returns bool if read succeeded/failed
extern char *serialGetPort();
