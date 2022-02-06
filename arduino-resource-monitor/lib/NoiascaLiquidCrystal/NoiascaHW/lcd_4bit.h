/*
  Noiasca Liquid Crystal - 2.x - Parallel 4bit 
  
  Open topics
  - Support RW pin
  - POSITIVE/NEGATIVE
  
  copyright 2021 noiasca noiasca@yahoo.com
  
  2021-10-30 Print  
  2021-08-14 support of 4004
  2021-03-13 new development
*/

#pragma once
#include <NoiascaLiquidCrystal.h>
#include <Print.h>

#define FAST                           // activates faster routines for digitalWrite - deactivate if you don't want to use

#if defined(FAST) && !defined(ARDUINO_ARCH_ESP8266) && !defined(ARDUINO_ARCH_ESP32)
 #define DIGITALWRITE digitalWriteFast
#else
 #define DIGITALWRITE digitalWrite     // use the standard function from the core
 #if defined(FAST)
  #undef FAST
 #endif
#endif

class LiquidCrystal_4bit_base : public Print, public LiquidCrystal_dummy {
  public:     
 
    // 9 Parameter
    LiquidCrystal_4bit_base(uint8_t rsPin, uint8_t enPin, 
                            uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                            uint8_t blPin, 
                            uint8_t cols, uint8_t rows) : 
      LiquidCrystal_dummy(cols, rows),
      rsPin(rsPin), enPin(enPin), en2Pin(255),
      blPin(blPin), blType(POSITIVE),
      dataPin{d4Pin, d5Pin, d6Pin, d7Pin}
      {}
    
    // 10 Parameter
    LiquidCrystal_4bit_base(uint8_t rsPin, uint8_t enPin, 
                            uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                            uint8_t blPin, t_backlightPol blType,
                            uint8_t cols, uint8_t rows) : 
      LiquidCrystal_dummy(cols, rows),
      rsPin(rsPin), enPin(enPin), en2Pin(255),
      blPin(blPin), blType(blType),
      dataPin{d4Pin, d5Pin, d6Pin, d7Pin}
      {}
      
    // 11 Parameter - parent interface for 4004
    LiquidCrystal_4bit_base(uint8_t rsPin, uint8_t enPin, uint8_t en2Pin,
                            uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                            uint8_t blPin, t_backlightPol blType,
                            uint8_t cols, uint8_t rows) : 
      LiquidCrystal_dummy(cols, rows),
      rsPin(rsPin), enPin(enPin), en2Pin(en2Pin),
      blPin(blPin), blType(blType),
      dataPin{d4Pin, d5Pin, d6Pin, d7Pin}
      {} 
      
    size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("4bit_base write"));
      send(value, rsDR);
      return 1;
    }
      
    virtual void hwInit()  {                               // hardware specific, virtual due to 4004 class
      DEBUG_PRINTLN(F("hwInit()"));
      pinMode(rsPin, OUTPUT);
      DIGITALWRITE(rsPin, LOW);
      //if (rwPin != 255) {
      //  DIGITALWRITE(rwPin, LOW);
      //  pinMode(rwPin, OUTPUT);
      //}
      pinMode(enPin, OUTPUT);
      DIGITALWRITE(enPin, LOW);
      //if (en2Pin < 255)                                  // 4004 will get a separate class (hence this method is virtual)
      //{ 
      //  DIGITALWRITE(en2Pin, LOW);
      //  pinMode(en2Pin, OUTPUT);
      //}
      if (blPin < 255)                                     
      {
        DIGITALWRITE(blPin, LOW);
        pinMode(blPin, OUTPUT);
      }
      for (uint8_t i = 0; i < 4; i++)
        pinMode(dataPin[i], OUTPUT);
    }

    void begin(){                                          // 4bit standard
      hwInit();
      // Datasheet p46 Figure 24 - 4bit
      delayMicroseconds(40);                               // wait for more than 40µs
      write4bits((functionset | functionset_8bit) >> 4);   // 01 function set (interface is 8 bits long)
      delay(5);                                            // wait for more than 4.1ms
      write4bits((functionset | functionset_8bit) >> 4);   // 02 function set (interface is 8 bits long)
      delayMicroseconds(100);                              // wait for mor than 100µs
      write4bits((functionset | functionset_8bit) >> 4);   // 03 function set (interface is 8 bits long)
      write4bits(functionset >> 4);                        // 04 function set (Set interface to be 4 bits long)
      // never found a 5x10 display, so functionset_font was excluded
      command(functionset | functionset_lines);            // 05 function set, specify the number of display lines and character font
      command(displaycontrol | displaycontrol_on);         // 06 display on off control  0x08 | 0x04
      clear();                                             // 07 display clear - slow command. There is a method anyway
      command(entrymodeset | entrymode_increment);         // 08 entry mode set 
    }
    
    void backlight(void) {
      if (blPin < 255)
         switch(blType)
        {
          case FAULTY : 
            pinMode(blPin, INPUT);      // turn on backlight SAFE
            break;
          case NEGATIVE : 
            DIGITALWRITE(blPin, LOW);
            break;
          default : 
            DIGITALWRITE(blPin, HIGH);
        }       
    }
    
        // Turn the (optional) backlight off/on
    void noBacklight(void) {
      if (blPin < 255)
        switch(blType)
        {
          case FAULTY : 
            pinMode(blPin, OUTPUT);    // turn off backlight SAFE
            break;
          case NEGATIVE : 
            DIGITALWRITE(blPin, HIGH);
            break;
          default : 
            DIGITALWRITE(blPin, LOW);
        }
    }
    
    // alias for LCD API
    void setBacklight(uint8_t new_val) {
      if (new_val) {
        backlight();		               // turn backlight on
      } else {
        noBacklight();		             // turn backlight off
      }
    }

  protected:
    // HW specific - 4 bit common GPIOs
    const uint8_t rsPin, enPin, en2Pin;// GPIOs where the LCD is connected to
    // const uint8_t rwPin;            // the Read Write select is not used in this library. Always write.
    const uint8_t blPin;               // the backlight pin ist not mandatory, could be hardware specific (one day...)
    const t_backlightPol blType;       // polarity and type of backlight control
    const uint8_t dataPin[4];          // GPIOs for data pins of LCD

    void send(uint8_t value, uint8_t rs, uint8_t rw = rwWrite){     // 4 bit type only
      DEBUG_PRINTLN(F("4bit_base send()"));     
      // eigentlich schöner wären die rsPin/rwPin Sachen hier, besser weil nur einmal
      write4bits(value >> 4, rs, rw);
      write4bits(value, rs, rw);
      delayMicroseconds(waitshort); // standard delay after send
    }

    void write4bits(uint8_t value, uint8_t rs = rsIR, uint8_t rw = rwWrite) {  // niedrigster HW Zugriff
      DEBUG_PRINTLN(F("4bit_base write4bits()"));
      (void)rw;
      DIGITALWRITE(rsPin, rs);
      //if (rwPIN != 255) DIGITALWRITE(rwPin, rwW);        // for future use
      for (uint8_t i = 0; i < 4; i++) {
        DIGITALWRITE(dataPin[i], (value & (1 << i))); 
      }
      pulseEnable();
    }

/*
    void pulseEnable(void)                                           // 4004 is a separate class
    {
      DEBUG_PRINTLN(F("4bit_base pulseEnable() with 4004"));
      if (cols == 40 && rows == 4 && currentRow == 255)
      { 
        DIGITALWRITE(enPin, HIGH);
        DIGITALWRITE(en2Pin, HIGH);
        delayMicroseconds(1);            // p52 230ns / p58 figure 25 (p 49 PWeh 450ns) 
        DIGITALWRITE(enPin, LOW);
        DIGITALWRITE(en2Pin, LOW);
      }
      else if (cols == 40 && rows == 4 && currentRow >= 2)  
      {
        DIGITALWRITE(en2Pin, HIGH);
        delayMicroseconds(1);
        DIGITALWRITE(en2Pin, LOW);
      }
      else
      {
        DIGITALWRITE(enPin, HIGH);
        delayMicroseconds(1);
        DIGITALWRITE(enPin, LOW);
      } 
      delayMicroseconds(1);            // tbc: to ensure tah 10ns & th 10ns
    }
*/
    virtual void pulseEnable(void)                                 // MISSING tbd Zusammenführen in write4bits?
    {
      DEBUG_PRINTLN(F("4bit_base pulseEnable"));
      DIGITALWRITE(enPin, HIGH);
      delayMicroseconds(1);            // p52 230ns / p58 figure 25 (p 49 PWeh 450ns) 
      DIGITALWRITE(enPin, LOW); 
      delayMicroseconds(1);            // tbc: to ensure tah 10ns & th 10ns
    }

#if defined(FAST)
    // I expect that people know what they do with their ports, 
    // therefore I use a slimmer (and faster) version of digitalWrite
    // if you encounter any problems with newer boards - deactivate FAST at the top of this file
    void digitalWriteFast(uint8_t pin, uint8_t val) 
      {
        uint8_t bit = digitalPinToBitMask(pin);
        uint8_t port = digitalPinToPort(pin);
        volatile uint8_t *out = portOutputRegister(port);
        if (val == LOW) {
          *out &= ~bit;
        } else {
          *out |= bit;
        }
      }
#endif
};

class LiquidCrystal_4bit : public LiquidCrystal_4bit_base {
  protected:
    using CallBack = uint8_t (*)(uint32_t &special, uint8_t &value);
    CallBack funcPtr; 
    
  public:
    // 9 Parameter
    LiquidCrystal_4bit     (uint8_t rsPin, uint8_t enPin, 
                            uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                            uint8_t blPin, 
                            uint8_t cols, uint8_t rows) : 
      LiquidCrystal_4bit_base(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, cols, rows),
      funcPtr(convert)        // function pointer to default converter
      {}

    // 10 Parameter
    LiquidCrystal_4bit     (uint8_t rsPin, uint8_t enPin, 
                            uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                            uint8_t blPin, 
                            uint8_t cols, uint8_t rows, CallBack funcPtr) : 
      LiquidCrystal_4bit_base(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, cols, rows),
      funcPtr(funcPtr)        
      {}
      
    // 10 Parameter
    LiquidCrystal_4bit     (uint8_t rsPin, uint8_t enPin, 
                            uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                            uint8_t blPin, t_backlightPol blType,
                            uint8_t cols, uint8_t rows) : 
      LiquidCrystal_4bit_base(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, blType, cols, rows),
      funcPtr(convert)        // function pointer to default converter       
      {}  
      
    // 11 Parameter
    LiquidCrystal_4bit     (uint8_t rsPin, uint8_t enPin, 
                            uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                            uint8_t blPin, t_backlightPol blType,
                            uint8_t cols, uint8_t rows, CallBack funcPtr) : 
      LiquidCrystal_4bit_base(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, blType, cols, rows),
      funcPtr(funcPtr)        
      {}
    
    inline size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("4bit write"));
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


/*
    the 4004 display get separate classes 
    it could be integrated in the 4bit_base, but it saves some bytes in the 4bit_base if we maintain it separately.
*/
class LiquidCrystal_4bit_4004_base : public LiquidCrystal_4bit_base {
  public:
    // 11 Parameter
    LiquidCrystal_4bit_4004_base(uint8_t rsPin, uint8_t enPin, uint8_t en2Pin,
                                 uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                                 uint8_t blPin, t_backlightPol blType,
                                 uint8_t cols, uint8_t rows) : 
      LiquidCrystal_4bit_base(rsPin, enPin, en2Pin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, blType, cols, rows)
      {}      
      
    void hwInit() override {                                       // hardware specific
      DEBUG_PRINTLN(F("4bit_base_4004 hwInit()"));
      pinMode(rsPin, OUTPUT);
      DIGITALWRITE(rsPin, LOW);
      //if (rwPin != 255) {
      //  DIGITALWRITE(rwPin, LOW);
      //  pinMode(rwPin, OUTPUT);
      //}
      pinMode(enPin, OUTPUT);
      DIGITALWRITE(enPin, LOW);
      if (en2Pin < 255)                                    // 4004
      { 
        DIGITALWRITE(en2Pin, LOW);
        pinMode(en2Pin, OUTPUT);
      }
      if (blPin < 255)                                     
      {
        DIGITALWRITE(blPin, LOW);
        pinMode(blPin, OUTPUT);
      }
      for (uint8_t i = 0; i < 4; i++)
        pinMode(dataPin[i], OUTPUT);
    }

  protected:
    void pulseEnable(void) override {                                           // support of 4004
      DEBUG_PRINTLN(F("4bit_4004_base pulseEnable()"));
      //if (cols == 40 && rows == 4 && currentRow == 255)                       // as this method is anyway 4004 specific, simplify
      if (currentRow == 255)
      { 
        DIGITALWRITE(enPin, HIGH);
        DIGITALWRITE(en2Pin, HIGH);
        delayMicroseconds(1);            // p52 230ns / p58 figure 25 (p 49 PWeh 450ns) 
        DIGITALWRITE(enPin, LOW);
        DIGITALWRITE(en2Pin, LOW);
      }
      //else if (cols == 40 && rows == 4 && currentRow >= 2)
      else if (currentRow >= 2)    
      {
        DIGITALWRITE(en2Pin, HIGH);
        delayMicroseconds(1);
        DIGITALWRITE(en2Pin, LOW);
      }
      else
      {
        DIGITALWRITE(enPin, HIGH);
        delayMicroseconds(1);
        DIGITALWRITE(enPin, LOW);
      } 
      delayMicroseconds(1);            // tbc: to ensure tah 10ns & th 10ns
    }
};


class LiquidCrystal_4bit_4004 : public LiquidCrystal_4bit_4004_base {
  protected:
    using CallBack = uint8_t (*)(uint32_t &special, uint8_t &value);
    CallBack funcPtr; 
  
  public:    
    // 11 Parameter  
    LiquidCrystal_4bit_4004(uint8_t rsPin, uint8_t enPin, uint8_t en2Pin,
                            uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                            uint8_t blPin, t_backlightPol blType,
                            uint8_t cols, uint8_t rows) : 
      LiquidCrystal_4bit_4004_base(rsPin, enPin, en2Pin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, blType, cols, rows),
      funcPtr(convert)        // function pointer to default converter
      {}
    
    // 12 Parameter
    LiquidCrystal_4bit_4004(uint8_t rsPin, uint8_t enPin, uint8_t en2Pin,
                            uint8_t d4Pin, uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin, 
                            uint8_t blPin, t_backlightPol blType,
                            uint8_t cols, uint8_t rows,
                            CallBack funcPtr) : 
      LiquidCrystal_4bit_4004_base(rsPin, enPin, en2Pin, d4Pin, d5Pin, d6Pin, d7Pin, blPin, blType, cols, rows),
      funcPtr(funcPtr)        // function pointer to default converter
      {}      
      
    inline size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("4bit_4004 write"));
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
