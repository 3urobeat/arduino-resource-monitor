/*
  Noiasca Liquid Crystal 2.x - I2C native Wire
  
  LCD driver AIP31068L
  RGB driver PCA9633
  Native I2C Interface
  8bit mode
  
  Hardware
  - Sureeno with RGB Chip at 0x60           Available here: https://s.click.aliexpress.com/e/_dXgx0kh
  - Grove (Seeed) with RGB Chip at 0x62     Available here: https://amzn.to/352Kblx
  
  Attention: This classes are NOT for LED I2C extender chips.
    
  Open topics
  - 
  
  copyright 2021 noiasca noiasca@yahoo.com
  
  2021-11-27 TwoWire.h
  2021-05-30 new development
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
    base class for AIP31068L LCD driver
    native 8bit Wire interface
************************************************************************** */

class LiquidCrystal_Wire_base : public Print, public LiquidCrystal_dummy {
  protected:
    const uint8_t lcdAddr;                       // I2C address of display
    const uint8_t waitshort = 41;                // AIP31068L datasheet p15 using the value fo data write (instead of 37)
    const uint16_t waitlong = 1530;              // AIP31068L datasheet p15 (instead of 1520)

    void send(uint8_t value, uint8_t rs, uint8_t rw = rwWrite)       // 4 bit type only - generic
    {
      DEBUG_PRINTLN(F("Wire_base send"));  
      write8bits(value, rs, rw);
      delayMicroseconds(waitshort);    // standard delay after send
    }
        
    void write8bits(uint8_t value, uint8_t rs = rsIR, uint8_t rw = rwWrite) {  // low level HW access in 8 bit mode
      DEBUG_PRINTLN(F("Wire_base write4bits"));
      (void)rw;
      Wire.beginTransmission(lcdAddr);
      if (rs == rsDR) 
        Wire.write(0x40);              // set bit7 to 0 (last control byte) and set bit6 (0x40) (datasheet p12)
      else
        Wire.write(0);                 // set bit7 to 0 (last control byte) and bit6 to 0 (datasheet p12)
      Wire.write(value);
      Wire.endTransmission();
    }
  
  public:     
    LiquidCrystal_Wire_base(uint8_t lcdAddr, uint8_t cols, uint8_t rows) : 
      LiquidCrystal_dummy(cols, rows),
      lcdAddr{lcdAddr}
      {}

    size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("Wire_base write"));
      send(value, rsDR);
      return 1;
    }      
         
    virtual void hwInit() {
      DEBUG_PRINTLN(F("Wire_base hwInit"));
#if defined(__AVR__)
      if (TWCR == 0) {
        Wire.begin();                                      // only call when not started before
        DEBUG_PRINTLN(F("E: add Wire.begin() in sketch!"));
      }
#endif
    }

/*  
    init()
    LCD API: Initializes the display.
    Clears the screen and puts the cursor to 0, 0
    noiasca: methods are on HW implementation level
             I prefer the begin() methods
             init() will be just an alias for begin()
*/
    
    void begin()                                         // 8bit AIP31068L
    {
      DEBUG_PRINTLN(F("Wire_base begin"));
      hwInit();                                          // init HW    
                                                         // Datasheet p14 - 8bit:
      delayMicroseconds(31);                             // wait for more than 30µs after VDD rises to 4.5v - other HW might be taken some time already, anyhow, lets stick to the datasheet
      command(functionset | functionset_8bit | functionset_lines | functionset_font);     // function set 0x20 x10 0x08 0x04 
                                                         // wait for more than 39us is covered in command()
      command(displaycontrol | displaycontrol_on);       // display on off control  0x08 | 0x04  (but cursor off, blink off)
                                                         // wait for more than 39us is covered in command()
      clear();                                           // display clear - slow command. There is a method anyway
                                                         // wait for more than 1.53ms is part of clear()
      command(entrymodeset | entrymode_increment);       // entry mode set 
    }

    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS){
      (void)cols;
      (void)rows;
      (void)charsize;
      begin();
    }
      
    void init() {begin();};
};


/*
   I2C default class with special character support
*/
class LiquidCrystal_Wire : public LiquidCrystal_Wire_base {
  protected:
    using CallBack = uint8_t (*)(uint32_t &special, uint8_t &value);
    CallBack funcPtr;  
    
  public:
    LiquidCrystal_Wire(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) :
      LiquidCrystal_Wire_base(lcd_Addr, lcd_cols, lcd_rows),
      funcPtr{convert}       // function pointer to default converter
      {}
    
    // with function pointer to individual callback
    LiquidCrystal_Wire(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows, CallBack funcPtr) :
      LiquidCrystal_Wire_base(lcd_Addr, lcd_cols, lcd_rows),
      funcPtr{funcPtr}       // function pointer to individual converter
      {}   
   
    size_t write(uint8_t value) {      // decided against inline
      DEBUG_PRINTLN(F("I2C write"));
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
  Wire/I2c Display with RGB Backlight PCA9633
    Sureeno or Grove 
*/
// native 8bit Wire interface
class LiquidCrystal_Wire_RGB : public LiquidCrystal_Wire_base {
  protected:
    const uint8_t rgbAddr;                                 // hardwired on Sureeno display
    byte blRed = 127, blGreen = 127, blBlue = 127;         // backlight default colors (medium grey/white)
    uint8_t backlightState;                                // keep track of the backlight state 
    using CallBack = uint8_t (*)(uint32_t &special, uint8_t &value);
    CallBack funcPtr;  
    
    // for the RGB IC PCA9633                      
    static constexpr uint8_t RGB_ADDR = 0x60;    // default value for the RGB chip
    // datahseet table 7
    static constexpr uint8_t REG_MODE1 = 0x00;   // MODE1
    static constexpr uint8_t REG_MODE2 = 0x01;   // MODE2
    static constexpr uint8_t REG_BLUE =  0x02;   // pwm0
    static constexpr uint8_t REG_GREEN = 0x03;   // pwm1
    static constexpr uint8_t REG_RED =   0x04;   // pwm2
    //                                   0x06    // GRPPWM
    //                                   0x07    // GRPFREQ
    static constexpr uint8_t REG_OUTPUT = 0x08;  // LED driver output state, LEDOUT
  
  public:
    // defaulted rgbAddr, defaulted callback function pointer
    LiquidCrystal_Wire_RGB(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) : 
      LiquidCrystal_Wire_base(lcd_Addr, lcd_cols, lcd_rows),
      rgbAddr{RGB_ADDR},               // default value from define
      funcPtr{convert}                 // function pointer to default converter
      {
        backlightState = 255;
      }
      
    // defaulted rgbAddr but dedicated function pointer
    LiquidCrystal_Wire_RGB(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows, CallBack funcPtr) : 
      LiquidCrystal_Wire_base(lcd_Addr, lcd_cols, lcd_rows),
      rgbAddr{RGB_ADDR},               // default value from define
      funcPtr{funcPtr}                 // function pointer to individual converter
      {
        backlightState = 255;
      }
    
    // dedicated rgbAddr but defaulted callback function pointer
    LiquidCrystal_Wire_RGB(uint8_t lcd_Addr, uint8_t rgbAddr, uint8_t lcd_cols, uint8_t lcd_rows) : 
      LiquidCrystal_Wire_base(lcd_Addr, lcd_cols, lcd_rows),
      rgbAddr{rgbAddr},
      funcPtr{convert}                 // function pointer to default converter
      {
        backlightState = 255;
      }
      
    // dedicated rgbAddr and dedicated function pointer
    LiquidCrystal_Wire_RGB(uint8_t lcd_Addr, uint8_t rgbAddr, uint8_t lcd_cols, uint8_t lcd_rows, CallBack funcPtr) : 
      LiquidCrystal_Wire_base(lcd_Addr, lcd_cols, lcd_rows),
      rgbAddr{rgbAddr},
      funcPtr{funcPtr}                 // function pointer to individual converter
      {
        backlightState = 255;
      }

    inline size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("Wire_RGB write"));
      switch(funcPtr (special, value))
      {
        case NOPRINT :                 // don't print
          break;                       
        case ADDE :                    // print an additional e after character
          send(value, rsDR);             
          send('e', rsDR);             // add a small e after the printed character
          break;                       
        default :                      // includes PRINT: just print
          send(value, rsDR);
      }
      return 1; // allways assume success
    }        
      
    /*    
       Turn the (optional) backlight on
       for these displays it's done by hardware with the RGB chip
    */               
    void backlight(void) {
      setRegRGB(REG_OUTPUT, 0xFF);     // table 13: LED driver x individual brightness and group dimming/blinking can be controlled through its PWMx register and the GRPPWM registers.
    }
    
    /*    
       switch on the backlight
       for these displays it's done by hardware with the RGB chip
    */
    void noBacklight(void) {
      setRegRGB(REG_OUTPUT, 0x00);     // table 13: LED driver x is off
    }

    /*    
       set brightness of LED backlight
    */    
    void setBacklight(uint8_t new_val)
    {
      backlightState = new_val;      
      setRGB(blRed, blGreen, blBlue); 
    }

    /*    
       switch on the backlight - LED blinking
    */
    void blinkBacklight(void)
    { 
      // blinking period is controlled through 256 linear steps from 00h (41 ms, frequency 24 Hz) to FFh (10.73 s).
      // blink period in seconds = (<reg 7> + 1) / 24
      // on/off ratio = <reg 6> / 256
      setRegRGB(0x07, 0x17);           // blink every second
      setRegRGB(0x06, 0x7F);           // half on, half off     GRPPWM
    }
    
    /*    
       switch off the backlight - LED blinking
    */    
    void noBlinkBacklight(void) 
    {
      setRegRGB(0x07, 0x00);           // GRPFREQ
      setRegRGB(0x06, 0xFF);           // GRPPWM
    }
    
    /*
       initiliazes the general hardware
       in this class, also the RGB hardware
       MISSING: tbd make it protected
    */
    void hwInit(void) override{        // low level hardware related initialisation, i.e. the ports or an expander
      DEBUG_PRINTLN(F("Wire hwInit"));
#if defined(__AVR__)
  if (TWCR == 0) {
        Wire.begin();                  // only call when not started before
        DEBUG_PRINTLN(F("E: add Wire.begin() in sketch!"));
      }
#endif
      initRGB();                       // that class must ensure to get the RGB chip initialized
    }
    
    /*
       initiliazes the RGB driver chip
       MISSING: tbd make it protected
    */
    void initRGB()
    {
      DEBUG_PRINTLN(F("Wire initRGB"));
      // reset chips on bus            // datasheet chapter 7.6 Software Reset
      Wire.beginTransmission(0x03);    // SWRST I2C-bus address ‘0000 011’ 
      Wire.write(0xA5);                // Byte 1 = A5h: the PCA9633 acknowledges this value only
      Wire.write(0x5A);                // Byte 2 = 5Ah: the PCA9633 acknowledges this value only
      Wire.endTransmission();          // the PCA9633 then resets to the default value (power-up value)        
      // backlight init
      setRegRGB(REG_MODE1, 0);         // table 8: Normal mode, does not respond to I2C-bus subaddress, does not respond to LED All Call I2C-bus address
      // set LEDs controllable by both PWM and GRPPWM registers
      setRegRGB(REG_OUTPUT, 0xFF);     // table 13: LED driver x individual brightness and group dimming/blinking can be controlled through its PWMx register and the GRPPWM registers.
      // set MODE2 values
      // 0010 0000 -> 0x20  (DMBLNK to 1, ie blinky mode)
      setRegRGB(REG_MODE2, 0x20);      // table 9: Bit 5 Group control = blinking
    }
    
    /* 
        sets a RGB Chip register to a value (i.e. one specific color register)
        low level function to I2C registers
        MISSING: tbd: make it protected
    */
    void setRegRGB(unsigned char reg, unsigned char value) {
      Wire.beginTransmission(rgbAddr);
      Wire.write(reg);
      Wire.write(value);
      Wire.endTransmission();
    }
    
    /* 
        sets all three backlight colors
        Sends data with autoincrement, which should be faster but needs 32 bytes more than the former version 
    */
    void setRGB(unsigned char r, unsigned char g, unsigned char b) {   
      // 
      blRed = r;
      blGreen = g;
      blBlue = b;
      const byte autoincrement = 0x80;  // datasheet table 6, Auto-Increment for all registers. D3, D2, D1, D0 roll over to ‘0000’ after the last register (1100) is accessed.
      Wire.beginTransmission(rgbAddr);
      Wire.write((REG_BLUE | autoincrement));
      Wire.write(b * backlightState / 255U);
      Wire.write(g * backlightState / 255U);
      Wire.write(r * backlightState / 255U);
      Wire.endTransmission();     
    }
   
    /*
        sets the three backlight colors
    */
    void setRGB(uint32_t newColor) {
      uint8_t r = newColor >> 16;
      uint8_t g = (newColor & 0xFF00) >> 8;
      uint8_t b = newColor & 0xFF;
      setRGB(r, g, b);  
    }
};

/* ****************************************************************************
    no class for custom pin assignment
   ************************************************************************** */

/* ****************************************************************************
    no class for 4004 display
    (not seen so far)
   ************************************************************************* */
   