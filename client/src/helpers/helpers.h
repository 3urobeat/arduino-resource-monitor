/*
 * File: helpers.h
 * Project: arduino-resource-monitor
 * Created Date: 17.11.2023 17:18:28
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-20 22:20:48
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include "main.h"


// Stores all current measurements
#define dataSize 16 // Intentionally larger than in the server code because we need to store the units here as well

struct MeasurementTypes {
    char cpuLoad[dataSize];   // in %
    char cpuTemp[dataSize];   // in °C
    char ramUsage[dataSize];  // in GB
    char swapUsage[dataSize]; // in GB
    char gpuLoad[dataSize];   // in %
    char gpuTemp[dataSize];   // in °C
};

extern struct MeasurementTypes measurementsCache;


// Functions defined in helpers
void handleBacklight();
void setupBacklight();

void handleDataInput(char *str);

char *fillRow(char *str);

void updateDisplay();


// C++ functions need a prefix when viewed from a C++ compiler in order to allow C files to call them
#ifdef __cplusplus
    extern "C" void setupSerial(uint32_t baudRate);
    extern "C" bool serialIsAvailable();
    extern "C" void serialPrint(const char *str);
    extern "C" char serialRead();

    extern "C" void lcdSetupDisplay(int addr, uint8_t cols, uint8_t rows);
    extern "C" void lcdDisplaySplashScreen(const char *statusMsg);
    extern "C" void lcdPrint(const char *str);
    extern "C" void lcdAlignedPrint(const char *align, const char *str, uint8_t width);
    extern "C" void lcdCenterPrint(const char *str, uint8_t row, bool callClearLine);
    extern "C" void lcdSetCursor(uint8_t col, uint8_t row);
    extern "C" void lcdSetBacklight(uint8_t state);
#else
    extern void setupSerial(uint32_t baudRate);
    extern bool serialIsAvailable();
    extern void serialPrint(const char *str);
    extern char serialRead();

    extern void lcdSetupDisplay(int addr, uint8_t cols, uint8_t rows);
    extern void lcdDisplaySplashScreen(const char *statusMsg);
    extern void lcdPrint(const char *str);
    extern void lcdAlignedPrint(const char *align, const char *str, uint8_t width);
    extern void lcdCenterPrint(const char *str, uint8_t row, bool callClearLine);
    extern void lcdSetCursor(uint8_t col, uint8_t row);
    extern void lcdSetBacklight(uint8_t state);
#endif
