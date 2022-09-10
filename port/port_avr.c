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

#if defined (__AVR__) || defined (__AVR_ARCH__)

#include <stdio.h>
#include "uart.h"
#include "pin_macros.h"
#include "pins.h"

#include <util/atomic.h>
#include <util/delay_basic.h>
#include <avr/wdt.h>
#include "LiquidCrystal4b.h"

typedef enum {
    LCD_4b_IDLE     = 0,
    LCD_4b_HIGH     = 2,
    LCD_4b_HIGH_W   = 3,
    LCD_4b_LOW      = 4,
    LCD_4b_LOW_W    = 5,
    LCD_4b_DONE     = 6

} mcu_Write_4b_modes;

//-------------------------------------------------
volatile uint8_t _lcd_data_buffer[64] = {0};
volatile uint8_t _lcd_data_comdata[8] = {0};
volatile uint8_t _lcd_index_insert = 0;
volatile uint8_t _lcd_index_read = 0;
volatile mcu_Write_4b_modes __lcd4b_state = LCD_4b_IDLE;

volatile uint8_t _lcd_block = 0;

void mcu_lcd_insert(const uint8_t rs, const uint8_t ch) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        uint8_t a = _lcd_index_insert / 8;
        uint8_t b = _lcd_index_insert % 8;
        if (rs) {
            _lcd_data_comdata[a] |= (1U << b);
        } else {
            _lcd_data_comdata[a] &= (~(1U << b));
        }
        _lcd_data_buffer[_lcd_index_insert] = ch;
        ++_lcd_index_insert;
        if ( _lcd_index_insert >= sizeof(_lcd_data_buffer) ) { _lcd_index_insert = 0; }
        if ( _lcd_index_insert == _lcd_index_read ) { ++_lcd_index_read; }
    }
}

void mcu_lcd_write4b(uint8_t value) {
    _delay_loop_1(255);
    if ((value >> 0) & 0x01) { ON(PIN_LCD_D4); } else { OFF(PIN_LCD_D4); }
    if ((value >> 1) & 0x01) { ON(PIN_LCD_D5); } else { OFF(PIN_LCD_D5); }
    if ((value >> 2) & 0x01) { ON(PIN_LCD_D6); } else { OFF(PIN_LCD_D6); }
    if ((value >> 3) & 0x01) { ON(PIN_LCD_D7); } else { OFF(PIN_LCD_D7); }
//     _delay_loop_1(255);
    __asm__ __volatile__ ("nop");
    __asm__ __volatile__ ("nop");

}


volatile bool __elapsed_100ms = 0;
volatile bool __elapsed_1_sec = 0;
volatile uint8_t  __counter_10   = 0;
volatile uint8_t  __counter_100  = 0;
volatile uint8_t  __counter_100x10 = 0;
volatile uint32_t timer0_millis = 0;
volatile uint32_t timer0_uptime_s = 0;
volatile uint32_t timer0_unix_time_s = 1662385700;

ISR(TIMER0_COMPA_vect) {
    ++__counter_10;

    if ( __counter_10 >= 10) {
        __counter_10 = 0;
        ++timer0_millis;    // millis
        ++__counter_100;

        if ( __counter_100 >= 100) {
            __counter_100 = 0;
            __elapsed_100ms = 1;
            ++__counter_100x10;

            if ( __counter_100x10 >= 10) {
                __counter_100x10 = 0;
                ++timer0_uptime_s;  // sec
                ++timer0_unix_time_s; // unix time
                if (_lcd_block) { --_lcd_block; }
                __elapsed_1_sec = 1;
            }

        }
    } else {

        switch (__lcd4b_state) {
            case LCD_4b_IDLE: {
                if ( _lcd_index_read != _lcd_index_insert ) {
                    uint8_t a = _lcd_index_read / 8;
                    uint8_t b = _lcd_index_read % 8;
                    if (_lcd_data_comdata[a] & (1UL << b)) {
                        ON(PIN_LCD_RS);
                    } else {
                        OFF(PIN_LCD_RS);
                    }
                    OFF(PIN_LCD_EN);
                    __lcd4b_state = LCD_4b_HIGH;
                }
            }
            break;

            case LCD_4b_HIGH: {
                OFF(PIN_LCD_EN);
                uint8_t a = _lcd_data_buffer[_lcd_index_read] >> 4;
                mcu_lcd_write4b(a);
                __lcd4b_state = LCD_4b_HIGH_W;
            }
            break;

            case LCD_4b_HIGH_W:
                ON(PIN_LCD_EN);
                __lcd4b_state = LCD_4b_LOW;
                break;

            case LCD_4b_LOW: {
                OFF(PIN_LCD_EN);
                uint8_t a = _lcd_data_buffer[_lcd_index_read];
                mcu_lcd_write4b(a);
                __lcd4b_state = LCD_4b_LOW_W;
            }
            break;

            case LCD_4b_LOW_W:
                ON(PIN_LCD_EN);
                __lcd4b_state = LCD_4b_DONE;
                break;

            case LCD_4b_DONE:
                OFF(PIN_LCD_EN);
                OFF(PIN_LCD_RS);
                ++_lcd_index_read;
                if ( _lcd_index_read >= sizeof(_lcd_data_buffer) ) _lcd_index_read = 0;
                __lcd4b_state = LCD_4b_IDLE;
                break;
        }
    }

}

uint32_t millis() {
    uint32_t r;
    ATOMIC_BLOCK(ATOMIC_FORCEON) { r = timer0_millis; }
    return r;
}

uint32_t uptime_sec() {
    uint32_t r;
    ATOMIC_BLOCK(ATOMIC_FORCEON) { r = timer0_uptime_s; }
    return r;
}


uint32_t unixtime_get() {
    uint32_t r;
    ATOMIC_BLOCK(ATOMIC_FORCEON) { r = timer0_unix_time_s; }
    return r;
}

void unixtime_set(uint32_t unixtime) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) { timer0_unix_time_s = unixtime; }
}

void lcd_block_set(uint8_t sec) {
    _lcd_block = sec;
}

bool elapsed_1sec() {
    if (__elapsed_1_sec) {
        __elapsed_1_sec = 0;
        return true;
    }
    return false;
}

bool elapsed_100ms() {
    if (__elapsed_100ms) {
        __elapsed_100ms = 0;
        return true;
    }
    return false;
}


void mcu_wdt_restart() {
    wdt_enable(WDTO_15MS);
    while (1) {}
}

int __putc_uart0(char ch, FILE *stream) {
    (void)stream;
    if (ch == '\n') uart0_putc('\r');
    uart0_putc(ch);
    return 0;
}


int __putc_lcd0(char ch, FILE *stream) {
    (void)stream;
    if (_lcd_block) { return 0; }
    LiquidCrystal_send(ch, LCD_SEND_DATA);
    return 0;
}

static FILE my_std_out;
static FILE my_std_err;
// static FILE my_std_out;


void init() {

    MCUSR = 0;
    wdt_enable(WDTO_2S);
    wdt_reset();
    cli(); // stop interrupts

    //-------------------------------------------------- PINS
    DRIVER(PIN_LED_RED,     OUT);   ON(PIN_LED_RED);
    DRIVER(PIN_LED_GREEN,   OUT);   OFF(PIN_LED_GREEN);
    DRIVER(PIN_LED_BLUE,    OUT);   OFF(PIN_LED_BLUE);
    DRIVER(PIN_CS_ADE,      OUT);   ON(PIN_CS_ADE);
    DRIVER(PIN_CS_NFC,      OUT);   ON(PIN_CS_NFC);
    DRIVER(PIN_RF24_CSN,    OUT);   ON(PIN_RF24_CSN);
    DRIVER(PIN_GFI,         IN);
    DRIVER(PIN_RF24_CE,     OUT);   OFF(PIN_RF24_CE);
    DRIVER(PIN_MOSI,    OUT);   ON(PIN_MOSI);   //  PB5 Active High MOSI
    DRIVER(PIN_MISO,    IN);                    //  PB6 Active High MISO
    DRIVER(PIN_SCK,     OUT);   ON(PIN_SCK);    //  PB7 Active High SCK
    // ALERT skipping LCD pins
    DRIVER(PIN_MENNEKES_LOCK,   OUT);   OFF(PIN_MENNEKES_LOCK);
    DRIVER(PIN_MENNEKES_UNLOCK, OUT);   OFF(PIN_MENNEKES_UNLOCK);
    DRIVER(PIN_PILO,            OUT);   OFF(PIN_PILO);
    DRIVER(PIN_GSM_POWER,       OUT);   OFF(PIN_GSM_POWER);
    DRIVER(PIN_GFITEST,         OUT);   OFF(PIN_GFITEST);
    DRIVER(PIN_AC_RELAY,        OUT);   OFF(PIN_AC_RELAY);
    //--------------------------------------------------

    // http://www.8bit-era.cz/arduino-timer-interrupts-calculator.html
    // TIMER 0 for interrupt frequency 10000 Hz:

    TCCR0A = 0; // set entire TCCR0A register to 0
    TCCR0B = 0; // same for TCCR0B
    TCNT0  = 0; // initialize counter value to 0
    // set compare match register for 10000 Hz increments
    OCR0A = 249; // = 20000000 / (8 * 10000) - 1 (must be <256)
    // turn on CTC mode
    TCCR0B |= (1 << WGM01);
    // Set CS02, CS01 and CS00 bits for 8 prescaler
    TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);

    uart0_init(UART_BAUD_SELECT(115200UL));

    fdev_setup_stream( &my_std_out, __putc_uart0, NULL, _FDEV_SETUP_WRITE);
    stdout = &my_std_out;

    fdev_setup_stream( &my_std_err, __putc_lcd0,   NULL, _FDEV_SETUP_WRITE);
    stderr = &my_std_err;

    sei();

    LiquidCrystal_begin();
}

void __gsm_on_off(bool on) {
    on ? ON(PIN_GSM_POWER) : OFF(PIN_GSM_POWER);
}


#endif
