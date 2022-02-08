/*
 * File: main.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 21:12:30
 * Author: 3urobeat
 * 
 * Last Modified: 08.02.2022 19:01:11
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
const unsigned int checkInterval = 10; //ms between checks for stringComplete
const unsigned int baud = 19200;
char version[] = "v0.2.0";

LiquidCrystal_PCF8574 lcd(0x27, maxcol, 4);

char inputStrings[maxrow][maxcol + 5]; //our 4x20 display can show 80 chars
bool stringComplete = false;

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
    //Update screen if a new and complete string was recieved
    if (stringComplete) {
        for (int i = 0; i < maxrow; i++) {
            lcd.setCursor(0, i);
            lcd.print(inputStrings[i]);

            memset(inputStrings[i], 0, sizeof inputStrings[i]); //reset content of row in inputStrings
        }

        //reset
        stringComplete = false;
        timeSinceLastSignal = 0;
        displayingConnectionLostMsg = false;
    }
}


//SerialEvent occurs whenever a new data is being recieved and runs between loop() iterations
void serialEvent() {
    int currentrow = 0;
    
    while (Serial.available() && !stringComplete) {
        char inChar = (char) Serial.read();

        //delay(25); //without this delay the Arduino never considers the string as completely recieved and does not print anything. idk exactly why but it works now

        //continue with next line if this char is a line break and set stringComplete to true if text for all rows was recieved
        if (inChar == 0x0a) {
            currentrow++;

            if (currentrow >= maxrow) {
                currentrow = 0;
                stringComplete = true;
            }
        } else {
            strncat(inputStrings[currentrow], &inChar, 1); //add recieved char to the end of inputString
        }
    }
}