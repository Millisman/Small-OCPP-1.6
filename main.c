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

#include "port/port.h"
#include "port/LiquidCrystal4b.h"
#include <stdio.h>
#include <unistd.h>
#include "ws16_gsm/ws16.h"

void cmd_freemem() {
    extern int __heap_start;
    extern int *__brkval;
    int v;
    int Free__Ram = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    LiquidCrystal_setCursor(0,0);
    fprintf_P(LCD, PSTR("FREE_RAM=%4d"), Free__Ram);
}


int main() {
    init();
    LiquidCrystal_blink();
    printf_P(PSTR("init ok\n"));

    while (1) {
        ws_update();

        if (elapsed_100ms()) {
            ws_update_100ms();
        }

        if (elapsed_1sec()) {
            ws_update_1s();
            cmd_freemem();
        }

        wdt_reset();
    }
    return 0;
}
