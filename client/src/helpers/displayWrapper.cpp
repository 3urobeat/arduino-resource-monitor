/*
 * File: handleDisplay.cpp
 * Project: arduino-resource-monitor
 * Created Date: 2023-11-17 19:04:54
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-21 20:45:36
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include <NoiascaHW/lcd_PCF8574.h>
#include <lcdHelper.h>

#include "helpers.h"


lcdHelper<LiquidCrystal_PCF8574> *lcd;


/**
 * Inits the display
 */
void lcdSetupDisplay(int addr, uint8_t cols, uint8_t rows)
{
    lcd = new lcdHelper<LiquidCrystal_PCF8574>(addr, cols, rows);

    // Initiate display
    Wire.begin();
    lcd->begin();
}

/**
 * Writes splash screen to the display with a customizeable status message
 */
void lcdDisplaySplashScreen(const char *statusMsg)
{
    lcd->clear();
    lcd->centerPrint("Resource Monitor", 0, true);
    lcd->centerPrint(version, 1, true);
    lcd->centerPrint(statusMsg, 3, true);
}

void lcdPrint(const char *str)
{
    lcd->print(str);
}

void lcdAlignedPrint(const char *align, const char *str, uint8_t width)
{
    lcd->alignedPrint(align, str, width);
}

void lcdCenterPrint(const char *str, uint8_t row, bool callClearLine)
{
    lcd->centerPrint(str, row, callClearLine);
}

void lcdSetCursor(uint8_t col, uint8_t row)
{
    lcd->setCursor(col, row);
}

void lcdSetBacklight(uint8_t state)
{
    lcd->setBacklight(state);
}
