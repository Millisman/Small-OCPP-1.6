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

#include <stdint.h>
#include <stdbool.h>

#define modem stdin

typedef enum {
    conn_Unknown,
    conn_Disconnected,
    conn_Connecting,
    conn_Connected
}   conn_progress_t;


conn_progress_t CONN_start_connect(const char *host, const char *port);
void CONN_update();
void CONN_write_ofs();
void     CONN_size_set(uint16_t size);
void  CONN_GET_IP_STATUS();
// uint16_t CONN_data_accept_get();


void _hal_modem_init_port();

void gsm_proc_init();
void gsm_proc_begin();
void gsm_proc_update();
void gsm_proc_update_100ms();
void gsm_proc_update_1s();
void gsm_proc_deactivate();

char uart1_getc(void);
uint16_t uart1_available(void);
