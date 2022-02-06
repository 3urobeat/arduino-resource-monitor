/*
   Noiasca Liquid Crystal - NoiascaConverter.cpp
   
   This file contains 
   - LCD specific converters (functions)
   
   copyright 2021 noiasca noiasca@yahoo.com
   
   by noiasca
   2021-09-06 convert_custom checks also the 1 byte UTF-8 against user array
   2021-02-22 all converters: hardened against malformed UTF-8 data
   2021-02-15 reorganised converter files (brought converters to this file)
   2021-02-14 replaced ANSI by ASCII (kept ANSI as alias)
   2021-02-12 Cyrillic converter, SPLC780D1_002A Cyrillic
   2020-09-20 SPLC780D1_003A European
*/

#include <NoiascaConverter.h>
// in:  references to special and the actual character 
// out: true if printable, false if the character is not printable
//             valid values  0 don't print
//                           1 print
//                           2 print from second pattern (e.g. for the ST7070)
//                           4 add an e

/*
  no convertion at all (dummy implementation)
*/
uint8_t convert_dummy(uint32_t &special, uint8_t &value)
{
  (void) special;
  (void) value;
  return 1;
}

/*
   Converts Ä--> A
   HD44780 A00
   could be replaced by convert_uml if we would accept the additional 3 lines of Step2
   but as this is the standard converter, I'll keep it as it is
*/
uint8_t convert(uint32_t &special, uint8_t &value) {
  //Serial.print("D040 convert 0x"); Serial.println(value, HEX);
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)        // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)   // sequence byte (Folgebyte)
  {
    special = special << 8;            // MISSING absichern gegen 3tes Folgebyte
    special += value;
    if (special > 0xC000 && special < 0xE000)            // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)      // 3 byte character: 0b11100000 00000000 00000000 im dritten byte von hinten
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)             // 4 byte character: 0b11110000 ganz vorne
    {
      isWriteable = PRINT;
    }
  }
  else                                         // not a sequence byte - reset special
  {                                            // implicit all 0x0nnnnnnnn
     special = 0;
  }
  if (special == 0)          // Single Character (implicit) 
  {
    switch ( value)          // overwrite missmatching characters between UTF-8 and charset of ROM Code A00
    {
      case 0x5c :            // REVERSE SOLIDUS (better known as backslash)
        value = '|';         // no better alternativ in char set A00
        break;
      case 0x7e :            // tilde ~
        value = '-';
        break;
    }
    isWriteable = PRINT;     // any other character just send to display
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup with in UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    for (size_t i = 0; i < sizeof(ROM_A00) / sizeof(ROM_A00[0]); i++)
    {
      index = pgm_read_dword_near(&ROM_A00[i].utf8);
      if (index == (special & 0xFFFF))
      {
        memcpy_P(&value, &ROM_A00[i].c, 1);
        break;
      }
    }
    // Step 2: Ä Ö Ü overruleUml
    // not in this converter 
    // Step 3: Send
    // any other character just send to display
    special = 0;
    // Step 4: only in Ae Variant to add the e
  }
  return isWriteable;                  
}

/*
   Converts Ä--> Ae
   HD44780 A00
*/
uint8_t convert_ae(uint32_t &special, uint8_t &value) {
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)        // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)   // sequence byte (Folgebyte)
  {
    special = special << 8;            // MISSING should get improvement against 3rd following byte
    special += value;
    if (special > 0xC000 && special < 0xE000)            // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)      // 3 byte character: 0b11100000 00000000 00000000 im dritten byte von hinten
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)             // 4 byte character: 0b11110000 ganz vorne
    {
      isWriteable = PRINT;
    }
  }
  else                                         // not a sequence byte - reset special
  {            
     special = 0;
  }
  byte position = -1;
  if (special == 0)          // Single Character
  {
    switch ( value)          // overwrite missmatching characters between UTF-8 and charset of ROM Code A00
    {
      case 0x5c :            // REVERSE SOLIDUS (better known as backslash)
        value = '|';         // no better alternativ in char set A00
        break;
      case 0x7e :            // tilde ~
        value = '-';
        break;
    }
    isWriteable = 1;         // any other character just send to display
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup with in UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    for (size_t i = 0; i < sizeof(ROM_A00) / sizeof(ROM_A00[0]); i++)
    {
      index = pgm_read_dword_near(&ROM_A00[i].utf8);
      if (index == (special & 0xFFFF))
      {
        memcpy_P(&value, &ROM_A00[i].c, 1);
        position = i;
         break;
      }
    }
    // Step 2: Ä Ö Ü overruleUml
    // not needed, is in mapping table
    // Step 3: Send
    // any other character just send to display
    isWriteable = PRINT;
    special = 0;
    // Step 4: Ae Variant to add the e
    if (position < 3) isWriteable = ADDE;
  }
  return isWriteable;                  // assume success
}

/*
   Converts Ä --> Any character handed over in the parameter
   "internal" main function which will be called from several other converters
   accepts parameters for Ä Ö Ü
*/
uint8_t convert_uml(uint32_t &special, uint8_t &value, char umlA, char umlO, char umlU) {
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)        // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)   // sequence byte (Folgebyte)
  {
    special = special << 8;            // MISSING should get improvement against 3rd following byte
    special += value;
    if (special > 0xC000 && special < 0xE000)            // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)      // 3 byte character: 0b11100000 00000000 00000000 im dritten byte von hinten
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)             // 4 byte character: 0b11110000 ganz vorne
    {
      isWriteable = PRINT;
    }
  }
  else                                         // not a sequence byte - reset special
  {            
     special = 0;
  }
  if (special == 0)          // Single Character
  {
    switch ( value)          // overwrite missmatching characters between UTF-8 and charset of ROM Code A00
    {
      case 0x5c :            // REVERSE SOLIDUS (better known as backslash)
        value = '|';         // no better alternativ in char set A00
        break;
      case 0x7e :            // tilde ~
        value = '-';
        break;
    }
    // any other character just send to display
    isWriteable = PRINT;
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup with in UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    for (size_t i = 0; i < sizeof(ROM_A00) / sizeof(ROM_A00[0]); i++)
    {
      index = pgm_read_dword_near(&ROM_A00[i].utf8);
      if (index == (special & 0xFFFF))
      {
        memcpy_P(&value, &ROM_A00[i].c, 1);
        break;
      }
    }
    // Step 2: Ä Ö Ü overruleUml
    if (special == 0xC384) value = umlA;
    else if (special == 0xC396) value = umlO;
    else if (special == 0xC39C) value = umlU;
    // Step 3: Send
    // any other character just send to display
    special = 0;
    // Step 4: 
  }
  return isWriteable;                  // assume success
}

/*
   Converts Ä--> ä
   HD44780 A00 (implicit)
*/
uint8_t convert_small(uint32_t &special, uint8_t &value)    // Ä -> ä
{
  return convert_uml(special, value, 0xE1, 0xEF, 0xF5);
}

/*
   Converts Ä--> Ä
   HD44780 A00 (implicit)
   needs 3 special characters for Ä Ö Ü
*/
uint8_t convert_special(uint32_t &special, uint8_t &value) // Ä -> Ä using special characters
{
  return convert_uml(special, value, 5, 6, 7);             // replaces German umlaut with special character @ 5, 6, 7
}

/*
  Converter for ST7070 
  (small variant)
*/
uint8_t convert_ST7070(uint32_t &special, uint8_t &value) {  
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)                  // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)             // sequence byte (Folgebyte)
  {
    special = special << 8;                                // MISSING should get improvement against 3rd following byte
    special += value;
    if (special > 0xC000 && special < 0xE000)              // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)   // 3 byte character: 0b11100000 00000000 00000000 in MSB
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)                         // 4 byte character: 0b11110000 in MSB
    {
      isWriteable = PRINT;
    }
  }
  else                                                     // not a sequence byte - reset special
  {            
     special = 0;
  }
  bool hiPattern = false;
  if (special == 0)          // Single Character
  {
    switch ( value)          // overwrite missmatching characters between UTF-8 and charset of ROM Code A00
    {
      case 0x5c :            // REVERSE SOLIDUS (better known as backslash)
        value = 0x88;        // available in ST7070 charset
        break;
      case 0x7e :            // tilde ~
        value = '-';
        break;
    }
    isWriteable = PRINT;     // any other character just send to display
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup with in UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    
    if (special >= 0xC380 && special <= 0xC38B)  // series of Latin letters
    {
      value = special - 0xC380 + 0x14;           // LATIN CAPITAL LETTER A WITH GRAVE is at 0x14 in Pattern 1
    }
    else if (special >= 0xC38C && special <= 0xC3bf)  // series of Latin letters
    {
      value = special - 0xc38c;                  // Ì LATIN CAPITAL LETTER I WITH GRAVE is first letter in Pattern 2
      hiPattern = true;
    }
    else
    {
      for (size_t i = 0; i < sizeof(ROM_ST7070) / sizeof(ROM_ST7070[0]); i++)
      {
        index = pgm_read_dword_near(&ROM_ST7070[i].utf8);
        if (index == (special & 0xFFFF))
        {
          memcpy_P(&value, &ROM_ST7070[i].c, 1);
          break;
        }
      }
    }
    // Step 2: Ä Ö Ü overruleUml - not needed, in mapping table
    // Step 3: Output
    if (hiPattern) 
      isWriteable = PRINT2;
    else
      isWriteable = PRINT;         // any other character just send to display
    special = 0;
    // Step 4:
  }
  return isWriteable;
}  


/*
  Converter for ST7070 
  (extended version)
*/
uint8_t convert_ST7070_ext(uint32_t &special, uint8_t &value) {
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)                  // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)             // sequence byte (Folgebyte)
  {
    special = special << 8;                                // MISSING should get improvement against 3rd following byte
    special += value;
    if (special > 0xC000 && special < 0xE000)              // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)   // 3 byte character: 0b11100000 00000000 00000000 in MSB
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)                         // 4 byte character: 0b11110000 in MSB
    {
      isWriteable = PRINT;
    }
  }
  else                                                     // not a sequence byte - reset special
  {            
     special = 0;
  }
  bool hiPattern = false;
  if (special == 0)          // Single Character
  {
    switch ( value)          // overwrite missmatching characters between UTF-8 and charset of ROM Code A00
    {
      case 0x5c :            // REVERSE SOLIDUS (better known as backslash)
        value = 0x88;        // available in ST7070 charset
        break;
      case 0x7e :            // tilde ~
        value = '-';
        break;
    }
    isWriteable = PRINT;     // any other character just send to display
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup with in UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    if (special >= 0xC380 && special <= 0xC38B)            // series of Latin letters
    {
      value = special - 0xC380 + 0x14;                     // LATIN CAPITAL LETTER A WITH GRAVE is at 0x14 in Pattern 1
    }
    else if (special >= 0xC38C && special <= 0xC3bf)       // series of Latin letters
    {
      value = special - 0xc38c;        // Ì LATIN CAPITAL LETTER I WITH GRAVE is first letter in Pattern 2
      hiPattern = true;
    }
    else
    {
      bool found = false;
      for (size_t i = 0; i < sizeof(ROM_ST7070) / sizeof(ROM_ST7070[0]); i++)
      {
        index = pgm_read_dword_near(&ROM_ST7070[i].utf8);
        if (index == (special & 0xFFFF))
        {
          memcpy_P(&value, &ROM_ST7070[i].c, 1);
          found = true;
          break;
        }
      }
      // if still not found, try second pattern
      if (!found)
      {
        for (size_t i = 0; i < sizeof(ROM_ST7070_P2) / sizeof(ROM_ST7070_P2[0]); i++)
        {
          index = pgm_read_dword_near(&ROM_ST7070_P2[i].utf8);
          if (index == (special & 0xFFFF))
          {
            memcpy_P(&value, &ROM_ST7070_P2[i].c, 1);
            hiPattern = true;
            found = true;
            break;
          }
        }
      }
    }
    // Step 2: Ä Ö Ü overruleUml - not needed, in mapping table
    // Step 3: Output
    if (hiPattern) 
      isWriteable = PRINT2;
    else
      isWriteable = PRINT;
    special = 0;
    // Step 4:
  }
  return isWriteable;
}

/*
   SPLC780D1 002A
   different ROM - more characters
   cyrillic in the upper area
   code duplicated convert_SPLC780D1_002A, convert_SPLC780D1_003A
*/
uint8_t convert_SPLC780D1_002A(uint32_t &special, uint8_t &value) {
  //Serial.print("D457 convert_SPLC780D1_002A 0x"); Serial.println(value, HEX);
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)        // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)   // sequence byte (Folgebyte)
  {
    special = special << 8;                      // MISSING should get improvement to check on 3rd sequence byte
    special += value;
    if (special > 0xC000 && special < 0xE000)    // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)      // 3 byte character: 0b11100000 00000000 00000000 im dritten byte von hinten
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)               // 4 byte character: 0b11110000 ganz vorne
    {
      isWriteable = PRINT;
    }
  }
  else                                                     // not a sequence byte - reset special
  {            
     special = 0;
  }
  if (special == 0)          // Single Character
  {
    // no difference mapping to ASCII needed
    isWriteable = PRINT;     // any other character just send to display
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup with in UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    for (size_t i = 0; i < sizeof(ROM_SPLC780D1_002A) / sizeof(ROM_SPLC780D1_002A[0]); i++)
    {
      index = pgm_read_dword_near(&ROM_SPLC780D1_002A[i].utf8);
      if (index == (special & 0xFFFF))
      {
        memcpy_P(&value, &ROM_SPLC780D1_002A[i].c, 1);
        break;
      }
    }
    // Step 2: Ä Ö Ü overruleUml
    // not in this converter 
    // Step 3: Send
    // any other character just send to display
    special = 0;
    // Step 4: only in Ae Variant to add the e
  }
  return isWriteable;                  
}

/*
   SPLC780D1 003A
   no mapping in ASCII needed
   different ROM - more characters
   European
   code duplicated convert_SPLC780D1_002A, convert_SPLC780D1_003A
*/
uint8_t convert_SPLC780D1_003A(uint32_t &special, uint8_t &value) {
  //Serial.print("D521 convert_SPLC780D1_003A 0x"); Serial.println(value, HEX);
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)        // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)   // sequence byte (Folgebyte)
  {
    special = special << 8;                      // MISSING should get improvement to check on 3rd sequence byte
    special += value;
    if (special > 0xC000 && special < 0xE000)    // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)   // 3 byte character: 0b11100000 00000000 00000000 im dritten byte von hinten
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)               // 4 byte character: 0b11110000 ganz vorne
    {
      isWriteable = PRINT;
    }
  }
  else                                                     // not a sequence byte - reset special
  {            
     special = 0;
  }
  if (special == 0)          // Single Character
  {
    // no difference mapping to ASCII needed
    isWriteable = PRINT;     // any other character just send to display
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup with in UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    for (size_t i = 0; i < sizeof(ROM_SPLC780D1_003A) / sizeof(ROM_SPLC780D1_003A[0]); i++)
    {
      index = pgm_read_dword_near(&ROM_SPLC780D1_003A[i].utf8);
      if (index == (special & 0xFFFF))
      {
        memcpy_P(&value, &ROM_SPLC780D1_003A[i].c, 1);
        break;
      }
    }
    // Step 2: Ä Ö Ü overruleUml
    // not in this converter 
    // Step 3: Send
    // any other character just send to display
    special = 0;
    // Step 4: only in Ae Variant to add the e
  }
  return isWriteable;                  
}


/*
   Generic converter WITHOUT HW specific, without äöü overrule (as this should be in the ROM table)
   needs additional parameters to define the table
   code based on convert_SPLC780D1_002A, convert_SPLC780D1_003A - but changed to accept the mapping table
*/
uint8_t convert_generic(uint32_t &special, uint8_t &value, const Mapping *language, size_t lang_len) {
  //Serial.print("D521 convert_SPLC780D1_003A 0x"); Serial.println(value, HEX);
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)        // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)   // sequence byte (Folgebyte)
  {
    special = special << 8;                      // MISSING should get improvement to check on 3rd sequence byte
    special += value;
    if (special > 0xC000 && special < 0xE000)    // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)   // 3 byte character: 0b11100000 00000000 00000000 im dritten byte von hinten
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)               // 4 byte character: 0b11110000 ganz vorne
    {
      isWriteable = PRINT;
    }
  }
  else                                                     // not a sequence byte - reset special
  {            
    special = 0;
  }
  if (special == 0)          // Single Character
  {
    // no difference mapping to ASCII needed
    isWriteable = PRINT;     // any other character just send to display
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup with in UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    for (size_t i = 0; i < lang_len; i++)
    {
    index = pgm_read_dword_near(&language[i].utf8);
      if (index == (special & 0xFFFF))
      {
        memcpy_P(&value, &language[i].c, 1);
        break;
      }
    }
    // Step 2: Ä Ö Ü overruleUml
    // not in this converter 
    // Step 3: Send
    // any other character just send to display
    special = 0;
    // Step 4: only in Ae Variant to add the e
  }
  return isWriteable;                  
}

/*
   LEC1621 font 1 
*/
uint8_t convert_LEC1621_01(uint32_t &special, uint8_t &value) {
  return convert_generic(special, value, ROM_LEC1621_01, sizeof(ROM_LEC1621_01)/sizeof(ROM_LEC1621_01[0]));
} 

/*
   LEC1621 font 2 
   MISSING: the Latin letters are in ascending order and should be implemented in a separate converter 
            similar to ST7070_ext to save flash memory (3 x 16 characters)
            the mapping is complete, but there are several 7bit characters not according to ASCII
            this should be fixed with a separate converter
*/
uint8_t convert_LEC1621_02(uint32_t &special, uint8_t &value) {
  return convert_generic(special, value, ROM_LEC1621_02, sizeof(ROM_LEC1621_02)/sizeof(ROM_LEC1621_02[0]));
} 

/*
   LEC1621 font 3 
   reuse of ROM_SPLC780D1_003A
*/
uint8_t convert_LEC1621_03(uint32_t &special, uint8_t &value) {
  return convert_generic(special, value, ROM_SPLC780D1_003A, sizeof(ROM_SPLC780D1_003A)/sizeof(ROM_SPLC780D1_003A[0]));
} 

/*
   HD44780 A00
   Replaces not existing diacritic letters with their native counterpart
   accepts addtional parameter for the language mapping (language families)
   will be called by several specific alias functions
*/
uint8_t convert_HW(uint32_t &special, uint8_t &value, const Mapping *language, size_t lang_len) {
  //Serial.print("D584 convert 0x"); Serial.println(value, HEX);
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)        // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)   // sequence byte (Folgebyte)
  {
    special = special << 8;                      // MISSING should get improvement to check on 3rd sequence byte
    special += value;
    if (special > 0xC000 && special < 0xE000)              // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)   // 3 byte character: 0b11100000 00000000 00000000 im dritten byte von hinten
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)             // 4 byte character: 0b11110000 ganz vorne
    {
      isWriteable = PRINT;
    }
  }
  else                                                     // not a sequence byte - reset special
  {            
     special = 0;
  }
  if (special == 0)          // Single Character
  {
    switch ( value)          // overwrite missmatching characters between UTF-8 and charset of ROM Code A00
    {
      case 0x5c :            // REVERSE SOLIDUS (better known as backslash)
        value = '|';         // no better alternativ in char set A00
        break;
      case 0x7e :            // tilde ~
      value = '-';
        break;
    }
    isWriteable = PRINT;     // any other character just send to display
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup with in UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    bool found = false;
    for (size_t i = 0; i < sizeof(ROM_A00) / sizeof(ROM_A00[0]); i++)
    {
      index = pgm_read_dword_near(&ROM_A00[i].utf8);
      if (index == (special & 0xFFFF))
      {
        memcpy_P(&value, &ROM_A00[i].c, 1);
        found = true;
        break;
      }
    }
    // Step 2: Ä Ö Ü overruleUml - Language Specific
    if (!found)
    {
      for (size_t i = 0; i < lang_len; i++)
      {
        index = pgm_read_dword_near(&language[i].utf8);
        if (index == (special & 0xFFFF))
        {
          memcpy_P(&value, &language[i].c, 1);
          break;
        }
      }  
    } 
    // Step 3: Send
    // any other character just send to display
    special = 0;
    // Step 4: only in Ae Variant to add the e
  }
  return isWriteable;                  
}


/*
  Cyrillic to plain ASCII
*/
uint8_t convert_cyrillic_min(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, cyrillic_GOST16876_71_1, sizeof(cyrillic_GOST16876_71_1)/sizeof(cyrillic_GOST16876_71_1[0]));
}

/*
  the full ASCII converter
*/
uint8_t convert_ASCII(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, UTF8_ASCII, sizeof(UTF8_ASCII)/sizeof(UTF8_ASCII[0]));
} 

// MISSING: to be deleted in 1.2.4
// Alias function for backward compatibility
uint8_t convert_ANSI(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, UTF8_ASCII, sizeof(UTF8_ASCII)/sizeof(UTF8_ASCII[0]));
} 

/*
  The following converters are smaller subsets of the convert_ASCII to save flash/program memory
*/

/*
  Western
*/
uint8_t convert_gmw(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_gmw, sizeof(lang_gmw)/sizeof(lang_gmw[0]));
} 

/*
  Northern
*/
uint8_t convert_gmq(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_gmq, sizeof(lang_gmq)/sizeof(lang_gmq[0]));
} 

/*
  Slavic
*/
uint8_t convert_sla(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_sla, sizeof(lang_sla)/sizeof(lang_sla[0]));
} 
/*
  Romance
*/
uint8_t convert_roa(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_roa, sizeof(lang_roa)/sizeof(lang_roa[0]));
} 

/*
  German
*/
uint8_t convert_de(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_de, sizeof(lang_de)/sizeof(lang_de[0]));
  }

/*
  English
*/
uint8_t convert_en(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_en, sizeof(lang_en)/sizeof(lang_en[0]));
}

/*
  Spanish and Catalan
*/
uint8_t convert_es(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_es, sizeof(lang_es)/sizeof(lang_es[0]));
}  

/*
  Finnish
*/
uint8_t convert_fi(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_fi, sizeof(lang_fi)/sizeof(lang_fi[0]));
}

/*
  French
*/
uint8_t convert_fr(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_fr, sizeof(lang_fr)/sizeof(lang_fr[0]));
}

/*
  Hungarian
*/
uint8_t convert_hu(uint32_t &special, uint8_t &value) {
  return convert_HW(special, value, lang_hu, sizeof(lang_hu)/sizeof(lang_hu[0]));
}


/*
   maps UTF-8 characters including up to 8 custom characters
   it is somehow similar to convert_uml but it relies on 
   an (extern) array for the replacement to custom characters
*/
uint8_t convert_custom (uint32_t &special, uint8_t &value)
{
  uint8_t isWriteable = NOPRINT;
  if ((value & 0b11000000) == 0b11000000)        // start byte
  {
    special = value;
  }
  else if ((value & 0b11000000) == 0b10000000)   // sequence byte (Folgebyte)
  {
    special = special << 8;            // MISSING should get improvement against 3rd following byte
    special += value;
    if (special > 0xC000 && special < 0xE000)    // if 2 byte character
    {
      isWriteable = PRINT;
    }
    else if (special > 0xE00000 && special < 0xF0000000)   // 3 byte character: 0b11100000 00000000 00000000 im dritten byte von hinten
    {
      isWriteable = PRINT;
    }
    else if (special > 0xF0000000)               // 4 byte character: 0b11110000 ganz vorne
    {
      isWriteable = PRINT;
    }
  }
  else                                           // not a sequence byte - reset special
  {            
     special = 0;
  }
  if (special == 0)          // Single Character
  {
    switch ( value)          // overwrite missmatching characters between UTF-8 and charset of ROM Code A00
    {
      case 0x5c :            // REVERSE SOLIDUS (better known as backslash)
        value = '|';         // no better alternativ in char set A00
        break;
      case 0x7e :            // tilde ~
        value = '-';
        break;
    }
    // check against defined character array
    for (byte i = 0; i < 8; i++)
    {
      if (value == utf8_to_customChar[i])
      {
        value = i;
        break;               // found, no need to continue the search
      }
    }
    isWriteable = PRINT;
  }
  else if (isWriteable)      // was UTF-8 2 bytes or more
  {
    // Step 1: general ROM Mapping
    // Lookup within UTF8->ROM Mapping overrides the character to be send to the LCD
    uint16_t index = 0;
    for (size_t i = 0; i < sizeof(ROM_A00) / sizeof(ROM_A00[0]); i++)
    {
      index = pgm_read_dword_near(&ROM_A00[i].utf8);
      if (index == (special & 0xFFFF))
      {
        memcpy_P(&value, &ROM_A00[i].c, 1);
        break;
      }
    }
    // Step 2: Special character overrule
    for (byte i = 0; i < 8; i++)
    {
      if (special == utf8_to_customChar[i])
      {
        value = i;
        break;               // found, no need to continue the search
      }
    }
    // Step 3: Send
    // any other character just send to display
    special = 0;
    // Step 4:
  }
  return isWriteable;                  // assume success
}

uint32_t utf8_to_customChar[8]; // needed for convert_custom - hast to be declared extern in .h

