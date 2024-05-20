/*
 * File: handleDisplay.c
 * Project: arduino-resource-monitor
 * Created Date: 2024-05-20 22:12:08
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-20 22:19:57
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


/**
 * Writes currently stored measurements data to the screen
 */
void updateDisplay()
{
    // Construct title
    lcdSetCursor(0, 0);
    lcdPrint("Resource Monitor    ");

    // Construct the CPU row
    lcdSetCursor(0, 1);
    lcdPrint("CPU: ");
    lcdAlignedPrint("right", measurementsCache.cpuLoad, 5); // 5 from the last print (aka 5 + 5 in this case)
    lcdAlignedPrint("right", measurementsCache.cpuTemp, 8); // 8 from the last print (aka 8 + 5 + 5 in this case)

    // Construct the RAM row
    lcdSetCursor(0, 2);
    lcdPrint("RAM: ");
    lcdAlignedPrint("right", measurementsCache.ramUsage, 6);
    lcdAlignedPrint("right", measurementsCache.swapUsage, 7);

    // Construct the GPU row
    lcdSetCursor(0, 3);
    lcdPrint("GPU: ");
    lcdAlignedPrint("right", measurementsCache.gpuLoad, 5);
    lcdAlignedPrint("right", measurementsCache.gpuTemp, 8);
}
