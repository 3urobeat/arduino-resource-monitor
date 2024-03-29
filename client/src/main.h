/*
 * File: main.h
 * Project: arduino-resource-monitor
 * Created Date: 05.02.2022 12:22:33
 * Author: 3urobeat
 *
 * Last Modified: 19.12.2023 16:36:59
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include <NoiascaHW/lcd_PCF8574.h>
#include <lcdHelper.h>

#include "helpers/helpers.h"


// Config variables
#define version "v0.6.2"

#define maxcol 20
#define maxrow 4

#define baud 9600


extern lcdHelper<LiquidCrystal_PCF8574> lcd;

extern uint32_t timeSinceLastSignal;
extern bool     displayingSplashScreen;
