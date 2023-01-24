/*
 * File: main.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 21:12:30
 * Author: 3urobeat
 * 
 * Last Modified: 24.01.2023 17:41:37
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. 
 */


#include "main.h"


// Config variables
const uint8_t  maxcol = 20;
const uint8_t  maxrow = 4;
const uint32_t checkInterval = 25; // ms between checks for stringComplete
const uint32_t baud = 9600;

char version[] = "v0.3.0";


// Create lcd obj
lcdHelper<LiquidCrystal_PCF8574> lcd(0x27, maxcol, maxrow);


// Runtime vars
uint32_t timeSinceLastSignal = 0;
bool     displayingConnectionLostMsg = false;


// Setup stuff on poweron
void setup() {

    // Initiate display
    Wire.begin();
    lcd.begin();
    lcd.backlight();

    Serial.begin(baud);

    // Print startup screen
    lcd.centerPrint("Resource Monitor", 0, true);
    lcd.centerPrint(version, 1, true);
    delay(500);

    // Wait for serial connection
    lcd.centerPrint("Waiting...", 3, true);
    
    while (!Serial.available()) { }

    // Clear lcd when ready and enter loop()
    lcd.clear();
}


// Check every few ms for new data
void loop() {

    // Count checkInterval and display Lost Connection message after 10 seconds
    if (timeSinceLastSignal >= 10000) {
        if (displayingConnectionLostMsg) return;

        lcd.clear();
        lcd.setCursor(0, 0);

        lcd.centerPrint("Resource Monitor", 0, true);
        lcd.centerPrint(version, 1, true);
        lcd.centerPrint("Lost Connection!", 3, true);

        displayingConnectionLostMsg = true;
    } else {
        timeSinceLastSignal += checkInterval;
    }

    delay(checkInterval);
}


// Refresh a line
void printInputString(char *str) {
    char tempStr[maxcol + 5] = "";
    strncpy(tempStr, str + 2, maxcol); // Subtring str to remove start char, row and end char

    int row = str[1] - '0'; // Get row by converting char to int: https://stackoverflow.com/a/868508

    lcd.setCursor(0, row);
    lcd.limitedPrint(tempStr, maxcol);

    timeSinceLastSignal = 0; // Reset time since last signal
    displayingConnectionLostMsg = false;
}


// SerialEvent occurs whenever a new data is being recieved and runs between loop() iterations
void serialEvent() {
    char inputString[maxcol + 5] = "";

    while (Serial.available()) {
        char inChar = (char) Serial.read();

        delay(25);

        // Continue with next line if this char is a line break and set stringComplete to true if text for all rows was recieved
        if (inChar == '#') {
            // Validate string and abort if it doesn't start with ~ and end with # or is too long. Tinti2013 and I spent 2 hours debugging and found out that the Arduino Nano is basically sometimes loosing chars which didn't happen with the ESP8266
            if (inputString[0] == '~' && strlen(inputString) <= maxcol + 3) {                
                printInputString(inputString);
            }

            // Clear data from inputString when it has been printed
            memset(inputString, 0, sizeof(inputString));
        } else {
            strncat(inputString, &inChar, 1); // Add received char to the end of inputString
        }
    }
}