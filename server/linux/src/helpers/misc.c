/*
 * File: misc.c
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-19 18:19:26
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-22 18:42:15
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


/**
 * C string startsWith implementation - https://stackoverflow.com/a/4770992
 */
bool strStartsWith(const char *searchFor, const char *searchInStr)
{
    return strncmp(searchFor, searchInStr, strlen(searchFor)) == 0;
}


/**
 * Formats float below 100 to always show one and 100-999 to show no decimal. Writes result into dest.
 */
void floatToFixedLengthStr(char *dest, float num)
{
    if (num > 999.9) return;

    memset(dest, '\0', 4); // Clear the memory we need
    gcvt(num, 3, dest);

    for (int i = 0; i < 3; i++) // Nifty loop for number formatting: Make numbers below 100 show always one decimal.
    {
        if (*(dest + i) == '.' || dest[i] == '\0')
        {
            *(dest + i) = '.';
            *(dest + i + 1) = *(dest + i + 1) ? *(dest + i + 1) : '0';
            *(dest + i + 2) = '\0';
            break;
        }
    }
}
