/*
 * File: main.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 21:12:30
 * Author: 3urobeat
 * 
 * Last Modified: 09.02.2022 00:12:50
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file. 
 */


#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include "helpers/helpers.h"

const int maxcol = 20;
const int maxrow = 4;
const unsigned int checkInterval = 25; //ms between checks for stringComplete
const unsigned int baud = 9600;
char version[] = "v0.3.0";

LiquidCrystal_PCF8574 lcd(0x27, maxcol, 4);

unsigned int timeSinceLastSignal = 0;
bool displayingConnectionLostMsg = false;


//Setup stuff on poweron
void setup() {

    //initiate display
    Wire.begin();
    lcd.begin();
    lcd.backlight();

    Serial.begin(baud);

    //Print startup screen
    centerPrint("Resource Monitor", 0, true);
    centerPrint(version, 1, true);
    delay(500);

    //wait for serial connection
    centerPrint("Waiting...", 3, true);
    
    while (!Serial.available()) { }

    //Clear lcd when ready and enter loop()
    lcd.clear();
}


//Check every few ms for new data
void loop() {

    //Count checkInterval and display Lost Connection message after 10 seconds
    if (timeSinceLastSignal >= 10000) {
        if (displayingConnectionLostMsg) return;

        lcd.clear();
        lcd.setCursor(0, 0);

        centerPrint("Resource Monitor", 0, true);
        centerPrint(version, 1, true);
        centerPrint("Lost Connection!", 3, true);

        displayingConnectionLostMsg = true;
    } else {
        timeSinceLastSignal += checkInterval;
    }

    delay(checkInterval);
}


//Refresh a line
void printInputString(char *str) {
    char tempStr[maxcol + 5] = "";
    strncpy(tempStr, str + 2, maxcol); //subtring str to remove start char, row and end char

    int row = str[1] - '0'; //get row by converting char to int: https://stackoverflow.com/a/868508

    lcd.setCursor(0, row);
    lcd.print(tempStr);

    timeSinceLastSignal = 0; //reset time since last signal
    displayingConnectionLostMsg = false;
}


//SerialEvent occurs whenever a new data is being recieved and runs between loop() iterations
void serialEvent() {
    char inputString[maxcol + 5] = "";

    while (Serial.available()) {
        char inChar = (char) Serial.read();

        delay(25);

        //continue with next line if this char is a line break and set stringComplete to true if text for all rows was recieved
        if (inChar == '#') {
            //validate string and abort if it doesn't start with ~ and end with # or is too long. Tinti2013 and I spent 2 hours debugging and found out that the Arduino Nano is basically sometimes loosing chars which didn't happen with the ESP8266
            if (inputString[0] == '~' && strlen(inputString) <= maxcol + 3) {                
                printInputString(inputString);
            }

            memset(inputString, 0, sizeof inputString); //necessary as inputString otherwise still contains this data in the next iteration
        } else {
            strncat(inputString, &inChar, 1); //add recieved char to the end of inputString
        }
    }
}