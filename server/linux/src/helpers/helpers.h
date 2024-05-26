/*
 * File: helpers.h
 * Project: arduino-resource-monitor
 * Created Date: 2023-01-24 17:14:44
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-26 14:17:28
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include "../server.h"


// Functions to export
extern bool strStartsWith(const char *searchFor, const char *searchInStr);
extern void floatToFixedLengthStr(char *dest, float num);
