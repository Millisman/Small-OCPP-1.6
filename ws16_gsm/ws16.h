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

#define LOG_ON

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "ws16_ext.h"

typedef struct __attribute__((__packed__)) {
    uint8_t content[4];
    uint8_t mask[4];
} ws_header_t;


typedef struct __attribute__((__packed__)) {
    uint16_t    tail;
    ws_header_t head;
    uint8_t     buff[WS16_TX_BUFF_SIZE];
} ws_tx_t;

typedef struct __attribute__((__packed__)) {
    uint16_t    count;
    uint8_t     buff[WS16_RX_BUFF_SIZE];
    uint8_t     save_size;
    char       *save_ptr;
} ws_rx_t;

typedef enum {
    ws_not_init,
    ws_start_conn,
    ws_get_upgrade,
    ws_get_upgrade_prompt,
    ws_get_upgrade_reply,
    ws_send_ws_size,
    ws_send_ws_content,
    ws_wait_data_accept,
    ws_IDLE,
} ws_init_t;


typedef enum {
    ws_stream_counter,
    ws_stream_direct,
    ws_stream_ws_masking
} ws_stream_print_t;

void ws_update();
void ws_receive();
void __ws_init_stream();
void ws_update_100ms();
void ws_update_1s();
void ws_disconnect();

#ifdef LOG_ON
void print_unmasked();
#endif


void ws_print_key_GET();
