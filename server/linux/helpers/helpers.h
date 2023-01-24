/*
 * File: helpers.h
 * Project: arduino-resource-monitor
 * Created Date: 24.01.2023 17:14:44
 * Author: 3urobeat
 * 
 * Last Modified: 24.01.2023 20:47:19
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. 
 */


#pragma once

#include "server.h"


namespace measurements {
    extern char cpuLoad[8];
    extern char cpuTemp[8];
    extern char ramUsage[16];
    extern char swapUsage[16];
    extern char nvidiaLoad[8];
    extern char nvidiaTemp[8];
};

extern void getMeasurements();

extern void sendMeasurements();

extern char *mystrcat(char *dest, const char *src);
extern void fillRow(int row);
extern void catFixedRight(char *str, int col, int row);
extern void removeLineBreak(char *str);