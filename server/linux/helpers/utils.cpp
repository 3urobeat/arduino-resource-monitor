/*
 * File: utils.cpp
 * Project: arduino-resource-monitor
 * Created Date: 24.01.2023 18:13:33
 * Author: 3urobeat
 * 
 * Last Modified: 30.06.2023 09:56:57
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. 
 */


#include "helpers.h"


// Better concat function
char *mystrcat(char *dest, const char *src) // Credit: https://stackoverflow.com/a/21881314
{
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return --dest;
}


// Fills a row with spaces to overwite any left over characters
void fillRow(int row)
{
    strncat(fullStr[row], "                    ", displayCols - strlen(fullStr[row])); // Interestingly a degree symbol ° is not counted by strlen, causing the hashtag being offset by one char in the CPU and GPU row
}


// Positions a str with the last char to a fixed column by adding spaces and then concats str to fullStr
void catFixedRight(char *str, int col, int row)
{
    int spacesToAdd = (col - 1) - ((strlen(fullStr[row]) - 1) + (strlen(str) - 1));

    if (spacesToAdd > 0) strncat(fullStr[row], "                    ", spacesToAdd);
    
    strcat(fullStr[row], str);
}


// Command responses sometimes have a trailing line break, this function removes them
void removeLineBreak(char *str)
{
    if (str[strlen(str) - 1] == '\n') {
        char buf[16] = "";
        strncat(buf, str, strlen(str) - 1);
        strcpy(str, buf);
    }
}