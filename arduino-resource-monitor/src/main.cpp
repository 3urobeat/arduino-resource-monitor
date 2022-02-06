/*
 * File: main.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 21:12:30
 * Author: 3urobeat
 * 
 * Last Modified: 06.02.2022 19:15:35
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
#include <ESP8266WiFi.h>          //needed so that Serial is declared, pls remove when changing board
#include "helpers/helpers.h"

const int maxcol = 20;
const unsigned int baud = 19200;
char version[] = "v0.1.0";

LiquidCrystal_PCF8574 lcd(0x27, maxcol, 4);

char inputString[81]; //our 4x20 display can show 80 chars
bool stringComplete = false;


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
    delay(1000);

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
        lcd.setCursor(0, 0);
        lcd.print(inputString); //the string is always 80 chars long and lcd.print() will always continue writing in the next line

        delay(100);

        //reset
        memset(inputString, 0, sizeof inputString);
        stringComplete = false;
    }
}


//SerialEvent occurs whenever a new data is being recieved and runs between loop() iterations
void serialEvent() {
    while (Serial.available() && !stringComplete) {
        char inChar = (char) Serial.read();

        //set stringComplete to true if we reached the last char, which is a line break, or inputString is full otherwise append char to string
        if (inChar == '\n' || strlen(inputString) >= 79) stringComplete = true;
            else strncat(inputString, &inChar, 1); //add recieved char to the end of inputString
    }
}