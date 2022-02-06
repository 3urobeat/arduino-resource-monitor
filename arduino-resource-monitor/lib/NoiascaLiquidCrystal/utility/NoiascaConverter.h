/*
   Noiasca Liquid Crystal - NoiascaConverter.h
   
   This file contains 
   - LCD specific converters (functions)
   - LCD specific mapping tables (constants in PROGMEM) to mapp a UTF-8 to a LCD Character ROM
   
   It is a separate file, because these mappings are very specific to LCD Hardware 
   
   copyright 2021 noiasca noiasca@yahoo.com

   Version
   2021-10-30 constexpr
   2021-04-04 updated Cyrillic converter & SPLC780D1_002A, changed to UTF-8 w/o BOM 
   2021-02-15 reorganised converter files (brought converters to this file, brought mapping to this file)
   2021-02-12 Cyrillic converter, SPLC780D1_002A Cyrillic
   2021-01-08 changed access specifier (release candidate 1.2.2)
   2020-12-20 optional normalization of UTF-8 characters
   2020-10-11 more Latin characters for SPLC780D1
   2020-10-06 SPLC780D1_003A European (release candidate 1.2.0)
 */

#pragma once
#ifndef NOIASCA_CONVERTER_H
#define NOIASCA_CONVERTER_H
#include <Arduino.h>
#include <utility/NoiascaUTF8.h>      // the struct Mapping is defined there

// these tables map the UTF-8 characters to an existing ROM Address on the LCD:
// the struct Mapping is definied in NoiascaUTF8.h

// the standard HD44780 A00 ROM character set (English - Japanese)
const Mapping ROM_A00[] PROGMEM = {
  {0xC384, 'A'},   // Ä   // large Umlauts may be overwritten in the write method
  {0xC396, 'O'},   // Ö
  {0xC39C, 'U'},   // Ü
  {0xC3A4, 0xE1},  // ä   // sorted to top, assumption: ä ö ü are used more often
  {0xC3B6, 0xEF},  // ö
  {0xC3BC, 0xF5},  // ü
  {0xC39F, 0xE2},  // ß to GREEK SMALL LETTER BETA
  {0xC2A3, 0xED},  // £ Pound SIGN
  {0xC2A5, 0x5C},  // ¥ YEN SIGN
  {0xC2B0, 0xDF},  // DEGREE SIGN#
  {0xC2B5, 0xE4},  // MICRO SIGN
  {0xC2B7, 0xA5},  // MIDDLE DOT
  {0xC3B1, 0xEE},  // LATIN SMALL LETTER N WITH TILDE
  {0xC3B7, 0xFD},  // DIVISION SIGN
  {0xC69F, 0xF2},  // LATIN CAPITAL LETTER O WITH MIDDLE TILDE  could also be used for e2 88 91 N-ARY SUMMATION
  {0xCEA3, 0xF6},  // GREEK CAPITAL LETTER SIGMA
  {0xCEB1, 0xE0},  // GREEK SMALL LETTER ALPHA
  {0xCEB2, 0xE2},  // GREEK SMALL LETTER BETA
  {0xCEB5, 0xE3},  // GREEK SMALL LETTER EPSILON
  {0xCEBC, 0xE4},  // GREEK SMALL LETTER MU
  {0xCF80, 0xF7},  // GREEK SMALL LETTER PI
  {0xCF81, 0xE6},  // GREEK SMALL LETTER RHO
  {0xCEA9, 0xF4},  // GREEK CAPITAL LETTER OMEGA
  // some 3byte characters padded to 2 byte
  {0x8690, 0x7F},  // LEFTWARDS ARROW                         0xE28690
  {0x8692, 0x7E},  // RIGHTWARDS ARROW                        0xE28692
  {0x889A, 0xE8},  // SQUARE ROOT                             0xE2889A
  {0x889D, 0xE0},  // PROPORTIONAL TO                         0xE2889D
  {0x889E, 0xF3},  // Infiniy                                 0xE2889E
  {0xe1a0, 0xED}   // LATIN CAPITAL LETTER L WITH DOUBLE BAR  0xE2B1A0
};


// the ST7070 ROM character set (page 1)
const Mapping ROM_ST7070[] PROGMEM = {
  {0xc2a1, 0x86},  // ¡	INVERTED EXCLAMATION MARK
  {0xc2a5, 0x90},  // § SECTION SIGN
  {0xc2a6, 0x87},  //	¦ BROKEN BAR
  {0xc2ac, 0x8b},  // ¬	NOT SIGN
  {0xc2af, 0x89},  // ¯	MACRON
  {0xc2b6, 0x97},  // ¶ PILCROW SIGN
  {0xc2bf, 0x13},  // ¿ INVERTED QUESTION MARK
  
  {0xC2A2, 0x8C},  // cent SIGN ?
  {0xC2A3, 0x95},  // £ Pound SIGN ist eigentlich ed
  {0xC2A5, 0x5C},  // ¥ YEN SIGN
  {0xC2B0, 0xDF},  // DEGREE SIGN#
  {0xC2B5, 0xE4},  // MICRO SIGN
  {0xC2B7, 0xA5},  // MIDDLE DOT
  {0xC3B1, 0xEE},  // LATIN SMALL LETTER N WITH TILDE vieleicht besser N mit -
  {0xC3B7, 0xFD},  // DIVISION SIGN 
  {0xC69F, 0xF2},  // LATIN CAPITAL LETTER O WITH MIDDLE TILDE  could also be used for e2 88 91 N-ARY SUMMATION 
  {0xc899, 0x8f},  //	?
  {0xc89b, 0x74},  //	?   substitude
  
  {0xCEA3, 0xF6},  // GREEK CAPITAL LETTER SIGMA
  {0xCEB1, 0xE0},  // GREEK SMALL LETTER ALPHA
  {0xCEB2, 0xE2},  // GREEK SMALL LETTER BETA
  {0xCEB5, 0xE3},  // GREEK SMALL LETTER EPSILON
  {0xCEBC, 0xE4},  // GREEK SMALL LETTER MU
  {0xCF80, 0xF7},  // GREEK SMALL LETTER PI
  {0xCF81, 0xE6},  // GREEK SMALL LETTER RHO
  {0xCEA9, 0xF4},  // GREEK CAPITAL LETTER OMEGA
  {0xd080, 0x1C},  // Ѐ CYRILLIC CAPITAL LETTER IE WITH GRAVE
  {0xd081, 0x1F},  // Ё CYRILLIC CAPITAL LETTER IO
  
  // some 3byte characters padded to 2 byte
  {0xb9A3, 0xF8},  // LATIN SMALL LETTER S WITH DOT BELOW 0xE1b9a3
  {0x8690, 0x7F},  // LEFTWARDS ARROW     0xE28690
  {0x8692, 0x7E},  // RIGHTWARDS ARROW    0xE28692
  {0x889A, 0xE8},  // SQUARE ROOT         0xE2889A
  {0x889D, 0xE0},  // PROPORTIONAL TO     0xE2889D
  {0x889E, 0xF3},  // Infiniy             0xE2889E
  {0x82A0, 0x8A},  // NOT EQUAL TO        0xE289A0
  {0x89A4, 0x8F},  // LESS-THAN or EQUAL TO 0xE289A4
  {0x89A5, 0x0A},  // GREATER-THAN OR EQUAL TO   0xE289A5
  {0x89AA, 0x9D},  // MUCH LESS-THAN      0xE289AA
  {0x89AB, 0x9E},  // Much GREATER-THAN   0xE289AB
  {0x9688, 0xFF},  // ¦	e2 96 88	FULL BLOCK
  {0x96b6, 0x12},  // ?	e2 96 b6	BLACK RIGHT-POINTING TRIANGLE
  {0x9780, 0xFE},  // ?	e2 97 80	BLACK LEFT-POINTING TRIANGLE
  {0x9786, 0x11},  // ?	e2 97 86	BLACK DIAMOND
  {0x97C7, 0x10},  //	?	e2 97 87	WHITE DIAMOND  ? da kommt aber eine Art ¦ BROKEN BAR ?
};
//#define startIndexPattern1 0


// the ST7070 ROM character set (page 2)
// greek 0x70 - 0xAF missing in the table
const Mapping ROM_ST7070_P2[] PROGMEM = {
  {0xc482, 0x4c},  //	A  LATIN CAPITAL LETTER A WITH BREVE ?
  {0xc483, 0x5a},  //	a  LATIN SMALL LETTER A WITH BREVE
  {0xc484, 0x34},  //	A  LATIN CAPTIAL LETTER A WITH OGONEK
  {0xc485, 0x3f},  //	a  LATIN SMALL LETTER A WITH OGONEK
  {0xc486, 0x4e},  //	C  LATIN CAPITAL LETTER C WITH ACUTE
  {0xc487, 0x5c},  //	c  LATIN SMALL LETTER C WITH ACUTE
  {0xc48c, 0x4f},  //	C  LATIN CAPITAL LETTER C WITH CARON
  {0xc48d, 0x5d},  //	c  LATIN SMALL LETTER C WITH CARON
  {0xc48e, 0x52},  //	D  LATIN CAPITAL LETTER D WITH CARON
  {0xc48f, 0x61},  // d   substitude
  {0xc491, 0x61},  //	d  LATIN SMALL LETTER D WITH STROKE
  {0xc498, 0x50},  //	E  LATIN CAPITAL LETTER E WITH OGONEK
  {0xc499, 0x5e},  //	e  LATIN SMALL LETTER E WITH OGONEK
  {0xc49a, 0x51},  //	E  LATIN CAPITAL LETTER E WITH CARON
  {0xc49b, 0x5f},  //	e  LATIN SMALL LETTER E WITH CARON
  {0xc49e, 0x6b},  //	G  LATIN CAPITAL LETTER G WITH BREVE
  {0xc49f, 0x6e},  //	g  LATIN SMALL LETTER G WITH BREVE
  {0xc4b0, 0x6c},  //	I  LATIN CAPITAL LETTER I WITH DOT ABOVE
  {0xc4b9, 0x37},  //	L  LATIN CAPITAL LETTER L WITH ACUTE
  {0xc4ba, 0x5b},  //	l  LATIN SMALL LETTER L WITH ACUTE
  {0xc4be, 0x5b},  // l   substitude
  {0xc581, 0x36},  //	L  LATIN CAPITAL LETTER L WITH STROKE
  {0xc582, 0x41},  //	l  LATIN SMALL LETTER L WITH STROKE
  {0xc583, 0x53},  //	N  LATIN CAPITAL LETTER N WITH ACUTE
  {0xc584, 0x62},  //	n  LATIN SMALL LETTER N WITH ACUTE
  {0xc587, 0x54},  //	N  LATIN CAPITAL LETTER N WITH CARON
  {0xc5b8, 0x8f},  // Ÿ
  {0xc588, 0x63},  //	n  LATIN SMALL LETTER N WITH CARON
  {0xc590, 0x09},  // O   Ersatz auf klein
  {0xc594, 0x4b},  //	R  LATIN CAPITAL LETTER R WITH ACUTE
  {0xc595, 0x59},  //	r  LATIN SMALL LETTER R WITH ACUTE
  {0xc598, 0x55},  //	R  LATIN CAPITAL LETTER R WITH CARON
  {0xc599, 0x64},  //	r  LATIN SMALL LETTER R WITH CARON
  {0xc59a, 0x38},  //	S  LATIN CAPITAL LETTER S WITH ACUTE
  {0xc59b, 0x44},  //	s  LATIN SMALL LETTER S WITH ACUTE
  {0xc59e, 0x3a},  //	S  LATIN CAPITAL LETTER S WITH CEDILLA
  {0xc59f, 0x46},  //	s  LATIN SMALL LETTER S WITH CEDILLA
  {0xc5a0, 0x39},  //	Š  LATIN CAPITAL LETTER S WITH CARON
  {0xc5a1, 0x45},  //	š  LATIN SMALL LETTER S WITH CARON
  {0xc5a2, 0x58},  //	T  LATIN CAPITAL LETTER T WITH CEDILLA
  {0xc5a3, 0x67},  //	t  LATIN SMALL LETTER T WITH CEDILLA
  {0xc5a4, 0x3b},  //	T  LATIN CAPITAL LETTER T WITH CARON
  {0xc5a5, 0x47},  //	t  LATIN SMALL LETTER T WITH CARON
  {0xc5ae, 0x56},  //	U  LATIN CAPITAL LETTER U WITH RING ABOVE
  {0xc5af, 0x65},  //	u  LATIN SMALL LETTER U WITH RING ABOVE
  {0xc5b0, 0x57},  //	U  LATIN CAPITAL LETTER U WITH DOUBLE ACUTE
  {0xc5b1, 0x66},  //	u  LATIN SMALL LETTER U WITH DOUBLE ACUTE
  {0xc5b9, 0x3c},  //	Z  LATIN CAPITAL LETTER Z WITH ACUTE
  {0xc5ba, 0x48},  //	z  LATIN SMALL LETTER Z WITH ACUTE
  {0xc5bb, 0x3e},  //	Z  LATIN CAPITAL LETTER Z WITH DOT ABOVE
  {0xc5bc, 0x4a},  //	z  LATIN SMALL LETTER Z WITH DOT ABOVE
  {0xc5bd, 0x3d},  //	Ž  LATIN CAPITAL LETTER Z WITH CARON
  {0xc5be, 0x49},  //	ž  LATIN SMALL LETTER Z WITH CARON
  {0xc68c, 0x61},  //	?  LATIN SMALL LETTER D WITH TOPBAR
  {0xc78d, 0x5C},  //	A  LATIN CAPITAL LETTER A WITH CARON
  {0xd082, 0xB0},  // Ђ  CYRILLIC CAPITAL LETTER DJE
  {0xd083, 0xB1},  // Ѓ  CYRILLIC CAPITAL LETTER GJE
  {0xd084, 0xB2},  // Є  CYRILLIC CAPITAL LETTER UKRAINIAN IE
  {0xd085, 0xB3},  // Ѕ  CYRILLIC CAPITAL LETTER DZE
  {0xd086, 0xB4},  // І  CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
  {0xd087, 0xB5},  // Ї  CYRILLIC CAPITAL LETTER YI
  {0xd088, 0xB6},  // Ј  CYRILLIC CAPITAL LETTER JE        MISSING: tbc visual check necessary - was in the empty area of the char set
  {0xd089, 0xB7},  // Љ  CYRILLIC CAPITAL LETTER LJE
  {0xd08a, 0xB8},  // Њ  CYRILLIC CAPITAL LETTER NJE
  {0xd08b, 0xB9},  // Ћ  CYRILLIC CAPITAL LETTER TSHE
  {0xd08c, 0xBA},  // Ќ  CYRILLIC CAPITAL LETTER KJE
  {0xd08d, 0xC2},  // Ѝ  CYRILLIC CAPITAL LETTER I WITH GRAVE         MISSING: tbd gravei is missing 
  {0xd08e, 0xBB},  // Ў  CYRILLIC CAPITAL LETTER SHORT U
  {0xd08f, 0xBC},  // Џ  CYRILLIC CAPITAL LETTER DZHE
  {0xd090, 'A'},   // А  CYRILLIC CAPITAL LETTER A
  {0xd091, 0xBD},  // Б  CYRILLIC CAPITAL LETTER BE
  {0xd092, 'B'},   // В  CYRILLIC CAPITAL LETTER VE
  {0xd093, 0xBE},  // Г  CYRILLIC CAPITAL LETTER GHE
  {0xd094, 0xBF},  // Д  CYRILLIC CAPITAL LETTER DE
  {0xd095, 'E'},   // Е  CYRILLIC CAPITAL LETTER IE
  {0xd096, 0xC0},  // Ж  CYRILLIC CAPITAL LETTER ZHE
  {0xd097, 0xC1},  // З  CYRILLIC CAPITAL LETTER ZE
  {0xd098, 0xC2},  // И  CYRILLIC CAPITAL LETTER I
  {0xd099, 0xC3},  // Й  CYRILLIC CAPITAL LETTER SHORT I
  {0xd09a, 0xC4},  // К  CYRILLIC CAPITAL LETTER KA
  {0xd09b, 0xC5},  // Л  CYRILLIC CAPITAL LETTER ELj
  {0xd09c, 'M'},   // М  CYRILLIC CAPITAL LETTER EM
  {0xd09d, 'N'},   // Н  CYRILLIC CAPITAL LETTER EN
  {0xd09e, 'O'},   // О  CYRILLIC CAPITAL LETTER O
  {0xd09f, 0xC6},  // П  CYRILLIC CAPITAL LETTER PE        MISSING: tbd this character was just an used one and looked quite similar
  {0xd0a0, 'P'},   // Р  CYRILLIC CAPITAL LETTER ER
  {0xd0a1, 'C'},   // С  CYRILLIC CAPITAL LETTER ES
  {0xd0a2, 'T'},   // Т  CYRILLIC CAPITAL LETTER TE
  {0xd0a3, 0xC7},  // У  CYRILLIC CAPITAL LETTER U
  {0xd0a4, 0xC8},  // Ф  CYRILLIC CAPITAL LETTER EF
  {0xd0a5, 'X'},   // Х  CYRILLIC CAPITAL LETTER HA
  {0xd0a6, 0xC9},  // Ц  CYRILLIC CAPITAL LETTER TSE
  {0xd0a7, 0xCA},  // Ч  CYRILLIC CAPITAL LETTER CHE
  {0xd0a8, 0xCB},  // Ш  CYRILLIC CAPITAL LETTER SHA
  {0xd0a9, 0xCC},  // Щ  CYRILLIC CAPITAL LETTER SHCHA
  {0xd0aa, 0xCD},  // Ъ  CYRILLIC CAPITAL LETTER HARD SIGN
  {0xd0ab, 0xCE},  // Ы  CYRILLIC CAPITAL LETTER YERU
  {0xd0ac, 0xCF},  // Ь  CYRILLIC CAPITAL LETTER SOFT SIGN
  {0xd0ad, 0xD0},  // Э  CYRILLIC CAPITAL LETTER E
  {0xd0ae, 0xD1},  // Ю  CYRILLIC CAPITAL LETTER YU - JU
  {0xd0af, 0xD2},  // Я  CYRILLIC CAPITAL LETTER YA  - JA
  {0xd0b0, 'a'},   // а  CYRILLIC SMALL LETTER A
  {0xd0b1, 0xD3},  // б  CYRILLIC SMALL LETTER BE
  {0xd0b2, 0xD4},  // в  CYRILLIC SMALL LETTER VE
  {0xd0b3, 0xD5},  // г  CYRILLIC SMALL LETTER GHE
  {0xd0b4, 0xD6},  // д  CYRILLIC SMALL LETTER DE
  {0xd0b5, 'e'},   // е  CYRILLIC SMALL LETTER IE
  {0xd0b6, 0xD7},  // ж  CYRILLIC SMALL LETTER ZHE
  {0xd0b7, 0xD8},  // з  CYRILLIC SMALL LETTER ZE
  {0xd0b8, 0xD9},  // и  CYRILLIC SMALL LETTER I
  {0xd0b9, 0xDA},  // й  CYRILLIC SMALL LETTER SHORT I
  {0xd0ba, 0xDB},  // к  CYRILLIC SMALL LETTER KA
  {0xd0bb, 0xDC},  // л  CYRILLIC SMALL LETTER EL
  {0xd0bc, 0xDD},  // м  CYRILLIC SMALL LETTER EM
  {0xd0bd, 0xDE},  // н  CYRILLIC SMALL LETTER EN
  {0xd0be, 'o'},   // о  CYRILLIC SMALL LETTER O
  {0xd0bf, 0xDF},  // п  CYRILLIC SMALL LETTER PE
  {0xd180, 'p'},   // р  CYRILLIC SMALL LETTER ER
  {0xd181, 'c'},   // с  CYRILLIC SMALL LETTER ES
  {0xd182, 0xE0},  // т  CYRILLIC SMALL LETTER TE
  {0xd183, 'y'},   // у  CYRILLIC SMALL LETTER U
  {0xd184, 0xE1},  // ф  CYRILLIC SMALL LETTER EF
  {0xd185, 'x'},   // х  CYRILLIC SMALL LETTER HA
  {0xd186, 0xE2},  // ц  CYRILLIC SMALL LETTER TSE
  {0xd187, 0xE3},  // ч  CYRILLIC SMALL LETTER CHE
  {0xd188, 0xE4},  // ш  CYRILLIC SMALL LETTER SHA
  {0xd189, 0xE5},  // щ  CYRILLIC SMALL LETTER SHCHA
  {0xd18a, 0xE6},  // ъ  CYRILLIC SMALL LETTER HARD SIGN
  {0xd18b, 0xE7},  // ы  CYRILLIC SMALL LETTER YERU
  {0xd18c, 0xE8},  // ь  CYRILLIC SMALL LETTER SOFT SIGN
  {0xd18d, 0xE9},  // э  CYRILLIC SMALL LETTER E
  {0xd18e, 0xEA},  // ю  CYRILLIC SMALL LETTER YU
  {0xd18f, 0xEB},  // я  CYRILLIC SMALL LETTER YA
  // Pre 1918 letters
  {0xd190, 0x1C},  // ѐ  CYRILLIC SMALL LETTER IE WITH GRAVE         MISSING: tbc visual check necessary - Latin?
  {0xd191, 0x1F},  // ё  CYRILLIC SMALL LETTER IO                    MISSING: tbc visual check necessary - Latin?
  {0xd192, 0xED},  // ђ  CYRILLIC SMALL LETTER DJE                   MISSING: tbc visual check necessary 
  {0xd193, 0xD5},  // ѓ  CYRILLIC SMALL LETTER GJE                   MISSING: tbc visual check necessary
  {0xd194, 0xEE},  // є  CYRILLIC SMALL LETTER UKRAINIAN IE
  {0xd195, 0xEF},  // ѕ  CYRILLIC SMALL LETTER DZE
  {0xd196, 0xF0},  // і  CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
  {0xd197, 0xF1},  // ї  CYRILLIC SMALL LETTER YI
  {0xd198, 'j'},   // ј  CYRILLIC SMALL LETTER JE                    MISSING: tbc visual check necessary - 0xF2 could be an alternative
  {0xd199, 0xF3},  // љ  CYRILLIC SMALL LETTER LJE                   MISSING: tbc visual check necessary - similarity?
  {0xd19a, 0xF3},  // њ  CYRILLIC SMALL LETTER NJE
  {0xd19b, 0xF4},  // ћ  CYRILLIC SMALL LETTER TSHE
  {0xd1a2, 'E'},   // Ѣ  CYRILLIC CAPITAL LETTER YAT
  {0xd1a3, 'e'},   // ѣ  CYRILLIC SMALL LETTER YAT
  {0xd1b2, 'O'},   // Ѳ  CYRILLIC CAPITAL LETTER FITA                MISSING: tbc visual check necessary - similarity?
  {0xd1b3, 'o'},   // ѳ  CYRILLIC SMALL LETTER FITA                  MISSING: tbc visual check necessary - similarity? 
  {0xd1b4, 'Y'},   // Ѵ  CYRILLIC CAPITAL LETTER IZHITSA
  {0xd1b5, 'y'}    // ѵ  CYRILLIC SMALL LETTER IZHITSA 
};

// character mapping for the SPLC780D1 002A ROM (Cyrillic characters)
// sorted by UTF8 - 2 byte characters only
// this table also contains some normalization of characters if not supported by the ROM
// not complete, missing end of datasheet rows 7, D, E, complete row F, 
const Mapping ROM_SPLC780D1_002A[] PROGMEM = {
  {0xd081, 0xA2}, // Ё  CYRILLIC CAPITAL LETTER IO
  {0xd086, 'I'},  // ?  CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
  {0xd090, 'A'},  // А  CYRILLIC CAPITAL LETTER A
  {0xd091, 0xA0}, // Б  CYRILLIC CAPITAL LETTER BE
  {0xd092, 'B'},  // В  CYRILLIC CAPITAL LETTER VE
  {0xd093, 0xA1}, // Г  CYRILLIC CAPITAL LETTER GHE
  {0xd094, 0xE0}, // Д  CYRILLIC CAPITAL LETTER DE
  {0xd095, 'E'},  // Е  CYRILLIC CAPITAL LETTER IE
  {0xd096, 0xA3}, // Ж  CYRILLIC CAPITAL LETTER ZHE
  {0xd097, 0xA4}, // З  CYRILLIC CAPITAL LETTER ZE
  {0xd098, 0xA5}, // И  CYRILLIC CAPITAL LETTER I
  {0xd099, 0xA6}, // Й  CYRILLIC CAPITAL LETTER SHORT I
  {0xd09a, 'K'},  // К  CYRILLIC CAPITAL LETTER KA
  {0xd09b, 0xA7}, // Л  CYRILLIC CAPITAL LETTER EL
  {0xd09c, 'M'},  // М  CYRILLIC CAPITAL LETTER EM
  {0xd09d, 'H'},  // Н  CYRILLIC CAPITAL LETTER EN
  {0xd09e, 'O'},  // О  CYRILLIC CAPITAL LETTER O
  {0xd09f, 0xA8}, // П  CYRILLIC CAPITAL LETTER PE
  {0xd0a0, 'P'},  // Р  CYRILLIC CAPITAL LETTER ER
  {0xd0a1, 'C'},  // С  CYRILLIC CAPITAL LETTER ES
  {0xd0a2, 'T'},  // Т  CYRILLIC CAPITAL LETTER TE
  {0xd0a3, 0xA9}, // У  CYRILLIC CAPITAL LETTER U
  {0xd0a4, 0xAA}, // Ф  CYRILLIC CAPITAL LETTER EF
  {0xd0a5, 'X'},  // Х  CYRILLIC CAPITAL LETTER HA
  {0xd0a6, 0xE1}, // Ц  CYRILLIC CAPITAL LETTER TSE        ??? visuell checken ob ok
  {0xd0a7, 0xAB}, // Ч  CYRILLIC CAPITAL LETTER CHE
  {0xd0a8, 0xAC}, // Ш  CYRILLIC CAPITAL LETTER SHA
  {0xd0a9, 0xE2}, // Щ  CYRILLIC CAPITAL LETTER SHCHA
  {0xd0aa, 0xAD}, // Ъ  CYRILLIC CAPITAL LETTER HARD SIGN
  {0xd0ab, 0xAE}, // Ы  CYRILLIC CAPITAL LETTER YERU
  {0xd0ac, 'b'},  // Ь  CYRILLIC CAPITAL LETTER SOFT SIGN
  {0xd0ad, 0xAF}, // Э  CYRILLIC CAPITAL LETTER E
  {0xd0ae, 0xB0}, // Ю  CYRILLIC CAPITAL LETTER YU - JU
  {0xd0af, 0xB1}, // Я  CYRILLIC CAPITAL LETTER YA  - JA
  {0xd0b0, 'a'},  // а  CYRILLIC SMALL LETTER A
  {0xd0b1, 0xB2}, // б  CYRILLIC SMALL LETTER BE
  {0xd0b2, 0xB3}, // в  CYRILLIC SMALL LETTER VE
  {0xd0b3, 0xB4}, // г  CYRILLIC SMALL LETTER GHE
  {0xd0b4, 0xE4}, // д  CYRILLIC SMALL LETTER DE           ??? visuell checken ob OK
  {0xd0b5, 'e'},  // е  CYRILLIC SMALL LETTER IE           war 0xb5, e ist aber besser
  {0xd0b6, 0xB6}, // ж  CYRILLIC SMALL LETTER ZHE
  {0xd0b7, 0xB7}, // з  CYRILLIC SMALL LETTER ZE
  {0xd0b8, 0xB8}, // и  CYRILLIC SMALL LETTER I
  {0xd0b9, 0xB9}, // й  CYRILLIC SMALL LETTER SHORT I
  {0xd0ba, 0xBA}, // к  CYRILLIC SMALL LETTER KA
  {0xd0bb, 0xBB}, // л  CYRILLIC SMALL LETTER EL
  {0xd0bc, 0xBC}, // м  CYRILLIC SMALL LETTER EM
  {0xd0bd, 0xBD}, // н  CYRILLIC SMALL LETTER EN
  {0xd0be, 'o'},  // о  CYRILLIC SMALL LETTER O
  {0xd0bf, 0xBE}, // п  CYRILLIC SMALL LETTER PE
  {0xd180, 'p'},  // р  CYRILLIC SMALL LETTER ER
  {0xd181, 'c'},  // с  CYRILLIC SMALL LETTER ES
  {0xd182, 0xBF}, // т  CYRILLIC SMALL LETTER TE
  {0xd183, 0xC0}, // у  CYRILLIC SMALL LETTER U
  {0xd184, 0xE4}, // ф  CYRILLIC SMALL LETTER EF         
  {0xd185, 'x'},  // х  CYRILLIC SMALL LETTER HA
  {0xd186, 0xE5}, // ц  CYRILLIC SMALL LETTER TSE
  {0xd187, 0xC0}, // ч  CYRILLIC SMALL LETTER CHE        
  {0xd188, 0xC1}, // ш  CYRILLIC SMALL LETTER SHA
  {0xd189, 0xE6}, // щ  CYRILLIC SMALL LETTER SHCHA
  {0xd18a, 0xC2}, // ъ  CYRILLIC SMALL LETTER HARD SIGN
  {0xd18b, 0xC3}, // ы  CYRILLIC SMALL LETTER YERU
  {0xd18c, 0xC4}, // ь  CYRILLIC SMALL LETTER SOFT SIGN
  {0xd18d, 0xC5}, // э  CYRILLIC SMALL LETTER E
  {0xd18e, 0xC6}, // ю  CYRILLIC SMALL LETTER YU
  {0xd18f, 0xC7}, // я  CYRILLIC SMALL LETTER YA
  // Pre 1918 letters
  {0xd190, 'e'},  // ѐ  CYRILLIC SMALL LETTER IE WITH GRAVE
  {0xd191, 0xB5}, // ё  CYRILLIC SMALL LETTER IO
  {0xd192, 'h'},  // ђ  CYRILLIC SMALL LETTER DJE          - substitute ASCII
  {0xd193, 'r'},  // ѓ  CYRILLIC SMALL LETTER GJE          - substitute ASCII
  {0xd196, 'i'},  // і  CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
  {0xd197, 'i'},  // ї  CYRILLIC SMALL LETTER YI           - substitute ASCII
  {0xd1a2, 'E'},  // Ѣ  CYRILLIC CAPITAL LETTER YAT        - substitute ASCII 
  {0xd1a3, 'e'},  // ѣ  CYRILLIC SMALL LETTER YAT          - substitute ASCII
  {0xd1b2, 'F'},  // Ѳ  CYRILLIC CAPITAL LETTER FITA       - substitute ASCII according GOST
  {0xd1b3, 'f'},  // ѳ  CYRILLIC SMALL LETTER FITA         - substitute ASCII according GOST
  {0xd1b4, 'Y'},  // Ѵ  CYRILLIC CAPITAL LETTER IZHITSA    - substitute ASCII according GOST
  {0xd1b5, 'y'},  // ѵ  CYRILLIC SMALL LETTER IZHITSA      - substitute ASCII according GOST
};


// character mapping for the SPLC780D1 003A ROM (European Languages)
// plus supplements for European Languages
// splitted in 2 byte and 3 byte, then sorted by ROM address
// this table also contains some normalization of characters if not supported by the ROM
const Mapping ROM_SPLC780D1_003A[] PROGMEM = {
  // German Umlauts sorted to top, assumption: ä ö ü are used more often
  {0xC3A4, 0x84},    // ä   
  {0xC3B6, 0x94},    // ö
  {0xC3BC, 0x81},    // ü
  {0xC384, 0x8E},    // Ä  
  {0xC396, 0x99},    // Ö
  {0xC39C, 0x9A},    // Ü   
  {0xC39F, 0xE0},    // ß  to GREEK SMALL LETTER BETA
                     
  {0xc2b1, 0x11},    // ±  PLUS-MINUS SIGN
  // lot of characters in 3 byte section
  {0xC2B2, 0x1E},    // ²  Superscript Two 
  {0xC2B3, 0x1F},    // ³  Superscript Three
  // Latin-1 supplement
  {0xc387, 0x80},    // Ç  LATIN CAPITAL LETTER C WITH CEDILLA
                     // ü
  {0xc3a9, 0x82},    // é  LATIN SMALL LETTER E WITH ACUTE
  {0xc3a5, 0x83},    // å  LATIN SMALL LETTER A WITH RING ABOVE
                     // ä
  {0xc3a0, 0x85},    // à  LATIN SMALL LETTER A WITH GRAVE
                     // ä  at top
  {0xc3a5, 0x86},    // å  LATIN SMALL LETTER A WITH RING ABOVE
  {0xc3a7, 0x87},    // ç  LATIN SMALL LETTER C WITH CEDILLA
  {0xc3aa, 0x88},    // ê  LATIN SMALL LETTER E WITH CIRCUMFLEX
  {0xc3ab, 0x89},    // ë  LATIN SMALL LETTER E WITH DIAERESIS
  {0xc3a8, 0x8A},    // è  LATIN SMALL LETTER E WITH GRAVE
  {0xc3af, 0x8B},    // ï  LATIN SMALL LETTER I WITH DIAERESIS
  {0xC3ae, 0x8C},    // î  LATIN SMALL LETTER I WITH CIRCUMFLEX
  {0xc3ac, 0x8D},    // ì  LATIN SMALL LETTER I WITH GRAVE
  {0xc380, 'A'},     // À	 LATIN CAPITAL LETTER A WITH GRAVE
  {0xc381, 'A'},     // Á	 LATIN CAPITAL LETTER A WITH ACUTE
                     // Ä  at top
  {0xc385, 0x8F},    // Å  LATIN CAPITAL LETTER A WITH RING ABOVE  
  
  {0xc389, 0x90},    // É  LATIN CAPITAL LETTER E WITH ACUTE
  {0xc392, 'O'},     // Ò	 LATIN CAPITAL LETTER O WITH GRAVE
  {0xc393, 'O'},     // Ó  LATIN CAPITAL LETTER O WITH ACUTE
  {0xc394, 'O'},     // Ô  LATIN CAPITAL LETTER O WITH CIRCUMFLEX
  {0xc399, 'U'},     // Ù	 LATIN CAPITAL LETTER U WITH GRAVE
  {0xc39a, 'U'},     // Ú	 LATIN CAPITAL LETTER U WITH ACUTE
  {0xc3a6, 0x91},    // æ  LATIN SMALL LETTER AE
  {0xc386, 0x92},    // Æ  LATIN CAPITAL LETTER AE
  {0xc38a, 'E'},     // Ê  LATIN CAPITAL LETTER E WITH CIRCUMFLEX
  {0xc38b, 'E'},     // Ë  LATIN CAPITAL LETTER E WITH DIAERESIS
  {0xc38c, 'I'},     // Í  LATIN CAPITAL LETTER I WITH ACUTE
  {0xc38d, 'I'},
  {0xc38e, 'I'},
  {0xc38f, 'I'},     // Ï	 LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc39b, 'U'},     // Û	 LATIN CAPITAL LETTER U WITH CIRCUMFLEX
  {0xc3b4, 0x93},    // ô  LATIN SMALL LETTER O WITH CIRCUMFLEX
                     // ö  at top
  {0xc3b2, 0x95},    // ò  LATIN SMALL LETTER O WITH GRAVE
  {0xc3bd, 'y'},     // ý	 LATIN SMALL LETTER Y WITH ACUTE
  {0xc3bb, 0x96},    // û  LATIN SMALL LETTER U WITH CIRCUMFLEX
  {0xc3b9, 0x97},    // ù  LATIN SMALL LETTER U WITH GRAVE
  {0xc3bf, 0x98},    // ÿ  LATIN SMALL LETTER Y WITH DIAERESIS
                     // Ö  at top
                     // Ü  at top
  {0xc3b1, 0x9B},    // ñ  LATIN SMALL LETTER N WITH TILDE
  {0xc391, 0x9C},    // Ñ  LATIN CAPITAL LETTER N WITH TILDE
                     //    latin small letter a with macron below - not available https://en.wikipedia.org/wiki/Macron_below
                     //    latin small letter o with macron below - not available https://en.wikipedia.org/wiki/Macron_below
  {0xc2bf, 0x9F},    // ¿  INVERTED QUESTION MARK
                     
  {0xc3a1, 0xA0},    // á  LATIN SMALL LETTER A WITH ACUTE
  {0xc3ad, 0xA1},    // í  LATIN SMALL LETTER I WITH ACUTE
  {0xc3b3, 0xA2},    // ó  LATIN SMALL LETTER O WITH ACUTE
  {0xc3ba, 0xA3},    // ú  LATIN SMALL LETTER U WITH ACUTE
  {0xc8bb, 0xA4},    // ?  LATIN CAPITAL LETTER C WITH STROKE
  {0xC2A2, 0xA4},    // ¢  CENT SIGN (alternative)
  {0xC2A3, 0xA5},    // £  Pound SIGN
  {0xC2A5, 0xA6},    // ¥  YEN SIGN
                     // P  PESETA SIGN e2 82 a7	- available but not implemented
                     // f
                     // i
  {0xc383, 0xAA},    // Ã  LATIN CAPITAL LETTER A WITH TILDE
  {0xc38b, 'E'},     // Ë  LATIN CAPITAL LETTER E WITH DIAERESIS
  
  {0xc482, 'A'},     // A  LATIN CAPITAL LETTER A WITH BREVE
  {0xc483, 'a'},     // a	 LATIN SMALL LETTER A WITH BREVE
  {0xc484, 'A'},     //	A	 LATIN CAPTIAL LETTER A WITH OGONEK
  {0xc485, 'a'},     //	a  LATIN SMALL LETTER A WITH OGONEK
  {0xc486, 'C'},     //	C	 LATIN CAPITAL LETTER C WITH ACUTE
  {0xc487, 'c'},     //	c  LATIN SMALL LETTER C WITH ACUTE  
  
  {0xc491, 0xAB},    // d  LATIN SMALL LETTER D WITH STROKE - fraglich
  {0xc498, 'E'},     // E  LATIN CAPITAL LETTER E WITH OGONEK
  {0xc499, 'e'},     // e  LATIN SMALL LETTER E WITH OGONEK
  {0xc4ba, 'l'},     // l	 LATIN SMALL LETTER L WITH ACUTE
  {0xc4b9, 'L'},     // L  LATIN CAPITAL LETTER L WITH ACUTE
  {0xc48c, 'C'},     // C  LATIN CAPITAL LETTER C WITH CARON
  {0xc48d, 'c'},     // c  LATIN SMALL LETTER C WITH CARON
  {0xc48f, 'd'},     // d  substitude
  {0xc4be, 'l'},     // l	 LATIN SMALL LETTER L WITH CARON
 
  {0xc581, 'L'},     //	L	 LATIN CAPITAL LETTER L WITH STROKE
  {0xc582, 'l'},     //	l	 LATIN SMALL LETTER L WITH STROKE
  {0xc583, 'N'},     //	N  LATIN CAPITAL LETTER N WITH ACUTE
  {0xc584, 'n'},     //	n  LATIN SMALL LETTER N WITH ACUTE  
  {0xc587, 'N'},     //	N  LATIN CAPITAL LETTER N WITH CARON
  {0xc588, 'n'},     //	n  LATIN SMALL LETTER N WITH CARON
  {0xc590, 0x99},    // O  Ersatz auf Ö
  {0xc592, 0x99},    // Œ	 LATIN CAPITAL LIGATURE OE replaced by Ö
  {0xc593, 0x94},    // œ  LATIN SMALL LIGATURE OE  - replaced by ö
  {0xc595, 'r'},     // r	 LATIN SMALL LETTER R WITH ACUTE
  {0xc59a, 'S'},	   // S	 LATIN CAPITAL LETTER S WITH ACUTE
  {0xc59b, 's'},     // s  LATIN SMALL LETTER S WITH ACUTE
  {0xc5a0, 'S'},     // Š  LATIN CAPITAL LETTER S WITH CARON
  {0xc5a1, 's'},     // š	 LATIN SMALL LETTER S WITH CARON
  {0xc5b0, 0x9A},    // U  LATIN CAPITAL LETTER U WITH DOUBLE ACUTE - replaced Ü
  {0xc5b1, 0x81},    // u  LATIN SMALL LETTER U WITH DOUBLE ACUTE - replaced ü
  
  {0xc5bb, 'Z'},     //	Z  LATIN CAPITAL LETTER Z WITH DOT ABOVE
  {0xc5bc, 'z'},     //	z  LATIN SMALL LETTER Z WITH DOT ABOVE
  {0xc5bd, 'Z'},     // Ž  LATIN CAPITAL LETTER Z WITH CARON
  {0xc5be, 'z'},     // ž  LATIN SMALL LETTER Z WITH CARON 
  
  {0xc8ac, 0xAC},    // ?  LATIN CAPITAL LETTER O WITH TILDE AND MACRON   - fraglich
  {0xc3b5, 0xAD},    // õ  LATIN SMALL LETTER O WITH TILDE
  {0xc382, 'A'},     // Â  LATIN CAPITAL LETTER A WITH CIRCUMFLEX
  {0xc398, 0xAE},    // Ø  LATIN CAPITAL LETTER O WITH STROKE
  {0xc395, 'O'},     // Õ  LATIN CAPITAL LETTER O WITH TILDE
  {0xc3a2, 0x83},    // â  LATIN SMALL LETTER A WITH CIRCUMFLEX
  {0xc3b8, 0xAF},    // ø  LATIN SMALL LETTER O WITH STROKE - fraglich
  {0xc3bf, 'y'},     // ÿ	 LATIN SMALL LETTER Y WITH DIAERESIS
  {0xc5a1, 's'},     
  {0xc5a5, 't'},
  {0xc5af, 'u'},     // u	 LATIN SMALL LETTER U WITH RING ABOVE
  {0xc5b0, 0x9A},    // U  LATIN CAPITAL LETTER U WITH DOUBLE ACUTE --> Ersatz auf Ü
  {0xc5be, 'z'},
  {0xc5b8, 'Y'},     // Ÿ	 LATIN CAPITAL LETTER Y WITH DIAERESIS
  {0xc5b9, 'Z'},     // Z  LATIN CAPITAL LETTER Z WITH ACUTE
  {0xc5ba, 'z'},     //	z  LATIN SMALL LETTER Z WITH ACUT
  {0xc898, 'S'},     // ?  LATIN CAPITAL LETTER S WITH COMMA BELOW
  {0xc899, 's'},     // ?  LATIN SMALL LETTER S WITH COMMA BELOW
  
  {0xCB99, 0xB0},    // ?  DOT ABOVE
  {0xc2a8, 0xB1},    // ¨  DIAERESIS
  {0xC2B0, 0xB2},    //    DEGREE SIGN
  {0xcc80, 0xB3},    // `    COMBINING GRAVE ACCENT 
  {0xcc81, 0xB4},    // ´   COMBINING ACUTE ACCENT
  {0xc2bd, 0xB5},    // ½   VULGAR FRACTION ONE HALF
  {0xc2bc, 0xB6},    // ¼  VULGAR FRACTION ONE QUARTER
  
  {0xc397, 0xB7},    // ×  MULTIPLICATION SIGN
  {0xc3b7, 0xB8},    // ÷  DIVISION SIGN
                     //     small equal     --> 3byte
                     //     larger equal    --> 3byte
  {0xc2ab, 0xBB},    // «   LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
  {0xc2bb, 0xBC},    // »   RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
                     //     not equal       --> 3Byte
                     //     square root 3byte 

  {0xcc85, 0xBF},    // ¯  COMBINING OVERLINE
  {0xc89b, 't'},     // ?  substitude
  
  
  // most of the row 0xC is 3 byte
  {0xc2ae, 0xCE},    // ®  REGISTERED SIGN
  {0xc2a9, 0xCF},    // ©  COPYRIGHT SIGN
  
                     //    trade mark  --> 3byte
                     //    Latin Cross --> 3byte
  {0xC2A7, 0xD2},    // §  Section Sign
  {0xc2b5, 0xEA},    // µ  GREEK SMALL LETTER MU
  {0xC2b6, 0xD3},    // ¶  Pilcrow Sign
  {0xcd93, 0xd4},    // G  GREEK CAPITAL LETTER GAMMA
  {0xce94, 0xd5},    // ?  ce 94  GREEK CAPITAL LETTER DELTA ? only because it could fit in this order
  {0x8abf, 0xd5},    // ?  RIGHT TRIANGLE e2 8a bf 
  {0xce98, 0xd6},    // T  GREEK CAPITAL LETTER THETA
  {0xce9b, 0xD7},    // ?  GREEK CAPITAL LETTER LAMDA
  {0xce94, 0xD8},    // ?  GREEK CAPITAL LETTER XI
  {0xdea0, 0xD9},    // ?  GREEK CAPITAL LETTER PI
  {0xcea3, 0xDA},    // S  GREEK CAPITAL LETTER SIGMA
  {0xcea5, 0xDB},    // ?  GREEK CAPITAL LETTER UPSILON
  {0xcea6, 0xDC},    // F  GREEK CAPITAL LETTER PHI
  {0xcea8, 0xDD},    // ?  GREEK CAPITAL LETTER PSI
  {0xcea9, 0xDE},    // O  GREEK CAPITAL LETTER OMEGA
  {0xceb1, 0xDf},    // a  GREEK SMALL LETTER ALPHA
  
  {0xceb2, 0xe0},    // ß  GREEK SMALL LETTER BETA
  {0xceb3, 0xE1},    // ?  GREEK SMALL LETTER GAMMA
  {0xceb4, 0xE2},    // d  GREEK SMALL LETTER DELTA  
  {0xceb5, 0xE3},    // e  GREEK SMALL LETTER EPSILON
  {0xceb6, 0xE4},    // ?  GREEK SMALL LETTER ZETA
  {0xceb7, 0xE5},    // ?  GREEK SMALL LETTER ETA
  {0xceb8, 0xE6},    // ?  GREEK SMALL LETTER THETA 
  {0xceb9, 0xE7},    // ?  GREEK SMALL LETTER IOTA
  {0xceba, 0xE8},    // ?  GREEK SMALL LETTER KAPPA
  {0xcebb, 0xE9},    // ?  GREEK SMALL LETTER LAMDA
  {0xcebc, 0xEA},    // µ  GREEK SMALL LETTER MU --> also used for micro
  
  {0xcebd, 0xEB},    // ?  GREEK SMALL LETTER NU
  {0xcebe, 0xEC},    // ?  GREEK SMALL LETTER XI
  {0xcf80, 0xED},    // p  GREEK SMALL LETTER PI
  {0xcf81, 0xEE},    // ?  GREEK SMALL LETTER RHO
  
  {0xcf84, 0xF0},    // t  GREEK SMALL LETTER TAU
  {0xcf85, 0xF1},    // ?  GREEK SMALL LETTER UPSILON
  {0xcf87, 0xF2},    // ?  GREEK SMALL LETTER CHI
  {0xcf88, 0xF3},    // ?  GREEK SMALL LETTER PSI
  {0xcf89, 0xF4},    // ?  GREEK SMALL LETTER OMEGA
                     //    pfeile --> 3byte
                     //    white rectangle -->
                     //    -
                     //    service mark -> 3yte 
                     //    SOUND RECORDING COPYRIGHT  -> 3byte
  
  // here starts the 3byte section
  // but I store only the lower 2 bytes of the character 
  // E2 3byte
  {0x8893, 0x10},    // ±  MINUS-OR-PLUS SIGN      0xe28893
  {0x89a1, 0x11},    // =  IDENTICAL TO            0xe289a1
  {0x8eb2, 0x12},    // ?  SUMMATION TOP           0xe28eb2  
  {0x8eb3, 0x13},    // ?  SUMMATION BOTTOM        0xe28eb3    
  {0x8ea7, 0x14},    //    LEFT CURLY BRACKET UPPER HOOK   
  {0x8e8e, 0x15},    //    LEFT CURLY BRACKET LOWER HOOK
  {0x8eab, 0x16},    //    RIGHT CURLY BRACKET UPPER HOOK   
  {0x8ead, 0x17},    //    RIGHT CURLY BRACKET LOWER HOOK
  {0x8eb0, 0x18},    // ?  UPPER LEFT OR LOWER RIGHT CURLY BRACKET SECTION  0xe28eb0  
  {0x8eb1, 0x19},    // ?  UPPER RIGHT OR LOWER LEFT CURLY BRACKET SECTION  0xe28eb1  
  {0x8988, 0x1a},    // ˜  ALMOST EQUAL TO       0xe28988  
  {0x88ab, 0x1b},    // ?  INTEGRAL              0xe288ab  
  {0x88bc, 0x1d},    // ~  TILDE OPERATOR        0xe288bc    
  
  {0x889A, 0xBE},    //    SQUARE ROOT           0xE2889A
  {0x89A0, 0xBD},    // ?  NOT EQUAL             0xE289A0
  {0x89a4, 0xB9},    // =                        0xE289A4
  {0x89A5, 0xB9},    // =                        0xE289A5

  {0x8ca0, 0xC0},    // (  TOP HALF INTEGRAL     0xe28ca0  
  {0x8ca1, 0xC1},    // )  BOTTOM HALF INTEGRAL  0xe28ca1    
  {0x889E, 0xc2},    //    Infiniy               0xE2889E  
  {0x8abe, 0xc3},    // ?  Right Angle with Arc  0xE28ABE
  {0x86B2, 0xC4},    // ?  Downwards Arrow with Tip Leftwards 0xE2 0x86 0xB2
  {0x8691, 0xC5},    // ?  Upwards Arrow         0xE286x91
  {0x8693, 0xC6},    // ?  Downwards Arrow       0xE286x93
  {0x8692, 0xC7},    // ?  Rightwards Arrow      0xE286x92
  {0x8690, 0xC8},    // ?  Leftwards Arrow       0xE286x90
  {0x8c88, 0xb9},    // ?  LEFT CEILING          0xe28cx88  
  {0x8c89, 0xba},    // ?  RIGHT CEILING         0xe28cx89
  {0x8c8a, 0xbb},    // ?  LEFT FLOOR            0xe28cx8a  
  {0x8c8b, 0xbc},    // ?  RIGHT FLOOR           0xe28cx8b  
  {0x8b85, 0xbd},    // · DOT OPERATOR           0xe28bx85  
  
  {0x84A2, 0xD0},    // ™  Trade Mark Sign       0xE284A2
  {0x9C9D, 0xD1},    //?  Latin Cross            0xE29C9D
  {0x8b81, 0xD7},    // ?   N-ARY LOGICAL OR     0xe2b81  
  {0x888f, 0xD9},    // ?  N-ARY PRODUCT         0xe288f  
  {0x8891, 0xDA},    // ?   N-ARY SUMMATION      0xe2891  
  {0x84A6, 0xDE},    // ?  Ohm Sign              0xE284A6  (also the Omega)
  {0x889c, 0xDF},    // ? PROPORTIONAL TO        0xe2889d (it's more a Greek small letter alpha)
  
  {0x86A4, 0xF9},    // ? Leftwards Arrow from Bar         0xE286A4
  {0x86a6, 0xFB},    // ?  Rightwards Arrow from Bar       0xE286A6
  {0x96AF, 0xFC},    // ?  White Vertical Rectangle        0xE296AF
  {0x84a0, 0xFE},    // ?  SERVICE MARK                    0xe284a0
  {0x8497, 0xFF},    // ?  SOUND RECORDING COPYRIGHT       0xe28497
};


// character mapping for the LEC1621_01 ROM (Western European - I)
// plus supplements for European Languages
// splitted in 2 byte and 3 byte, then sorted by ROM address - currently unsorted
// this table also contains some normalization of German Umlauts if not supported by the ROM
// the mapping is not complete
const Mapping ROM_LEC1621_01[] PROGMEM = {
  // German Umlauts sorted to top, assumption: ä ö ü are used more often
                      
  // Latin-1 supplement
  {0xC2A5, 0x5C},    // ¥  YEN SIGN
  {0xc39b, 0x80},    // Û  LATIN CAPITAL LETTER U WITH CIRCUMFLEX
  {0xc399, 0x81},    // Ù	 LATIN CAPITAL LETTER U WITH GRAVE
  {0xc39a, 0x82},    // Ú	 LATIN CAPITAL LETTER U WITH ACUTE
  {0xC39C, 0x83},    // Ü 
  {0xc3bb, 0x84},    // û  LATIN SMALL LETTER U WITH CIRCUMFLEX
  {0xc3b9, 0x85},    // ù  LATIN SMALL LETTER U WITH GRAVE
  {0xc3ba, 0x86},    // ú  LATIN SMALL LETTER U WITH ACUTE  
  {0xC3BC, 'u'},     // ü  
  {0xc394, 0x87},    // Ô  LATIN CAPITAL LETTER O WITH CIRCUMFLEX  
  {0xc392, 0x88},    // Ò  LATIN CAPITAL LETTER O WITH GRAVE
  {0xc393, 0x89},    // Ó  LATIN CAPITAL LETTER O WITH ACUTE
  {0xC396, 'O'},     // Ö  
  // O with stroke below ?
  {0xc3b4, 0x8B},    // ô  LATIN SMALL LETTER O WITH CIRCUMFLEX
  {0xc3b2, 0x8C},    // ò  LATIN SMALL LETTER O WITH GRAVE
  {0xc3b3, 0x8D},    // ó  LATIN SMALL LETTER O WITH ACUTE   
  {0xC3B6, 0x8E},    // ö
  {0xc2bf, 0x8F},    // ¿  INVERTED QUESTION MARK
  {0xc38a, 0x90},    // Ê  LATIN CAPITAL LETTER E WITH CIRCUMFLEX    
  {0xc388, 0x91},    // È  LATIN CAPITAL LETTER E WITH GRAVE
  {0xc389, 0x92},    // É  LATIN CAPITAL LETTER E WITH ACUTE
  {0xc38b, 0x93},    // Ë  LATIN CAPITAL LETTER E WITH DIAERESIS
  {0xc3aa, 0x94},    // ê  LATIN SMALL LETTER E WITH CIRCUMFLEX
  {0xc3a8, 0x95},    // è  LATIN SMALL LETTER E WITH GRAVE
  {0xc3a9, 0x96},    // é  LATIN SMALL LETTER E WITH ACUTE
  {0xc3ab, 0x97},    // ë  LATIN SMALL LETTER E WITH DIAERESIS  
  // A with dot above ?
  {0xC384, 0x99},    // Ä 
  // a with dot above ? 
  {0xc3a5, 0x9A},    // å  LATIN SMALL LETTER A WITH RING ABOVE   -  naja eigentlich a mit Punkt
  {0xc3a2, 0x9B},    // â  LATIN SMALL LETTER A WITH CIRCUMFLEX  
  {0xc3a0, 0x9C},    // à  LATIN SMALL LETTER A WITH GRAVE
  {0xc3a1, 0x9D},    // á  LATIN SMALL LETTER A WITH ACUTE 
  {0xC3A4, 0x9E},    // ä   
  // A with stroke below ?
  // a with stroke below ?
  {0xC3ae, 0xA1},    // î  LATIN SMALL LETTER I WITH CIRCUMFLEX  
  {0xc3ac, 0xA2},    // ì  LATIN SMALL LETTER I WITH GRAVE  
  {0xc3ad, 0xA3},    // í  LATIN SMALL LETTER I WITH ACUTE 
  {0xc3af, 0xA4},    // ï  LATIN SMALL LETTER I WITH DIAERESIS
  // i ?
  {0xc391, 0xA6},    // Ñ  LATIN CAPITAL LETTER N WITH TILDE
  {0xc3b1, 0xA7},    // ñ  LATIN SMALL LETTER N WITH TILDE
  // n ?
  // g with stroke below ?
  // g upper ?
  // v upper ?
  // v upper with stroke ?
  // W upper with stroke ?
  // m upper with stroke ?
  {0xc386, 0xAF},    // Æ  LATIN CAPITAL LETTER AE 
  {0xC2A7, 0xB0},    // §  Section Sign 
  {0xc2b1, 0xB1},    // ±  PLUS-MINUS SIGN  
  {0xc4b3, 0xB2},    // ĳ  LATIN SMALL LIGATURE IJ  
  // arrow 3 byte
  // arrow 3 byte
  // f
  {0xC2A3, 0xB7},    // £  Pound SIGN
  // arrow 3 byte
  // arrow 3 byte
  // arrow 3 byte
  {0xC2b6, 0xBC},    // ¶  Pilcrow Sign 
  {0xc2bd, 0xBD},    // ½  VULGAR FRACTION ONE HALF         - there a several variants in the rom
  {0xc2bc, 0xBE},    // ¼  VULGAR FRACTION ONE QUARTER       - there a several variants in the rom
  {0xc2be, 0xBF},    // ¾  VULGAR FRACTION THREE QUARTERS
  {0xc3b8, 0xC0},    // ø  LATIN SMALL LETTER O WITH STROKE
  {0xc4bf, 0xC1},    // Ŀ  LATIN CAPITAL LETTER L WITH MIDDLE DOT
  {0xc490, 0xC2},    // Đ  LATIN CAPITAL LETTER D WITH STROKE
  {0xC39F, 0xC3},    // ß  LATIN SMALL LETTER SHARP S
  {0xc387, 0xC4},    // Ç  LATIN CAPITAL LETTER C WITH CEDILLA
  // 
  //
  // 4 corners?
  {0xcebc, 0xC8},    // μ	GREEK SMALL LETTER MU
  {0xC2B5, 0xC8},    // µ MICRO SIGN
  {0xc398, 0xC9},    // Ø LATIN CAPITAL LETTER O WITH STROKE
  {0xc3bf, 0xCA},    // ÿ LATIN SMALL LETTER Y WITH DIAERESIS  
  {0xc383, 0xCB},    // Ã LATIN CAPITAL LETTER A WITH TILDE 
  {0xc8bb, 0xCC},    // Ȼ LATIN CAPITAL LETTER C WITH STROKE 
  {0xc3a3, 0xCD},	   // ã LATIN SMALL LETTER A WITH TILDE
  {0xc395, 0xCE},    // Õ LATIN CAPITAL LETTER O WITH TILDE
  {0xc3b5, 0xCF},    // õ LATIN SMALL LETTER O WITH TILDE
  {0xCB99, 0xD0},    // ? DOT ABOVE
  {0xc2a8, 0xD1},    // ¨ DIAERESIS
  {0xC2B0, 0xD2},    // ° DEGREE SIGN
  {0xcc80, 0xD3},    // ` COMBINING GRAVE ACCENT 
  {0xcc81, 0xD4},    // ´ COMBINING ACUTE ACCENT
  {0xc3b7, 0xD6},    // ÷ DIVISION SIGN
  {0xc2ab, 0xD7},    // « LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
  {0xc2bb, 0xD8},    // » RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
  //l with dot
  {0x5c,   0xDA},    // \	 REVERSE SOLIDUS  - MISSING ... wird so nicht gehen
  {0xc397, 0xDB},    // ×  MULTIPLICATION SIGN
  {0xc2ae, 0xDC},    // ®  REGISTERED SIGN
  {0xc2a9, 0xDD},    // ©  COPYRIGHT SIGN
  // ™  Trade Mark Sign       0xE284A2  
  // drei strich

  {0xceb1, 0xE0},    // α  GREEK SMALL LETTER ALPHA
  // 1/3 --> 3Byte
  {0xc2bd, 0xE2},    // ½  VULGAR FRACTION ONE HALF          - there is a second variant in the rom
  {0xc2bc, 0xE3},    // ¼  VULGAR FRACTION ONE QUARTER       - there is a second variant in the rom
  //2/3  --> 3byte
  {0xc2be, 0xE5},    // ¾  VULGAR FRACTION THREE QUARTERS
  // g with decender
  // arrow 3 byte
  // arrow 3 byte
  // j with decender
  // ▯  White Vertical Rectangle        0xE296AF
  // sieht aus wie kts
  // revers R
  // HO
  // W
  
  // p with decender
  // q mit unterlänge
  {0xceb8, 0xF2},    // θ GREEK SMALL LETTER THETA
  // ∞  INFINITY e2 88 9e 
  {0xcf89, 0xF4},    // ?  GREEK SMALL LETTER OMEGA
  {0xceb2, 0xF5},    // β  GREEK SMALL LETTER BETA
  {0xcea3, 0xF6},    // Σ  GREEK CAPITAL LETTER SIGMA // S, Summe
  //
  //
  // y with decender
  // b like
  // II
  // reverse N with tilde
  // U
  // b with stroke
  // █	 FULL BLOCK   	e2 96 88
 
  // 3 byte characters
  {0x8692, 0x7E},    // →  Rightwards Arrow      0xE286x92  
  {0x8690, 0x7F},    // ←  Leftwards Arrow       0xE286x90  
  {0x8691, 0xB3},    // ↑  Upwards Arrow         0xE286x91
  {0x8693, 0xB4},    // ↓  Downwards Arrow       0xE286x93
  {0x86b2, 0xB5},    // ↲  DOWNWARDS ARROW WITH TIP LEFTWARDS e2 86 b2
  {0x87a5, 0xB8},    // ⇥  RIGHTWARDS ARROW TO BAR    e2 87 a5
  {0x84A2, 0xDE},    // ™  Trade Mark Sign       0xE284A2   
  {0x84A6, 0xF4},    // ?  Ohm Sign              0xE284A6  (also the Omega)
  {0x8891, 0xF6},    // ∑  N-ARY SUMMATION        e2 88 91	
  {0x889e, 0xF3},    // ∞  INFINITY e2 88 9e  
  {0x96AF, 0xEB},    // ▯  White Vertical Rectangle        0xE296AF
  {0x8593, 0xE1},    // ⅓  VULGAR FRACTION ONE THIRD   e2 85 93	
  {0x8594, 0xE4},    // ⅔  VULGAR FRACTION TWO THIRDS  e2 85 94	
  {0x9688, 0xFF},    // █	 FULL BLOCK   	                 e2 96 88
  
/* 
  // ASCII normalization - deactivated due to memory reasons
  {0xc395, 'O'},     // Õ  LATIN CAPITAL LETTER O WITH TILDE
  {0xc396, 'O'},     // Ö  LATIN CAPITAL LETTER O WITH DIAERESIS
  {0xc3b0, 'o'},     // ð  LATIN SMALL LETTER ETH                  ?? 
  {0xc3a7, 'c'},     // ç  LATIN SMALL LETTER C WITH CEDILLA
  {0xc385, 'A'},     // Å  LATIN CAPITAL LETTER A WITH RING ABOVE  
  {0xc392, 'O'},     // Ò	 LATIN CAPITAL LETTER O WITH GRAVE
  {0xc393, 'O'},     // Ó  LATIN CAPITAL LETTER O WITH ACUTE
  {0xc394, 'O'},     // Ô  LATIN CAPITAL LETTER O WITH CIRCUMFLEX
  {0xc3a6, 'a'},     // æ  LATIN SMALL LETTER AE
  {0xc38c, 'I'},     // Í  LATIN CAPITAL LETTER I WITH ACUTE
  {0xc38d, 'I'},
  {0xc38e, 'I'},
  {0xc38f, 'I'},     // Ï	 LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc3bd, 'y'},     // ý	 LATIN SMALL LETTER Y WITH ACUTE
                     //    latin small letter a with macron below - not available https://en.wikipedia.org/wiki/Macron_below
                     //    latin small letter o with macron below - not available https://en.wikipedia.org/wiki/Macron_below
                     // f
                     // i
  {0xc482, 'A'},     // A  LATIN CAPITAL LETTER A WITH BREVE
  {0xc483, 'a'},     // a	 LATIN SMALL LETTER A WITH BREVE
  {0xc484, 'A'},     //	A	 LATIN CAPTIAL LETTER A WITH OGONEK
  {0xc485, 'a'},     //	a  LATIN SMALL LETTER A WITH OGONEK
  {0xc486, 'C'},     //	C	 LATIN CAPITAL LETTER C WITH ACUTE
  {0xc487, 'c'},     //	c  LATIN SMALL LETTER C WITH ACUTE  
  
  {0xc491, 'd'},     // d  LATIN SMALL LETTER D WITH STROKE - fraglich
  {0xc498, 'E'},     // E  LATIN CAPITAL LETTER E WITH OGONEK
  {0xc499, 'e'},     // e  LATIN SMALL LETTER E WITH OGONEK
  {0xc4ba, 'l'},     // l	 LATIN SMALL LETTER L WITH ACUTE
  {0xc4b9, 'L'},     // L  LATIN CAPITAL LETTER L WITH ACUTE
  {0xc48c, 'C'},     // C  LATIN CAPITAL LETTER C WITH CARON
  {0xc48d, 'c'},     // c  LATIN SMALL LETTER C WITH CARON
  {0xc48f, 'd'},     // d  substitude
  {0xc4be, 'l'},     // l	 LATIN SMALL LETTER L WITH CARON
 
  {0xc581, 'L'},     //	L	 LATIN CAPITAL LETTER L WITH STROKE
  {0xc582, 'l'},     //	l	 LATIN SMALL LETTER L WITH STROKE
  {0xc583, 'N'},     //	N  LATIN CAPITAL LETTER N WITH ACUTE
  {0xc584, 'n'},     //	n  LATIN SMALL LETTER N WITH ACUTE  
  {0xc587, 'N'},     //	N  LATIN CAPITAL LETTER N WITH CARON
  {0xc588, 'n'},     //	n  LATIN SMALL LETTER N WITH CARON
  {0xc590, 'O'},     // Ő	 LATIN CAPITAL LETTER O WITH DOUBLE ACUTE
  {0xc592, 'O'},     // Œ	 LATIN CAPITAL LIGATURE OE 
  {0xc593, 'o'},     // œ  LATIN SMALL LIGATURE OE
  {0xc595, 'r'},     // r	 LATIN SMALL LETTER R WITH ACUTE
  {0xc59a, 'S'},	   // S	 LATIN CAPITAL LETTER S WITH ACUTE
  {0xc59b, 's'},     // s  LATIN SMALL LETTER S WITH ACUTE
  {0xc5a0, 'S'},     // Š  LATIN CAPITAL LETTER S WITH CARON
  {0xc5a1, 's'},     // š	 LATIN SMALL LETTER S WITH CARON
  {0xc5b0, 'U'},     // U  LATIN CAPITAL LETTER U WITH DOUBLE ACUTE
  {0xc5b1, 'u'},     // u  LATIN SMALL LETTER U WITH DOUBLE ACUTE
  {0xc5bb, 'Z'},     //	Z  LATIN CAPITAL LETTER Z WITH DOT ABOVE
  {0xc5bc, 'z'},     //	z  LATIN SMALL LETTER Z WITH DOT ABOVE
  {0xc5bd, 'Z'},     // Ž  LATIN CAPITAL LETTER Z WITH CARON
  {0xc5be, 'z'},     // ž  LATIN SMALL LETTER Z WITH CARON 
  {0xc8ac, 'O'},     // ?  LATIN CAPITAL LETTER O WITH TILDE AND MACRON
  {0xc382, 'A'},     // Â  LATIN CAPITAL LETTER A WITH CIRCUMFLEX
  {0xc398, 'O'},     // Ø  LATIN CAPITAL LETTER O WITH STROKE
  {0xc395, 'O'},     // Õ  LATIN CAPITAL LETTER O WITH TILDE
  {0xc3a2, 'a'},     // â  LATIN SMALL LETTER A WITH CIRCUMFLEX
  {0xc3b8, 'o'},     // ø  LATIN SMALL LETTER O WITH STROKE - fraglich
  {0xc3bf, 'y'},     // ÿ	 LATIN SMALL LETTER Y WITH DIAERESIS
  {0xc5a1, 's'},     
  {0xc5a5, 't'},
  {0xc5af, 'u'},     // u	 LATIN SMALL LETTER U WITH RING ABOVE
  {0xc5b0, 'U'},     // U  LATIN CAPITAL LETTER U WITH DOUBLE ACUTE
  {0xc5be, 'z'},
  {0xc5b8, 'Y'},     // Ÿ	 LATIN CAPITAL LETTER Y WITH DIAERESIS
  {0xc5b9, 'Z'},     // Z  LATIN CAPITAL LETTER Z WITH ACUTE
  {0xc5ba, 'z'},     //	z  LATIN SMALL LETTER Z WITH ACUT
  {0xc898, 'S'},     // ?  LATIN CAPITAL LETTER S WITH COMMA BELOW
  {0xc899, 's'},     // ?  LATIN SMALL LETTER S WITH COMMA BELOW
  {0xc89b, 't'},     // ?  substitude
  
  // most of the row 0xC is 3 byte
  // unused - needs rework
  {0xC2A7, 0xD2},    // §  Section Sign
  {0xc2b5, 0xEA},    // µ  GREEK SMALL LETTER MU

  {0xcd93, 0xd4},    // G  GREEK CAPITAL LETTER GAMMA
  {0xce94, 0xd5},    // ?  ce 94  GREEK CAPITAL LETTER DELTA ? only because it could fit in this order
  {0x8abf, 0xd5},    // ?  RIGHT TRIANGLE e2 8a bf 
  {0xce98, 0xd6},    // T  GREEK CAPITAL LETTER THETA
  {0xce9b, 0xD7},    // ?  GREEK CAPITAL LETTER LAMDA
  {0xce94, 0xD8},    // ?  GREEK CAPITAL LETTER XI
  {0xdea0, 0xD9},    // ?  GREEK CAPITAL LETTER PI
  {0xcea3, 0xDA},    // S  GREEK CAPITAL LETTER SIGMA
  {0xcea5, 0xDB},    // ?  GREEK CAPITAL LETTER UPSILON
  {0xcea6, 0xDC},    // F  GREEK CAPITAL LETTER PHI
  {0xcea8, 0xDD},    // ?  GREEK CAPITAL LETTER PSI
  {0xcea9, 0xDE},    // O  GREEK CAPITAL LETTER OMEGA
  {0xceb1, 0xDf},    // a  GREEK SMALL LETTER ALPHA
  
  {0xceb2, 0xe0},    // ß  GREEK SMALL LETTER BETA
  {0xceb3, 0xE1},    // ?  GREEK SMALL LETTER GAMMA
  {0xceb4, 0xE2},    // d  GREEK SMALL LETTER DELTA  
  {0xceb5, 0xE3},    // e  GREEK SMALL LETTER EPSILON
  {0xceb6, 0xE4},    // ?  GREEK SMALL LETTER ZETA
  {0xceb7, 0xE5},    // ?  GREEK SMALL LETTER ETA
  {0xceb8, 0xE6},    // ?  GREEK SMALL LETTER THETA 
  {0xceb9, 0xE7},    // ?  GREEK SMALL LETTER IOTA
  {0xceba, 0xE8},    // ?  GREEK SMALL LETTER KAPPA
  {0xcebb, 0xE9},    // ?  GREEK SMALL LETTER LAMDA
  {0xcebc, 0xEA},    // µ  GREEK SMALL LETTER MU --> also used for micro
  
  {0xcebd, 0xEB},    // ?  GREEK SMALL LETTER NU
  {0xcebe, 0xEC},    // ?  GREEK SMALL LETTER XI
  {0xcf80, 0xED},    // p  GREEK SMALL LETTER PI
  {0xcf81, 0xEE},    // ?  GREEK SMALL LETTER RHO
  
  {0xcf84, 0xF0},    // t  GREEK SMALL LETTER TAU
  {0xcf85, 0xF1},    // ?  GREEK SMALL LETTER UPSILON
  {0xcf87, 0xF2},    // ?  GREEK SMALL LETTER CHI
  {0xcf88, 0xF3},    // ?  GREEK SMALL LETTER PSI

                     //    pfeile --> 3byte
                     //    white rectangle -->
                     //    -
                     //    service mark -> 3yte 
                     //    SOUND RECORDING COPYRIGHT  -> 3byte
  
  // here starts the 3byte section
  // but I store only the lower 2 bytes of the character 
  // E2 3byte
  {0x8893, 0x10},    // ±  MINUS-OR-PLUS SIGN      0xe28893
  {0x89a1, 0x11},    // =  IDENTICAL TO            0xe289a1
  {0x8eb2, 0x12},    // ?  SUMMATION TOP           0xe28eb2  
  {0x8eb3, 0x13},    // ?  SUMMATION BOTTOM        0xe28eb3    
  {0x8ea7, 0x14},    //    LEFT CURLY BRACKET UPPER HOOK   
  {0x8e8e, 0x15},    //    LEFT CURLY BRACKET LOWER HOOK
  {0x8eab, 0x16},    //    RIGHT CURLY BRACKET UPPER HOOK   
  {0x8ead, 0x17},    //    RIGHT CURLY BRACKET LOWER HOOK
  {0x8eb0, 0x18},    // ?  UPPER LEFT OR LOWER RIGHT CURLY BRACKET SECTION  0xe28eb0  
  {0x8eb1, 0x19},    // ?  UPPER RIGHT OR LOWER LEFT CURLY BRACKET SECTION  0xe28eb1  
  {0x8988, 0x1a},    // ˜  ALMOST EQUAL TO       0xe28988  
  {0x88ab, 0x1b},    // ?  INTEGRAL              0xe288ab  
  {0x88bc, 0x1d},    // ~  TILDE OPERATOR        0xe288bc    
  
  {0x889A, 0xBE},    //    SQUARE ROOT           0xE2889A
  {0x89A0, 0xBD},    // ?  NOT EQUAL             0xE289A0
  {0x89a4, 0xB9},    // =                        0xE289A4
  {0x89A5, 0xB9},    // =                        0xE289A5

  {0x8ca0, 0xC0},    // (  TOP HALF INTEGRAL     0xe28ca0  
  {0x8ca1, 0xC1},    // )  BOTTOM HALF INTEGRAL  0xe28ca1    
  {0x889E, 0xc2},    //    Infiniy               0xE2889E  
  {0x8abe, 0xc3},    // ?  Right Angle with Arc  0xE28ABE
  {0x86B2, 0xC4},    // ?  Downwards Arrow with Tip Leftwards 0xE2 0x86 0xB2
  {0x8691, 0xC5},    // ?  Upwards Arrow         0xE286x91
  {0x8693, 0xC6},    // ?  Downwards Arrow       0xE286x93
  {0x8692, 0xC7},    // ?  Rightwards Arrow      0xE286x92
  {0x8690, 0xC8},    // ?  Leftwards Arrow       0xE286x90
  {0x8c88, 0xb9},    // ?  LEFT CEILING          0xe28cx88  
  {0x8c89, 0xba},    // ?  RIGHT CEILING         0xe28cx89
  {0x8c8a, 0xbb},    // ?  LEFT FLOOR            0xe28cx8a  
  {0x8c8b, 0xbc},    // ?  RIGHT FLOOR           0xe28cx8b  
  {0x8b85, 0xbd},    // · DOT OPERATOR           0xe28bx85  
  
  {0x9C9D, 0xD1},    //?  Latin Cross            0xE29C9D
  {0x8b81, 0xD7},    // ?   N-ARY LOGICAL OR     0xe2b81  
  {0x888f, 0xD9},    // ?  N-ARY PRODUCT         0xe288f  
  {0x8891, 0xDA},    // ?   N-ARY SUMMATION      0xe2891  

  {0x889c, 0xDF},    // ? PROPORTIONAL TO        0xe2889d (it's more a Greek small letter alpha)
  {0x86A4, 0xF9},    // ? Leftwards Arrow from Bar         0xE286A4
  {0x86a6, 0xFB},    // ?  Rightwards Arrow from Bar       0xE286A6
*/
};


// character mapping for the LEC1621_02 ROM (English Russian)
// sorted by UTF-8
// Cyrillic follows ROM_SPLC780D1_002A
// MISSING: the Latin letters are straight forward and should be implemented in a separate converter (similar to ST7070_ext)
// the mapping is complete, but there are several 7bit characters not according to ASCII
const Mapping ROM_LEC1621_02[] PROGMEM = {
  // LATIN
  {0xc380, 0x10},    // À  LATIN CAPITAL LETTER A WITH GRAVE
  {0xc381, 0x11},    // Á  LATIN CAPITAL LETTER A WITH ACUTE
  {0xc382, 0x12},    // Â  LATIN CAPITAL LETTER A WITH CIRCUMFLEX
  {0xc383, 0x13},    // Ã  LATIN CAPITAL LETTER A WITH TILDE
  {0xc384, 0x14},    // Ä  LATIN CAPITAL LETTER A WITH DIAERESIS
  {0xc385, 0x15},    // Å  LATIN CAPITAL LETTER A WITH RING ABOVE
  {0xc386, 0x16},    // Æ  LATIN CAPITAL LETTER AE
  {0xc387, 0x17},    // Ç  LATIN CAPITAL LETTER C WITH CEDILLA
  {0xc388, 0x18},    // È  LATIN CAPITAL LETTER E WITH GRAVE
  {0xc389, 0x19},    // É  LATIN CAPITAL LETTER E WITH ACUTE
  {0xc38a, 0x1A},    // Ê  LATIN CAPITAL LETTER E WITH CIRCUMFLEX
  {0xc38b, 0x1B},    // Ë  LATIN CAPITAL LETTER E WITH DIAERESIS
  {0xc38c, 0x1C},    // Ì  LATIN CAPITAL LETTER I WITH GRAVE
  {0xc38d, 0x1D},    // Í  LATIN CAPITAL LETTER I WITH ACUTE
  {0xc38e, 0x1E},    // Î  LATIN CAPITAL LETTER I WITH CIRCUMFLEX
  {0xc38f, 0x1F},    // Ï  LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc390, 0x80},    // Ð  LATIN CAPITAL LETTER ETH
  {0xc391, 0x81},    // Ñ  LATIN CAPITAL LETTER N WITH TILDE
  {0xc392, 0x82},    // Ò  LATIN CAPITAL LETTER O WITH GRAVE
  {0xc393, 0x83},    // Ó  LATIN CAPITAL LETTER O WITH ACUTE
  {0xc394, 0x84},    // Ô  LATIN CAPITAL LETTER O WITH CIRCUMFLEX
  {0xc395, 0x85},    // Õ  LATIN CAPITAL LETTER O WITH TILDE
  {0xc396, 0x86},    // Ö  LATIN CAPITAL LETTER O WITH DIAERESIS
  {0xc397, 0x87},    // ×  MULTIPLICATION SIGN
  {0xc398, 0x88},    // Ø  LATIN CAPITAL LETTER O WITH STROKE
  {0xc399, 0x89},    // Ù  LATIN CAPITAL LETTER U WITH GRAVE
  {0xc39a, 0x8A},    // Ú  LATIN CAPITAL LETTER U WITH ACUTE
  {0xc39b, 0x8B},    // Û  LATIN CAPITAL LETTER U WITH CIRCUMFLEX
  {0xc39c, 0x8C},    // Ü  LATIN CAPITAL LETTER U WITH DIAERESIS
  {0xc39d, 0x8D},    // Ý  LATIN CAPITAL LETTER Y WITH ACUTE
  {0xc39e, 0x8E},    // Þ  LATIN CAPITAL LETTER THORN  ????
  {0xc39f, 0x8F},    // ß  LATIN SMALL LETTER SHARP S
  {0xc3a0, 0x90},    // à  LATIN SMALL LETTER A WITH GRAVE
  {0xc3a1, 0x91},    // á  LATIN SMALL LETTER A WITH ACUTE
  {0xc3a2, 0x92},    // â  LATIN SMALL LETTER A WITH CIRCUMFLEX
  {0xc3a3, 0x93},    // ã  LATIN SMALL LETTER A WITH TILDE
  {0xc3a4, 0x94},    // ä  LATIN SMALL LETTER A WITH DIAERESIS
  {0xc3a5, 0x95},    // å  LATIN SMALL LETTER A WITH RING ABOVE
  {0xc3a6, 0x96},    // æ  LATIN SMALL LETTER AE
  {0xc3a7, 0x97},    // ç  LATIN SMALL LETTER C WITH CEDILLA
  {0xc3a8, 0x98},    // è  LATIN SMALL LETTER E WITH GRAVE
  {0xc3a9, 0x99},    // é  LATIN SMALL LETTER E WITH ACUTE
  {0xc3aa, 0x9A},    // ê  LATIN SMALL LETTER E WITH CIRCUMFLEX
  {0xc3ab, 0x9B},    // ë  LATIN SMALL LETTER E WITH DIAERESIS
  {0xc3ac, 0x9C},    // ì  LATIN SMALL LETTER I WITH GRAVE
  {0xc3ad, 0x9D},    // í  LATIN SMALL LETTER I WITH ACUTE
  {0xc3ae, 0x9E},    // î  LATIN SMALL LETTER I WITH CIRCUMFLEX
  {0xc3af, 0x9F},    // ï  LATIN SMALL LETTER I WITH DIAERESIS
  // no more LATIN
  // some replacement for GERMAN
  {0xc3b6, 'o'},    // ö  LATIN SMALL LETTER O WITH DIAERESIS
  {0xc3bc, 'u'},    // ü  LATIN SMALL LETTER U WITH DIAERESIS
  
  // Cyrillic
  {0xd081, 0xA2}, // Ё  CYRILLIC CAPITAL LETTER IO
  {0xd086, 'I'},  // ?  CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
  {0xd090, 'A'},  // А  CYRILLIC CAPITAL LETTER A
  {0xd091, 0xA0}, // Б  CYRILLIC CAPITAL LETTER BE
  {0xd092, 'B'},  // В  CYRILLIC CAPITAL LETTER VE
  {0xd093, 0xA1}, // Г  CYRILLIC CAPITAL LETTER GHE
  {0xd094, 0xE0}, // Д  CYRILLIC CAPITAL LETTER DE
  {0xd095, 'E'},  // Е  CYRILLIC CAPITAL LETTER IE
  {0xd096, 0xA3}, // Ж  CYRILLIC CAPITAL LETTER ZHE
  {0xd097, 0xA4}, // З  CYRILLIC CAPITAL LETTER ZE
  {0xd098, 0xA5}, // И  CYRILLIC CAPITAL LETTER I
  {0xd099, 0xA6}, // Й  CYRILLIC CAPITAL LETTER SHORT I
  {0xd09a, 'K'},  // К  CYRILLIC CAPITAL LETTER KA
  {0xd09b, 0xA7}, // Л  CYRILLIC CAPITAL LETTER EL
  {0xd09c, 'M'},  // М  CYRILLIC CAPITAL LETTER EM
  {0xd09d, 'H'},  // Н  CYRILLIC CAPITAL LETTER EN
  {0xd09e, 'O'},  // О  CYRILLIC CAPITAL LETTER O
  {0xd09f, 0xA8}, // П  CYRILLIC CAPITAL LETTER PE
  {0xd0a0, 'P'},  // Р  CYRILLIC CAPITAL LETTER ER
  {0xd0a1, 'C'},  // С  CYRILLIC CAPITAL LETTER ES
  {0xd0a2, 'T'},  // Т  CYRILLIC CAPITAL LETTER TE
  {0xd0a3, 0xA9}, // У  CYRILLIC CAPITAL LETTER U
  {0xd0a4, 0xAA}, // Ф  CYRILLIC CAPITAL LETTER EF
  {0xd0a5, 'X'},  // Х  CYRILLIC CAPITAL LETTER HA
  {0xd0a6, 0xE1}, // Ц  CYRILLIC CAPITAL LETTER TSE
  {0xd0a7, 0xAB}, // Ч  CYRILLIC CAPITAL LETTER CHE
  {0xd0a8, 0xAC}, // Ш  CYRILLIC CAPITAL LETTER SHA
  {0xd0a9, 0xE2}, // Щ  CYRILLIC CAPITAL LETTER SHCHA
  {0xd0aa, 0xAD}, // Ъ  CYRILLIC CAPITAL LETTER HARD SIGN
  {0xd0ab, 0xAE}, // Ы  CYRILLIC CAPITAL LETTER YERU
  {0xd0ac, 'b'},  // Ь  CYRILLIC CAPITAL LETTER SOFT SIGN
  {0xd0ad, 0xAF}, // Э  CYRILLIC CAPITAL LETTER E
  {0xd0ae, 0xB0}, // Ю  CYRILLIC CAPITAL LETTER YU - JU
  {0xd0af, 0xB1}, // Я  CYRILLIC CAPITAL LETTER YA  - JA
  {0xd0b0, 'a'},  // а  CYRILLIC SMALL LETTER A
  {0xd0b1, 0xB2}, // б  CYRILLIC SMALL LETTER BE
  {0xd0b2, 0xB3}, // в  CYRILLIC SMALL LETTER VE
  {0xd0b3, 0xB4}, // г  CYRILLIC SMALL LETTER GHE
  {0xd0b4, 0xE3}, // д  CYRILLIC SMALL LETTER DE 
  {0xd0b5, 'e'},  // е  CYRILLIC SMALL LETTER IE           war 0xb5, e ist aber besser
  {0xd0b6, 0xB6}, // ж  CYRILLIC SMALL LETTER ZHE
  {0xd0b7, 0xB7}, // з  CYRILLIC SMALL LETTER ZE
  {0xd0b8, 0xB8}, // и  CYRILLIC SMALL LETTER I
  {0xd0b9, 0xB9}, // й  CYRILLIC SMALL LETTER SHORT I
  {0xd0ba, 0xBA}, // к  CYRILLIC SMALL LETTER KA
  {0xd0bb, 0xBB}, // л  CYRILLIC SMALL LETTER EL
  {0xd0bc, 0xBC}, // м  CYRILLIC SMALL LETTER EM
  {0xd0bd, 0xBD}, // н  CYRILLIC SMALL LETTER EN
  {0xd0be, 'o'},  // о  CYRILLIC SMALL LETTER O
  {0xd0bf, 0xBE}, // п  CYRILLIC SMALL LETTER PE
  {0xd180, 'p'},  // р  CYRILLIC SMALL LETTER ER
  {0xd181, 'c'},  // с  CYRILLIC SMALL LETTER ES
  {0xd182, 0xBF}, // т  CYRILLIC SMALL LETTER TE
  {0xd183, 0xC0}, // у  CYRILLIC SMALL LETTER U
  {0xd184, 0xE4}, // ф  CYRILLIC SMALL LETTER EF         
  {0xd185, 'x'},  // х  CYRILLIC SMALL LETTER HA
  {0xd186, 0xE5}, // ц  CYRILLIC SMALL LETTER TSE
  {0xd187, 0xC0}, // ч  CYRILLIC SMALL LETTER CHE        
  {0xd188, 0xC1}, // ш  CYRILLIC SMALL LETTER SHA
  {0xd189, 0xE6}, // щ  CYRILLIC SMALL LETTER SHCHA
  {0xd18a, 0xC2}, // ъ  CYRILLIC SMALL LETTER HARD SIGN
  {0xd18b, 0xC3}, // ы  CYRILLIC SMALL LETTER YERU
  {0xd18c, 0xC4}, // ь  CYRILLIC SMALL LETTER SOFT SIGN
  {0xd18d, 0xC5}, // э  CYRILLIC SMALL LETTER E
  {0xd18e, 0xC6}, // ю  CYRILLIC SMALL LETTER YU
  {0xd18f, 0xC7}, // я  CYRILLIC SMALL LETTER YA
  // Pre 1918 letters
  {0xd190, 'e'},  // ѐ  CYRILLIC SMALL LETTER IE WITH GRAVE
  {0xd191, 0x9b}, // ё  CYRILLIC SMALL LETTER IO
  {0xd192, 'h'},  // ђ  CYRILLIC SMALL LETTER DJE          - substitute ASCII
  {0xd193, 'r'},  // ѓ  CYRILLIC SMALL LETTER GJE          - substitute ASCII
  {0xd196, 'i'},  // і  CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
  {0xd197, 'i'},  // ї  CYRILLIC SMALL LETTER YI           - substitute ASCII
  {0xd1a2, 'E'},  // Ѣ  CYRILLIC CAPITAL LETTER YAT        - substitute ASCII 
  {0xd1a3, 'e'},  // ѣ  CYRILLIC SMALL LETTER YAT          - substitute ASCII
  {0xd1b2, 'F'},  // Ѳ  CYRILLIC CAPITAL LETTER FITA       - substitute ASCII according GOST
  {0xd1b3, 'f'},  // ѳ  CYRILLIC SMALL LETTER FITA         - substitute ASCII according GOST
  {0xd1b4, 'Y'},  // Ѵ  CYRILLIC CAPITAL LETTER IZHITSA    - substitute ASCII according GOST
  {0xd1b5, 'y'},  // ѵ  CYRILLIC SMALL LETTER IZHITSA      - substitute ASCII according GOST
};


// convert the actual special and value into a printable character
// MISSING: port to enum class
constexpr byte NOPRINT = 0;
constexpr byte PRINT = 1;
constexpr byte PRINT2 = 2;
constexpr byte ADDE = 4;


// special characters for Ä Ö Ü in an array for easier handling regards convert_special
// It is somehow redundant to all the special characters in NoiascaCustomCharacter.h 
// to be used with the createUML method - but it still saves some flash memory.
const PROGMEM uint8_t UML[3][8] = {
  {0b10001, 0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0},          // German Ä
  {0b10001, 0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0},          // German Ö
  {0b10001, 0b00000, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110, 0}           // German Ü
};


/* 
   Character ROM specific converters
   these prepared character converters are hardware specific
   mapping tables in this file
*/
uint8_t convert_dummy(uint32_t &special, uint8_t &value);            // empty implementation
uint8_t convert(uint32_t &special, uint8_t &value);                  // HD44780_A00 Ä -> A. Converts large german umlauts to letters without diareses.
uint8_t convert_ae(uint32_t &special, uint8_t &value);               // HD44780_A00 Ä -> Ae. Extends large letter umlauts with a small letter e.
uint8_t convert_uml(uint32_t &special, uint8_t &value, char umlA, char umlO, char umlU);    // base converter with special handling of the 3 German umlauts
uint8_t convert_small(uint32_t &special, uint8_t &value);            // HD44780_A00 Ä -> ä. Converts large german umlauts to their small letters. That's not so bad if you compare the Ü/ü or Ö/ö and large letter Ä are seldom used!
uint8_t convert_special(uint32_t &special, uint8_t &value);          // HD44780_A00 Ä -> Ä. Uses custom characters to get a precise replacements
uint8_t convert_ST7070(uint32_t &special, uint8_t &value);           // ST7070 smaller version for Latin
uint8_t convert_ST7070_ext(uint32_t &special, uint8_t &value);       // ST7070 extended for nearly all characters in font2
uint8_t convert_SPLC780D1_002A(uint32_t &special, uint8_t &value);   // SPLC780D1 with 002A ROM only (Cyrillic)
uint8_t convert_SPLC780D1_003A(uint32_t &special, uint8_t &value);   // SPLC780D1 with 003A ROM only (European) also for some OLED font table 3
//uint8_t convert_HD44780A02(uint32_t &special, uint8_t &value);     // HD44780_A02 - alternative ROM A02 - European (latin) and some cyrillic letters - tbd

// this example callback converter maps UTF-8 characters including up to 8 custom characters
uint8_t convert_custom (uint32_t &special, uint8_t &value);          // HD44780 + up to 8 special characters

uint8_t convert_generic(uint32_t &special, uint8_t &value, const Mapping *language, size_t lang_len);  // NOT HW specific, accepts any mapping table
//uint8_t convert_LEC1621_00(uint32_t &special, uint8_t &value);     // OLED LEC1621 font 0 English Japanese (Hebrew) - not implemented (due to lack of hebrew knowledge). Use HD44780_A00 instead
uint8_t convert_LEC1621_01(uint32_t &special, uint8_t &value);       // OLED LEC1621 font 1 Western European
uint8_t convert_LEC1621_02(uint32_t &special, uint8_t &value);       // OLED LEC1621 font 2 English Russian
uint8_t convert_LEC1621_03(uint32_t &special, uint8_t &value);       // OLED LEC1621 font 3 (will reuse SPLC780D1_003A)

extern uint32_t utf8_to_customChar[8];                                // has to be extern in .h

/*
   language specific converters
   converts letters with diakritic marks to their plain ASCII represantant 
   The mapping tables are the external .h and needs to be included
   currently this main function relies is specific to the HD44780 because it replaces two signes in the lower 127 bytes
   but no other special characters in the upper 128 ROM are used
*/
uint8_t convert_HW(uint32_t &special, uint8_t &value, const Mapping *language, size_t lang_len);       // HD44780 Ä -> A. Converts large german umlauts to letters without diareses.

// the write method can only call converters with two parameters
// therefore there are these alias functions to hand over the language mapping
uint8_t convert_ASCII(uint32_t &special, uint8_t &value);            // Latin to ASCII replacement

uint8_t convert_gmw(uint32_t &special, uint8_t &value);              // West Germanic (Danish, Swedish, Norwagian) 
uint8_t convert_gmq(uint32_t &special, uint8_t &value);              // North Germanic (Danish, Swedish, Norwagian) 
uint8_t convert_roa(uint32_t &special, uint8_t &value);              // Romance
uint8_t convert_sla(uint32_t &special, uint8_t &value);              // Slavik (Czech, Slovac, Polish, Slovene)
uint8_t convert_de(uint32_t &special, uint8_t &value);               // German Ä -> A. Converts large german umlauts to letters without diareses.
uint8_t convert_en(uint32_t &special, uint8_t &value);               // English 
uint8_t convert_es(uint32_t &special, uint8_t &value);               // Spanish
uint8_t convert_fr(uint32_t &special, uint8_t &value);               // French
uint8_t convert_fi(uint32_t &special, uint8_t &value);               // Finish
uint8_t convert_hu(uint32_t &special, uint8_t &value);               // Hungarian
uint8_t convert_cyrillic_min(uint32_t &special, uint8_t &value);     // cyrillic to ASCII replacement


// deprecated alias functions - to be removed in 1.2.2 + 3 or after 2021-08-15
// Latin to ASCII replacement - to be replaced two version after 1.2.2
#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
  uint8_t __attribute__((deprecated("use convert_ASCII() instead"))) convert_ANSI(uint32_t &special, uint8_t &value);
#elif defined(__GNUC__)
  uint8_t __attribute__((deprecated)) convert_ANSI(uint32_t &special, uint8_t &value);
#else
  uint8_t convert_ANSI(uint32_t &special, uint8_t &value);
#endif

#endif // old include guard
