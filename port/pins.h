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

#pragma once
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-attributes"

#include "pins.h"

#define PIN_LED_RED     A, 0, H //  PA0 Active High, pull down 10K
#define PIN_LED_GREEN   A, 1, H //  PA1 Active High, pull down 10K
#define PIN_LED_BLUE    A, 2, H //  PA2 Active High, pull down 10K

#define PIN_CS_ADE      B, 0, H //  PB0 Active High ADE77_CSN, need pull up, active - LOW
#define PIN_CS_NFC      B, 1, H //  PB1 Active High RC522_CSN, need pull up, active - LOW
#define PIN_GFI         B, 2, H //  PB2 Active High INT 2
#define PIN_RF24_CE     B, 3, H //  PB3 Active High RF24_CE,  NRF24L01 chip ENABLE,
                                //  out, active HIGH, free (pulldown in NRF24)
#define PIN_RF24_CSN    B, 4, H //  PB4 Active High RF24_CSN, NRF24L01 chip nSELECT,
                                // out, active LOW, free (pullup in NRF24)

#define PIN_MOSI        B, 5, H //  PB5 Active High MOSI
#define PIN_MISO        B, 6, H //  PB6 Active High MISO
#define PIN_SCK         B, 7, H //  PB7 Active High SCK

#define PIN_LCD_RS          C, 0, H  // PC0 Active High PC0_LCD_RS LCD RS
#define PIN_LCD_EN          C, 1, H  // PC1 Active High PC1_LCD_E  LCD E
#define PIN_LCD_D4          C, 2, H  // PC2 Active High PC2_LCD_D4 LCD D4
#define PIN_LCD_D5          C, 3, H  // PC3 Active High PC3_LCD_D5 LCD D5
#define PIN_LCD_D6          C, 4, H  // PC4 Active High PC4_LCD_D6 LCD D6
#define PIN_LCD_D7          C, 5, H  // PC5 Active High PC5_LCD_D7 LCD D7
#define PIN_MENNEKES_LOCK   C, 6, H  // PC6 Active High PC6_2LOCK   LOCK,   pull-down 10K, active- UP
#define PIN_MENNEKES_UNLOCK C, 7, H  // PC7 Active High PC7_2UNLOCK UNLOCK, pull-down 10K, active- UP
// UART0 PD0
// UART0 PD1
// UART1 PD2
// UART1 PD3
#define PIN_PILO            D, 4, H // PD4 PILO
#define PIN_GSM_POWER       D, 5, H // PD5 Enable Modem
#define PIN_GFITEST         D, 6, H // PD6 pin is supposed to be wrapped around the CT 5+ times
#define PIN_AC_RELAY        D, 7, H // PD7 Active High PD7_RELAY Relay/Contactor driver, pull-down 10K, active- UP
