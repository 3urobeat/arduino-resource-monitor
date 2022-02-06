/*
  Noiasca Liquid Crystal 2.x - I2C PCF8574 4bit - fast Version
  
  FAST class with different output logic
  
  Open topics
  - Support RW pin
  - LiquidCrystal_PCF8574_custompin
  - LiquidCrystal_PCF8574_4004
  
  copyright 2021 noiasca noiasca@yahoo.com
  
  2021-11-27 TwoWire.h
  2021-11-14 OK
  2021-10-29 new development based on lcd_PCF8574.h
*/

#pragma once
#include <NoiascaLiquidCrystal.h>
#include <NoiascaBufferPrint.h>               // instead of default Print.h

#ifndef TwoWire_h
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
#pragma message ("You might miss #include <Wire.h> in your sketch!" )
#endif 
#endif

// There should be a BUFFER_LENGTH 32 in the Arduino Wire.h.
// The ESP8266 has BUFFER_LENGTH 128
// The ESP32 I2C_BUFFER_LENGTH 128
// if not found hardcoded fallback to 32

#if defined(ARDUINO_ARCH_ESP32)
#define NI_BUFFER_LENGTH I2C_BUFFER_LENGTH
#elif defined(BUFFER_LENGTH)
#define NI_BUFFER_LENGTH BUFFER_LENGTH
#else
#define NI_BUFFER_LENGTH 32
#pragma message ("Used Wire.h library seems do have no BUFFER_LENGTH. Will use 32." )
#endif

/* ****************************************************************************
    class for fixed pin assignment
    used with most common PCF8574 modules
    This variant will use pin assignment bitwise
    Uses NoiascaBufferPrint to for faster I2C communication
   ************************************************************************** */

class LiquidCrystal_PCF8574_fast_base : public NoiascaBufferPrint, public LiquidCrystal_dummy {
  public:     
      // most common pin mapping for PCF8574
    LiquidCrystal_PCF8574_fast_base(uint8_t lcdAddr,
                                    uint8_t cols, uint8_t rows) : 
      LiquidCrystal_dummy(cols, rows),
      lcdAddr(lcdAddr)
      {} 

    size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("PCF8574_fast_base write"));
      send(value, rsDR);
      return 1;
    }      
         
    void hwInit() {
      DEBUG_PRINTLN(F("PCF8574_fast_base hwInit"));
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
      if (TWCR == 0) {
        Wire.begin();                                      // only call when not started before
        DEBUG_PRINTLN(F("E: add Wire.begin() in your sketch!"));
      }
#endif
    }

    void begin()                                           // 4bit standard
    {
      DEBUG_PRINTLN(F("PCF8574_fast_base begin"));
      hwInit();
      // Datasheet p46 Figure 24 - 4bit
      delayMicroseconds(41);                               // wait for more than 40µs
      write4bits((functionset | functionset_8bit) >> 4);   // 01 function set (interface is 8 bits long)
      delay(5);                                            // wait for more than 4.1ms
      write4bits((functionset | functionset_8bit) >> 4);   // 02 function set (interface is 8 bits long)
      delayMicroseconds(101);                              // wait for mor than 100µs
      write4bits((functionset | functionset_8bit) >> 4);   // 03 function set (interface is 8 bits long)
      write4bits(functionset >> 4);                        // 04 function set (Set interface to be 4 bits long)
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
      DEBUG_PRINTLN(F("PCF8574_fast_base setBacklight"));
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
    size_t txCounter = 0;                        // not transmitted bytes in I2C TX Buffer
    
    void send(uint8_t value, uint8_t rs, uint8_t rw = rwWrite)                 // 4 bit type only - generic
    {
      DEBUG_PRINTLN(F("PCF8574_fast_base send"));  
      //(void)rwWrite;
      write4bits(value >> 4, rs, rw);
      write4bits(value, rs, rw);
      delayMicroseconds(waitshort);    // standard delay after send
    }
        
    virtual void write4bits(uint8_t value, uint8_t rs = rsIR, uint8_t rw = rwWrite) {    // low level HW access
      DEBUG_PRINTLN(F("PCF8574_fast_base write4bits"));
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
    
    /*
    // concept only, 
    // I2C Buffer on Arduino is limited to 32 byte
    // so this proof of concept flush will break for > 32 byte 
    void flushOld() 
    {
      // Vorsicht:
      // I2C Buffersize ist 32. 
      // Pro ausgedrucktem Zeichen braucht es 4 Byte.
      // somit müsste man alle 8 Zeichen eine Übertragung starten
      //
      Wire.beginTransmission(lcdAddr);
      for (size_t i = 0; i < getActualBufferSize(); i++)
      {
        // call converter check here if internal[i] is to be printed

        // if != 0 print here: 
        byte value = internal[i] >> 4;
        // hibyte
        byte out = value << 4;           // datapins are 7654xxxx
        byte after = 0;
        out |= rsPin;                    // rsPin 0b00000001
        //if (rw = rwRead) out |= rwPin; // for future use
        if (backlightState)
        {
          out |= blPin;                  // blPin 0b0001000
          //after = blPin;               // old version
        }
        after = out;
        out |= enPin;                    // enPin 0b0000100    
        
        Wire.write(out);
        Wire.write(after);               // to be tested with different hardware
            
        // lowbyte
        value = internal[i] & 0b1111;
        out = value << 4;                // datapins are 7654xxxx
        after = 0;
        out |= rsPin;                    // rsPin 0b00000001
        //if (rw = rwRead) out |= rwPin; // for future use
        if (backlightState)
        {
          out |= blPin;                  // blPin 0b0001000
          //after = blPin;               // old version
        }
        after = out;
        out |= enPin;                    // enPin 0b0000100    
        
        Wire.write(out);
        Wire.write(after);               // to be tested with different hardware

      }
      Wire.endTransmission();  
      setActualBufferSize = 0;
    }
    */
    
    // adds a 4bit nibble to the Wire TX buffer
    // and sets the other lines as needed
    void store4bits(byte value)
    {
        // one nibble
        byte out = value << 4;           // datapins are 7654xxxx
        byte after = 0;
        out |= rsPin;                    // rsPin 0b00000001
        //if (rw = rwRead) out |= rwPin; // for future use
        if (backlightState)
        {
          out |= blPin;                  // blPin 0b0001000
          //after = blPin;               // old version
        }
        after = out;
        out |= enPin;                    // enPin 0b0000100    
        Wire.write(out);
        Wire.write(after);               // to be tested with different hardware
        txCounter += 2;
        // as long as the buffer is even, we can fill it up
        if (txCounter >= NI_BUFFER_LENGTH )  
        {
          Wire.endTransmission();
          txCounter = 0;
          Wire.beginTransmission(lcdAddr);
          delayMicroseconds(waitshort);          // standard delay after send
        }
    }
    
    // splits a value into two 4bit nibbles
    // function will be reused later for "ADDE" 
    void storeChar(byte value)
    {
      store4bits(value >> 4);      // hinibble
      store4bits(value & 0b1111);  // lonibble
    }
    
    /*
        collect data from internal buffer and forward it to I2C TX buffer
        send chunks of 32 byte maximum
        base version without converter
    */
    void flush()
    {
      Wire.beginTransmission(lcdAddr);
      for (size_t i = 0; i < getBufferLength(); i++)
      {
        byte value = internal[i];
        // any converter check here if value is to be printed
        // if != 0 print here: 
        storeChar(value);
      }
      Wire.endTransmission();
      txCounter = 0;
      delayMicroseconds(waitshort);              // standard delay after send      
      resetBuffer();
    }

};

/* ****************************************************************************
   I2C default class with special character support
   ************************************************************************* */

class LiquidCrystal_PCF8574_fast : public LiquidCrystal_PCF8574_fast_base {
  protected:
    using CallBack = uint8_t (*)(uint32_t &special, uint8_t &value);
    CallBack funcPtr;  
    
  public:
    LiquidCrystal_PCF8574_fast(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) :
      LiquidCrystal_PCF8574_fast_base(lcd_Addr, lcd_cols, lcd_rows),
      funcPtr(convert)       // function pointer to default converter
      {}
    
    // with function pointer to individual callback
    LiquidCrystal_PCF8574_fast(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows, CallBack funcPtr) :
      LiquidCrystal_PCF8574_fast_base(lcd_Addr, lcd_cols, lcd_rows),
      funcPtr(funcPtr)       // function pointer to individual converter
      {}   
   
    size_t write(uint8_t value) {      // decided against inline
      DEBUG_PRINTLN(F("PCF8574_fast write"));
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
    
    void flush()
    {
      Wire.beginTransmission(lcdAddr);
      for (size_t i = 0; i < getBufferLength(); i++)
      {
        byte value = internal[i];
        byte action = funcPtr (special, value);
        if (action == PRINT || action == ADDE) 
        {  
          // any converter check here if value is to be printed
          // if != 0 print here: 
          storeChar(value);
          if (action == ADDE)  
          {
            storeChar('e');
          }
        }        
      }
      Wire.endTransmission();
      delayMicroseconds(waitshort);                // standard delay after send      
      resetBuffer();
    }
};
