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

#if defined (__linux) || defined (__linux__) || defined (__gnu_linux__) \
|| defined (__unix) || defined (__unix__) \
|| defined (__x86_64) || defined (__x86_64__)

#define LOG_ON
#define _GNU_SOURCE

#include "../port/port.h"
#include "../utils/base64.h"
#include "../utils/sha1.h"
#include "ws16.h"
#include "conn.h"
#include <stdio.h>

extern ws_tx_t WsTx;
volatile ws_stream_print_t ws_stream_print = ws_stream_direct;

static ssize_t __ws_write(void *c, const char *buf, size_t size) {
    (void)c;

    for (size_t i = 0; i < size; ++i) {

        printf("%c", buf[i]);

        switch (ws_stream_print) {
            case ws_stream_counter:
                break;

            case ws_stream_direct:
                WsTx.buff[WsTx.tail] = buf[i];
                break;

            case ws_stream_ws_masking:
                WsTx.buff[WsTx.tail] = (buf[i] ^ WsTx.head.mask[WsTx.tail % sizeof(WsTx.head.mask)]);
                break;
        }

        if (WsTx.tail < sizeof(WsTx.buff)) {
            WsTx.tail++;
        } else {
            printf_P(PSTR("\n=ws_write overflow=\n"));
            return WsTx.tail;
        }
    }
    return size;
}

static FILE *ws_stream;
void __ws_init_stream() {
    ws_stream = fopencookie(
        NULL,
        "w",
        (cookie_io_functions_t) {
            NULL,       // cookie_read_function_t
            __ws_write, // cookie_write_function_t
            NULL,       // cookie_seek_function_t
            NULL        // cookie_close_function_t
        }
    );
    stdin = ws_stream;
    setbuf(stdin, NULL);
}



#endif
