/*
 * File: main.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 21:12:30
 * Author: 3urobeat
 *
 * Last Modified: 17.11.2023 21:50:31
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "main.h"


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
        timeSinceLastSignal += 25;
    }

    delay(25);

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
                handleDataInput(inputString); // Process input
                printCurrentDataToDisplay();  // ...and update the screen
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
