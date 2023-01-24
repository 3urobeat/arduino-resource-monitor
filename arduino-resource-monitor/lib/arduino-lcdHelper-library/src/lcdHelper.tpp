/*
 * File: lcdHelper.tpp
 * Project: arduino-lcdHelper-library
 * Created Date: 28.08.2022 22:55:04
 * Author: 3urobeat
 * 
 * Last Modified: 05.12.2022 21:55:08
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file. 
 */

// Template implementation file
// https://stackoverflow.com/a/495056


template <typename lcd>
void lcdHelper<lcd>::clearLine(uint8_t row)
{
    // Print _lcdCols amount of spaces to effectively clear the line
    this->setCursor(0, row);

    for (uint8_t i = 0; i < this->_lcdCols; i++) {
        this->print(" ");
    }
}

template <typename lcd>
void lcdHelper<lcd>::centerPrint(const char *str, uint8_t row, bool callClearLine)
{
    // clear the line first to avoid old characters corrupting the text when content is not the same
    if (callClearLine) this->clearLine(row);

    // Calculate column
    int offset = this->_lcdCols - this->utf8_strlen(str);
    if (offset < 0) offset = 0; //set offset to 0 if it would be negative

    this->setCursor(offset / 2, row); //center char array
    this->print(str);
}

template <typename lcd>
void lcdHelper<lcd>::movingPrint(const char *str, uint8_t *moveOffset, uint8_t width)
{
    // Check if we actually have to move something
    if (utf8_strlen(str) > width) {
        // Fix for UTF-8 chars: Increase moveOffset if a two byte long char will now leave the display to move it off completely and display the next char
        uint8_t char0Len = (*(str + *moveOffset) & 0xc0) != 0x80;
        if (char0Len == 0) (*moveOffset)++;

        // Reset if char array was fully displayed by checking if what is left over of the char array would not fill the whole width anymore
        if (utf8_strlen(str + *moveOffset) < width) *moveOffset = 0;

        // Print width amount of chars, starting from current moveOffset using our fancy limitedPrint function to correctly display two byte long chars (UTF-8)
        this->limitedPrint(str + *moveOffset, width);

        // Increase offset
        (*moveOffset)++;
    } else {
        this->print(str);
    }
}

template <typename lcd>
void lcdHelper<lcd>::animationPrint(const char **animationArray, uint8_t animationSize, uint8_t *animationFrame, uint8_t col, uint8_t row)
{
    // Print current frame and overwrite previous one
    this->setCursor(col, row);
    this->print(animationArray[*animationFrame]);

    // Increment index or reset if all frames were displayed
    (*animationFrame)++;
    
    if (*animationFrame > animationSize - 1) *animationFrame = 0;
}

template <typename lcd>
void lcdHelper<lcd>::alignedPrint(const char *align, const char *str, uint8_t width)
{
    // Workarounds to correctly display Umlaute
    size_t len = utf8_strlen(str); // length on the display
    size_t blen = strlen(str);     // actual length in mem

    char temp[width + (blen - len) + 1] = ""; // blen - len to correct deviation caused by 2 byte long chars (Umlaute)

    // check if we even have to do something
    if (len == width) {
        this->print(str); 
        return;
    }

    // check if char array is too long, cut it and display it as is
    if (len > width) {
        strncpy(temp, str, width);
        this->print(temp);

    } else { //if shorter, align text as requested

        // switch case doesn't work with char arrays so here we go
        if (strcmp(align, "left") == 0) {
            strcpy(temp, str);
            memset(temp + blen, ' ', width - len); // fill remaining space with spaces and keep existing null byte at the end

        } else if (strcmp(align, "center") == 0) {
            int offset = (width - len) / 2; // calculate offset to the left

            memset(temp, ' ', offset); // offset str with spaces
            strcat(temp, str);         // put str into the middle
            memset(temp + offset + blen, ' ', width - offset - len); // fill remaining space with spaces
            
        } else if (strcmp(align, "right") == 0) {
            memset(temp, ' ', width - len); // offset char array
            strcpy(temp + width - len, str);
        }

        this->print(temp);
    }
}

template <typename lcd> 
void lcdHelper<lcd>::limitedPrint(const char *str, uint8_t length)
{
    // Check if we actually have to do something
    if (this->utf8_strlen(str) > length) {
        uint8_t currentLen = 0; // UTF-8
        uint8_t index      = 0; // Actual location in the char arr
 
        // Print all chars until utf8_strlen() == length is reached
        while (currentLen < length) { // Check above guarantees we can't exceed
            uint8_t thisCharLen = (*(str + index + 1) & 0xc0) != 0x80; // Count only one byte of a 2 byte long char (from utf8_strlen())

            // If we've encountered a 2 byte long char, we need to copy this and the next byte and print as a whole
            if (thisCharLen == 0) {
                char temp[3] = ""; // Create 2 byte + null char long temp char arr // TODO: Can this be optimized to work without temp?

                strncpy(temp, str + index, 2); // Copy both bytes, then print
                this->print(temp);
                
                index += 2; // Count both bytes
            } else {
                this->print(*(str + index)); // Simply print this char
                
                index++; // Count this one char
            }

            currentLen++;
        }
    } else {
        this->print(str); // Print as is if str is shorter than length
    }
}

template <typename lcd>
size_t lcdHelper<lcd>::utf8_strlen(const char *str)
{
    size_t len = 0;
    while (*str) len += (*str++ & 0xc0) != 0x80;
    return len;
}

template <typename lcd>
char* lcdHelper<lcd>::toFixedLengthNumber(char *dest, int num, uint8_t len)
{
    char numStr[len + 1] = ""; // no real way to check how long num will be so let's just use len as it will always be >=

    itoa(num, numStr, 10); // convert int and save into numStr

    memset(dest, '0', len - strlen(numStr)); // set all chars except the ones used by numStr to 0 (not \0 kek)
    strcpy(dest + (len - strlen(numStr)), numStr); // finally add the number itself to the end (use strcpy instead of strcat to make sure numStr ends up at the correct place)

    return dest; // return pointer to dest again to make using the function inside another call easier
}