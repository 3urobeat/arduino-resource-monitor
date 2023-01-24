/*
   Noiasca Liquid Crystal - Custom Characters
   
   This file contains custom characters (special characters) in 5 x 7 bitmap for LCDs
   All characters are 5 x 7 only. Nevertheless the bitmap is 8 rows high as the creatChar will load 8 bytes into the LCD. 
   The last row could be used for a descender or an underscore. And that's the reason, why I keep the font prepared for 5 x 8.
   
   It is a separate file because it could be expanded a lot in the future.
   
   naming convention is according to the UTF-8 naming, 
   without noun "letter", wihtout preposition "with", 
   
   copyright 2021 noiasca noiasca@yahoo.com
   
   Version
   2021-09-06 latin small with descender
   2021-02-16 brought to PROGMEM
   2021-02-14 first version
*/

#pragma once
#ifndef NOIASCA_CUSTOM_CHARACTERS_H
#define NOIASCA_CUSTOM_CHARACTERS_H
// latin characters with diacritic marks
const uint8_t latin_capital_a_circumflex[]  PROGMEM = {0b01100, 0b10010, 0b01100, 0b10010, 0b11110, 0b10010, 0b10010 ,0}; // {0xc382, 'A'}, // Â  LATIN CAPITAL LETTER A WITH CIRCUMFLEX 
const uint8_t latin_capital_a_diareses[]    PROGMEM = {0b01010, 0b00000, 0b01110, 0b10001, 0b11111, 0b10001, 0b10001 ,0}; // {0xc384, 'A'}, // Ä  LATIN CAPITAL LETTER A WITH DIAERESIS
//const uint8_t latin_capital_a_diareses[]  PROGMEM = {0b10001, 0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001 ,0}; // {0xc384, 'A'}, // Ä  LATIN CAPITAL LETTER A WITH DIAERESIS - Variant
const uint8_t latin_capital_a_ring_above[]  PROGMEM = {0b00100, 0b01010, 0b01110, 0b10001, 0b11111, 0b10001, 0b10001 ,0}; // {0xc385, 'A'}, // Å  LATIN CAPITAL LETTER A WITH RING ABOVE
const uint8_t latin_capital_ae[]            PROGMEM = {0b00000, 0b00000, 0b01011, 0b10100, 0b11110, 0b10100, 0b10111 ,0}; // {0xc386, 'A'}, // Æ  LATIN CAPITAL LETTER AE
const uint8_t latin_capital_c_cedilla[]     PROGMEM = {0b01110, 0b10001, 0b10000, 0b10001, 0b01110, 0b00100, 0b01000 ,0}; // {0xc387, 'C'}, // Ç  LATIN CAPITAL LETTER C WITH CEDILLA
const uint8_t latin_capital_e_diaresis[]    PROGMEM = {0b01010, 0b00000, 0b11111, 0b10000, 0b11110, 0b10000, 0b11111 ,0}; // {0xc38b, 'E'}, // Ë  LATIN CAPITAL LETTER E WITH DIAERESIS
const uint8_t latin_capital_i_circumflex[]  PROGMEM = {0b00100, 0b01010, 0b01110, 0b00100, 0b00100, 0b00100, 0b01110 ,0}; // {0xc38e, 'I'}, // Î  LATIN CAPITAL LETTER I WITH CIRCUMFLEX
const uint8_t latin_small_a_circumflex[]    PROGMEM = {0b00100, 0b01010, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111 ,0}; // {0xc3a2, 'a'}, // â  LATIN SMALL LETTER A WITH CIRCUMFLEX 
const uint8_t latin_small_c_cedilla[]       PROGMEM = {0b00000, 0b01110, 0b10000, 0b10000, 0b01110, 0b00100, 0b01000 ,0}; // {0xc3a7, 'c'}, // ç  LATIN SMALL LETTER C WITH CEDILLA
const uint8_t latin_small_e_diaresis[]      PROGMEM = {0b01010, 0b00000, 0b01110, 0b10001, 0b11111, 0b10000, 0b01110 ,0}; // {0xc3ab, 'e'}, // ë  LATIN SMALL LETTER E WITH DIAERESIS
const uint8_t latin_small_i_circumflex[]    PROGMEM = {0b00100, 0b01010, 0b00000, 0b01100, 0b00100, 0b00100, 0b00100 ,0}; // {0xc3ae, 'i'}, // î  LATIN SMALL LETTER I WITH CIRCUMFLEX
const uint8_t latin_capital_o_diareses[]    PROGMEM = {0b01010, 0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110 ,0}; // {0xc396, 'O'}, // Ö  LATIN CAPITAL LETTER O WITH DIAERESIS
//const uint8_t latin_capital_o_diareses[]  PROGMEM = {0b10001, 0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110 ,0}; // {0xc396, 'O'}, // Ö  LATIN CAPITAL LETTER O WITH DIAERESIS - Variant
const uint8_t latin_capital_o_stroke[]      PROGMEM = {0b00000, 0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b01110 ,0}; // {0xc398, 'O'}, // Ø  LATIN CAPITAL LETTER O WITH STROKE 
const uint8_t latin_capital_o_grave[]       PROGMEM = {0b01000, 0b00100, 0b00110, 0b01001, 0b01001, 0b01001, 0b00110 ,0}; // {0xc392, 'O'}, // Ò  LATIN CAPITAL LETTER O WITH GRAVE
const uint8_t latin_capital_o_tilde[]       PROGMEM = {0b01010, 0b10100, 0b01100, 0b10010, 0b10010, 0b10010, 0b01100 ,0}; // {0xc395, 'O'}, // Õ  LATIN CAPITAL LETTER O WITH TILDE
const uint8_t latin_capital_u_diareses[]    PROGMEM = {0b10001, 0b00000, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110 ,0}; // {0xc39c, 'U'}, // Ü  LATIN CAPITAL LETTER U WITH DIAERESIS
const uint8_t latin_small_a_ring_above []   PROGMEM = {0b00100, 0b01010, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111 ,0}; // {0xc3a5, 'a'}, // å  LATIN SMALL LETTER A WITH RING ABOVE
const uint8_t latin_small_ae[]              PROGMEM = {0b00000, 0b00000, 0b11111, 0b00101, 0b11111, 0b10100, 0b11111 ,0}; // {0xc3a6, 'a'}, // æ  LATIN SMALL LETTER AE
const uint8_t latin_small_o_grave[]         PROGMEM = {0b01000, 0b00100, 0b00000, 0b00110, 0b01001, 0b01001, 0b00110 ,0}; // {0xc3b2, 'o'}, // ò  LATIN SMALL LETTER O WITH GRAVE  
const uint8_t latin_small_o_tilde[]         PROGMEM = {0b01010, 0b10100, 0b00000, 0b01100, 0b10010, 0b10010, 0b01100 ,0}; // {0xc3b5, 'o'}, // õ  LATIN SMALL LETTER O WITH TILDE
const uint8_t latin_small_o_stroke[]        PROGMEM = {0b00000, 0b00000, 0b01110, 0b10011, 0b10101, 0b11001, 0b01110 ,0}; // {0xc3b8, 'o'}, // ø  LATIN SMALL LETTER O WITH STROKE
const uint8_t latin_capital_c_caron[]       PROGMEM = {0b01010, 0b00100, 0b01110, 0b10001, 0b10000, 0b10001, 0b01110 ,0}; // {0xc48c, 'C'}, // Č  LATIN CAPITAL LETTER C WITH CARON
const uint8_t latin_small_c_caron[]         PROGMEM = {0b01010, 0b00100, 0b01110, 0b10000, 0b10000, 0b10000, 0b01110 ,0}; // {0xc48d, 'c'}, // č  LATIN SMALL LETTER C WITH CARON
const uint8_t latin_capital_s_caron[]       PROGMEM = {0b01010, 0b00100, 0b01111, 0b10000, 0b01110, 0b00001, 0b11110 ,0}; // {0xc5a0, 'S'}, // Š  LATIN CAPITAL LETTER S WITH CARON
const uint8_t latin_small_s_caron[]         PROGMEM = {0b01010, 0b00100, 0b01110, 0b10000, 0b01110, 0b00001, 0b01110 ,0}; // {0xc5a1, 's'}, // š  LATIN SMALL LETTER S WITH CARON
const uint8_t latin_capital_z_caron[]       PROGMEM = {0b01010, 0b00100, 0b11111, 0b00010, 0b00100, 0b01000, 0b11111 ,0}; // {0xc5bd, 'Z'}, // Ž  LATIN CAPITAL LETTER Z WITH CARON
const uint8_t latin_small_z_caron[]         PROGMEM = {0b01010, 0b00100, 0b01110, 0b00010, 0b00100, 0b01000, 0b01110 ,0}; // {0xc5be, 'z'}, // ž  LATIN SMALL LETTER Z WITH CARON
const uint8_t latin_capital_d_stroke[]      PROGMEM = {0b11100, 0b10010, 0b10001, 0b11001, 0b10001, 0b10010, 0b11100 ,0}; // {0xc490, 'D'}, // Đ  LATIN CAPITAL LETTER D WITH STROKE
const uint8_t latin_small_d_stroke[]        PROGMEM = {0b00010, 0b00111, 0b00010, 0b00010, 0b01110, 0b10010, 0b01110 ,0}; // {0xc491, 'd'}, // đ  LATIN SMALL LETTER D WITH STROKE
const uint8_t latin_capital_s_comma_below[] PROGMEM = {0b01111, 0b10000, 0b01110, 0b00001, 0b11110, 0b00010, 0b00110 ,0}; // {0xc898, 'S'}, // Ș  LATIN CAPITAL LETTER S WITH COMMA BELOW
const uint8_t latin_small_s_comma_below[]   PROGMEM = {0b01110, 0b01000, 0b01110, 0b00010, 0b01110, 0b00010, 0b00110 ,0}; // {0xc899, 's'}, // ș  LATIN SMALL LETTER S WITH COMMA BELOW
const uint8_t latin_capital_t_comma_below[] PROGMEM = {0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01000 ,0}; // {0xc89a, 'T'}, // Ț  LATIN CAPITAL LETTER T WITH COMMA BELOW
const uint8_t latin_small_t_comma_below[]   PROGMEM = {0b01000, 0b11100, 0b01000, 0b01001, 0b00110, 0b00010, 0b00100 ,0}; // {0xc89b, 't'}, // ț  LATIN SMALL LETTER T WITH COMMA BELOW
const uint8_t latin_small_sharp_s[]         PROGMEM = {0b01110, 0b10001, 0b10110, 0b10001, 0b10001, 0b10001, 0b10110, 0}; // {0xC39F, 's'}, // ß  LATIN SMALL LETTER SHARP S 
// define small letters with descender g, j, p, q and y
const byte latin_small_g_descender[]        PROGMEM = {0b00000, 0b00000, 0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b01110};
const byte latin_small_j_descender[]        PROGMEM = {0b00001, 0b00000, 0b00001, 0b00001, 0b00001, 0b00001, 0b01001, 0b00110};
const byte latin_small_p_descender[]        PROGMEM = {0b00000, 0b00000, 0b01110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000};
const byte latin_small_q_descender[]        PROGMEM = {0b00000, 0b00000, 0b00110, 0b01001, 0b01001, 0b00111, 0b00001, 0b00001};
const byte latin_small_y_descender[]        PROGMEM = {0b00000, 0b00000, 0b01001, 0b01001, 0b01001, 0b00111, 0b00001, 0b01110};
#endif
