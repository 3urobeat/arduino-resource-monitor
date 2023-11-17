/*
 * File: handleData.cpp
 * Project: steam-comment-service-bot
 * Created Date: 17.11.2023 17:48:54
 * Author: 3urobeat
 *
 * Last Modified: 17.11.2023 17:57:06
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


// Refresh a line on the screen
void printInputString(char *str) {
    char tempStr[maxcol + 5] = "";
    strncpy(tempStr, str + 2, maxcol); // Subtring str to remove start char, row and end char

    int row = str[1] - '0'; // Get row by converting char to int: https://stackoverflow.com/a/868508

    // Clear display if splash screen is currently shown
    if (displayingSplashScreen) lcd.clear();

    // Print result into the correct line
    lcd.setCursor(0, row);
    lcd.limitedPrint(tempStr, maxcol);

    timeSinceLastSignal = 0; // Reset time since last signal
    displayingSplashScreen = false;
}
