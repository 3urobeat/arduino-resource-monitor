/*
   Noiasca Liquid Crystal - Noiasca UTF8
   
   mapping to convert UTF-8 characters to their plain ASCII (Lower ANSI) represantation
   
   It is a separate file, because it could be used for other Hardware also.
   These converters are not LCD specific.
   
   ASCII
   ****
   The full UTF-8 ASCII mapping will take around 1500 bytes 
   and covers characters which are used only in some specific languages
   there are also some smaller subsets available
   
   Language families:
   ******************
   Language families combine several languages and most of Europe can be covered quite well with
   
   gmq - west germanic
   gmq - north germanic
   roa - romance
   sla - slavic
   
   language specific
   ******************
   The smallest flash usage can be achieved with language specific mappings. 
   They are available  if I need them or if a language doesn't fit 
   well in one of the above language families.
   
   en - English - only for test purposes, English has some deprecated letters
   es - Spanish - also includes Catalan ca
   fr - French - but large anyway. So you still can use romance-roa instead
   fi - Finnisch - this Uralic language doesn't fit to well into the existing language families 
   ge - German - as it is my native language
   hu - Hungarian - this Uralic language doesn't fit to well into the existing language families
   
   Cyrillic
   ******************
   cyrillic_GOST16876_71_1 - a smaller set of Cyrillic converted according GOST16876-71 (1)
   
   Open topics
   -
   
   copyright 2021 noiasca noiasca@yahoo.com
   
   Version
   2021-02-14 replaced ANSI by ASCII, include guard
   2021-02-12 Cyrillic converter   
   2020-12-20 initial version
*/

#pragma once
#ifndef NOIASCA_UTF8_H
#define NOIASCA_UTF8_H
// the structure of the UTF8 -> single character (mapping)
struct Mapping {
  uint16_t utf8; // the 2 least significant bytes of the 4 byte UTF-8 character
  uint8_t c;     // the final 1 byte charactter to print
};


// Latin-1 Supplement (without c2 control, reduced c2 symbols)
// not included are
//   0xc280 - 0xc29F these are control bytes, only in Windows-1252 (CP-1252) which is a superset of ISO 8859-1, also called ISO Latin-1, these are printable characters
//   0xc29F - 0xc2BF as these are mostly symbols (beside two added for Spanish)
// Latin Extended-A 
// Latin Extended-B 
// Spacing Modifier Letters (without cb modifier, 
// Cobining Ciacritical Marks (cc combining, )
// Greek and Coptic (cd greek, )
// Cyrillic (d0 cyrillic)
// Cyrillic Supplement
   
const Mapping UTF8_ASCII[] PROGMEM = {
  {0xc2a1, '!'},      // ¡	INVERTED EXCLAMATION MARK
  {0xc2aa, 'a'},      // ª	FEMININE ORDINAL INDICATOR
  {0xc2ba, 'o'},      // º	MASCULINE ORDINAL INDICATOR
  {0xc2bf, '?'},      // ¿  INVERTED QUESTION MARK  
  {0xc380, 'A'},      // À	LATIN CAPITAL LETTER A WITH GRAVE
  {0xc381, 'A'},      // Á	LATIN CAPITAL LETTER A WITH ACUTE
  {0xc382, 'A'},      // Â	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
  {0xc383, 'A'},      // Ã	LATIN CAPITAL LETTER A WITH TILDE
  {0xc384, 'A'},      // Ä	LATIN CAPITAL LETTER A WITH DIAERESIS
  {0xc385, 'A'},      // Å	LATIN CAPITAL LETTER A WITH RING ABOVE
  {0xc386, 'A'},      // Æ	LATIN CAPITAL LETTER AE
  {0xc387, 'C'},      // Ç	LATIN CAPITAL LETTER C WITH CEDILLA
  {0xc388, 'E'},      // È	LATIN CAPITAL LETTER E WITH GRAVE
  {0xc389, 'E'},      // É	LATIN CAPITAL LETTER E WITH ACUTE
  {0xc38a, 'E'},      // Ê	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
  {0xc38b, 'E'},      // Ë	LATIN CAPITAL LETTER E WITH DIAERESIS
  {0xc38c, 'I'},      // Ì	LATIN CAPITAL LETTER I WITH GRAVE
  {0xc38d, 'I'},      // Í	LATIN CAPITAL LETTER I WITH ACUTE
  {0xc38e, 'I'},      // Î	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
  {0xc38f, 'I'},      // Ï	LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc390, 'D'},      // Ð	LATIN CAPITAL LETTER ETH
  {0xc391, 'N'},      // Ñ	LATIN CAPITAL LETTER N WITH TILDE
  {0xc392, 'O'},      // Ò	LATIN CAPITAL LETTER O WITH GRAVE
  {0xc393, 'O'},      // Ó	LATIN CAPITAL LETTER O WITH ACUTE
  {0xc394, 'O'},      // Ô	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
  {0xc395, 'O'},      // Õ	LATIN CAPITAL LETTER O WITH TILDE
  {0xc396, 'O'},      // Ö	LATIN CAPITAL LETTER O WITH DIAERESIS
  {0xc397, '*'},      // ×	MULTIPLICATION SIGN
  {0xc398, 'O'},      // Ø	LATIN CAPITAL LETTER O WITH STROKE
  {0xc399, 'U'},      // Ù	LATIN CAPITAL LETTER U WITH GRAVE
  {0xc39a, 'U'},      // Ú	LATIN CAPITAL LETTER U WITH ACUTE
  {0xc39b, 'U'},      // Û	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
  {0xc39c, 'U'},      // Ü	LATIN CAPITAL LETTER U WITH DIAERESIS
  {0xc39d, 'Y'},      // Ý	LATIN CAPITAL LETTER Y WITH ACUTE
  {0xc39e, 'p'},      // Þ	LATIN CAPITAL LETTER THORN - somehow
  {0xc39f, 's'},      // ß	LATIN SMALL LETTER SHARP S
  {0xc3a0, 'a'},      // à	LATIN SMALL LETTER A WITH GRAVE
  {0xc3a1, 'a'},      // á	LATIN SMALL LETTER A WITH ACUTE
  {0xc3a2, 'a'},      // â	LATIN SMALL LETTER A WITH CIRCUMFLEX
  {0xc3a3, 'a'},      // ã	LATIN SMALL LETTER A WITH TILDE
  {0xc3a4, 'a'},      // ä	LATIN SMALL LETTER A WITH DIAERESIS
  {0xc3a5, 'a'},      // å	LATIN SMALL LETTER A WITH RING ABOVE
  {0xc3a6, 'a'},      // æ	LATIN SMALL LETTER AE
  {0xc3a7, 'c'},      // ç	LATIN SMALL LETTER C WITH CEDILLA
  {0xc3a8, 'e'},      // è	LATIN SMALL LETTER E WITH GRAVE
  {0xc3a9, 'e'},      // é	LATIN SMALL LETTER E WITH ACUTE
  {0xc3aa, 'e'},      // ê	LATIN SMALL LETTER E WITH CIRCUMFLEX
  {0xc3ab, 'e'},      // ë	LATIN SMALL LETTER E WITH DIAERESIS
  {0xc3ac, 'i'},      // ì	LATIN SMALL LETTER I WITH GRAVE
  {0xc3ad, 'i'},      // í	LATIN SMALL LETTER I WITH ACUTE
  {0xc3ae, 'i'},      // î	LATIN SMALL LETTER I WITH CIRCUMFLEX
  {0xc3af, 'i'},      // ï	LATIN SMALL LETTER I WITH DIAERESIS
  {0xc3b0, 'e'},      // ð	LATIN SMALL LETTER ETH
  {0xc3b1, 'n'},      // ñ	LATIN SMALL LETTER N WITH TILDE
  {0xc3b2, 'o'},      // ò	LATIN SMALL LETTER O WITH GRAVE
  {0xc3b3, 'o'},      // ó	LATIN SMALL LETTER O WITH ACUTE
  {0xc3b4, 'o'},      // ô	LATIN SMALL LETTER O WITH CIRCUMFLEX
  {0xc3b5, 'o'},      // õ	LATIN SMALL LETTER O WITH TILDE
  {0xc3b6, 'o'},      // ö	LATIN SMALL LETTER O WITH DIAERESIS
  {0xc3b7, '/'},      // ÷	DIVISION SIGN - somehow
  {0xc3b8, 'o'},      // ø	LATIN SMALL LETTER O WITH STROKE
  {0xc3b9, 'u'},      // ù	LATIN SMALL LETTER U WITH GRAVE
  {0xc3ba, 'u'},      // ú	LATIN SMALL LETTER U WITH ACUTE
  {0xc3bb, 'u'},      // û	LATIN SMALL LETTER U WITH CIRCUMFLEX
  {0xc3bc, 'u'},      // ü	LATIN SMALL LETTER U WITH DIAERESIS
  {0xc3bd, 'y'},      // ý	LATIN SMALL LETTER Y WITH ACUTE
  {0xc3be, 'p'},      // þ	LATIN SMALL LETTER THORN - somehow
  {0xc3bf, 'y'},      // ÿ	LATIN SMALL LETTER Y WITH DIAERESIS
  {0xc480, 'A'},      // Ā	LATIN CAPITAL LETTER A WITH MACRON
  {0xc481, 'a'},      // ā	LATIN SMALL LETTER A WITH MACRON
  {0xc482, 'A'},      // Ă	LATIN CAPITAL LETTER A WITH BREVE
  {0xc483, 'a'},      // ă	LATIN SMALL LETTER A WITH BREVE
  {0xc484, 'A'},      // Ą	LATIN CAPITAL LETTER A WITH OGONEK
  {0xc485, 'a'},      // ą	LATIN SMALL LETTER A WITH OGONEK
  {0xc486, 'C'},      // Ć	LATIN CAPITAL LETTER C WITH ACUTE
  {0xc487, 'c'},      // ć	LATIN SMALL LETTER C WITH ACUTE
  {0xc488, 'C'},      // Ĉ	LATIN CAPITAL LETTER C WITH CIRCUMFLEX
  {0xc489, 'c'},      // ĉ	LATIN SMALL LETTER C WITH CIRCUMFLEX
  {0xc48a, 'C'},      // Ċ	LATIN CAPITAL LETTER C WITH DOT ABOVE
  {0xc48b, 'c'},      // ċ	LATIN SMALL LETTER C WITH DOT ABOVE
  {0xc48c, 'C'},      // Č	LATIN CAPITAL LETTER C WITH CARON
  {0xc48d, 'c'},      // č	LATIN SMALL LETTER C WITH CARON
  {0xc48e, 'D'},      // Ď	LATIN CAPITAL LETTER D WITH CARON
  {0xc48f, 'd'},      // ď	LATIN SMALL LETTER D WITH CARON
  {0xc490, 'D'},      // Đ	LATIN CAPITAL LETTER D WITH STROKE
  {0xc491, 'd'},      // đ	LATIN SMALL LETTER D WITH STROKE
  {0xc492, 'E'},      // Ē	LATIN CAPITAL LETTER E WITH MACRON
  {0xc493, 'e'},      // ē	LATIN SMALL LETTER E WITH MACRON
  {0xc494, 'E'},      // Ĕ	LATIN CAPITAL LETTER E WITH BREVE
  {0xc495, 'e'},      // ĕ	LATIN SMALL LETTER E WITH BREVE
  {0xc496, 'E'},      // Ė	LATIN CAPITAL LETTER E WITH DOT ABOVE
  {0xc497, 'e'},      // ė	LATIN SMALL LETTER E WITH DOT ABOVE
  {0xc498, 'E'},      // Ę	LATIN CAPITAL LETTER E WITH OGONEK
  {0xc499, 'e'},      // ę	LATIN SMALL LETTER E WITH OGONEK
  {0xc49a, 'E'},      // Ě	LATIN CAPITAL LETTER E WITH CARON
  {0xc49b, 'e'},      // ě	LATIN SMALL LETTER E WITH CARON
  {0xc49c, 'G'},      // Ĝ	LATIN CAPITAL LETTER G WITH CIRCUMFLEX
  {0xc49d, 'g'},      // ĝ	LATIN SMALL LETTER G WITH CIRCUMFLEX
  {0xc49e, 'G'},      // Ğ	LATIN CAPITAL LETTER G WITH BREVE
  {0xc49f, 'g'},      // ğ	LATIN SMALL LETTER G WITH BREVE
  {0xc4a0, 'G'},      // Ġ	LATIN CAPITAL LETTER G WITH DOT ABOVE
  {0xc4a1, 'g'},      // ġ	LATIN SMALL LETTER G WITH DOT ABOVE
  {0xc4a2, 'G'},      // Ģ	LATIN CAPITAL LETTER G WITH CEDILLA
  {0xc4a3, 'g'},      // ģ	LATIN SMALL LETTER G WITH CEDILLA
  {0xc4a4, 'H'},      // Ĥ	LATIN CAPITAL LETTER H WITH CIRCUMFLEX
  {0xc4a5, 'h'},      // ĥ	LATIN SMALL LETTER H WITH CIRCUMFLEX
  {0xc4a6, 'H'},      // Ħ	LATIN CAPITAL LETTER H WITH STROKE
  {0xc4a7, 'h'},      // ħ	LATIN SMALL LETTER H WITH STROKE
  {0xc4a8, 'I'},      // Ĩ	LATIN CAPITAL LETTER I WITH TILDE
  {0xc4a9, 'i'},      // ĩ	LATIN SMALL LETTER I WITH TILDE
  {0xc4aa, 'I'},      // Ī	LATIN CAPITAL LETTER I WITH MACRON
  {0xc4ab, 'i'},      // ī	LATIN SMALL LETTER I WITH MACRON
  {0xc4ac, 'I'},      // Ĭ	LATIN CAPITAL LETTER I WITH BREVE
  {0xc4ad, 'i'},      // ĭ	LATIN SMALL LETTER I WITH BREVE
  {0xc4ae, 'I'},      // Į	LATIN CAPITAL LETTER I WITH OGONEK
  {0xc4af, 'i'},      // į	LATIN SMALL LETTER I WITH OGONEK
  {0xc4b0, 'I'},      // İ	LATIN CAPITAL LETTER I WITH DOT ABOVE
  {0xc4b1, 'i'},      // ı	LATIN SMALL LETTER DOTLESS I
  {0xc4b2, 'I'},      // Ĳ	LATIN CAPITAL LIGATURE IJ  - somehow - J should be added for Dutch
  {0xc4b3, 'i'},      // ĳ	LATIN SMALL LIGATURE IJ - somehow - J should be added for Dutch
  {0xc4b4, 'J'},      // Ĵ	LATIN CAPITAL LETTER J WITH CIRCUMFLEX
  {0xc4b5, 'j'},      // ĵ	LATIN SMALL LETTER J WITH CIRCUMFLEX
  {0xc4b6, 'K'},      // Ķ	LATIN CAPITAL LETTER K WITH CEDILLA
  {0xc4b7, 'k'},      // ķ	LATIN SMALL LETTER K WITH CEDILLA
  {0xc4b8, 'k'},      // ĸ	LATIN SMALL LETTER KRA
  {0xc4b9, 'L'},      // Ĺ	LATIN CAPITAL LETTER L WITH ACUTE
  {0xc4ba, 'l'},      // ĺ	LATIN SMALL LETTER L WITH ACUTE
  {0xc4bb, 'L'},      // Ļ	LATIN CAPITAL LETTER L WITH CEDILLA
  {0xc4bc, 'l'},      // ļ	LATIN SMALL LETTER L WITH CEDILLA
  {0xc4bd, 'L'},      // Ľ	LATIN CAPITAL LETTER L WITH CARON
  {0xc4be, 'l'},      // ľ	LATIN SMALL LETTER L WITH CARON
  {0xc4bf, 'L'},      // Ŀ	LATIN CAPITAL LETTER L WITH MIDDLE DOT
  {0xc580, 'l'},      // ŀ	LATIN SMALL LETTER L WITH MIDDLE DOT
  {0xc581, 'L'},      // Ł	LATIN CAPITAL LETTER L WITH STROKE
  {0xc582, 'l'},      // ł	LATIN SMALL LETTER L WITH STROKE
  {0xc583, 'N'},      // Ń	LATIN CAPITAL LETTER N WITH ACUTE
  {0xc584, 'n'},      // ń	LATIN SMALL LETTER N WITH ACUTE
  {0xc585, 'N'},      // Ņ	LATIN CAPITAL LETTER N WITH CEDILLA
  {0xc586, 'n'},      // ņ	LATIN SMALL LETTER N WITH CEDILLA
  {0xc587, 'N'},      // Ň	LATIN CAPITAL LETTER N WITH CARON
  {0xc588, 'n'},      // ň	LATIN SMALL LETTER N WITH CARON
  {0xc589, 'n'},      // ŉ	LATIN SMALL LETTER N PRECEDED BY APOSTROPHE
  {0xc58a, 'N'},      // Ŋ	LATIN CAPITAL LETTER ENG
  {0xc58b, 'n'},      // ŋ	LATIN SMALL LETTER ENG
  {0xc58c, 'O'},      // Ō	LATIN CAPITAL LETTER O WITH MACRON
  {0xc58d, 'o'},      // ō	LATIN SMALL LETTER O WITH MACRON
  {0xc58e, 'O'},      // Ŏ	LATIN CAPITAL LETTER O WITH BREVE
  {0xc58f, 'o'},      // ŏ	LATIN SMALL LETTER O WITH BREVE
  {0xc590, 'O'},      // Ő	LATIN CAPITAL LETTER O WITH DOUBLE ACUTE
  {0xc591, 'o'},      // ő	LATIN SMALL LETTER O WITH DOUBLE ACUTE
  {0xc592, 'O'},      // Œ	LATIN CAPITAL LIGATURE OE
  {0xc593, 'o'},      // œ	LATIN SMALL LIGATURE OE
  {0xc594, 'R'},      // Ŕ	LATIN CAPITAL LETTER R WITH ACUTE
  {0xc595, 'r'},      // ŕ	LATIN SMALL LETTER R WITH ACUTE
  {0xc596, 'R'},      // Ŗ	LATIN CAPITAL LETTER R WITH CEDILLA
  {0xc597, 'r'},      // ŗ	LATIN SMALL LETTER R WITH CEDILLA
  {0xc598, 'R'},      // Ř	LATIN CAPITAL LETTER R WITH CARON
  {0xc599, 'r'},      // ř	LATIN SMALL LETTER R WITH CARON
  {0xc59a, 'S'},      // Ś	LATIN CAPITAL LETTER S WITH ACUTE
  {0xc59b, 'S'},      // ś	LATIN SMALL LETTER S WITH ACUTE
  {0xc59c, 'S'},      // Ŝ	LATIN CAPITAL LETTER S WITH CIRCUMFLEX
  {0xc59d, 's'},      // ŝ	LATIN SMALL LETTER S WITH CIRCUMFLEX
  {0xc59e, 'S'},      // Ş	LATIN CAPITAL LETTER S WITH CEDILLA
  {0xc59f, 's'},      // ş	LATIN SMALL LETTER S WITH CEDILLA
  {0xc5a0, 'S'},      // Š	LATIN CAPITAL LETTER S WITH CARON
  {0xc5a1, 's'},      // š	LATIN SMALL LETTER S WITH CARON
  {0xc5a2, 'T'},      // Ţ	LATIN CAPITAL LETTER T WITH CEDILLA
  {0xc5a3, 't'},      // ţ	LATIN SMALL LETTER T WITH CEDILLA
  {0xc5a4, 'T'},      // Ť	LATIN CAPITAL LETTER T WITH CARON
  {0xc5a5, 't'},      // ť	LATIN SMALL LETTER T WITH CARON
  {0xc5a6, 'T'},      // Ŧ	LATIN CAPITAL LETTER T WITH STROKE
  {0xc5a7, 't'},      // ŧ	LATIN SMALL LETTER T WITH STROKE
  {0xc5a8, 'U'},      // Ũ	LATIN CAPITAL LETTER U WITH TILDE
  {0xc5a9, 'u'},      // ũ	LATIN SMALL LETTER U WITH TILDE
  {0xc5aa, 'U'},      // Ū	LATIN CAPITAL LETTER U WITH MACRON
  {0xc5ab, 'u'},      // ū	LATIN SMALL LETTER U WITH MACRON
  {0xc5ac, 'U'},      // Ŭ	LATIN CAPITAL LETTER U WITH BREVE
  {0xc5ad, 'u'},      // ŭ	LATIN SMALL LETTER U WITH BREVE
  {0xc5ae, 'U'},      // Ů	LATIN CAPITAL LETTER U WITH RING ABOVE
  {0xc5af, 'u'},      // ů	LATIN SMALL LETTER U WITH RING ABOVE
  {0xc5b0, 'U'},      // Ű	LATIN CAPITAL LETTER U WITH DOUBLE ACUTE
  {0xc5b1, 'u'},      // ű	LATIN SMALL LETTER U WITH DOUBLE ACUTE
  {0xc5b2, 'U'},      // Ų	LATIN CAPITAL LETTER U WITH OGONEK
  {0xc5b3, 'u'},      // ų	LATIN SMALL LETTER U WITH OGONEK
  {0xc5b4, 'W'},      // Ŵ	LATIN CAPITAL LETTER W WITH CIRCUMFLEX
  {0xc5b5, 'w'},      // ŵ	LATIN SMALL LETTER W WITH CIRCUMFLEX
  {0xc5b6, 'Y'},      // Ŷ	LATIN CAPITAL LETTER Y WITH CIRCUMFLEX
  {0xc5b7, 'y'},      // ŷ	LATIN SMALL LETTER Y WITH CIRCUMFLEX
  {0xc5b8, 'Y'},      // Ÿ	LATIN CAPITAL LETTER Y WITH DIAERESIS
  {0xc5b9, 'Z'},      // Ź	LATIN CAPITAL LETTER Z WITH ACUTE
  {0xc5ba, 'z'},      // ź	LATIN SMALL LETTER Z WITH ACUTE
  {0xc5bb, 'Z'},      // Ż	LATIN CAPITAL LETTER Z WITH DOT ABOVE
  {0xc5bc, 'z'},      // ż	LATIN SMALL LETTER Z WITH DOT ABOVE
  {0xc5bd, 'Z'},      // Ž	LATIN CAPITAL LETTER Z WITH CARON
  {0xc5be, 'z'},      // ž	LATIN SMALL LETTER Z WITH CARON
  {0xc5bf, 's'},      // ſ	LATIN SMALL LETTER LONG S
  {0xc680, 'b'},      // ƀ	LATIN SMALL LETTER B WITH STROKE
  {0xc681, 'B'},      // Ɓ	LATIN CAPITAL LETTER B WITH HOOK
  {0xc682, 'B'},      // Ƃ	LATIN CAPITAL LETTER B WITH TOPBAR
  {0xc683, 'b'},      // ƃ	LATIN SMALL LETTER B WITH TOPBAR
  {0xc684, 'B'},      // Ƅ	LATIN CAPITAL LETTER TONE SIX - somehow
  {0xc685, 'b'},      // ƅ	LATIN SMALL LETTER TONE SIX - somehow
  {0xc686, 'O'},      // Ɔ	LATIN CAPITAL LETTER OPEN O
  {0xc687, 'C'},      // Ƈ	LATIN CAPITAL LETTER C WITH HOOK
  {0xc688, 'c'},      // ƈ	LATIN SMALL LETTER C WITH HOOK
  {0xc689, 'D'},      // Ɖ	LATIN CAPITAL LETTER AFRICAN D
  {0xc68a, 'D'},      // Ɗ	LATIN CAPITAL LETTER D WITH HOOK
  {0xc68b, 'D'},      // Ƌ	LATIN CAPITAL LETTER D WITH TOPBAR
  {0xc68c, 'd'},      // ƌ	LATIN SMALL LETTER D WITH TOPBAR
  //{0xc68d, ''},      // ƍ	LATIN SMALL LETTER TURNED DELTA
  {0xc68e, 'E'},      // Ǝ	LATIN CAPITAL LETTER REVERSED E
  //{0xc68f, ''},      // Ə	LATIN CAPITAL LETTER SCHWA
  {0xc690, 'E'},      // Ɛ	LATIN CAPITAL LETTER OPEN E
  {0xc691, 'F'},      // Ƒ	LATIN CAPITAL LETTER F WITH HOOK
  {0xc692, 'f'},      // ƒ	LATIN SMALL LETTER F WITH HOOK
  {0xc693, 'G'},      // Ɠ	LATIN CAPITAL LETTER G WITH HOOK
  //{0xc694, ''},      // Ɣ	LATIN CAPITAL LETTER GAMMA
  //{0xc695, ''},      // ƕ	LATIN SMALL LETTER HV
  //{0xc696, ''},      // Ɩ	LATIN CAPITAL LETTER IOTA
  {0xc697, 'I'},       // Ɨ	LATIN CAPITAL LETTER I WITH STROKE
  {0xc698, 'K'},       // Ƙ	LATIN CAPITAL LETTER K WITH HOOK
  {0xc699, 'k'},       // ƙ	LATIN SMALL LETTER K WITH HOOK
  {0xc69a, 'l'},       // ƚ	LATIN SMALL LETTER L WITH BAR
  //{0xc69b, ''},      // ƛ	LATIN SMALL LETTER LAMBDA WITH STROKE
  {0xc69c, 'M'},       // Ɯ	LATIN CAPITAL LETTER TURNED M
  {0xc69d, 'N'},       // Ɲ	LATIN CAPITAL LETTER N WITH LEFT HOOK
  {0xc69e, 'n'},       // ƞ	LATIN SMALL LETTER N WITH LONG RIGHT LEG
  {0xc69f, 'O'},       // Ɵ	LATIN CAPITAL LETTER O WITH MIDDLE TILDE
  {0xc6a0, 'o'},       // ơ	LATIN SMALL LETTER O WITH HORN
  //{0xc6a2, ''},      // Ƣ	LATIN CAPITAL LETTER OI
  //{0xc6a3, ''},      // ƣ	LATIN SMALL LETTER OI
  {0xc6a4, 'P'},       // Ƥ	LATIN CAPITAL LETTER P WITH HOOK
  {0xc6a5, 'p'},       // ƥ	LATIN SMALL LETTER P WITH HOOK
  //{0xc6a6, ''},      // Ʀ	LATIN LETTER YR
  //{0xc6a7, ''},      // Ƨ	LATIN CAPITAL LETTER TONE TWO
  //{0xc6a8, ''},      // ƨ	LATIN SMALL LETTER TONE TWO
  //{0xc6a9, ''},      // Ʃ	LATIN CAPITAL LETTER ESH
  //{0xc6aa, ''},      // ƪ	LATIN LETTER REVERSED ESH LOOP
  {0xc6ab, 't'},       // ƫ	LATIN SMALL LETTER T WITH PALATAL HOOK
  {0xc6ac, 'T'},       // Ƭ	LATIN CAPITAL LETTER T WITH HOOK
  {0xc6ad, 't'},       // ƭ	LATIN SMALL LETTER T WITH HOOK
  {0xc6ae, 'T'},       // Ʈ	LATIN CAPITAL LETTER T WITH RETROFLEX HOOK
  {0xc6af, 'U'},       // Ư	LATIN CAPITAL LETTER U WITH HORN
  {0xc6b0, 'u'},       // ư	LATIN SMALL LETTER U WITH HORN
  //{0xc6b1, ''},      // Ʊ	LATIN CAPITAL LETTER UPSILON
  {0xc6b2, 'V'},       // Ʋ	LATIN CAPITAL LETTER V WITH HOOK
  {0xc6b3, 'Y'},       // Ƴ	LATIN CAPITAL LETTER Y WITH HOOK
  {0xc6b4, 'y'},       // ƴ	LATIN SMALL LETTER Y WITH HOOK
  {0xc6b5, 'Z'},       // Ƶ	LATIN CAPITAL LETTER Z WITH STROKE
  {0xc6b6, 'z'},       // ƶ	LATIN SMALL LETTER Z WITH STROKE
  //{0xc6b7, ''},      // Ʒ	LATIN CAPITAL LETTER EZH
  //{0xc6b8, ''},      // Ƹ	LATIN CAPITAL LETTER EZH REVERSED
  //{0xc6b9, ''},      // ƹ	LATIN SMALL LETTER EZH REVERSED
  //{0xc6ba, ''},      // ƺ	LATIN SMALL LETTER EZH WITH TAIL
  //{0xc6bb, ''},      // ƻ	LATIN LETTER TWO WITH STROKE
  //{0xc6bc, ''},      // Ƽ	LATIN CAPITAL LETTER TONE FIVE
  //{0xc6bd, ''},      // ƽ	LATIN SMALL LETTER TONE FIVE
  //{0xc6be, ''},      // ƾ	LATIN LETTER INVERTED GLOTTAL STOP WITH STROKE
  //{0xc6bf, ''},      // ƿ	LATIN LETTER WYNN
  //{0xc780, ''},      // ǀ	LATIN LETTER DENTAL CLICK
  //{0xc781, ''},      // ǁ	LATIN LETTER LATERAL CLICK
  //{0xc782, ''},      // ǂ	LATIN LETTER ALVEOLAR CLICK
  //{0xc783, ''},      // ǃ	LATIN LETTER RETROFLEX CLICK
  {0xc784, 'D'},      // Ǆ	LATIN CAPITAL LETTER DZ WITH CARON
  {0xc785, 'D'},      // ǅ	LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON
  {0xc786, 'd'},      // ǆ	LATIN SMALL LETTER DZ WITH CARON
  {0xc787, 'L'},      // Ǉ	LATIN CAPITAL LETTER LJ
  {0xc788, 'L'},      // ǈ	LATIN CAPITAL LETTER L WITH SMALL LETTER J
  {0xc789, 'l'},      // ǉ	LATIN SMALL LETTER LJ
  {0xc78a, 'N'},      // Ǌ	LATIN CAPITAL LETTER NJ
  {0xc78b, 'N'},      // ǋ	LATIN CAPITAL LETTER N WITH SMALL LETTER J
  {0xc78c, 'n'},      // ǌ	LATIN SMALL LETTER NJ
  {0xc78d, 'A'},      // Ǎ	LATIN CAPITAL LETTER A WITH CARON
  {0xc78e, 'a'},      // ǎ	LATIN SMALL LETTER A WITH CARON
  {0xc78f, 'I'},      // Ǐ	LATIN CAPITAL LETTER I WITH CARON
  {0xc790, 'i'},      // ǐ	LATIN SMALL LETTER I WITH CARON
  {0xc791, 'O'},      // Ǒ	LATIN CAPITAL LETTER O WITH CARON
  {0xc792, 'o'},      // ǒ	LATIN SMALL LETTER O WITH CARON
  {0xc793, 'U'},      // Ǔ	LATIN CAPITAL LETTER U WITH CARON
  {0xc794, 'u'},      // ǔ	LATIN SMALL LETTER U WITH CARON
  {0xc795, 'U'},      // Ǖ	LATIN CAPITAL LETTER U WITH DIAERESIS AND MACRON
  {0xc796, 'u'},      // ǖ	LATIN SMALL LETTER U WITH DIAERESIS AND MACRON
  {0xc797, 'U'},      // Ǘ	LATIN CAPITAL LETTER U WITH DIAERESIS AND ACUTE
  {0xc798, 'u'},      // ǘ	LATIN SMALL LETTER U WITH DIAERESIS AND ACUTE
  {0xc799, 'U'},      // Ǚ	LATIN CAPITAL LETTER U WITH DIAERESIS AND CARON
  {0xc79a, 'u'},      // ǚ	LATIN SMALL LETTER U WITH DIAERESIS AND CARON
  {0xc79b, 'U'},      // Ǜ	LATIN CAPITAL LETTER U WITH DIAERESIS AND GRAVE
  {0xc79c, 'u'},      // ǜ	LATIN SMALL LETTER U WITH DIAERESIS AND GRAVE
  {0xc79d, 'e'},      // ǝ	LATIN SMALL LETTER TURNED E
  {0xc79e, 'A'},      // Ǟ	LATIN CAPITAL LETTER A WITH DIAERESIS AND MACRON
  {0xc79f, 'a'},      // ǟ	LATIN SMALL LETTER A WITH DIAERESIS AND MACRON
  {0xc7a0, 'A'},      // Ǡ	LATIN CAPITAL LETTER A WITH DOT ABOVE AND MACRON
  {0xc7a1, 'a'},      // ǡ	LATIN SMALL LETTER A WITH DOT ABOVE AND MACRON
  {0xc7a2, 'A'},      // Ǣ	LATIN CAPITAL LETTER AE WITH MACRON
  {0xc7a3, 'a'},      // ǣ	LATIN SMALL LETTER AE WITH MACRON
  {0xc7a4, 'G'},      // Ǥ	LATIN CAPITAL LETTER G WITH STROKE
  {0xc7a5, 'g'},      // ǥ	LATIN SMALL LETTER G WITH STROKE
  {0xc7a6, 'G'},      // Ǧ	LATIN CAPITAL LETTER G WITH CARON
  {0xc7a7, 'g'},      // ǧ	LATIN SMALL LETTER G WITH CARON
  {0xc7a8, 'K'},      // Ǩ	LATIN CAPITAL LETTER K WITH CARON
  {0xc7a9, 'k'},      // ǩ	LATIN SMALL LETTER K WITH CARON
  {0xc7aa, 'O'},      // Ǫ	LATIN CAPITAL LETTER O WITH OGONEK
  {0xc7ab, 'o'},      // ǫ	LATIN SMALL LETTER O WITH OGONEK
  {0xc7ac, 'O'},      // Ǭ	LATIN CAPITAL LETTER O WITH OGONEK AND MACRON
  {0xc7ad, 'o'},      // ǭ	LATIN SMALL LETTER O WITH OGONEK AND MACRON
  //{0xc7ae, ''},      // Ǯ	LATIN CAPITAL LETTER EZH WITH CARON
  //{0xc7af, ''},      // ǯ	LATIN SMALL LETTER EZH WITH CARON
  {0xc7b0, 'J'},      // ǰ	LATIN SMALL LETTER J WITH CARON
  {0xc7b1, 'D'},      // Ǳ	LATIN CAPITAL LETTER DZ
  {0xc7b2, 'd'},      // ǲ	LATIN CAPITAL LETTER D WITH SMALL LETTER Z
  {0xc7b3, 'd'},      // ǳ	LATIN SMALL LETTER DZ
  {0xc7b4, 'G'},      // Ǵ	LATIN CAPITAL LETTER G WITH ACUTE
  {0xc7b5, 'g'},      // ǵ	LATIN SMALL LETTER G WITH ACUTE
  //{0xc7b6, ''},      // Ƕ	LATIN CAPITAL LETTER HWAIR
  //{0xc7b7, ''},      // Ƿ	LATIN CAPITAL LETTER WYNN
  {0xc7b8, 'N'},      // Ǹ	LATIN CAPITAL LETTER N WITH GRAVE
  {0xc7b9, 'n'},      // ǹ	LATIN SMALL LETTER N WITH GRAVE
  {0xc7ba, 'A'},      // Ǻ	LATIN CAPITAL LETTER A WITH RING ABOVE AND ACUTE
  {0xc7bb, 'a'},      // ǻ	LATIN SMALL LETTER A WITH RING ABOVE AND ACUTE
  {0xc7bc, 'A'},      // Ǽ	LATIN CAPITAL LETTER AE WITH ACUTE
  {0xc7bd, 'a'},      // ǽ	LATIN SMALL LETTER AE WITH ACUTE
  {0xc7be, 'O'},      // Ǿ	LATIN CAPITAL LETTER O WITH STROKE AND ACUTE
  {0xc7bf, 'o'},      // ǿ	LATIN SMALL LETTER O WITH STROKE AND ACUTE
  {0xc880, 'A'},      // Ȁ	LATIN CAPITAL LETTER A WITH DOUBLE GRAVE
  {0xc881, 'a'},      // ȁ	LATIN SMALL LETTER A WITH DOUBLE GRAVE
  {0xc882, 'A'},      // Ȃ	LATIN CAPITAL LETTER A WITH INVERTED BREVE
  {0xc883, 'a'},      // ȃ	LATIN SMALL LETTER A WITH INVERTED BREVE
  {0xc884, 'E'},      // Ȅ	LATIN CAPITAL LETTER E WITH DOUBLE GRAVE
  {0xc885, 'e'},      // ȅ	LATIN SMALL LETTER E WITH DOUBLE GRAVE
  {0xc886, 'E'},      // Ȇ	LATIN CAPITAL LETTER E WITH INVERTED BREVE
  {0xc887, 'e'},      // ȇ	LATIN SMALL LETTER E WITH INVERTED BREVE
  {0xc888, 'I'},      // Ȉ	LATIN CAPITAL LETTER I WITH DOUBLE GRAVE
  {0xc889, 'i'},      // ȉ	LATIN SMALL LETTER I WITH DOUBLE GRAVE
  {0xc88a, 'I'},      // Ȋ	LATIN CAPITAL LETTER I WITH INVERTED BREVE
  {0xc88b, 'i'},      // ȋ	LATIN SMALL LETTER I WITH INVERTED BREVE
  {0xc88c, 'O'},      // Ȍ	LATIN CAPITAL LETTER O WITH DOUBLE GRAVE
  {0xc88d, 'o'},      // ȍ	LATIN SMALL LETTER O WITH DOUBLE GRAVE
  {0xc88e, 'O'},      // Ȏ	LATIN CAPITAL LETTER O WITH INVERTED BREVE
  {0xc88f, 'o'},      // ȏ	LATIN SMALL LETTER O WITH INVERTED BREVE
  {0xc890, 'R'},      // Ȑ	LATIN CAPITAL LETTER R WITH DOUBLE GRAVE
  {0xc891, 'r'},      // ȑ	LATIN SMALL LETTER R WITH DOUBLE GRAVE
  {0xc892, 'R'},      // Ȓ	LATIN CAPITAL LETTER R WITH INVERTED BREVE
  {0xc893, 'r'},      // ȓ	LATIN SMALL LETTER R WITH INVERTED BREVE
  {0xc894, 'U'},      // Ȕ	LATIN CAPITAL LETTER U WITH DOUBLE GRAVE
  {0xc895, 'u'},      // ȕ	LATIN SMALL LETTER U WITH DOUBLE GRAVE
  {0xc896, 'U'},      // Ȗ	LATIN CAPITAL LETTER U WITH INVERTED BREVE
  {0xc897, 'u'},      // ȗ	LATIN SMALL LETTER U WITH INVERTED BREVE
  {0xc898, 'S'},      // Ș	LATIN CAPITAL LETTER S WITH COMMA BELOW
  {0xc899, 's'},      // ș	LATIN SMALL LETTER S WITH COMMA BELOW
  {0xc89a, 'T'},      // Ț	LATIN CAPITAL LETTER T WITH COMMA BELOW
  {0xc89b, 't'},      // ț	LATIN SMALL LETTER T WITH COMMA BELOW
  //{0xc89c, ''},      // Ȝ	LATIN CAPITAL LETTER YOGH
  //{0xc89d, ''},      // ȝ	LATIN SMALL LETTER YOGH
  {0xc89e, 'H'},      // Ȟ	LATIN CAPITAL LETTER H WITH CARON
  {0xc89f, 'h'},      // ȟ	LATIN SMALL LETTER H WITH CARON
  {0xc8a0, 'N'},      // Ƞ	LATIN CAPITAL LETTER N WITH LONG RIGHT LEG
  {0xc8a1, 'd'},      // ȡ	LATIN SMALL LETTER D WITH CURL
  {0xc8a2, 'O'},      // Ȣ	LATIN CAPITAL LETTER OU
  {0xc8a3, 'o'},      // ȣ	LATIN SMALL LETTER OU
  {0xc8a4, 'Z'},      // Ȥ	LATIN CAPITAL LETTER Z WITH HOOK
  {0xc8a5, 'z'},      // ȥ	LATIN SMALL LETTER Z WITH HOOK
  {0xc8a6, 'A'},      // Ȧ	LATIN CAPITAL LETTER A WITH DOT ABOVE
  {0xc8a7, 'a'},      // ȧ	LATIN SMALL LETTER A WITH DOT ABOVE
  {0xc8a8, 'E'},      // Ȩ	LATIN CAPITAL LETTER E WITH CEDILLA
  {0xc8a9, 'e'},      // ȩ	LATIN SMALL LETTER E WITH CEDILLA
  {0xc8aa, 'O'},      // Ȫ	LATIN CAPITAL LETTER O WITH DIAERESIS AND MACRON
  {0xc8ab, 'o'},      // ȫ	LATIN SMALL LETTER O WITH DIAERESIS AND MACRON
  {0xc8ac, 'O'},      // Ȭ	LATIN CAPITAL LETTER O WITH TILDE AND MACRON
  {0xc8ad, 'o'},      // ȭ	LATIN SMALL LETTER O WITH TILDE AND MACRON
  {0xc8ae, 'O'},      // Ȯ	LATIN CAPITAL LETTER O WITH DOT ABOVE
  {0xc8af, 'o'},      // ȯ	LATIN SMALL LETTER O WITH DOT ABOVE
  {0xc8b0, 'O'},      // Ȱ	LATIN CAPITAL LETTER O WITH DOT ABOVE AND MACRON
  {0xc8b1, 'o'},      // ȱ	LATIN SMALL LETTER O WITH DOT ABOVE AND MACRON
  {0xc8b2, 'y'},      // Ȳ	LATIN CAPITAL LETTER Y WITH MACRON
  {0xc8b3, 'y'},      // ȳ	LATIN SMALL LETTER Y WITH MACRON
  {0xc8b4, 'l'},      // ȴ	LATIN SMALL LETTER L WITH CURL
  {0xc8b5, 'n'},      // ȵ	LATIN SMALL LETTER N WITH CURL
  {0xc8b6, 't'},      // ȶ	LATIN SMALL LETTER T WITH CURL
  {0xc8b7, 'j'},      // ȷ	LATIN SMALL LETTER DOTLESS J
  //{0xc8b8, ''},      // ȸ	LATIN SMALL LETTER DB DIGRAPH
  //{0xc8b9, ''},      // ȹ	LATIN SMALL LETTER QP DIGRAPH
  {0xc8ba, 'A'},      // Ⱥ	LATIN CAPITAL LETTER A WITH STROKE
  {0xc8bb, 'C'},      // Ȼ	LATIN CAPITAL LETTER C WITH STROKE
  {0xc8bc, 'c'},      // ȼ	LATIN SMALL LETTER C WITH STROKE
  {0xc8bd, 'L'},      // Ƚ	LATIN CAPITAL LETTER L WITH BAR
  {0xc8be, 'T'},      // Ⱦ	LATIN CAPITAL LETTER T WITH DIAGONAL STROKE
  {0xc8bf, 's'},      // ȿ	LATIN SMALL LETTER S WITH SWASH TAIL
  {0xc980, 'z'},      // ɀ	LATIN SMALL LETTER Z WITH SWASH TAIL
  //{0xc981, ''},      // Ɂ	LATIN CAPITAL LETTER GLOTTAL STOP
  //{0xc982, ''},      // ɂ	LATIN SMALL LETTER GLOTTAL STOP
  {0xc983, 'B'},      // Ƀ	LATIN CAPITAL LETTER B WITH STROKE
  {0xc984, 'U'},      // Ʉ	LATIN CAPITAL LETTER U BAR
  {0xc985, 'V'},      // Ʌ	LATIN CAPITAL LETTER TURNED V
  {0xc986, 'E'},      // Ɇ	LATIN CAPITAL LETTER E WITH STROKE
  {0xc987, 'e'},      // ɇ	LATIN SMALL LETTER E WITH STROKE
  {0xc988, 'J'},      // Ɉ	LATIN CAPITAL LETTER J WITH STROKE
  {0xc989, 'j'},      // ɉ	LATIN SMALL LETTER J WITH STROKE
  {0xc98a, 'Q'},      // Ɋ	LATIN CAPITAL LETTER SMALL Q WITH HOOK TAIL
  {0xc98b, 'q'},      // ɋ	LATIN SMALL LETTER Q WITH HOOK TAIL
  {0xc98c, 'R'},      // Ɍ	LATIN CAPITAL LETTER R WITH STROKE
  {0xc98d, 'r'},      // ɍ	LATIN SMALL LETTER R WITH STROKE
  {0xc98e, 'Y'},      // Ɏ	LATIN CAPITAL LETTER Y WITH STROKE
  {0xc98f, 'y'},      // ɏ	LATIN SMALL LETTER Y WITH STROKE
  {0xc990, 'a'},      // ɐ	LATIN SMALL LETTER TURNED A
  //{0xc991, ''},      // ɑ	LATIN SMALL LETTER ALPHA
  //{0xc992, ''},      // ɒ	LATIN SMALL LETTER TURNED ALPHA
  {0xc993, 'b'},      // ɓ	LATIN SMALL LETTER B WITH HOOK
  {0xc994, 'o'},      // ɔ	LATIN SMALL LETTER OPEN O
  {0xc995, 'c'},      // ɕ	LATIN SMALL LETTER C WITH CURL
  {0xc996, 'd'},      // ɖ	LATIN SMALL LETTER D WITH TAIL
  {0xc997, 'd'},      // ɗ	LATIN SMALL LETTER D WITH HOOK
  {0xc998, 'e'},      // ɘ	LATIN SMALL LETTER REVERSED E
  //{0xc999, ''},      // ə	LATIN SMALL LETTER SCHWA
  //{0xc99a, ''},      // ɚ	LATIN SMALL LETTER SCHWA WITH HOOK
  {0xc99b, 'e'},      // ɛ	LATIN SMALL LETTER OPEN E
  {0xc99c, 'e'},      // ɜ	LATIN SMALL LETTER REVERSED OPEN E
  {0xc99d, 'e'},      // ɝ	LATIN SMALL LETTER REVERSED OPEN E WITH HOOK
  {0xc99e, 'e'},      // ɞ	LATIN SMALL LETTER CLOSED REVERSED OPEN E
  {0xc99f, 'j'},      // ɟ	LATIN SMALL LETTER DOTLESS J WITH STROKE
  {0xc9a0, 'g'},      // ɠ	LATIN SMALL LETTER G WITH HOOK
  {0xc9a1, 'g'},      // ɡ	LATIN SMALL LETTER SCRIPT G
  {0xc9a2, 'G'},      // ɢ	LATIN LETTER SMALL CAPITAL G
  //{0xc9a3, ''},      // ɣ	LATIN SMALL LETTER GAMMA
  //{0xc9a4, ''},      // ɤ	LATIN SMALL LETTER RAMS HORN
  {0xc9a5, 'h'},      // ɥ	LATIN SMALL LETTER TURNED H
  {0xc9a6, 'h'},      // ɦ	LATIN SMALL LETTER H WITH HOOK
  //{0xc9a7, ''},      // ɧ	LATIN SMALL LETTER HENG WITH HOOK
  {0xc9a8, 'i'},      // ɨ	LATIN SMALL LETTER I WITH STROKE
  //{0xc9a9, ''},      // ɩ	LATIN SMALL LETTER IOTA
  {0xc9aa, 'i'},      // ɪ	LATIN LETTER SMALL CAPITAL I
  {0xc9ab, 'l'},      // ɫ	LATIN SMALL LETTER L WITH MIDDLE TILDE
  {0xc9ac, 'l'},      // ɬ	LATIN SMALL LETTER L WITH BELT
  {0xc9ad, 'l'},      // ɭ	LATIN SMALL LETTER L WITH RETROFLEX HOOK
  //{0xc9ae, ''},      // ɮ	LATIN SMALL LETTER LEZH
  {0xc9af, 'm'},      // ɯ	LATIN SMALL LETTER TURNED M
  {0xc9b0, 'm'},      // ɰ	LATIN SMALL LETTER TURNED M WITH LONG LEG
  {0xc9b1, 'm'},      // ɱ	LATIN SMALL LETTER M WITH HOOK
  {0xc9b2, 'n'},      // ɲ	LATIN SMALL LETTER N WITH LEFT HOOK
  {0xc9b3, 'n'},      // ɳ	LATIN SMALL LETTER N WITH RETROFLEX HOOK
  {0xc9b4, 'n'},      // ɴ	LATIN LETTER SMALL CAPITAL N
  {0xc9b5, 'o'},      // ɵ	LATIN SMALL LETTER BARRED O
  {0xc9b6, 'o'},      // ɶ	LATIN LETTER SMALL CAPITAL OE
  {0xc9b7, 'o'},      // ɷ	LATIN SMALL LETTER CLOSED OMEGA
  //{0xc9b8, ''},      // ɸ	LATIN SMALL LETTER PHI
  {0xc9b9, 'r'},      // ɹ	LATIN SMALL LETTER TURNED R
  {0xc9ba, 'r'},      // ɺ	LATIN SMALL LETTER TURNED R WITH LONG LEG
  {0xc9bb, 'r'},      // ɻ	LATIN SMALL LETTER TURNED R WITH HOOK
  {0xc9bc, 'r'},      // ɼ	LATIN SMALL LETTER R WITH LONG LEG
  {0xc9bd, 'r'},      // ɽ	LATIN SMALL LETTER R WITH TAIL
  {0xc9be, 'r'},      // ɾ	LATIN SMALL LETTER R WITH FISHHOOK
  {0xc9bf, 'r'},      // ɿ	LATIN SMALL LETTER REVERSED R WITH FISHHOOK
  {0xca80, 'r'},      // ʀ	LATIN LETTER SMALL CAPITAL R
  {0xca81, 'R'},      // ʁ	LATIN LETTER SMALL CAPITAL INVERTED R
  {0xca82, 's'},      // ʂ	LATIN SMALL LETTER S WITH HOOK
  //{0xca83, ''},      // ʃ	LATIN SMALL LETTER ESH
  //{0xca84, ''},      // ʄ	LATIN SMALL LETTER DOTLESS J WITH STROKE AND HOOK
  //{0xca85, ''},      // ʅ	LATIN SMALL LETTER SQUAT REVERSED ESH
  //{0xca86, ''},      // ʆ	LATIN SMALL LETTER ESH WITH CURL
  {0xca87, 't'},      // ʇ	LATIN SMALL LETTER TURNED T
  {0xca88, 't'},      // ʈ	LATIN SMALL LETTER T WITH RETROFLEX HOOK
  {0xca89, 'u'},      // ʉ	LATIN SMALL LETTER U BAR
  {0xca8a, 'u'},      // ʊ	LATIN SMALL LETTER UPSILON
  {0xca8b, 'v'},      // ʋ	LATIN SMALL LETTER V WITH HOOK
  {0xca8c, 'v'},      // ʌ	LATIN SMALL LETTER TURNED V
  {0xca8d, 'w'},      // ʍ	LATIN SMALL LETTER TURNED W
  {0xca8e, 'y'},      // ʎ	LATIN SMALL LETTER TURNED Y
  {0xca8f, 'y'},      // ʏ	LATIN LETTER SMALL CAPITAL Y
  {0xca90, 'z'},      // ʐ	LATIN SMALL LETTER Z WITH RETROFLEX HOOK
  {0xca91, 'z'},      // ʑ	LATIN SMALL LETTER Z WITH CURL
  //{0xca92, ''},      // ʒ	LATIN SMALL LETTER EZH
  //{0xca93, ''},      // ʓ	LATIN SMALL LETTER EZH WITH CURL
  //{0xca94, ''},      // ʔ	LATIN LETTER GLOTTAL STOP
  //{0xca95, ''},      // ʕ	LATIN LETTER PHARYNGEAL VOICED FRICATIVE
  //{0xca96, ''},      // ʖ	LATIN LETTER INVERTED GLOTTAL STOP
  {0xca97, 'c'},      // ʗ	LATIN LETTER STRETCHED C
  //{0xca98, ''},      // ʘ	LATIN LETTER BILABIAL CLICK
  {0xca99, 'b'},      // ʙ	LATIN LETTER SMALL CAPITAL B
  {0xca9a, 'e'},      // ʚ	LATIN SMALL LETTER CLOSED OPEN E
  {0xca9b, 'g'},      // ʛ	LATIN LETTER SMALL CAPITAL G WITH HOOK
  {0xca9c, 'h'},      // ʜ	LATIN LETTER SMALL CAPITAL H
  {0xca9d, 'j'},      // ʝ	LATIN SMALL LETTER J WITH CROSSED-TAIL
  {0xca9e, 'k'},      // ʞ	LATIN SMALL LETTER TURNED K
  {0xca9f, 'l'},      // ʟ	LATIN LETTER SMALL CAPITAL L
  {0xcaa0, 'q'},      // ʠ	LATIN SMALL LETTER Q WITH HOOK
  //{0xcaa1, ''},      // ʡ	LATIN LETTER GLOTTAL STOP WITH STROKE
  //{0xcaa2, ''},      // ʢ	LATIN LETTER REVERSED GLOTTAL STOP WITH STROKE
  {0xcaa3, 'd'},      // ʣ	LATIN SMALL LETTER DZ DIGRAPH
  {0xcaa4, 'd'},      // ʤ	LATIN SMALL LETTER DEZH DIGRAPH
  {0xcaa5, 'd'},      // ʥ	LATIN SMALL LETTER DZ DIGRAPH WITH CURL
  {0xcaa6, 't'},      // ʦ	LATIN SMALL LETTER TS DIGRAPH
  {0xcaa7, 't'},      // ʧ	LATIN SMALL LETTER TESH DIGRAPH
  {0xcaa8, 't'},      // ʨ	LATIN SMALL LETTER TC DIGRAPH WITH CURL
  {0xcaa9, 'f'},      // ʩ	LATIN SMALL LETTER FENG DIGRAPH
  {0xcaaa, 'l'},      // ʪ	LATIN SMALL LETTER LS DIGRAPH
  {0xcaab, 'l'},      // ʫ	LATIN SMALL LETTER LZ DIGRAPH
  //{0xcaac, ''},      // ʬ	LATIN LETTER BILABIAL PERCUSSIVE
  //{0xcaad, ''},      // ʭ	LATIN LETTER BIDENTAL PERCUSSIVE
  {0xcaae, 'h'},      // ʮ	LATIN SMALL LETTER TURNED H WITH FISHHOOK
  {0xcaaf, 'h'},      // ʯ	LATIN SMALL LETTER TURNED H WITH FISHHOOK AND TAIL
  {0xcb96, '+'},      // ˖	MODIFIER LETTER PLUS SIGN
  {0xcb97, '-'},      // ˗	MODIFIER LETTER MINUS SIGN
  {0xcba1, 'l'},      // ˡ	MODIFIER LETTER SMALL L
  {0xcba2, 's'},      // ˢ	MODIFIER LETTER SMALL S
  {0xcba3, 'x'},      // ˣ	MODIFIER LETTER SMALL X
  {0xcda3, 'a'},      // ͣ	COMBINING LATIN SMALL LETTER A
  {0xcda4, 'e'},      // ͤ	COMBINING LATIN SMALL LETTER E
  {0xcda5, 'i'},      // ͥ	COMBINING LATIN SMALL LETTER I
  {0xcda6, 'o'},      // ͦ	COMBINING LATIN SMALL LETTER O
  {0xcda7, 'u'},      // ͧ	COMBINING LATIN SMALL LETTER U
  {0xcda8, 'c'},      // ͨ	COMBINING LATIN SMALL LETTER C
  {0xcda9, 'd'},      // ͩ	COMBINING LATIN SMALL LETTER D
  {0xcdaa, 'h'},      // ͪ	COMBINING LATIN SMALL LETTER H
  {0xcdab, 'm'},      // ͫ	COMBINING LATIN SMALL LETTER M
  {0xcdac, 'r'},      // ͬ	COMBINING LATIN SMALL LETTER R
  {0xcdad, 't'},      // ͭ	COMBINING LATIN SMALL LETTER T
  {0xcdae, 'v'},      // ͮ	COMBINING LATIN SMALL LETTER V
  {0xcdaf, 'x'},      // ͯ	COMBINING LATIN SMALL LETTER X
};

// West Germanic - gmw
// de en nl (sco fy)
const Mapping lang_gmw[] PROGMEM = {
  {0xC384, 'A'},      // Ä   // large Umlauts may be overwritten in the write method
  {0xc386, 'A'},      // Æ	LATIN CAPITAL LETTER AE
  {0xc384, 'A'},      // Ä	LATIN CAPITAL LETTER A WITH DIAERESIS
  {0xc38b, 'E'},      // Ë	LATIN CAPITAL LETTER E WITH DIAERESIS
  {0xc38f, 'I'},      // Ï	LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc396, 'O'},      // Ö	LATIN CAPITAL LETTER O WITH DIAERESIS
  {0xc39c, 'U'},      // Ü	LATIN CAPITAL LETTER U WITH DIAERESIS 
  {0xC39F, 's'},      // ß 
  {0xc3a4, 'a'},      // ä	LATIN SMALL LETTER A WITH DIAERESIS
  {0xc3a6, 'a'},      // æ	LATIN SMALL LETTER AE
  {0xc3af, 'i'},      // ï	LATIN SMALL LETTER I WITH DIAERESIS
  {0xc3ab, 'e'},      // ë	LATIN SMALL LETTER E WITH DIAERESIS
  {0xc3b6, 'o'},      // ö	LATIN SMALL LETTER O WITH DIAERESIS
  {0xc3bc, 'u'},      // ü	LATIN SMALL LETTER U WITH DIAERESIS
  {0xc4b2, 'I'},      // Ĳ	LATIN CAPITAL LIGATURE IJ  - somehow - J should be added for Dutch
  {0xc4b3, 'i'}       // ĳ	LATIN SMALL LIGATURE IJ - somehow - J should be added for Dutch
};

// North Germanic languages gmq
const Mapping lang_gmq[] PROGMEM = {
  {0xc386, 'A'},      // Æ	LATIN CAPITAL LETTER AE
  {0xc384, 'A'},      // Ä	LATIN CAPITAL LETTER A WITH DIAERESIS
  {0xc38b, 'E'},      // Ë	LATIN CAPITAL LETTER E WITH DIAERESIS
  {0xc38f, 'I'},      // Ï	LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc396, 'O'},      // Ö	LATIN CAPITAL LETTER O WITH DIAERESIS
  {0xc39c, 'U'},      // Ü	LATIN CAPITAL LETTER U WITH DIAERESIS  
  {0xc3a6, 'a'},      // æ	LATIN SMALL LETTER AE
  {0xc3a4, 'a'},      // ä	LATIN SMALL LETTER A WITH DIAERESIS
  {0xc3ab, 'e'},      // ë	LATIN SMALL LETTER E WITH DIAERESIS
  {0xc3af, 'i'},      // ï	LATIN SMALL LETTER I WITH DIAERESIS
  {0xc3b6, 'o'},      // ö	LATIN SMALL LETTER O WITH DIAERESIS
  {0xc3bc, 'u'},      // ü	LATIN SMALL LETTER U WITH DIAERESIS
  {0xc385, 'A'},      // Å  LATIN CAPITAL LETTER A WITH RING ABOVE
  {0xc392, 'O'},      // Ò	LATIN CAPITAL LETTER O WITH GRAVE
  {0xc398, 'O'},      // Ø	LATIN CAPITAL LETTER O WITH STROKE    
  {0xc3a5, 'a'},      // å  LATIN SMALL LETTER A WITH RING ABOVE
  {0xc3b2, 'o'},      // ò	LATIN SMALL LETTER O WITH GRAVE
  {0xc3b8, 'o'}       // ø	LATIN SMALL LETTER O WITH STROKE
};

// Romance - roa
// https://en.wikipedia.org/wiki/Romance_languages
// ca es et fr it pt ro
const Mapping lang_roa[] PROGMEM = {
  {0xc380, 'A'},      // À	LATIN CAPITAL LETTER A WITH GRAVE
  {0xc381, 'A'},      // Á	LATIN CAPITAL LETTER A WITH ACUTE
  {0xc383, 'A'},      // Ã	LATIN CAPITAL LETTER A WITH TILDE
  {0xc382, 'A'},      // Â	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
  {0xc386, 'A'},      // Æ	LATIN CAPITAL LETTER AE 
  {0xc387, 'C'},      // Ç	LATIN CAPITAL LETTER C WITH CEDILLA
  {0xc388, 'E'},      // È	LATIN CAPITAL LETTER E WITH GRAVE
  {0xc389, 'E'},      // É	LATIN CAPITAL LETTER E WITH ACUTE  
  {0xc38a, 'E'},      // Ê	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
  {0xc38b, 'E'},      // Ë	LATIN CAPITAL LETTER E WITH DIAERESIS
  {0xc38c, 'I'},      // Ì	LATIN CAPITAL LETTER I WITH GRAVE  
  {0xc38d, 'I'},      // Í	LATIN CAPITAL LETTER I WITH ACUTE  
  {0xc38e, 'I'},      // Î	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
  {0xc38f, 'I'},      // Ï	LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc391, 'N'},      // Ñ	LATIN CAPITAL LETTER N WITH TILDE - for Spanish words
  {0xc392, 'O'},      // Ò	LATIN CAPITAL LETTER O WITH GRAVE
  {0xc393, 'O'},      // Ó	LATIN CAPITAL LETTER O WITH ACUTE 
  {0xc394, 'O'},      // Ô	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
  {0xc395, 'O'},      // Õ	LATIN CAPITAL LETTER O WITH TILDE
  {0xc39a, 'U'},      // Ú	LATIN CAPITAL LETTER U WITH ACUTE  
  {0xc39b, 'U'},      // Û	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
  {0xc39c, 'U'},      // Ü	LATIN CAPITAL LETTER U WITH DIAERESIS  
  {0xc399, 'U'},      // Ù	LATIN CAPITAL LETTER U WITH GRAVE
  {0xc5b8, 'Y'},      // Ÿ	LATIN CAPITAL LETTER Y WITH DIAERESIS
  {0xc3a0, 'a'},      // à	LATIN SMALL LETTER A WITH GRAVE
  {0xc3a1, 'a'},      // á	LATIN SMALL LETTER A WITH ACUTE 
  {0xc3a2, 'a'},      // â	LATIN SMALL LETTER A WITH CIRCUMFLEX
  {0xc3a3, 'a'},      // ã	LATIN SMALL LETTER A WITH TILDE  
  {0xc3a6, 'a'},      // æ	LATIN SMALL LETTER AE 
  {0xc3a7, 'c'},      // ç	LATIN SMALL LETTER C WITH CEDILLA 
  {0xc3a8, 'e'},      // è	LATIN SMALL LETTER E WITH GRAVE
  {0xc3a9, 'e'},      // é	LATIN SMALL LETTER E WITH ACUTE   
  {0xc3aa, 'e'},      // ê	LATIN SMALL LETTER E WITH CIRCUMFLEX
  {0xc3ab, 'e'},      // ë	LATIN SMALL LETTER E WITH DIAERESIS
  {0xc3ac, 'i'},      // ì	LATIN SMALL LETTER I WITH GRAVE  
  {0xc3ad, 'i'},      // í	LATIN SMALL LETTER I WITH ACUTE
  {0xc3ae, 'i'},      // î	LATIN SMALL LETTER I WITH CIRCUMFLEX
  {0xc3af, 'i'},      // ï	LATIN SMALL LETTER I WITH DIAERESIS
  {0xc3b1, 'n'},      // ñ	LATIN SMALL LETTER N WITH TILDE
  {0xc3b2, 'o'},      // ò  LATIN SMALL LETTER O WITH GRAVE   
  {0xc3b3, 'o'},      // ó	LATIN SMALL LETTER O WITH ACUTE  
  {0xc3b4, 'o'},      // ô	LATIN SMALL LETTER O WITH CIRCUMFLEX
  {0xc3b5, 'o'},      // õ	LATIN SMALL LETTER O WITH TILDE  
  {0xc3ba, 'u'},      // ú	LATIN SMALL LETTER U WITH ACUTE   
  {0xc3bb, 'u'},      // û	LATIN SMALL LETTER U WITH CIRCUMFLEX
  {0xc3bc, 'u'},      // ü	LATIN SMALL LETTER U WITH DIAERESIS  
  {0xc3b9, 'u'},      // ù	LATIN SMALL LETTER U WITH GRAVE
  {0xc3bf, 'y'},      // ÿ	LATIN SMALL LETTER Y WITH DIAERESIS
  {0xc3a9, 'e'},      // é	LATIN SMALL LETTER E WITH ACUTE
  {0xc482, 'A'},      // Ă	LATIN CAPITAL LETTER A WITH BREVE
  {0xc483, 'a'},      // ă	LATIN SMALL LETTER A WITH BREVE  
  {0xc592, 'O'},      // Œ	LATIN CAPITAL LIGATURE OE
  {0xc593, 'o'},      // œ	LATIN SMALL LIGATURE OE
  {0xc898, 'S'},      // Ș	LATIN CAPITAL LETTER S WITH COMMA BELOW
  {0xc899, 's'},      // ș	LATIN SMALL LETTER S WITH COMMA BELOW
  {0xc89a, 'T'},      // Ț	LATIN CAPITAL LETTER T WITH COMMA BELOW
  {0xc89b, 't'},      // ț	LATIN SMALL LETTER T WITH COMMA BELOW 
  // for Spanish
  {0xc2a1, '!'},      // ¡	INVERTED EXCLAMATION MARK
  {0xc2aa, 'a'},      // ª	FEMININE ORDINAL INDICATOR
  {0xc2ba, 'o'},      // º	MASCULINE ORDINAL INDICATOR
  {0xc2bf, '?'}       // ¿  INVERTED QUESTION MARK
};

// German 
// not really used for the LCD
// as currently the HD44780 mapping also contains the German capital Umlauts
const Mapping lang_de[] PROGMEM =  {
  {0xC384, 'A'},      // Ä   // large Umlauts may be overwritten in the write method
  {0xC396, 'O'},      // Ö
  {0xC39C, 'U'},      // Ü
  {0xC39F, 's'},      // ß 
  {0xC3A4, 'a'},      // ä
  {0xC3B6, 'o'},      // ö
  {0xC3BC, 'u'}       // ü
};


// Slavic
// https://en.wikipedia.org/wiki/Slavic_languages
// cs pl sk sr
const Mapping lang_sla[] PROGMEM = {
// Czech cs
// https://en.wikipedia.org/wiki/Czech_orthography cs 42(!) Letters
  {0xc381, 'A'},      // Á	LATIN CAPITAL LETTER A WITH ACUTE
  {0xc389, 'E'},      // É	LATIN CAPITAL LETTER E WITH ACUTE
  {0xc38d, 'I'},      // Í	LATIN CAPITAL LETTER I WITH ACUTE
  {0xc393, 'O'},      // Ó	LATIN CAPITAL LETTER O WITH ACUTE
  {0xc39a, 'U'},      // Ú	LATIN CAPITAL LETTER U WITH ACUTE
  {0xc39d, 'Y'},      // Ý	LATIN CAPITAL LETTER Y WITH ACUTE   
  {0xc3a1, 'a'},      // á	LATIN SMALL LETTER A WITH ACUTE
  {0xc3a9, 'e'},      // é	LATIN SMALL LETTER E WITH ACUTE
  {0xc3ad, 'i'},      // í	LATIN SMALL LETTER I WITH ACUTE  
  {0xc3b3, 'o'},      // ó	LATIN SMALL LETTER O WITH ACUTE
  {0xc3ba, 'u'},      // ú	LATIN SMALL LETTER U WITH ACUTE
  {0xc3bd, 'y'},      // ý	LATIN SMALL LETTER Y WITH ACUTE   
  {0xc48c, 'C'},      // Č	LATIN CAPITAL LETTER C WITH CARON
  {0xc48d, 'c'},      // č	LATIN SMALL LETTER C WITH CARON
  {0xc48e, 'D'},      // Ď	LATIN CAPITAL LETTER D WITH CARON
  {0xc48f, 'd'},      // ď	LATIN SMALL LETTER D WITH CARON  
  {0xc49a, 'E'},      // Ě	LATIN CAPITAL LETTER E WITH CARON
  {0xc49b, 'e'},      // ě	LATIN SMALL LETTER E WITH CARON
  {0xc587, 'N'},      // Ň  LATIN CAPITAL LETTER N WITH CARON
  {0xc588, 'n'},      // ň  LATIN SMALL LETTER N WITH CARON
  {0xc598, 'R'},      // Ř	LATIN CAPITAL LETTER R WITH CARON
  {0xc599, 'r'},      // ř	LATIN SMALL LETTER R WITH CARON
  {0xc5a0, 'S'},      // Š	LATIN CAPITAL LETTER S WITH CARON
  {0xc5a1, 's'},      // š	LATIN SMALL LETTER S WITH CARON
  {0xc5a4, 'T'},      // Ť	LATIN CAPITAL LETTER T WITH CARON
  {0xc5a5, 't'},      // ť	LATIN SMALL LETTER T WITH CARON
  {0xc5ae, 'U'},      // Ů	LATIN CAPITAL LETTER U WITH RING ABOVE
  {0xc5af, 'u'},      // ů	LATIN SMALL LETTER U WITH RING ABOVE
  {0xc5bd, 'Z'},      // Ž	LATIN CAPITAL LETTER Z WITH CARON
  {0xc5be, 'z'},      // ž	LATIN SMALL LETTER Z WITH CARON
  // add on for Slovakian sk
  {0xc394, 'O'},      // Ô	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
  {0xc3b4, 'o'},      // ô	LATIN SMALL LETTER O WITH CIRCUMFLEX 
  {0xc4ba, 'l'},      // ĺ	LATIN SMALL LETTER L WITH ACUTE
  {0xc4b9, 'L'},      // Ĺ  LATIN CAPITAL LETTER L WITH ACUTE 
  {0xc594, 'R'},      // Ŕ	LATIN CAPITAL LETTER R WITH ACUTE
  {0xc595, 'r'},      // ŕ	LATIN SMALL LETTER R WITH ACUTE
  // add on for Polish pl
  // https://en.wikipedia.org/wiki/Polish_language#Orthography
  {0xc484, 'A'},      // Ą  LATIN CAPTIAL LETTER A WITH OGONEK
  {0xc485, 'a'},      // ą  LATIN SMALL LETTER A WITH OGONEK
  {0xc486, 'C'},      // Ć  LATIN CAPITAL LETTER C WITH ACUTE
  {0xc487, 'c'},      // ć  LATIN SMALL LETTER C WITH ACUTE
  {0xc498, 'E'},      // Ę  LATIN CAPITAL LETTER E WITH OGONEK
  {0xc499, 'e'},      // ę  LATIN SMALL LETTER E WITH OGONEK
  {0xc581, 'L'},      // Ł  LATIN CAPITAL LETTER L WITH STROKE
  {0xc582, 'l'},      // ł	LATIN SMALL LETTER L WITH STROKE
  {0xc583, 'N'},      // Ń  LATIN CAPITAL LETTER N WITH ACUTE
  {0xc584, 'n'},      // ń  LATIN SMALL LETTER N WITH ACUTE  
  {0xc59a, 'S'},      // Ś	LATIN CAPITAL LETTER S WITH ACUTE
  {0xc59b, 's'},      // ś  LATIN SMALL LETTER S WITH ACUTE
  {0xc5b9, 'Z'},      // Ź  LATIN CAPITAL LETTER Z WITH ACUTE
  {0xc5ba, 'z'},      // ź  LATIN SMALL LETTER Z WITH ACUTE
  {0xc5bb, 'Z'},      // Ż	LATIN CAPITAL LETTER Z WITH DOT ABOVE
  {0xc5bc, 'z'},      // ż	LATIN SMALL LETTER Z WITH DOT ABOVE    
  // Serbian sr
  // https://en.wikipedia.org/wiki/Serbian_language
  {0xc490, 'D'},      // Đ	LATIN CAPITAL LETTER D WITH STROKE
  {0xc491, 'd'},      // đ	LATIN SMALL LETTER D WITH STROKE
};


// Spanish es - and Catalan ca
// https://en.wikipedia.org/wiki/Spanish_orthography
const Mapping lang_es[] PROGMEM = {
  {0xc2a1, '!'},      // ¡	INVERTED EXCLAMATION MARK
  {0xc2aa, 'a'},      // ª	FEMININE ORDINAL INDICATOR
  {0xc2ba, 'o'},      // º	MASCULINE ORDINAL INDICATOR
  {0xc2bf, '?'},      // ¿  INVERTED QUESTION MARK
  {0xc381, 'A'},      // Á	LATIN CAPITAL LETTER A WITH ACUTE
  {0xc387, 'C'},      // Ç	LATIN CAPITAL LETTER C WITH CEDILLA
  {0xc389, 'E'},      // É	LATIN CAPITAL LETTER E WITH ACUTE
  {0xc38d, 'I'},      // Í	LATIN CAPITAL LETTER I WITH ACUTE
  {0xc391, 'N'},      // Ñ	LATIN CAPITAL LETTER N WITH TILDE
  {0xc393, 'O'},      // Ó	LATIN CAPITAL LETTER O WITH ACUTE
  {0xc39a, 'U'},      // Ú	LATIN CAPITAL LETTER U WITH ACUTE
  {0xc39c, 'U'},      // Ü	LATIN CAPITAL LETTER U WITH DIAERESIS
  {0xc3a1, 'a'},      // á	LATIN SMALL LETTER A WITH ACUTE                
  {0xc3a9, 'e'},      // é	LATIN SMALL LETTER E WITH ACUTE                
  {0xc3ad, 'i'},      // í	LATIN SMALL LETTER I WITH ACUTE
  {0xc3b1, 'n'},      // ñ	LATIN SMALL LETTER N WITH TILDE
  {0xc3b3, 'o'},      // ó	LATIN SMALL LETTER O WITH ACUTE
  {0xc3ba, 'u'},      // ú	LATIN SMALL LETTER U WITH ACUTE                
  {0xc3bc, 'u'},      // ü	LATIN SMALL LETTER U WITH DIAERESIS                
  {0xc3a7, 'c'},      // ç	LATIN SMALL LETTER C WITH CEDILLA
  // add on for catalan
  {0xc380, 'A'},      // À	LATIN CAPITAL LETTER A WITH GRAVE
  {0xc388, 'E'},      // È	LATIN CAPITAL LETTER E WITH GRAVE  
  {0xc38f, 'I'},      // Ï	LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc392, 'O'},      // Ò	LATIN CAPITAL LETTER O WITH GRAVE
  {0xc3a0, 'a'},      // à	LATIN SMALL LETTER A WITH GRAVE
  {0xc3a8, 'e'},      // è  LATIN SMALL LETTER E WITH GRAVE
  {0xc3af, 'i'},      // ï	LATIN SMALL LETTER I WITH DIAERESIS  
  {0xc3b2, 'o'},      // ò  LATIN SMALL LETTER O WITH GRAVE  
};

// French - fr
// https://en.wikipedia.org/wiki/French_orthography
const Mapping lang_fr[] PROGMEM = {
  {0xc380, 'A'},      // À	LATIN CAPITAL LETTER A WITH GRAVE
  {0xc382, 'A'},      // Â	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
  {0xc386, 'A'},      // Æ	LATIN CAPITAL LETTER AE 
  {0xc387, 'C'},      // Ç	LATIN CAPITAL LETTER C WITH CEDILLA
  {0xc388, 'E'},      // È	LATIN CAPITAL LETTER E WITH GRAVE
  {0xc38a, 'E'},      // Ê	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
  {0xc38b, 'E'},      // Ë	LATIN CAPITAL LETTER E WITH DIAERESIS
  {0xc38e, 'I'},      // Î	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
  {0xc38f, 'I'},      // Ï	LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc391, 'N'},      // Ñ	LATIN CAPITAL LETTER N WITH TILDE - for Spanish words 
  {0xc394, 'O'},      // Ô	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
  {0xc39b, 'U'},      // Û	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
  {0xc39c, 'U'},      // Ü	LATIN CAPITAL LETTER U WITH DIAERESIS  
  {0xc399, 'U'},      // Ù	LATIN CAPITAL LETTER U WITH GRAVE
  {0xc389, 'E'},      // É	LATIN CAPITAL LETTER E WITH ACUTE
  {0xc5b8, 'Y'},      // Ÿ	LATIN CAPITAL LETTER Y WITH DIAERESIS
  {0xc3a0, 'a'},      // à	LATIN SMALL LETTER A WITH GRAVE
  {0xc3a2, 'a'},      // â	LATIN SMALL LETTER A WITH CIRCUMFLEX
  {0xc3a6, 'a'},      // æ	LATIN SMALL LETTER AE 
  {0xc3a7, 'c'},      // ç	LATIN SMALL LETTER C WITH CEDILLA 
  {0xc3a8, 'e'},      // è	LATIN SMALL LETTER E WITH GRAVE
  {0xc3aa, 'e'},      // ê	LATIN SMALL LETTER E WITH CIRCUMFLEX
  {0xc3ab, 'e'},      // ë	LATIN SMALL LETTER E WITH DIAERESIS
  {0xc3ae, 'i'},      // î	LATIN SMALL LETTER I WITH CIRCUMFLEX
  {0xc3af, 'i'},      // ï	LATIN SMALL LETTER I WITH DIAERESIS
  {0xc3b1, 'n'},      // ñ	LATIN SMALL LETTER N WITH TILDE - for Spanish words
  {0xc3b4, 'o'},      // ô	LATIN SMALL LETTER O WITH CIRCUMFLEX
  {0xc3bb, 'u'},      // û	LATIN SMALL LETTER U WITH CIRCUMFLEX
  {0xc3bc, 'u'},      // ü	LATIN SMALL LETTER U WITH DIAERESIS  
  {0xc3b9, 'u'},      // ù	LATIN SMALL LETTER U WITH GRAVE
  {0xc3bf, 'y'},      // ÿ	LATIN SMALL LETTER Y WITH DIAERESIS
  {0xc3a9, 'e'},      // é	LATIN SMALL LETTER E WITH ACUTE
  {0xc592, 'O'},      // Œ	LATIN CAPITAL LIGATURE OE
  {0xc593, 'o'}       // œ	LATIN SMALL LIGATURE OE
};

// english - en
// MISSING: to be checked if exchangable with north germanic
const Mapping lang_en[] PROGMEM = {
  {0xc386, 'A'},      // Æ	LATIN CAPITAL LETTER AE
  {0xc384, 'A'},      // Ä	LATIN CAPITAL LETTER A WITH DIAERESIS
  {0xc38b, 'E'},      // Ë	LATIN CAPITAL LETTER E WITH DIAERESIS
  {0xc38f, 'I'},      // Ï	LATIN CAPITAL LETTER I WITH DIAERESIS
  {0xc396, 'O'},      // Ö	LATIN CAPITAL LETTER O WITH DIAERESIS
  {0xc39c, 'U'},      // Ü	LATIN CAPITAL LETTER U WITH DIAERESIS  
  {0xc3a6, 'a'},      // æ	LATIN SMALL LETTER AE
  {0xc3a4, 'a'},      // ä	LATIN SMALL LETTER A WITH DIAERESIS
  {0xc3ab, 'e'},      // ë	LATIN SMALL LETTER E WITH DIAERESIS
  {0xc3af, 'i'},      // ï	LATIN SMALL LETTER I WITH DIAERESIS
  {0xc3b6, 'o'},      // ö	LATIN SMALL LETTER O WITH DIAERESIS
  {0xc3bc, 'u'}       // ü	LATIN SMALL LETTER U WITH DIAERESIS
};

// Finnish - fi
//
const Mapping lang_fi[] PROGMEM = {
  {0xc384, 'A'},      // Ä	LATIN CAPITAL LETTER A WITH DIAERESIS
  {0xc385, 'A'},      // Å  LATIN CAPITAL LETTER A WITH RING ABOVE
  {0xc396, 'O'},      // Ö	LATIN CAPITAL LETTER O WITH DIAERESIS
  {0xc3a4, 'a'},      // ä	LATIN SMALL LETTER A WITH DIAERESIS  
  {0xc3a5, 'a'},      // å  LATIN SMALL LETTER A WITH RING ABOVE
  {0xc3b6, 'o'},      // ö	LATIN SMALL LETTER O WITH DIAERESIS  
  {0xc5a0, 'S'},      // Š	LATIN CAPITAL LETTER S WITH CARON
  {0xc5a1, 's'},      // š	LATIN SMALL LETTER S WITH CARON  
  {0xc5bd, 'Z'},      // Ž	LATIN CAPITAL LETTER Z WITH CARON
  {0xc5be, 'z'}       // ž	LATIN SMALL LETTER Z WITH CARON 
};


// Hungarian - hu
// https://en.wikipedia.org/wiki/Hungarian_alphabet
const Mapping lang_hu[] PROGMEM ={
  {0xc381, 'A'},      // Á	LATIN CAPITAL LETTER A WITH ACUTE
  {0xc389, 'E'},      // É	LATIN CAPITAL LETTER E WITH ACUTE
  {0xc393, 'O'},      // Ó	LATIN CAPITAL LETTER O WITH ACUTE
  {0xc396, 'O'},      // Ö	LATIN CAPITAL LETTER O WITH DIAERESIS
  {0xc39a, 'U'},      // Ú	LATIN CAPITAL LETTER U WITH ACUTE
  {0xc39c, 'U'},      // Ü	LATIN CAPITAL LETTER U WITH DIAERESIS
  {0xc3a1, 'a'},      // á	LATIN SMALL LETTER A WITH ACUTE            
  {0xc3a9, 'e'},      // é	LATIN SMALL LETTER E WITH ACUTE           
  {0xc3b3, 'o'},      // ó	LATIN SMALL LETTER O WITH ACUTE
  {0xc3b6, 'o'},      // ö	LATIN SMALL LETTER O WITH DIAERESIS 
  {0xc3ba, 'u'},      // ú	LATIN SMALL LETTER U WITH ACUTE           
  {0xc3bc, 'u'},      // ü	LATIN SMALL LETTER U WITH DIAERESIS 
  {0xc590, 'O'},      // Ő	LATIN CAPITAL LETTER O WITH DOUBLE ACUTE
  {0xc591, 'o'},      // ő	LATIN SMALL LETTER O WITH DOUBLE ACUTE            
  {0xc5b0, 'U'},      // Ű	LATIN CAPITAL LETTER U WITH DOUBLE ACUTE
  {0xc5b1, 'u'}       // ű	LATIN SMALL LETTER U WITH DOUBLE ACUTE   
};

// Cyrillic
// transliteration https://en.wikipedia.org/wiki/Romanization_of_Russian
// GOST 16876-71(1) because of single character
// smaller subset
const Mapping cyrillic_GOST16876_71_1[] PROGMEM = {
  {0xd081, 'E'},  // Ё  CYRILLIC CAPITAL LETTER IO
  {0xd090, 'A'},  // А  CYRILLIC CAPITAL LETTER A
  {0xd091, 'B'},  // Б  CYRILLIC CAPITAL LETTER BE
  {0xd092, 'V'},  // В  CYRILLIC CAPITAL LETTER VE
  {0xd093, 'G'},  // Г  CYRILLIC CAPITAL LETTER GHE
  {0xd094, 'D'},  // Д  CYRILLIC CAPITAL LETTER DE
  {0xd095, 'E'},  // Е  CYRILLIC CAPITAL LETTER IE
  {0xd096, 'Z'},  // Ж  CYRILLIC CAPITAL LETTER ZHE
  {0xd097, 'Z'},  // З  CYRILLIC CAPITAL LETTER ZE
  {0xd098, 'I'},  // И  CYRILLIC CAPITAL LETTER I
  {0xd099, 'J'},  // Й  CYRILLIC CAPITAL LETTER SHORT I
  {0xd09a, 'K'},  // К  CYRILLIC CAPITAL LETTER KA
  {0xd09b, 'L'},  // Л  CYRILLIC CAPITAL LETTER EL
  {0xd09c, 'M'},  // М  CYRILLIC CAPITAL LETTER EM
  {0xd09d, 'N'},  // Н  CYRILLIC CAPITAL LETTER EN
  {0xd09e, 'O'},  // О  CYRILLIC CAPITAL LETTER O
  {0xd09f, 'P'},  // П  CYRILLIC CAPITAL LETTER PE
  {0xd0a0, 'R'},  // Р  CYRILLIC CAPITAL LETTER ER
  {0xd0a1, 'S'},  // С  CYRILLIC CAPITAL LETTER ES
  {0xd0a2, 'T'},  // Т  CYRILLIC CAPITAL LETTER TE
  {0xd0a3, 'U'},  // У  CYRILLIC CAPITAL LETTER U
  {0xd0a4, 'F'},  // Ф  CYRILLIC CAPITAL LETTER EF
  {0xd0a5, 'H'},  // Х  CYRILLIC CAPITAL LETTER HA
  {0xd0a6, 'C'},  // Ц  CYRILLIC CAPITAL LETTER TSE
  {0xd0a7, 'C'},  // Ч  CYRILLIC CAPITAL LETTER CHE
  {0xd0a8, 'S'},  // Ш  CYRILLIC CAPITAL LETTER SHA
  {0xd0a9, 'S'},  // Щ  CYRILLIC CAPITAL LETTER SHCHA
  {0xd0aa, '"'},  // Ъ  CYRILLIC CAPITAL LETTER HARD SIGN
  {0xd0ab, 'Y'},  // Ы  CYRILLIC CAPITAL LETTER YERU
  {0xd0ac, '\''}, // Ь  CYRILLIC CAPITAL LETTER SOFT SIGN
  {0xd0ad, 'E'},  // Э  CYRILLIC CAPITAL LETTER E
  {0xd0ae, 'J'},  // Ю  CYRILLIC CAPITAL LETTER YU - JU
  {0xd0af, 'J'},  // Я  CYRILLIC CAPITAL LETTER YA  - JA
  {0xd0b0, 'a'},  // а  CYRILLIC SMALL LETTER A
  {0xd0b1, 'b'},  // б  CYRILLIC SMALL LETTER BE
  {0xd0b2, 'v'},  // в  CYRILLIC SMALL LETTER VE
  {0xd0b3, 'g'},  // г  CYRILLIC SMALL LETTER GHE
  {0xd0b4, 'd'},  // д  CYRILLIC SMALL LETTER DE
  {0xd0b5, 'e'},  // е  CYRILLIC SMALL LETTER IE
  {0xd0b6, 'z'},  // ж  CYRILLIC SMALL LETTER ZHE
  {0xd0b7, 'z'},  // з  CYRILLIC SMALL LETTER ZE
  {0xd0b8, 'z'},  // и  CYRILLIC SMALL LETTER I
  {0xd0b9, 'j'},  // й  CYRILLIC SMALL LETTER SHORT I
  {0xd0ba, 'k'},  // к  CYRILLIC SMALL LETTER KA
  {0xd0bb, 'l'},  // л  CYRILLIC SMALL LETTER EL
  {0xd0bc, 'm'},  // м  CYRILLIC SMALL LETTER EM
  {0xd0bd, 'n'},  // н  CYRILLIC SMALL LETTER EN
  {0xd0be, 'o'},  // о  CYRILLIC SMALL LETTER O
  {0xd0bf, 'p'},  // п  CYRILLIC SMALL LETTER PE
  {0xd180, 'r'},  // р  CYRILLIC SMALL LETTER ER
  {0xd181, 's'},  // с  CYRILLIC SMALL LETTER ES
  {0xd182, 't'},  // т  CYRILLIC SMALL LETTER TE
  {0xd183, 'u'},  // у  CYRILLIC SMALL LETTER U
  {0xd184, 'f'},  // ф  CYRILLIC SMALL LETTER EF
  {0xd185, 'h'},  // х  CYRILLIC SMALL LETTER HA
  {0xd186, 'c'},  // ц  CYRILLIC SMALL LETTER TSE
  {0xd187, 'c'},  // ч  CYRILLIC SMALL LETTER CHE
  {0xd188, 's'},  // ш  CYRILLIC SMALL LETTER SHA
  {0xd189, 's'},  // щ  CYRILLIC SMALL LETTER SHCHA
  {0xd18a, '"'},  // ъ  CYRILLIC SMALL LETTER HARD SIGN
  {0xd18b, 'y'},  // ы  CYRILLIC SMALL LETTER YERU
  {0xd18c, '\''}, // ь  CYRILLIC SMALL LETTER SOFT SIGN
  {0xd18d, 'e'},  // э  CYRILLIC SMALL LETTER E
  {0xd18e, 'j'},  // ю  CYRILLIC SMALL LETTER YU
  {0xd18f, 'j'},  // я  CYRILLIC SMALL LETTER YA
  // Pre 1918 letters
  {0xd086, 'I'},  // І  CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I  
  {0xd190, 'i'},  // ѐ  CYRILLIC SMALL LETTER IE WITH GRAVE
  {0xd191, 'e'},  // ё  CYRILLIC SMALL LETTER IO
  {0xd192, 'e'},  // ђ  CYRILLIC SMALL LETTER DJE
  {0xd193, 'y'},  // ѓ  CYRILLIC SMALL LETTER GJE
  {0xd196, 'i'},  // і  CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
  {0xd197, 'i'},  // ї  CYRILLIC SMALL LETTER YI
  {0xd1a2, 'E'},  // Ѣ  CYRILLIC CAPITAL LETTER YAT
  {0xd1a3, 'e'},  // ѣ  CYRILLIC SMALL LETTER YAT  
  {0xd1b2, 'F'},  // Ѳ  CYRILLIC CAPITAL LETTER FITA
  {0xd1b3, 'f'},  // ѳ  CYRILLIC SMALL LETTER FITA  
  {0xd1b4, 'Y'},  // Ѵ  CYRILLIC CAPITAL LETTER IZHITSA
  {0xd1b5, 'y'},  // ѵ  CYRILLIC SMALL LETTER IZHITSA
};
#endif
