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

#include "LiquidCrystal4b.h"
#include "port.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "pin_macros.h"
#include "pins.h"


static uint8_t _displaycontrol;

void LiquidCrystal_begin() {
    DRIVER(PIN_LCD_RS, OUT);
    DRIVER(PIN_LCD_EN, OUT);
    OFF(PIN_LCD_RS);
    OFF(PIN_LCD_EN);
    DRIVER(PIN_LCD_D4, OUT);
    DRIVER(PIN_LCD_D5, OUT);
    DRIVER(PIN_LCD_D6, OUT);
    DRIVER(PIN_LCD_D7, OUT);
    _delay_ms(50);
    LiquidCrystal_send(0x03, LCD_SEND_4b_INIT); // set 4 bit mode
    _delay_ms(5); // wait min 4.1 ms
    LiquidCrystal_send(0x03, LCD_SEND_4b_INIT);// set 4 bit mode
    _delay_ms(5); // wait min 4.1 ms
    LiquidCrystal_send(0x03, LCD_SEND_4b_INIT);// set 4 bit mode
    _delay_us(150);
    LiquidCrystal_send(0x02, LCD_SEND_4b_INIT);
    LiquidCrystal_send(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS, LCD_SEND_COMMAND);

    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LiquidCrystal_send(LCD_DISPLAYCONTROL | _displaycontrol, LCD_SEND_COMMAND);
    LiquidCrystal_display();
    LiquidCrystal_clear();
}

void LiquidCrystal_noDisplay() {
    _displaycontrol &= ~LCD_DISPLAYON;
    LiquidCrystal_send(LCD_DISPLAYCONTROL | _displaycontrol, LCD_SEND_COMMAND);
}

void LiquidCrystal_display() {
    _displaycontrol |= LCD_DISPLAYON;
    LiquidCrystal_send(LCD_DISPLAYCONTROL | _displaycontrol, LCD_SEND_COMMAND);
}

void LiquidCrystal_noCursor() {
    _displaycontrol &= ~LCD_CURSORON;
    LiquidCrystal_send(LCD_DISPLAYCONTROL | _displaycontrol, LCD_SEND_COMMAND);
}
void LiquidCrystal_cursor() {
    _displaycontrol |= LCD_CURSORON;
    LiquidCrystal_send(LCD_DISPLAYCONTROL | _displaycontrol, LCD_SEND_COMMAND);
}

void LiquidCrystal_noBlink() {
    _displaycontrol &= ~LCD_BLINKON;
    LiquidCrystal_send(LCD_DISPLAYCONTROL | _displaycontrol, LCD_SEND_COMMAND);
}

void LiquidCrystal_blink() {
    _displaycontrol |= LCD_BLINKON;
    LiquidCrystal_send(LCD_DISPLAYCONTROL | _displaycontrol, LCD_SEND_COMMAND);
}

void LiquidCrystal_scrollDisplayLeft(void) {
    LiquidCrystal_send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, LCD_SEND_COMMAND);
}

void LiquidCrystal_scrollDisplayRight(void) {
    LiquidCrystal_send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, LCD_SEND_COMMAND);
}

void LiquidCrystal_createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x07;
    LiquidCrystal_send(LCD_SETCGRAMADDR | (location << 3), LCD_SEND_COMMAND);
    for (uint8_t i=0; i<8; i++) {
        LiquidCrystal_send(charmap[i], LCD_SEND_DATA);
    }
}

void LiquidCrystal_clear() {
    LiquidCrystal_send(LCD_CLEARDISPLAY, LCD_SEND_COMMAND);
}

void LiquidCrystal_home() {
    LiquidCrystal_send(LCD_RETURNHOME, LCD_SEND_COMMAND);
}

void LiquidCrystal_setCursor(uint8_t col, uint8_t row) {
    static uint8_t offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    LiquidCrystal_send(LCD_SETDDRAMADDR | (col + offsets[row]), LCD_SEND_COMMAND);
}

void LiquidCrystal_send(const uint8_t data, const LiquidCrystal_send_t mode) {
    if (mode == LCD_SEND_4b_INIT) {
        mcu_lcd_write4b(data);
        OFF(PIN_LCD_EN);
        _delay_us(0.6);
        ON(PIN_LCD_EN);
        _delay_us(0.6);
        OFF(PIN_LCD_EN);
        _delay_us(40);
        return;
    }
    mcu_lcd_insert((mode == LCD_SEND_DATA) , data);
}
