/*
 * File: printMeasurements.cpp
 * Project: arduino-resource-monitor
 * Created Date: 24.01.2023 17:41:01
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


using namespace std;

chrono::time_point<chrono::steady_clock> lastWriteTime; // Track time to decide if we have to send an alive ping so the Arduino doesn't display the Lost Connection screen


/**
 * Send current measurements to Arduino
 */
void sendMeasurements()
{
    // Clear fullStr
    for (int i = 0; i < displayRows; i++) memset(fullStr[i], 0, sizeof fullStr[i]);


    // Format data in each row
    strcpy(fullStr[0], "Resource Monitor"); // Display a title in the first line
    fillRow(0);
    
    strcpy(fullStr[1], "CPU:");
    catFixedRight(measurements::cpuLoad, 9, 1);
    catFixedRight(measurements::cpuTemp, 18, 1);
    fillRow(1);
    
    strcpy(fullStr[2], "RAM:");
    catFixedRight(measurements::ramUsage, 10, 2);
    catFixedRight(measurements::swapUsage, 17, 2);
    fillRow(2);

    strcpy(fullStr[3], "GPU:");
    catFixedRight(measurements::gpuLoad, 9, 3);
    catFixedRight(measurements::gpuTemp, 18, 3);
    fillRow(3);


    // Send each row separately 
    for (int i = 0; i < displayRows; i++) { // Skip first line after first time
        char tempStr[displayCols + 5] = "~"; // Start with line start char for string validation on Arduino

        tempStr[1] = '0' + i; // Cool lifehack to convert ints < 10 to chars
        strcat(tempStr, fullStr[i]); // Add text
        strcat(tempStr, "#"); // Add line end char for string validation

        // Don't send message again if it didn't change
        if (strcmp(tempStr, lcdCache[i])) {
            //cout << "Sending (" << strlen(tempStr) << "): " << tempStr << endl;

            connection.write(tempStr);

            strcpy(lcdCache[i], tempStr); // Put into cache
            lastWriteTime = chrono::steady_clock::now(); // Refresh lastWriteTime

            // Wait a moment to let the Arduino relax
            auto x = chrono::steady_clock::now() + chrono::milliseconds(250);
            std::this_thread::sleep_until(x);

        } else {
            
            // Send alive ping if nothing was written in the last 5 secs to prevent Connection Lost screen from showing
            if (chrono::steady_clock::now() - lastWriteTime > chrono::milliseconds(5000)) {
                //cout << "Sending alive ping!" << endl;

                connection.write("~0Resource Monitor    #");
                lastWriteTime = chrono::steady_clock::now();
            }
        }
    }
}