/*
  Noiasca Liquid Crystal - 2.x
  
  copyright 2021 noiasca noiasca@yahoo.com
  
  2021-03-13 started new development
*/

#include <NoiascaLiquidCrystal.h>

LiquidCrystal_dummy::LiquidCrystal_dummy(uint8_t cols, uint8_t rows) :
  cols{cols},
  rows{rows}
  {
    if (rows == 4 && cols == 16)
    {
      rowStartingAddress[2] = 0x10;
      rowStartingAddress[3] = 0x50; 
    }
    else if (rows == 4 && cols == 40)
    {
      rowStartingAddress[2] = 0x00;    
      rowStartingAddress[3] = 0x40;
    }
    // the 1x16 (8+8) should be used as common two row display
  }
  
//LiquidCrystal_dummy::~LiquidCrystal_dummy(){}

void LiquidCrystal_dummy::send(uint8_t, uint8_t, uint8_t){}  // empty implementation
  
//size_t LiquidCrystal_dummy::write(uint8_t value) {
//  DEBUG_PRINTLN(F("dummy write"));
//  send(value, rsDR);
//  return 1;
//}
    
void LiquidCrystal_dummy::command(uint8_t value) {         // no inline
  send(value, rsIR);                                       // send will wait short
}
  
void LiquidCrystal_dummy::clear() {
  currentRow = 255;                                        // only for 4004
  command(cleardisplay); 
  currentRow = 0;                                          // only for 4004
  delayMicroseconds(waitlong - waitshort);                 // send (called in command) will wait short
}
  
void LiquidCrystal_dummy::home() {       
  currentRow = 0;                                          // only for 4004
  command(returnhome);
  delayMicroseconds(waitlong - waitshort);                 // p24 Table 6
}

void LiquidCrystal_dummy::setCursor(uint8_t newCol, uint8_t newRow) {
  // tbd: checks against > col (?), > Row
  currentRow = newRow; 
  command(setddramaddress | (rowStartingAddress[newRow] + newCol));
}
  
void LiquidCrystal_dummy::cursor() {
  displaycontrolFlag |= displaycontrol_cursor;             // p24 0b0001xCx
  command(displaycontrol | displaycontrolFlag);
}

void LiquidCrystal_dummy::noCursor() {
  displaycontrolFlag &= ~displaycontrol_cursor;            // p24 0b0001xCx
  command(displaycontrol | displaycontrolFlag);
}

void LiquidCrystal_dummy::blink() {
  displaycontrolFlag |= displaycontrol_blink;              // p24 0b0001xxB
  command(displaycontrol | displaycontrolFlag);
}

void LiquidCrystal_dummy::noBlink() {
  displaycontrolFlag &= ~displaycontrol_blink;             // p24 0b0001xxB
  command(displaycontrol | displaycontrolFlag);
}

void LiquidCrystal_dummy::on() {
  displaycontrolFlag |= displaycontrol_on;                 // p24 0b0001Dxxx
  command(displaycontrol | displaycontrolFlag);
}

void LiquidCrystal_dummy::off() {
  displaycontrolFlag &= ~displaycontrol_on;                // p24 0b0001Dxxx
  command(displaycontrol | displaycontrolFlag);
}

// additional methods (not from LCD API)

size_t LiquidCrystal_dummy::writeOld(uint8_t value) {
  send(value, rsDR);                                       // with activated register select bit
  return 1;
}

void LiquidCrystal_dummy::createUml() { 
  for (uint8_t u = 0; u < 3; u++)
  {
    byte address = 5 + u;    // offset 5 for first special character. We will store into 5, 6, 7
    command(setcgramaddress | (address << 3));
    for (uint8_t i = 0; i < 8; i++) {
      writeOld(pgm_read_byte_near(&UML[u][i]));
    }
  }
}
 
void LiquidCrystal_dummy::createChar(uint8_t address, const uint8_t charmap[]) {
  address &= 0x7;                                          // limit to 0..7 addresses
  command(setcgramaddress | (address << 3));
  for (uint8_t i = 0; i < 8; i++) {
    writeOld(charmap[i]);                                  // use writeOld to send data > 0x0B
  }
}

/*
  create custom character with date in Progmem
  takes 8 bytes for each character
*/
void LiquidCrystal_dummy::createChar_P(uint8_t address, const uint8_t *charmap) {
  address &= 0x7;                                          // limit to addresses 0..7 
  command(setcgramaddress | (address << 3));
  for(uint8_t i = 0; i < 8; i++)
  {
    writeOld(pgm_read_byte(charmap + i));                  // read custom character from PROGMEM
  }
}

/*
  change used font
  only available on some OLED displays which support extended commands
*/
void LiquidCrystal_dummy::setFont(uint8_t value) {
  value &= 0x3;              // accept only font definition
  command(0xEF);             // Enter Extend Command Table2
  command(0xFA);             // Double command
  command(0x80);             // Font Table & Cursor Blinking Duty Control Set
  //lcd.command(0x20);       // BD[2:0]=010: Blinking Duty=300ms
  //lcd.command(0x04 | 0x00);// 4+0 swtich to FT[1:0]=00, English Japanese
  //lcd.command(0x04 | 0x01);// 4+1 swtich to FT[1:0]=01, Western European - I 
  //lcd.command(0x04 | 0x02; // 4+2 swtich to FT[1:0]=10, English Russian
  //lcd.command(0x04 | 0x03);// 4+3 swtich to FT[1:0]=11, Western European - II 
                             // FTE=0: Disable software font table set function
                             // Font table is set by FT[1:0] terminals  
  command(0x04 | value);                            
  command(0xFE);             // Exit Extend Command Table2
  command(0xEB);             // Double command
}
