/*
 * File: lcd.h
 * Project: arduino-resource-monitor
 * Created Date: 05.02.2022 12:20:54
 * Author: 3urobeat
 * 
 * Last Modified: 05.02.2022 12:23:52
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file. 
 */


#include "main.h"

void clearLine(int maxcol, int row);
void lcdPrint(const char *str);
void centerPrint(const char *str, int row, bool callclearLine);