/*
  Noiasca Liquid Crystal 2.x - I2C PCF8574 4bit 
  
  Open topics
  - Support RW pin
  - rename .h to pcf8574
  
  copyright 2021 noiasca noiasca@yahoo.com
  
  2021-11-27 TwoWire.h
  2021-10-30 Print  
  2021-08-26 removed hardcoded Wire
  2021-08-16 4004
  2021-04-18 OK
  2021-03-16 new development
*/

#pragma once
#include <NoiascaLiquidCrystal.h>
#include <Print.h>

#ifndef TwoWire_h
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
#pragma message ("You might miss #include <Wire.h> in your sketch!" )
#endif 
#endif 

/* ****************************************************************************
    class for fixed pin assignment
    used with most common PCF8574 modules
    will save around 200 bytes Flash and 9 bytes SRAM compared to _custompin
    This variant will use pin assignment bitwise
   ************************************************************************** */

class LiquidCrystal_PCF8574_base : public Print, public LiquidCrystal_dummy {
  public:     
      // most used Pin mapping for PCF8574
    LiquidCrystal_PCF8574_base(uint8_t lcdAddr,
                               uint8_t cols, uint8_t rows) : 
      LiquidCrystal_dummy(cols, rows),
      lcdAddr(lcdAddr)
      {} 

    size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("PCF8574_base write"));
      send(value, rsDR);
      return 1;
    }        
         
    void hwInit() {
      DEBUG_PRINTLN(F("PCF8574_base hwInit"));
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
      if (TWCR == 0) {
        Wire.begin();                                      // only call when not started before
        DEBUG_PRINTLN(F("E: add Wire.begin() in sketch!"));
      }
#endif
    }

    void begin()                                           // 4bit standard
    {
      DEBUG_PRINTLN(F("PCF8574_base begin"));
      hwInit();
      // Datasheet p46 Figure 24 - 4bit
      delayMicroseconds(41);                               // wait for more than 40µs
      write4bits((functionset | functionset_8bit) >> 4);   // 01 function set (interface is 8 bits long)
      delay(5);                                            // wait for more than 4.1ms
      write4bits((functionset | functionset_8bit) >> 4);   // 02 function set (interface is 8 bits long)
      delayMicroseconds(101);                              // wait for mor than 100µs
      write4bits((functionset | functionset_8bit) >> 4);   // 03 function set (interface is 8 bits long)
      write4bits(functionset >> 4);                        // 04 function set (Set interface to be 4 bits long)
      // nun functionset_font | entfernt:
      command(functionset | functionset_lines);            // 05 function set, specify the number of display lines and character font
      command(displaycontrol | displaycontrol_on);         // 06 display on off control  0x08 | 0x04
      clear();                                             // 07 display clear - slow command. There is a method anyway
      command(entrymodeset | entrymode_increment);         // 08 entry mode set 
    }
    
    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS){
      (void)cols;
      (void)rows;
      (void)charsize;
      begin();
    }
    
    void init() {begin();};
    
    int setBacklight(uint8_t value){                       // LCD API
      DEBUG_PRINTLN(F("PCF8574_base setBacklight"));
      value ? backlightState = 1 : backlightState = 0;     // no PWM on PCF8574
      //Wire.beginTransmission(lcdAddr);
      //Wire.write(blPin);                                 // blPin 0b0001000
      command(0);
      return Wire.endTransmission(); 
    }
    
    int backlight() {
      return setBacklight(1);
    }

    int noBacklight() {
      return setBacklight(0);
    }  

  protected:
    const uint8_t lcdAddr;                       // I2C address of expander
    static const uint8_t rsPin = 0b00000001;     // GPIOs where LCD is connected to
    //static const uint8_t rwPin = 0b00000010;   // the Read Write select is not used in this library. Always write.
    static const uint8_t enPin = 0b00000100;     // the enable pin
    static const uint8_t blPin = 0b00001000;     // the backlight pin
    //const uint8_t dataPin[4];                  // data pins hi nibble 4 5 6 7
    uint8_t backlightState;                      // we need to keep track of the backlight pin on the port expander      

    void send(uint8_t value, uint8_t rs, uint8_t rw = rwWrite)                 // 4 bit type only - generic
    {
      DEBUG_PRINTLN(F("PCF8574_base send"));  
      //(void)rwWrite;
      write4bits(value >> 4, rs, rw);
      write4bits(value, rs, rw);
      delayMicroseconds(waitshort);    // standard delay after send
    }
        
    virtual void write4bits(uint8_t value, uint8_t rs = rsIR, uint8_t rw = rwWrite) {  // low level HW access
      DEBUG_PRINTLN(F("PCF8574_base write4bits"));
      (void)rw;
      byte out = value << 4;           // datapins are 7654xxxx
      byte after = 0;
      if (rs == rsDR) out |= rsPin;    // rsPin 0b00000001
      //if (rw = rwRead) out |= rwPin; // for future use
      if (backlightState)
      {
        out |= blPin;                  // blPin 0b0001000
        //after = blPin;               // old version
      }
      after = out;
      out |= enPin;                    // enPin 0b0000100
      Wire.beginTransmission(lcdAddr);
      Wire.write(out);
      Wire.write(after);               // to be tested with different hardware
      Wire.endTransmission();
      //Wire.beginTransmission(lcdAddr);
      //Wire.write(after);
      //Wire.endTransmission();      
    }
};


/* ****************************************************************************
   I2C default class with special character support
   ************************************************************************* */

class LiquidCrystal_PCF8574 : public LiquidCrystal_PCF8574_base {
  protected:
    using CallBack = uint8_t (*)(uint32_t &special, uint8_t &value);
    CallBack funcPtr;  
    
  public:
    LiquidCrystal_PCF8574(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) :
      LiquidCrystal_PCF8574_base(lcd_Addr, lcd_cols, lcd_rows),
      funcPtr(convert)       // function pointer to default converter
      {}
    
    // with function pointer to individual callback
    LiquidCrystal_PCF8574(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows, CallBack funcPtr) :
      LiquidCrystal_PCF8574_base(lcd_Addr, lcd_cols, lcd_rows),
      funcPtr(funcPtr)       // function pointer to individual converter
      {}   
   
    size_t write(uint8_t value) {      // decided against inline
      DEBUG_PRINTLN(F("PCF8574 write"));
      switch(funcPtr (special, value))
      {
        case NOPRINT :       // don't print
          break;
        case ADDE :          // print an additional e after character
          send(value, rsDR);   
          send('e', rsDR);   // add a small e after the printed character
          break;
        default :            // includes 1: just print
          send(value, rsDR);
      }
      return 1;
    }
};


/* ****************************************************************************
    class for custom pin assignment
    just in case your PCF8574 is not commonly wired
   ************************************************************************** */

class LiquidCrystal_PCF8574_custompin : public LiquidCrystal_PCF8574 {
  public:     
    LiquidCrystal_PCF8574_custompin( uint8_t lcdAddr,
                               uint8_t rsPin, uint8_t /*rsPin*/, uint8_t enPin, 
                               uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                               uint8_t blPin, t_backlightPol blType,
                               uint8_t cols, uint8_t rows) : 
      LiquidCrystal_PCF8574(lcdAddr, cols, rows),
      rsPin(1 << rsPin), enPin(1 << enPin), 
      blPin(1 << blPin), blType(blType),
      dataPin{(uint8_t)(1 << d4Pin), (uint8_t)(1 << d5Pin), (uint8_t)(1 << d6Pin), (uint8_t)(1 << d7Pin)}
      {}
  
    LiquidCrystal_PCF8574_custompin( uint8_t lcdAddr,
                               uint8_t rsPin, uint8_t /*rsPin*/, uint8_t enPin, 
                               uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                               uint8_t blPin, t_backlightPol blType,
                               uint8_t cols, uint8_t rows, CallBack funcPtr) : 
      LiquidCrystal_PCF8574(lcdAddr, cols, rows, funcPtr),
      rsPin(1 << rsPin), enPin(1 << enPin), 
      blPin(1 << blPin), blType(blType),
      dataPin{(uint8_t)(1 << d4Pin), (uint8_t)(1 << d5Pin), (uint8_t)(1 << d6Pin), (uint8_t)(1 << d7Pin)}
      {}
    
  protected:
    const uint8_t rsPin, enPin;        // GPIOs where LCD is connected to
    // const uint8_t rwPin;            // the Read Write select is not used in this library. Always write.
    const uint8_t blPin;               // the backlight pin ist not mandatory, could be hardware specific (one day...)
    const t_backlightPol blType;
    const uint8_t dataPin[4];          // GPIOs for data pins of LCD
   
    
    // erweitern: abhängig von current row  den  en oder en2 pulsen (if col 40 && row 4 )
    // oder besser, den en2 einmal im constructor abhängig von 40/4 ander setzen und hier nur mehr abhängig von der ROW 

    void write4bits(uint8_t value, uint8_t rs = rsIR, uint8_t rw = rwWrite) override {  // HW access 
      DEBUG_PRINTLN(F("PCF8574_custompin write4bits"));
      (void)rw;
      byte out = 0, after = 0;
      for (int i = 0; i < 4; i++)
      {
        if (value & 1 << i) out |= dataPin[i];
      }
      if (rs == rsDR) out |= rsPin;
      //if (rw = rwRead) out |= rwPin; // for future use
      if (backlightState != blType)                       // this will inverse the backlight: 1 0 -> 1;  0 0 -> 0; 1 1 -> 0; 0 1 -> 1
      {
        out |= blPin;
        //after = blPin;
      }
      after = out;
      out |= enPin;
      Wire.beginTransmission(lcdAddr);
      Wire.write(out);
      Wire.write(after);                                   // to be tested with different hardware
      Wire.endTransmission();
      //Wire.beginTransmission(lcdAddr);
      //Wire.write(after);
      //Wire.endTransmission();      
    }
};


// tbd: do we really need a PCF8574_4004_base?
//      if yes - this would be the right place for the implementation.


/* ****************************************************************************
    class for 4004 display
    no Pin left for RW. Ground RW
   ************************************************************************* */

class LiquidCrystal_PCF8574_4004 : public LiquidCrystal_PCF8574_custompin {
  public:     

    // 12 or 13 parameters - prefered constructor
    LiquidCrystal_PCF8574_4004(uint8_t lcdAddr,
                               uint8_t rsPin, uint8_t enPin, uint8_t en2Pin, 
                               uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                               uint8_t blPin, t_backlightPol blType,
                               uint8_t cols, uint8_t rows, CallBack funcPtr = convert) : 
      LiquidCrystal_PCF8574_custompin(lcdAddr, rsPin, 255, enPin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, blType, cols, rows, funcPtr),
      en2Pin(1 << en2Pin)
      {}

    // 13 or 14 parameters - don't use - as RW isn't used anyway in the _custompin class
    LiquidCrystal_PCF8574_4004(uint8_t lcdAddr,
                               uint8_t rsPin, uint8_t /*rwPin*/, uint8_t enPin, uint8_t en2Pin, 
                               uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                               uint8_t blPin, t_backlightPol blType,
                               uint8_t cols, uint8_t rows, CallBack funcPtr = convert) : 
      LiquidCrystal_PCF8574_custompin(lcdAddr, rsPin, 255, enPin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, blType, cols, rows, funcPtr),
      en2Pin(1 << en2Pin)
      {}
    
  protected:
    const byte en2Pin;       // GPIOs where LCD is connected to
    // const uint8_t rwPin;  // the Read Write select is not used in this library. Always write.

    void write4bits(uint8_t value, uint8_t rs = rsIR, uint8_t rw = rwWrite) override {  // HW access 
      DEBUG_PRINTLN(F("PCF8574_4004 write4bits"));
      (void)rw;
      byte out = 0, after = 0;
      for (int i = 0; i < 4; i++)
      {
        if (value & 1 << i) out |= dataPin[i];
      }
      if (rs == rsDR) out |= rsPin;
      //if (rw = rwRead) out |= rwPin;                     // for future use
      if (backlightState != blType)                        // this will inverse the backlight: 1 0 -> 1;  0 0 -> 0; 1 1 -> 0; 0 1 -> 1
      {
        out |= blPin;
        //after = blPin;
      }
      after = out;
 
      if (currentRow < 2) 
        out |= enPin;
      else if (currentRow < 4)
        out |= en2Pin;
      else
      {  
        out |= enPin;
        out |= en2Pin;
      }
      
      Wire.beginTransmission(lcdAddr);
      Wire.write(out);
      Wire.write(after);                                   // to be tested with different hardware
      Wire.endTransmission();
      //Wire.beginTransmission(lcdAddr);
      //Wire.write(after);
      //Wire.endTransmission();      
    }
    
};
