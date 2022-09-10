/*
 * MIT License
 *
 * Copyright (c) 2022 Sergey Kostyanoy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "port.h"
#include <stdio.h>

const char __HexDump0[] PROGMEM = "|  %s \n";
const char __HexDump1[] PROGMEM = "%02X ";

void Print_Space_N(const uint8_t size) {
    for (size_t x = 0; x < size; ++x) {
        printf_P(PSTR(" "));
    }
}

void HexDump(const void* data, const uint16_t size) {
    char ascii[17];
    uint16_t i, j;
    ascii[16] = '\0';
    printf_P(PSTR("\n* HexDump s=%u *\n"), size);
    for (i = 0; i < size; ++i) {
        printf_P(__HexDump1, ((unsigned char*)data)[i]);
        if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
            ascii[i % 16] = ((unsigned char*)data)[i];
        } else {
            ascii[i % 16] = '.';
        }
        if ((i+1) % 8 == 0 || i+1 == size) {
            Print_Space_N(1);
            if ((i+1) % 16 == 0) {
                printf_P(__HexDump0, ascii);
            } else if (i+1 == size) {
                ascii[(i+1) % 16] = '\0';
                if ((i+1) % 16 <= 8) {
                    Print_Space_N(1);
                }
                for (j = (i+1) % 16; j < 16; ++j) {
                    Print_Space_N(3);
                }
                printf_P(__HexDump0, ascii);
            }
        }
    }
    printf_P(PSTR("* HexDump END *\n"));
}
