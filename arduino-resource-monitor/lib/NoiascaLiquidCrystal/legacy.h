/*
   additional methods for backward compatibility
   
   these methods will assure backward compatibility to other libraries
   
   2021-08-15

*/

#pragma once

/*
#define LCD_5x10DOTS 0x04 //!< 10 pixel high font mode
#define LCD_5x8DOTS 0x00  //!< 8 pixel high font mode
*/


/*
    Move cursor and shift display without changing DDRAM content
*/
void scrollDisplayLeft() {
  command(cursorshift | cursorshift_displayshift);
}

/*
    Move cursor and shift display without changing DDRAM content
*/
void scrollDisplayRight() {
  command(cursorshift | cursorshift_displayshift | cursorshift_right);
}

/*
    shift the display
*/
void leftToRight() {
  entrymodesetFlag |= entrymode_shift;
  command(entrymodeset | entrymodesetFlag);
}

/*
    display shift
*/
void rightToLeft() {
  entrymodesetFlag &= ~entrymode_shift;
  command(entrymodeset | entrymodesetFlag);
}

/*
    scroll right from cursor 
*/
void autoscroll() {
  entrymodesetFlag |= entrymode_increment;
  command(entrymodeset | entrymodesetFlag);
}

/*
    scroll left from cursor
*/
void noAutoscroll() {
  entrymodesetFlag &= ~entrymode_increment;
  command(entrymodeset | entrymodesetFlag);
}
