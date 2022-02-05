/*
 * File: main.cpp
 * Project: arduino-resource-monitor
 * Created Date: 04.02.2022 21:12:30
 * Author: 3urobeat
 * 
 * Last Modified: 05.02.2022 12:29:41
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file. 
 */


#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>          //needed so that Serial is declared, pls remove when changing board
#include "helpers/helpers.h"

const int maxcol = 20;

char version[] = "v0.1.0";

LiquidCrystal_I2C lcd(0x27, maxcol, 4);


void setup() {

    //initiate display
    lcd.init();
    lcd.backlight();

    Serial.begin(9600);

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

void loop() {

    centerPrint("Done!", 2, false);

}