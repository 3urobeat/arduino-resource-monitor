/*
  Noiasca Liquid Crystal - 2.x - ALIAS 
  
  This is a file with alias classes, in case someone forgets to migrate to the new files & constructors 
  
  Open topics
  - tbd 2022: change message to alert

  copyright 2021 noiasca noiasca@yahoo.com
  
  2021-05-16
*/

#pragma once
#pragma message ("Warning: Don't include lcd_i2c.h. Use lcd_PCF8574.h instead!" )
// MISSING change to alert in 2022
#include <NoiascaHW/lcd_PCF8574.h>

/*
   Old I2C_base class 
*/
class LiquidCrystal_I2C_base : public LiquidCrystal_PCF8574_base {
  public:     
    LiquidCrystal_I2C_base(uint8_t lcdAddr,
                           uint8_t cols, uint8_t rows) : 
      LiquidCrystal_PCF8574_base(lcdAddr, cols, rows)
      {}      
};

/*
   Old I2C class 
*/
class LiquidCrystal_I2C : public LiquidCrystal_PCF8574 {
  public:
    LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) :
      LiquidCrystal_PCF8574(lcd_Addr, lcd_cols, lcd_rows)
      {}
    
    LiquidCrystal_I2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows, CallBack funcPtr) :
      LiquidCrystal_PCF8574(lcd_Addr, lcd_cols, lcd_rows, funcPtr)
      {}   
};

/*
   I2C class replaces capital Ä with Ae
*/
class LiquidCrystal_I2C_Ae : public LiquidCrystal_PCF8574 {
  public:
    LiquidCrystal_I2C_Ae(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) : 
      LiquidCrystal_PCF8574(lcd_Addr, lcd_cols, lcd_rows, convert_ae) {}
};

/*
   I2C small: adds 3 Variables for the replacement characters, defaults to the small character. Ä -> ä 
*/
class LiquidCrystal_I2C_Small : public LiquidCrystal_PCF8574 {
  public:
    LiquidCrystal_I2C_Small(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) : 
      LiquidCrystal_PCF8574(lcd_Addr, lcd_cols, lcd_rows, convert_small) {}
};

/*
   I2C specials: uses 3 special characters for capital letters Ä Ö Ü 
*/
class LiquidCrystal_I2C_Special : public LiquidCrystal_PCF8574 {
  public:
    LiquidCrystal_I2C_Special(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) : 
      LiquidCrystal_PCF8574(lcd_Addr, lcd_cols, lcd_rows, convert_special) {}
};

/*
   I2C ST7070: uses the extended characters of the ST7070
*/
class LiquidCrystal_I2C_ST7070 : public LiquidCrystal_PCF8574 {
  public:
    LiquidCrystal_I2C_ST7070(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) :
      LiquidCrystal_PCF8574(lcd_Addr, lcd_cols, lcd_rows, convert_ST7070) {}
};

/*
   I2C ST7070_Ext: even more characters from second pattern
*/
class LiquidCrystal_I2C_ST7070_Ext : public LiquidCrystal_PCF8574 {
  public:
    LiquidCrystal_I2C_ST7070_Ext(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) :
      LiquidCrystal_PCF8574(lcd_Addr, lcd_cols, lcd_rows, convert_ST7070_ext) {}
};
