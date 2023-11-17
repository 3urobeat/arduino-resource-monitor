/*
 * File: main.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 21:12:30
 * Author: 3urobeat
 *
 * Last Modified: 15.11.2023 22:30:58
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 3urobeat <https://github.com/3urobeat>
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

const char version[] = "v0.6.0";


// Create lcd obj
lcdHelper<LiquidCrystal_PCF8574> lcd(0x27, maxcol, maxrow);


// Runtime vars
uint32_t timeSinceLastSignal = 0;
bool     displayingSplashScreen = false;


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

    displayingSplashScreen = true;
    delay(500);

    lcd.centerPrint("Waiting...", 3, true);

}


// Check regularly if no data was received recently and display splash screen again
void loop() {

    // Count checkInterval and display Lost Connection message after 10 seconds
    if (timeSinceLastSignal >= 10000) {
        if (displayingSplashScreen) return;

        lcd.clear();
        lcd.setCursor(0, 0);

        lcd.centerPrint("Resource Monitor", 0, true);
        lcd.centerPrint(version, 1, true);
        lcd.centerPrint("Lost Connection!", 3, true);

        displayingSplashScreen = true;
    } else {
        timeSinceLastSignal += checkInterval;
    }

    delay(checkInterval);

}


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


// Processes header received from server, checks version and replies
void handleConnectionHandshake(char *serverHandshake) {

    // Send success header
    Serial.print("+0ResourceMonitorClient-");
    Serial.print(version);
    Serial.println("#");

}


// SerialEvent occurs whenever a new data is being recieved and runs between loop() iterations
void serialEvent() {
    char inputString[64] = "";

    // Read bytes from the data stream while it is active
    while (Serial.available()) {
        char inChar = (char) Serial.read();

        delay(25);

        // Abort if inputString is about to overflow, we sadly cannot process this transmission
        if (strlen(inputString) > sizeof(inputString) - 2) return;

        // Process data if a full line was received, this is indicated by the trailing char #.
        if (inChar == '#') {

            // If transmission starts with ~ then the server sent us new measurements
            if (inputString[0] == '~' && strlen(inputString) <= maxcol + 3) {
                printInputString(inputString);
            }

            // If transmission starts with + then the server just initiated a new connection
            if (inputString[0] == '+') {
                handleConnectionHandshake(inputString);
            }

            // Clear data from inputString when it has been printed
            memset(inputString, 0, sizeof(inputString));

        } else {
            strncat(inputString, &inChar, 1); // Add received char to the end of inputString
        }
    }
}
