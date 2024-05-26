/*
 * File: data.h
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-26 14:00:50
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-26 14:32:58
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include "../server.h"


// Functions to export
extern void getCmdStdout(char *dest, int size, const char *cmd);
extern void getFileContent(char *dest, int size, const char *path, const char delim);
#define getFileContentFull(dest, size, path) getFileContent(dest, size, path, '\0') // Overload to omit delimiter and read till null byte
