/*
 * File: getMeasurements.cpp
 * Project: arduino-resource-monitor
 * Created Date: 24.01.2023 17:40:48
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


namespace measurements {
    char cpuLoad[8] = "";
    char cpuTemp[8] = "";
    char ramUsage[16] = "";
    char swapUsage[16] = "";
    char gpuLoad[8] = "";
    char gpuTemp[8] = "";
};


// Function to run command and get output
void getStdoutFromCommand(char *dest, const char *cmd) // https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/  (converted from string to char arrs by me)
{
    FILE * stream;

    const int max_buffer = 256;
    char buffer[max_buffer];

    char tempCmd[256];
    strncpy(tempCmd, cmd, 250);
    strcat(tempCmd, " 2>&1");

    stream = popen(cmd, "r");

    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) strcat(dest, buffer);

        removeLineBreak(dest);
        pclose(stream);
    }
}


/**
 * Retreives measurements by running commands
 */
void getMeasurements()
{
    // Clear old data
    memset(measurements::cpuLoad,   0,  8);
    memset(measurements::cpuTemp,   0,  8);
    memset(measurements::ramUsage,  0, 16);
    memset(measurements::swapUsage, 0, 16);
    memset(measurements::gpuLoad,   0,  8);
    memset(measurements::gpuTemp,   0,  8);


    // Get CPU stats
    getStdoutFromCommand(measurements::cpuLoad, "mpstat 1 1 | grep -E -v '^Linux|^$' | awk -v c='%idle' 'NR==1 {for (i=1; i<=NF; i++) if ($i==c) break}''{print $(i)}' | tail -1 | awk '{print 100-$1}'"); // using this command for cutting the response was easier for now than doing it here - Credit: https://www.linuxquestions.org/questions/linux-newbie-8/need-to-get-average-idle-time-using-mpstat-4175545709/ (I added the subtraction)
    gcvt(round(atof(measurements::cpuLoad)), 3, measurements::cpuLoad); // Convert to float, floor, restrict digits to max 3 and convert float back to char arr
    strcat(measurements::cpuLoad, "%");

    getStdoutFromCommand(measurements::cpuTemp, cpuTempCmd);
    gcvt(round(atof(measurements::cpuTemp)), 3, measurements::cpuTemp); // Convert to float, floor, restrict digits to max 3 and convert float back to char arr
    strcat(measurements::cpuTemp, "°C");


    // Get RAM and Swap usage
    char ramUsageTemp[16] = "";
    getStdoutFromCommand(ramUsageTemp, "free -m | awk -v c=\\'used\\' 'NR==1 {for (i=1; i<=NF; i++) if ($i==c) break}''{print $(i-4)}' | head -2 | tail -1 | awk '{print $1/1000}'"); // awk converts MB to GB here

    for (int i = 0; i < 4; i++) { // Use a stupid for loop to avoid doing dtoa() stuff
        measurements::ramUsage[i] = ramUsageTemp[i];

        // If dot was reached, add one decimal and break loop
        if (ramUsageTemp[i] == '.') {
            measurements::ramUsage[i + 1] = ramUsageTemp[i + 1];
            measurements::ramUsage[i + 2] = '\0';
            break;
        }
    }
    strcat(measurements::ramUsage, "GB"); // Concat unit


    char swapUsageTemp[16] = "";
    getStdoutFromCommand(swapUsageTemp, "free -m | awk -v c=\\'used\\' 'NR==1 {for (i=1; i<=NF; i++) if ($i==c) break}''{print $(i-4)}' | tail -1 | awk '{print $1/1000}'"); // awk converts MB to GB here
    
    for (int i = 0; i < 4; i++) { // Use a stupid for loop to avoid doing dtoa() stuff
        measurements::swapUsage[i] = swapUsageTemp[i];

        // If dot was reached, add one decimal and break loop
        if (swapUsageTemp[i] == '.') {
            measurements::swapUsage[i + 1] = swapUsageTemp[i + 1];
            measurements::swapUsage[i + 2] = '\0';
            break;
        }
    }
    strcat(measurements::swapUsage, "GB"); // Concat unit


    // Get nvidia or amd gpu load and temp stats
    #if gpuType == 0
        getStdoutFromCommand(measurements::gpuLoad, "nvidia-settings -q GPUUtilization -t | awk -F '[,= ]' '{ print $2 }'"); // awk cuts response down to only the graphics parameter
        strcat(measurements::gpuLoad, "%");

        getStdoutFromCommand(measurements::gpuTemp, "nvidia-settings -q GPUCoreTemp -t");
        strcat(measurements::gpuTemp, "°C");
    #elif gpuType == 1
        getStdoutFromCommand(measurements::gpuLoad, gpuUtilCmd);
        gcvt(round(atof(measurements::gpuLoad)), 3, measurements::gpuLoad); // Convert to float, floor, restrict digits to max 3 and convert float back to char arr
        strcat(measurements::gpuLoad, "%");

        getStdoutFromCommand(measurements::gpuTemp, gpuTempCmd);
        gcvt(round(atof(measurements::gpuTemp)), 3, measurements::gpuTemp); // Convert to float, floor, restrict digits to max 3 and convert float back to char arr
        strcat(measurements::gpuTemp, "°C");
    #endif
}