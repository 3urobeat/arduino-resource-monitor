/*
  Noiasca Liquid Crystal - 2.x - I2C MCP23017 4bit 
  
  The MCP23017 is a I2C 16 channel port expander
  
  Open topics
  - Support RW pin
  - class for 4004 (40 characters, 4 rows) with two enable inputs
  
  copyright 2021 noiasca noiasca@yahoo.com
  
  Version
  2021-11-27 TwoWire.h
  2021-10-30 Print
  2021-08-26 removed hardcoded Wire.h
  2021-05-16 added callback
  2021-04-17 work in progress
  2021-04-17 stable 
  2021-04-12 first working code
  2021-03-30 new development
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
    class for custom pin assignment on the 16 channel MCP23017
    version: 
    - MCP23017 in "16" bit mode
      The expander always sends two bytes/16 bit
    - digitalWrite & pinMode
      As the LCD will use only 7 (or 8) pins, this class adds methods to use
      the remaining pins for basic read and write. This is done, because
      on each LCD write, also "non LCD" pins might be affected. Therefore
      The LCD instance needs to keep track of the "non LCD" pin states
      and you should change pins only via this class            
   ************************************************************************** */

class LiquidCrystal_MCP23017_custompin_base : public Print, public LiquidCrystal_dummy {
  public:     
    LiquidCrystal_MCP23017_custompin_base( uint8_t lcdAddr,
                               uint8_t rsPin, uint8_t /* rwPin */, uint8_t enPin, 
                               uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                               uint8_t blPin, t_backlightPol blType,
                               uint8_t cols, uint8_t rows) : 
      LiquidCrystal_dummy(cols, rows),
      lcdAddr{lcdAddr},
      rsPin{rsPin}, enPin{enPin}, 
      blPin{blPin},
      blType{blType},
      dataPin{(d4Pin), (d5Pin), (d6Pin), (d7Pin)}
      {}
    
    size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("MCP23017_base write"));
      send(value, rsDR);
      return 1;
    } 
               
    void hwInit() {
      DEBUG_PRINTLN(F("MCP23017_custompin_base hwInit"));
#if defined(__AVR__)
      if (TWCR == 0) {
        Wire.begin();                  // only call when not started before
        DEBUG_PRINTLN(F("E: add Wire.begin() in sketch!"));
      }
#endif
     // MCP23017; bank=0, sequence, up to two bytes/16 bit 

      Wire.beginTransmission(lcdAddr);
      Wire.write(0x05);                // if the IC was in bank=1 set the IOCON to 0 and therefore to bank = 0
      Wire.write(0b00000000);          // if we were already in bank = 0 then we just write to GPINTEN
      Wire.endTransmission();
     
      Wire.beginTransmission(lcdAddr);
      Wire.write(0x0A);                // now we are for sure in bank 0, set IOCON to 0 (including bank = 0)
      Wire.write(0b00000000);
      Wire.write(0b00000000);
      Wire.endTransmission();
     
      Wire.beginTransmission(lcdAddr);
      Wire.write(ioDirRegA);
      Wire.write(0b00000000);          // A 0 = Pin is configured as an output.
      Wire.write(0b00000000);          // B 0 = Pin is configured as an output.
      Wire.endTransmission();
      
      Wire.beginTransmission(lcdAddr);
      Wire.write(gpioRegA);
      //Wire.write(0b11000000);          // A on the keypad shield red an blue are inverted active MISSING: tbc no invert
      //Wire.write(0b00000001);          // B on the keypad shield green is inverted active MISSING: tbc no invert
      Wire.write(0);
      Wire.write(0);
      Wire.endTransmission();      
      
      //Wire.beginTransmission(lcdAddr);
      //Wire.write(gpPuA);               // GPIO Pull up Register
      //Wire.write(0b00000000);          
      //Wire.write(0b00000000);          
      //Wire.endTransmission();
    }
    
    void begin()                                           // 4bit standard
    {
      hwInit();
      // Datasheet p46 Figure 24 - 4bit
      delayMicroseconds(40);                               // wait for more than 40µs
      write4bits((functionset | functionset_8bit) >> 4);   // 01 function set (interface is 8 bits long)
      delay(5);                                            // wait for more than 4.1ms
      write4bits((functionset | functionset_8bit) >> 4);   // 02 function set (interface is 8 bits long)
      delayMicroseconds(100);                              // wait for mor than 100µs
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
    
    void init() {begin();};                                // alias for LCD API 
    
    // MISSING: handling of positive/negative
    int setBacklight (uint8_t value) {                               // LCD API
      DEBUG_PRINTLN(F("MCP23017_custompin_base setBacklight()"));
      if (blPin < 16)    
        if (blType == NEGATIVE) value = !value;
        this->digitalWrite(blPin, value);                            // will also store the state in pinState      
      return 0;   // assume access    
      // old way:
      //value ? backlightState = 1 : backlightState = 0;             // no PWM on MCP23017
      //bitWrite(pinStatus, blPin, value);
      //Wire.beginTransmission(lcdAddr);
      //Wire.write(gpioRegA);
      //Wire.write(_BV(blPin) * backlightState);
      //return Wire.endTransmission();
    }
    
    int backlight() {
      return setBacklight(1);
    }

    int noBacklight() {
      return setBacklight(0);
    }  
    
    void digitalWrite(uint8_t pin, uint8_t value)
    {
      DEBUG_PRINTLN(F("MCP23017_custompin_base digitalWrite()"));
      // buffer value
      if (value)
        bitSet(pinStatus, pin);
      else
        bitClear(pinStatus, pin);
      // output to expander
      Wire.beginTransmission(lcdAddr);
      Wire.write(gpioRegA);
      Wire.write((uint8_t)(pinStatus & 0xFF));   // low byte to register A
      Wire.write((uint8_t)(pinStatus >> 8));     // high byte shifted to register B
      Wire.endTransmission();  
      //Serial.print(pinStatus); Serial.print("\t IOs:"); Serial.println(pinStatus, BIN);
    }
    
    /*
      untested
    */
    uint8_t digitalRead(uint8_t pin)
    {
      DEBUG_PRINTLN(F("MCP23017_custompin_base digitalRead()"));
      uint8_t bit = pin % 8;                     // the pin on the port
      uint8_t port = pin / 8;                    // port A (0) or B (1)
      uint8_t reg = gpioRegA;                    // the register to be written
      if (port == 1) reg = gpioRegB;
      Wire.beginTransmission(lcdAddr);
      Wire.write(reg);
      Wire.endTransmission();
      byte actual = 0;
      if (Wire.requestFrom(lcdAddr, (uint8_t)1)) // avoid warning and hand over uint8_t, uint8_t
         actual = Wire.read();
      else
      {
        DEBUG_PRINTLN(F("E179: nothing received"));
      }
      //Serial.print("actual="); Serial.println(actual);
      return actual >> bit & 0x1;                // convert the answer: return only the bit only from the respective pin
    }
    
 /* 
    set the mode of the pin in an Arduino compatible style
    The constants are INPUT = 0; OUTPUT = 1, INPUT_PULLUP = 2
    and will be transfered to the MCP23017 ioDirReg and gpPu Registers
          
    Arduino #define   MCP23017 ioDirReg   gpPu
    INPUT        0                   1      0
    OUTPUT       1                   0   uneffected
    INPUT_PULLUP 2                   1      1
 */   
    void setPinMode(uint8_t pin, uint8_t mode)    
    {
      // mode expects OUTPUT, INPUT or INPUT_PULLUP
      uint8_t bit = pin % 8;                     // the pin on the port
      uint8_t port = pin / 8;                    // port A (0) or B (1)
      uint8_t reg = ioDirRegA;                   // the register to be written
      if (port == 1) reg = ioDirRegB;
      // I2C read old value
      Wire.beginTransmission(lcdAddr);
      Wire.write(reg);
      Wire.endTransmission();
      Wire.requestFrom(lcdAddr, (uint8_t)1);     // avoid warning and hand over uint8_t, uint8_t
      byte actual = Wire.read();  
      // update value (the bit)
      if (mode == OUTPUT)
        bitClear(actual, bit); 
      else
        bitSet(actual, bit); 
      //Serial.print("\t actual:"); Serial.println(actual, BIN);
      // I2C write new value 
      Wire.beginTransmission(lcdAddr);
      Wire.write(reg);
      Wire.write(actual);
      Wire.endTransmission();    
      
      if (mode == INPUT || mode == INPUT_PULLUP)
      {
        reg = gpPuA;
        if (port == 1) reg = gpPuB;
        // I2C read old value
        Wire.beginTransmission(lcdAddr);
        Wire.write(reg);
        Wire.endTransmission();
        Wire.requestFrom(lcdAddr, (uint8_t)1);   // avoid warning and hand over uint8_t, uint8_t
        byte actual = Wire.read();  
        // update value (the bit)
        if (mode == INPUT)
          bitWrite(actual, bit, 0);              // deactivate the pull up bit
        else
          bitWrite(actual, bit, 1);              // activate pull up 
        // I2C write new value 
        Wire.beginTransmission(lcdAddr);
        Wire.write(reg);
        Wire.write(actual);
        Wire.endTransmission(); 
      }  
    }

  protected:
    // registers of the MCP23017 in case of iocon.bank = 0  
    static const byte ioDirRegA = 0x00;// When a bit is clear, the corresponding pin becomes an output.
    static const byte ioDirRegB = 0x01;
    static const byte gpioRegA = 0x12; // Output Bank0
    static const byte gpioRegB = 0x13;
    static const byte gpPuA = 0x0c;    // GPIO PULL-UP RESISTOR REGISTER
    static const byte gpPuB = 0x0d;
    // other LCD member variables
    const uint8_t lcdAddr;             // I2C address of the expander
    const uint8_t rsPin, enPin;        // GPIOs where LCD is connected to
    // const uint8_t rwPin;            // the Read Write select is not used in this library. Always write.
    const uint8_t blPin;               // the backlight pin ist not mandatory, could be hardware specific (one day...)
    const t_backlightPol blType;       // POSITIVE/NEGATIVE...
    const uint8_t dataPin[4];          // GPIOs for data pins of LCD
    uint16_t pinStatus = 0;            // stores io state of other expander pins to make them available in parallel to the LCD pins
   
    /*
      send a value in two 4 bit nibbles to the LCD
    */
    void send(uint8_t value, uint8_t rs, uint8_t rw = rwWrite)                 // 4 bit type only - generic
        {
      DEBUG_PRINTLN(F("MCP23017_custompin_base send()"));
      write4bits(value >> 4, rs, rw);
      write4bits(value, rs, rw);
      delayMicroseconds(waitshort);    // standard delay after send
    }
    
    /*
      write 4 bit values to the expander in 16bit words
    */
    void write4bits(uint8_t value, uint8_t rs = rsIR, uint8_t rw = rwWrite) {  // HW access 
      DEBUG_PRINTLN(F("MCP23017_customPin_base write4bits()"));
      (void)rw;                        // not used in this implementation
      uint16_t out = pinStatus;        // start with the current active pins to keep them unchainged during LCD communication
      for (int i = 0; i < 4; i++)
      {
        if (value & 1 << i) out |= _BV(dataPin[i]);
      }
      if (rs == rsDR) out |= _BV(rsPin);
      //if (rw = rwRead) out |= rwPin;           // for future use
      uint16_t after = out;
      /*
      Wire.beginTransmission(lcdAddr);
      Wire.write(gpioRegA);
      Wire.write(out & 0x0F);
      Wire.write(out >> 8);
      Wire.endTransmission();
      */
      out |= _BV(enPin);                         // shortcut: send with activated enable pin - MISSING: tbd: 4004
      Wire.beginTransmission(lcdAddr);
      Wire.write(gpioRegA);
      Wire.write(out & 0xFF);                    // to register A
      Wire.write(out >> 8);                      // to register B
      Wire.endTransmission();
      //Serial.print(value, HEX); Serial.print("\t LCD:"); Serial.println(out, BIN);
      Wire.beginTransmission(lcdAddr);
      Wire.write(gpioRegA);
      Wire.write(after & 0xFF);
      Wire.write(after >> 8);                    // to be tested with different hardware
      Wire.endTransmission();   
    }
};

/*
   MCP23017 default class with special character support
*/
class LiquidCrystal_MCP23017_custompin : public LiquidCrystal_MCP23017_custompin_base {
  protected:
    using CallBack = uint8_t (*)(uint32_t &special, uint8_t &value);
    CallBack funcPtr;  
    
  public:
    // default converter
    LiquidCrystal_MCP23017_custompin(uint8_t lcdAddr,
                                     uint8_t rsPin, uint8_t rwPin, uint8_t enPin, 
                                     uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                                     uint8_t blPin, t_backlightPol blType,
                                     uint8_t cols, uint8_t rows) :
      LiquidCrystal_MCP23017_custompin_base(lcdAddr, rsPin, rwPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, blType, cols, rows),
      funcPtr(convert)       // function pointer to default converter
      {}
    
    // with function pointer to individual callback
    LiquidCrystal_MCP23017_custompin(uint8_t lcdAddr,
                                     uint8_t rsPin, uint8_t rwPin, uint8_t enPin, 
                                     uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                                     uint8_t blPin, t_backlightPol blType,
                                     uint8_t cols, uint8_t rows, CallBack funcPtr) :
      LiquidCrystal_MCP23017_custompin_base(lcdAddr, rsPin, rwPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, blType, cols, rows),
      funcPtr(funcPtr)       // function pointer to individual converter
      {}   
   
    size_t write(uint8_t value) {      // decided against inline
      DEBUG_PRINTLN(F("MCP23017_custompin write"));
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