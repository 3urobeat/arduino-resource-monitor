/*
 * File: server.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 20:47:18
 * Author: 3urobeat
 * 
 * Last Modified: 07.02.2022 18:10:17
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file. 
 */


//CMake Build Tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#build-and-run
//Run command from build folder: cmake .. 
//Then compile from build folder with command: cmake --build .

//I was unable to tell CMake to include other directories and wasted over an hour on it - so fuck it, I'm going to put everything in this file, idc about chaos right now.

#include "../lib/include/serial/serial.h" //include library (Tutorial used: https://youtu.be/uHw7QyL4CM8)
#include <iostream>
#include <chrono>
#include <math.h>
#include <thread>

using namespace std;

const unsigned int displayCols = 20;
const unsigned int displayRows = 4;

const char port[] = "/dev/ttyUSB0";
const unsigned int baud = 19200;
const char version[] = "v0.1.0";
const unsigned int checkInterval = 1000; //1 second is lowest value possibe as mpstat takes a second to collect data
const char cpuTempSensor[] = "k10temp-pci-00c3";

char cpuTempCmd[128];
char fullStr[displayCols * displayRows + 5];
char *p = fullStr;

serial::Serial connection(port, baud, serial::Timeout::simpleTimeout(3000)); //make a connection


//Command responses sometimes have a trailing line break, this function removes them
void removeLineBreak(char *str) {
    if (str[strlen(str) - 1] == '\n') {
        char buf[16] = "";
        strncat(buf, str, strlen(str) - 1);
        strcpy(str, buf);
    }
}


//Function to run command and get output
void getStdoutFromCommand(char* dest, const char* cmd) { //https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/  (converted from string to char arrs by me)
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


//Better concat function
char *mystrcat(char *dest, const char *src) //Credit: https://stackoverflow.com/a/21881314
{
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return --dest;
}


//Fills a row with spaces to overwite any left over characters and appends a line break, telling the Arduino to use the next row
void fillRow(int row) {
    for (int i = strlen(fullStr); i < (row * displayCols) - 2; i++) {
        p = mystrcat(p, " ");
    }

    p = mystrcat(p, "\n");
}


//Function that will get executed every checkInterval ms to get new sensor data and send it to the Arduino
void intervalEvent() {

    //Get CPU stats
    char cpuLoad[8] = "";
    getStdoutFromCommand(cpuLoad, "mpstat 1 1 | egrep -v '^Linux|^$' | awk -v c=\"%idle\" 'NR==1 {for (i=1; i<=NF; i++) if ($i==c) break}''{print $(i-1)}' | tail -1 | awk '{print 100-$1}'"); //using this command for cutting the response was easier for now than doing it here - Credit: https://www.linuxquestions.org/questions/linux-newbie-8/need-to-get-average-idle-time-using-mpstat-4175545709/ (I added the subtraction)
    gcvt(round(atof(cpuLoad)), 3, cpuLoad); //convert to float, floor, restrict digits to max 3 and convert float back to char arr
    strcat(cpuLoad, "%");

    char cpuTemp[8] = "";
    getStdoutFromCommand(cpuTemp, cpuTempCmd);
    gcvt(round(atof(cpuTemp)), 3, cpuTemp); //convert to float, floor, restrict digits to max 3 and convert float back to char arr
    strcat(cpuTemp, "°C");

    //Get RAM and Swap usage
    char ramUsage[16] = "";
    getStdoutFromCommand(ramUsage, "free -m | awk -v c=\\'used\\' 'NR==1 {for (i=1; i<=NF; i++) if ($i==c) break}''{print $(i-4)}' | head -2 | tail -1 | awk '{print $1/1000}'"); //awk converts MB to GB here
    gcvt(atof(ramUsage), 2, ramUsage); //convert to float, reduce digits and convert float back to char arr
    strcat(ramUsage, "GB");

    char swapUsage[16] = "";
    getStdoutFromCommand(swapUsage, "free -m | awk -v c=\\'used\\' 'NR==1 {for (i=1; i<=NF; i++) if ($i==c) break}''{print $(i-4)}' | tail -1 | awk '{print $1/1000}'"); //awk converts MB to GB here
    gcvt(atof(swapUsage), 2, swapUsage); //convert to float, reduce digits and convert float back to char arr
    strcat(swapUsage, "GB");

    //Get nvidia gpu load and temp stats
    char nvidiaLoad[8] = "";
    getStdoutFromCommand(nvidiaLoad, "nvidia-settings -q GPUUtilization -t | awk -F '[,= ]' '{ print $2 }'"); //awk cuts response down to only the graphics parameter
    strcat(nvidiaLoad, "%");

    char nvidiaTemp[8] = "";
    getStdoutFromCommand(nvidiaTemp, "nvidia-settings -q GPUCoreTemp -t");
    strcat(nvidiaTemp, "°C");


    //Format data to make on big string
    memset(fullStr, 0, sizeof fullStr);
    strcpy(fullStr, "Resource Monitor\n"); //display a title in the first line
    p = fullStr; //reset pointer
    
    p = mystrcat(p, "CPU: ");
    p = mystrcat(p, cpuLoad);
    p = mystrcat(p, "   ");
    p = mystrcat(p, cpuTemp);
    fillRow(2);
    
    p = mystrcat(p, "RAM: ");
    p = mystrcat(p, ramUsage);
    p = mystrcat(p, " ");
    p = mystrcat(p, swapUsage);
    fillRow(3);

    p = mystrcat(p, "GPU: ");
    p = mystrcat(p, nvidiaLoad);
    p = mystrcat(p, "   ");
    p = mystrcat(p, nvidiaTemp);
    fillRow(4);

    //cout << "Sending (" << strlen(fullStr) << "):\n" << fullStr << endl;

    //Send data
    connection.write(fullStr); //send data using library
}


//Entry point
int main() {
    cout << "arduino-resource-monitor by 3urobeat" << endl;
    cout << "Server for Linux " << version << " starting...\n" << endl;
    cout << "Opening port " << port << " and setting " << baud << " Baud..." << endl;

    //Check if port is now open
    if (connection.isOpen()) {
        cout << "Port opened successfully!" << endl;
    } else {
        cout << "Error: Port failed to open! Exiting..." << endl;
        exit(1);
    }

    connection.flushOutput(); //clear anything that may be buffered

    //Construct cpuTempCmd
    strcpy(cpuTempCmd, "sensors -u ");
    strcat(cpuTempCmd, cpuTempSensor);
    strcat(cpuTempCmd, " | grep temp1_input | sed 's/  temp1_input: //'"); //get temp1_input line and remove everything except the temp value
    
    //Start getting and sending sensor data
    cout << "\nStarting to send data..." << endl;

    //run intervalEvent() every checkInterval ms
    while (true) {
        intervalEvent();
        
        auto x = chrono::steady_clock::now() + chrono::milliseconds(checkInterval);
        this_thread::sleep_until(x);
    }
}