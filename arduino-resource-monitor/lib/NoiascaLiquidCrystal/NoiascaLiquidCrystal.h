/*
  Noiasca Liquid Crystal - 2.x
  
  copyright 2021 noiasca noiasca@yahoo.com
  
  Version
  2021-05-29 fixes für ESP8266, ESP32
  2021-05-17
  2021-03-13 started new development
*/


#pragma once
#include <Arduino.h>

//#define DEBUG_NLC    // activates debug messages

#ifdef DEBUG_NLC
  #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
  #define DEBUG_PRINTLN(x)
#endif

//#include <Print.h>

// the converters
#include <utility/NoiascaUTF8.h>                 // contains the plain UTF8 to ASCII mapping
#include <utility/NoiascaConverter.h>            // all converters
#include <utility/NoiascaCustomCharacters.h>     // some predefined custom characters

// backward compatibilty 
// for begin method:
#define LCD_5x10DOTS 0x04                        // F  5x10
#define LCD_5x8DOTS 0x00                         // F  5x7
typedef enum {POSITIVE, NEGATIVE, FAULTY} t_backlightPol; // compatible to the very common New Liquid Crystal

/*
   a dummy class as parent for all hardware implementations
   
*/
class LiquidCrystal_dummy {
//class LiquidCrystal_dummy : public Print {
  protected:
    // Table 6 Instructions p24
    static const uint8_t cleardisplay = 0x01;
    static const uint8_t returnhome = 0x02;
    static const uint8_t entrymodeset = 0x04;
    static const uint8_t displaycontrol = 0x08;
    static const uint8_t cursorshift = 0x10;
    static const uint8_t functionset = 0x20;
    static const uint8_t setcgramaddress = 0x40;           // characters
    static const uint8_t setddramaddress = 0x80;           // 
    // entry mode set 0x04
    static const uint8_t entrymode_shift = 0x01;           // S Shift
    static const uint8_t entrymode_increment = 0x02;       // I/D Increment/Decrement  
    // Display on/off controll 0x08                        
    static const uint8_t displaycontrol_blink = 0x01;      // B blinking cursor position
    static const uint8_t displaycontrol_cursor = 0x02;     // C cursor on
    static const uint8_t displaycontrol_on = 0x04;         // D display on
    // Curosr or display shift 0x10                        
    static const uint8_t cursorshift_right = 0x04;         // R/L Right Shift / Left Shift
    static const uint8_t cursorshift_displayshift = 0x08;  // S/C Display Shift / Cursor Move
    // function set 0x20                                   
    static const uint8_t functionset_font = 0x04;          // F sets the character font to 5x10  (5x7 is 0x00)
    static const uint8_t functionset_lines = 0x08;         // N Sets the number of display lines - 2 line mode
    static const uint8_t functionset_8bit = 0x10;          // DL sets the interface data length (8bit)
  
    // might be changed on instance level (one day...)
    const uint8_t waitshort = 37;                          // datasheet 37us 
    const uint16_t waitlong = 1500;
    uint8_t rowStartingAddress[4] {0x00, 0x40, 0x14, 0x54};// variables to enable modification for 16x4 and 40x4
    const uint8_t cols;                                    // real LCD colums - initilized with constructor
    const uint8_t rows;                                    // real LCD rows   - initilized with constructor
    uint8_t currentRow = 255;                              // keep track of the actual row, needed for 4004 
    
    // separate flag as there are several attributes to be set in a single method according to the LCD API (without the function set bit itself
    uint8_t entrymodesetFlag;                              // I/D S
    uint8_t displaycontrolFlag = displaycontrol_on;       
    //uint8_t cursorshiftFlag;                             // S/C R/L
 
    uint32_t special = 0;                                  // store special character 1680/64  --> move down to converter classes only
    
    // Pin control constants
    static const byte rsIR = 0;                            // register select: Instruction Register (commando)
    static const byte rsDR = 1;                            // register select: Data Register
    static const byte rwWrite = 0;                         // read/write WRITE
    static const byte rwRead = 1;                          // read/write READ
    
/* 
    must be implemented in HW class because classes differntiate between 4bit and 8bit
    virtual because other methods use send and so we need a late binding
    to make it compile for esp8266, esp32 we need it in the base class
*/ 
    
    virtual void send(uint8_t, uint8_t, uint8_t = rwWrite);
    //virtual void send(uint8_t, uint8_t, uint8_t = rwWrite) = 0; // avoids link error for esp8266 https://stackoverflow.com/questions/9406580/c-undefined-reference-to-vtable-and-inheritance
  
    //testwise local implementation
    //void send(uint8_t, uint8_t);
    //virtual void setRegisterSelect(uint8_t);
    //virtual void write4bits(uint8_t);
    
  public:
    LiquidCrystal_dummy(uint8_t, uint8_t);                 // constructor
    
    //virtual ~LiquidCrystal_dummy();
    

/*  
    init()
    LCD API: Initializes the display.
    Clears the screen and puts the cursor to 0, 0
    noiasca: begin() methods are on implementation level
             and therefore this alias also
*/
    
/* 
    setDelay(Cmd,Char)
    LCD API: Lets you override the static delays in the library.
    Some displays require delays to allow command to complete
    Typically there are two different delays.
    One for LCD commands, and one when sending characters to the display.
    The library should set the default for the display.
    noiasca: only empty alias
             no implementaion in this library, 
             but some classes might use different delays if necessary.
*/
    void setDelay(uint8_t, char);

/*
    write(value)
    LCD API: Raw Write a value to the display
    noiasca: the write is defined in the HW implementation
*/    
    //virtual size_t write(uint8_t);
//    size_t write(uint8_t);         - testweise für buffer deaktiviert
    
/*
    writeOld(value)
    noiasca: behaves like the original write 
*/
    size_t writeOld(uint8_t);                              // legacy write of special characters >=0xC0

/*
    LCD API: Send a command to the display, for commands not supported by the library
    noiasca: this a "transmit to LCD" with "Register select" = 0 Instruction Register
    (so something like a data command in contrast with 1 Data Register doesn't exist officially)
*/
    void command(uint8_t);

/*  
    LCD API: Clear the display and place cursor at 0,0
*/
    void clear();  
    
/*  
    home()
    LCD API: Home the cursor to 0,0 and leave displayed characters
*/
    void home();

/*
    setCursor(Row, Col)
    LCD API: Where Row 0-MAXLINEs, and Col 0-MAXCOLUMNS
    every common LCD implmentation is column - row, therefore also in this library
*/
    void setCursor(uint8_t, uint8_t);
    
/*
    cursor_on()
    LCD API: Turn the block cursor on
    noiasca: keep as alias only
*/
    void cursor();
    void cursor_on() {cursor();};
    
/* 
    cursor_off()
    LCD API: Turn the block cursor off
    noiasca: keep as alias only
*/
    void noCursor();
    void cursor_off() {noCursor();};

/*
    blink_on()
    LCD API: Turn on the blinking underline cursor
    noiasca: keep as alias only - use blink() instead
*/
    void blink();
    void blink_on() {blink();};
    
/*  
    blink_off()
    LCD API: Turn off the blinking underline cursor 
    noiasca: keep as alias only - use noBlink() instead
*/
    void noBlink();
    void blink_off() {noBlink();};

// Optional Functions:
// Should be included in the library, but might not be functional 

/*
    setBacklight(val)
    LCD API optional: Set the backlight off/on, or set Backlight brightness (0-255)
    If the display only has the option to turn the backlight on and off: 0 = off, >0 = on
    noiasca: available but, implemented in the HW classes
*/

/* 
    setContrast(val)
    LCD API optional: Set the contrast value of the display (0-255)
    noiasca: empty alias only
*/
    void setContrast(int);

/* 
    on()
    LCD API optional: Turn the LCD display on
    If the display does not have an option to turn on/off just turn backlight on/off
*/
    void on();

/* 
    off()
    LCD API optional: Turn the LCD display off
*/
    void off();

/*
    status()
    LCD API optional: Return the status of the display
    Returns the FIFO buffer on the robot-electronics display
    Can be used to get r/w status of hardwired displays 
*/

// Extend Functions:
// Things that are specific to a particular display 

/*
    load_custom_character(Char_Num,*Rows)
    LCD API extend: Load up a custom character [0-7]
    *Rows is an array of 7 bytes that contain bitmap
    To send custom character to display use lcd.write(0-7);
    See sample sketch on how to implement this.
    noiasca: most libraries use "createChar" instead
             so the LCD API name is used as alias only
             this library expects a 5x8(!) character - not 7 bytes like the API description
*/

    void createChar(const uint8_t, const uint8_t[]);       // all major libraries use this method name 
    void load_custom_character(const uint8_t Char_Num , const uint8_t Rows[]) {createChar(Char_Num, Rows);};
    void createChar_P(const uint8_t, const uint8_t *);     // the implementation for custom characters in PROGMEM      
   
/*
    keypad()
    LCD API extend: Read value from keypad
    Keypad page
    noiasca: no implementation
*/

/*
    printstr(Str)
    LCD API extend: Print a string without delay. 
    noiasca: no implementation
*/

// additional methods (not from LCD API)
/*
    creates 3 German Umlauts using Special Characters 5, 6, and 7
*/
    void createUml();
    
/*
  sets the used font
  some OLED displays support extended commands
  which is used for this command
*/    
    void setFont(uint8_t value = 0);
    
/*
  there are some legacy methods which are included for backward compatibilty
*/
#include <legacy.h>
    
};
