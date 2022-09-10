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

#if defined (__AVR__) || defined (__AVR_ARCH__)

#define LOG_ON
#define _GNU_SOURCE

#include "../port/port.h"
#include "../port/uart.h"
#include "ws16.h"
#include "conn.h"
#include <stdio.h>

extern ws_tx_t WsTx;

volatile ws_stream_print_t ws_stream_print;

int __putc_modem(char ch, FILE *stream) {
    (void)stream;

    uart0_putc(ch);

    switch (ws_stream_print) {
        case ws_stream_counter:
            break;

        case ws_stream_direct:
            if (WsTx.tail < sizeof(WsTx.buff)) {
                WsTx.buff[WsTx.tail] = ch;
            }
            break;

        case ws_stream_ws_masking:
            if (WsTx.tail < sizeof(WsTx.buff)) {
                WsTx.buff[WsTx.tail] = (ch ^ WsTx.head.mask[WsTx.tail % sizeof(WsTx.head.mask)]);
            }
            break;
    }

    WsTx.tail++;
    return 0;
}

static FILE my_std_in;
void __ws_init_stream() {
    fdev_setup_stream(&my_std_in, __putc_modem, NULL, _FDEV_SETUP_WRITE);
    stdin = &my_std_in;
}

#endif
