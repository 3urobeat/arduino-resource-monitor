/*
 * File: main.c
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-20 21:21:42
 * Author: 3urobeat
 *
 * Last Modified: 2025-12-16 17:43:42
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "main.h"


// Runtime vars
uint32_t timeSinceLastSignal = 0;
uint32_t handshakeStartTimestamp = 0;
bool     displayingSplashScreen = false;


// Setup stuff on poweron
void setup_c()
{
    // Init display
    lcdSetupDisplay(0x27, maxcol, maxrow);
    setupBacklight();

    // Init Serial
    setupSerial(baud);

    // Init measurementsCache with "/"
    strcpy(measurementsCache.cpuLoad, "/ ");
    strcpy(measurementsCache.cpuTemp, "/  ");
    strcpy(measurementsCache.ramUsage, "/  ");
    strcpy(measurementsCache.swapUsage, "/  ");
    strcpy(measurementsCache.gpuLoad, "/ ");
    strcpy(measurementsCache.gpuTemp, "/  ");

    // Print startup screen
    lcdDisplaySplashScreen("Waiting...");
    displayingSplashScreen = true;
}


// Check regularly if no data was received recently and display splash screen again
void loop_c()
{
    // Check if backlight should be toggled on every tick
    handleBacklight();

    // Count checkInterval and display Lost Connection message after 10 seconds
    if (timeSinceLastSignal >= 10000)
    {
        // Return to Waiting if handshake did not result in a connection
        if (handshakeStartTimestamp != 0 && millis() - handshakeStartTimestamp > 2500)
        {
            lcdDisplaySplashScreen("Waiting...");
            handshakeStartTimestamp = 0;
        }

        if (displayingSplashScreen) return;
        displayingSplashScreen = true;

        lcdDisplaySplashScreen("Lost Connection!");
    }
    else
    {
        timeSinceLastSignal += 25;
    }

    delay(25);
}


// SerialEvent occurs whenever a new data is being recieved and runs between loop() iterations
void serialEvent_c()
{
    char inputString[64] = "";

    // Read bytes while stream is available and buffer has space
    while (serialIsAvailable()
            && strlen(inputString) < sizeof(inputString))
    {
        char inChar = serialRead();

        // Break loop if end delimiter # was received. Don't append it.
        if (inChar == '#') break;

        // Add received char to the end of inputString
        strncat(inputString, &inChar, 1);

        delay(25);
    }

    // If transmission starts with ~ then the server sent us new measurements
    if (inputString[0] == '~' && strlen(inputString) <= maxcol + 3)
    {
        handleDataInput(inputString); // Process input
        updateDisplay();              // ...and update the screen

        // Update connection loss check vars
        timeSinceLastSignal = 0; // Reset time since last signal
        displayingSplashScreen = false;
    }

    // If transmission starts with + then the server just initiated a new connection
    if (inputString[0] == '+')
    {
        lcdCenterPrint("  Handshaking... ", 3, false); // Surrounded with spaces to overprint "Lost Connection!"
        handshakeStartTimestamp = millis();

        serialPrint(serialHeader);
        serialPrint(version);
        serialPrint(serialEOL);
    }

    // Clear data from inputString when it has been printed
    memset(inputString, 0, sizeof(inputString));
}
