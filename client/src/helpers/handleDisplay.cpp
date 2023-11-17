/*
 * File: handleDisplay.cpp
 * Project: arduino-resource-monitor
 * Created Date: 17.11.2023 19:04:54
 * Author: 3urobeat
 *
 * Last Modified: 17.11.2023 21:51:26
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


// Fills a str with spaces to match maxcol to overwite any left over characters on the display. Make sure `str` is at least as large as maxcol! Returns pointer to str.
char* fillRow(char *str) {
    strncat(str, "                    ", maxcol - strlen(str));

    return str;
}


// Refreshes display content
void printCurrentDataToDisplay() {

    // Temp str used for constructing all rows
    char tempStr[maxcol + 2] = "";


    // Construct title
    strncpy(tempStr, measurementsCache::titleRow, maxcol);

    lcd.setCursor(0, 0);
    lcd.print(fillRow(tempStr));


    // Construct the CPU row
    strcpy(tempStr, "CPU: ");
    fillRow(tempStr);

    lcd.setCursor(0, 1);
    lcd.print(tempStr);
    lcd.alignedPrint("right", measurementsCache::cpuLoad, 10);
    lcd.alignedPrint("right", measurementsCache::cpuTemp, 17);


    // Construct the RAM row
    strcpy(tempStr, "RAM: ");
    fillRow(tempStr);

    lcd.setCursor(0, 2);
    lcd.print(tempStr);
    lcd.alignedPrint("right", measurementsCache::ramUsage, 11);
    lcd.alignedPrint("right", measurementsCache::swapUsage, 18);


    // Construct the GPU row
    strcpy(tempStr, "GPU: ");
    fillRow(tempStr);

    lcd.setCursor(0, 3);
    lcd.print(tempStr);
    lcd.alignedPrint("right", measurementsCache::gpuLoad, 10);
    lcd.alignedPrint("right", measurementsCache::gpuTemp, 17);

}
