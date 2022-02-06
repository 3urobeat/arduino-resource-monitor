/*
  Noiasca Liquid Crystal - 2.x - SPI MCP23S08 4bit 
  
  Open topics
  - Support RW pin
  - custom pin
  - add digitalWriteFast like in parallel mode
  
  copyright 2021 noiasca noiasca@yahoo.com
  
  2021-10-30 Print 
  2021-05-17 OK
  2021-04-11 OK
  2021-03-16 new development
*/

#pragma once
#include <SPI.h>
#include <Print.h>

const byte ioDirRegA = 0x00;  // When a bit is clear, the corresponding pin becomes an output.
const byte gpioRegA = 0x09;   // Output Bank0


/* ****************************************************************************
    class for fixed pin assignment
   ************************************************************************** */

class LiquidCrystal_MCP23S08_base : public Print, public LiquidCrystal_dummy {
  public:     
    LiquidCrystal_MCP23S08_base( uint8_t lcdAddr,
                                 uint8_t csPin,                      // New Pinorder from 2.0
                                 uint8_t cols, uint8_t rows) : 
      LiquidCrystal_dummy(cols, rows),
      lcdAddr{lcdAddr},
      csPin{csPin}
      {}
      
    size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("MCP23S08_base write"));
      send(value, rsDR);
      return 1;
    }        
         
    void hwInit() {
      DEBUG_PRINTLN(F("MCP23S08_base hwInit"));
#if defined(__AVR__)
      // MISSING tbd: check if SPI was already started
#endif
      pinMode(csPin, OUTPUT);
      delay(1);
      digitalWrite(csPin, LOW);
      SPI.transfer(lcdAddr);
      SPI.transfer(ioDirRegA);
      SPI.transfer(0x00);                  // Set all pins to OUTPUT
      digitalWrite(csPin, HIGH);           // MISSING austauschen auf fast?
      delay(1);
      digitalWrite(csPin, LOW);
      SPI.transfer(lcdAddr);
      SPI.transfer(gpioRegA);
      SPI.transfer(0x00);                  // Set all pins LOW
      digitalWrite(csPin, HIGH);
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
    
    // alias for backward compatibility
    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS){
      (void)cols;
      (void)rows;
      (void)charsize;
      begin();
    }
    
/*  
    init()
    LCD API: Initializes the display.
    Clears the screen and puts the cursor to 0, 0
    noiasca: kept as alias for begin. 
             begin() methods are on implementation level
*/
    void init() {begin();};
    
    int setBacklight(uint8_t value){                       // LCD API
      int out = 0;
      if(value)
      { backlightState = 1 ;
        out = blPin;
      }
      else
      {
        backlightState = 0 ;
      }
      digitalWrite(csPin, LOW);
      SPI.transfer(lcdAddr);
      SPI.transfer(gpioRegA);
      SPI.transfer(out);
      digitalWrite(csPin, HIGH);   
      return 0; // assume success - no error
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
    //static const uint8_t rwPin = 0b00000010;   // the Read Write select is not used in this class. Always write.
    static const uint8_t enPin = 0b00000100;     // the enable pin
    static const uint8_t blPin = 0b00001000;     // 0b00001000 the backlight pin - MISSING: tbc if this interferes with the OLED
    const uint8_t csPin;
    //const uint8_t dataPin[4];                  // data pins hi nibble 4 5 6 7
    uint8_t backlightState;                      // we need to keep track of the backlight pin on the port expander      

    void send(uint8_t value, uint8_t rs, uint8_t rw = rwWrite)                 // 4 bit type only - generic
    {
      DEBUG_PRINTLN(F("MCP23S08_base send()"));  
      //Serial.print(rs); Serial.print("\t"); Serial.print (value, HEX); Serial.print("\t"); Serial.println(value, BIN);
      //(void)rwWrite;
      write4bits(value >> 4, rs, rw);  // high nibble
      write4bits(value & 0xF, rs, rw); // low nibble
      delayMicroseconds(waitshort);    // standard delay after send
    }
        
    virtual void write4bits(uint8_t value, uint8_t rs = rsIR, uint8_t rw = rwWrite) {  // low level HW access
      DEBUG_PRINTLN(F("MCP23S08_base write4bits()")); // differs slightly from I2C/pcf
      
      (void)rw;
      byte out = value << 4;           // datapins are 7654xxxx
      byte after = 0;
      if (rs == rsDR) out |= rsPin;    // rsPin 0b00000001 aktivieren fals notwendig
      //if (rw = rwRead) out |= rwPin; // for future use
      if (backlightState)
      {
        out |= blPin;                  // blPin 0b0001000
        //after = blPin;               // MISSING interessanter weise geht das beim I2C, aber nicht mit SPI
      }
      after = out;                     // MISSING bei SPI muss der Pegel bleiben
      out |= enPin;                    // enPin 0b0000100
     
      digitalWrite(csPin, LOW);
      SPI.transfer(lcdAddr);
      SPI.transfer(gpioRegA);
      SPI.transfer((int)(out));
      digitalWrite(csPin, HIGH);
      delayMicroseconds(1);	           // MISSING tbc datasheet 450ns enable pulse
      
      digitalWrite(csPin, LOW);
      SPI.transfer(lcdAddr);
      SPI.transfer(gpioRegA);
      SPI.transfer((int)(after));
      digitalWrite(csPin, HIGH);
      delayMicroseconds(1);	           // MISSING tbc datasheet 450ns enable pulse
    }
};


/*
   SPI default class with special character support
*/
class LiquidCrystal_MCP23S08 : public LiquidCrystal_MCP23S08_base {
  protected:
    using CallBack = uint8_t (*)(uint32_t &special, uint8_t &value);
    CallBack funcPtr;  
    
  public:
    LiquidCrystal_MCP23S08(uint8_t lcd_Addr, uint8_t csPin, uint8_t lcd_cols, uint8_t lcd_rows) :
      LiquidCrystal_MCP23S08_base(lcd_Addr, csPin, lcd_cols, lcd_rows),
      funcPtr(convert)        // function pointer to default converter
      {}
      
    LiquidCrystal_MCP23S08(uint8_t lcd_Addr, uint8_t csPin, uint8_t lcd_cols, uint8_t lcd_rows, CallBack funcPtr) :
      LiquidCrystal_MCP23S08_base(lcd_Addr, csPin, lcd_cols, lcd_rows),
      funcPtr(funcPtr)        
      {}
   
    inline size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("MCP23S08 write"));
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