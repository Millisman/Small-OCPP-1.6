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

#include "../port/port.h"

#include "ws16.h"
#include "conn.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "conn_sim800.h"

#if __has_include("../../secret.h")
#include "../../secret.h"
#else
const char http_path[]      PROGMEM = "/steve/websocket/CentralSystemService/";
const char http_host[]      PROGMEM = "example.com";
const char http_port[]      PROGMEM = "443";
#endif

#define WS_CHECKTABLE_COUNT 3
const char   str_websocket[]  PROGMEM = "websocket";
const char   str_Connection[] PROGMEM = "Connection:";
const char     str_upgrade[]  PROGMEM = "upgrade";
const char *ws_checktable[WS_CHECKTABLE_COUNT] = {
    str_upgrade,
    str_websocket,
    str_Connection
};

const char     str_http_11[]  PROGMEM = "HTTP/1.1 101";
const char     str_http_10[]  PROGMEM = "HTTP/1.0 101";
const char str_SecWsAccept[]  PROGMEM = "Sec-WebSocket-Accept:";
const char       str_UUID[]   PROGMEM = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

volatile ws_tx_t WsTx;
volatile ws_rx_t WsRx;

volatile ws_init_t ws_state = ws_not_init;
volatile uint16_t        to_server;
extern ws_stream_print_t ws_stream_print; // connector_*
extern SIM800_Man _Gsm_Modem;

void ws_update_100ms() {
    gsm_proc_update_100ms();
}


bool __get_ip_status_flag = false;

void ws_update_1s() {
    gsm_proc_update_1s();
    if (__get_ip_status_flag) {
        __get_ip_status_flag = false;
        CONN_GET_IP_STATUS();
    }
}

void ws_disconnect() {
    ws_state = ws_start_conn;
}


void ws_update() {

    gsm_proc_update();
    switch (ws_state) {

        case ws_not_init:
            __ws_init_stream();
            gsm_proc_init();

            WsTx.tail = 0;
            WsRx.count = 0;
            to_server = 0;
            ws_state = ws_start_conn;
            ws_stream_print = ws_stream_direct;

            break;

        case ws_start_conn:
            if (CONN_start_connect(http_host, http_port) == conn_Connected) {

                ws_state = ws_get_upgrade;
            }
            break;

        case ws_get_upgrade:
            for (uint8_t i = 0; i < sizeof(WsTx.head.mask); ++i) {
                WsTx.head.mask[i] = rand() % 256;
            }

            WsTx.tail = 0;
            ws_stream_print = ws_stream_counter;
            ws_print_key_GET();
            ws_stream_print = ws_stream_direct;
            to_server = WsTx.tail;
            WsTx.tail = 0;
            CONN_size_set(to_server);
            ws_state = ws_get_upgrade_prompt;
            break;

        case ws_get_upgrade_prompt:
            if (to_server == _Gsm_Modem.wait_schedule_size) { ; //send_schedule_size) {
                WsTx.tail = 0;
                ws_stream_print = ws_stream_direct;
                ws_print_key_GET();
                CONN_write_ofs();
                WsRx.count = 0;
                ws_state = ws_get_upgrade_reply;
            }
            break;

        case ws_get_upgrade_reply: {
                uint8_t ws_upgrade_reply_err = 1;
                if ( WsRx.count > 99 ) {
                    if ((strstr_P((const char *)WsRx.buff, str_http_11) != NULL) ||
                        (strstr_P((const char *)WsRx.buff, str_http_10) != NULL)) {
                        uint8_t check = 0;
                        for (uint8_t x = 0; x < WS_CHECKTABLE_COUNT; ++x) {
                            if (strcasestr_P((const char *)WsRx.buff, ws_checktable[x]) != NULL) { check++; }
                            if (check == WS_CHECKTABLE_COUNT) {
                                if (strcasestr_P((const char *)WsRx.buff, str_SecWsAccept) != NULL) {
                                        ws_stream_print = ws_stream_ws_masking;
                                        ws_upgrade_reply_err = 0;
                                        WsTx.tail = 0;
                                        WsRx.count = 0;
                                        WS16_ESTABLISHED();
                                        ws_state = ws_IDLE;

                                } else { ws_upgrade_reply_err = 5; } // bad WSACCEPT;
                            }
                        }
                        if (check != WS_CHECKTABLE_COUNT) { ws_upgrade_reply_err = 4; } // check string
                    } else { ws_upgrade_reply_err = 3; } // NO_HTTPx;
                } else { ws_upgrade_reply_err = 2; } // low size
                if (ws_upgrade_reply_err > 2) {
                    printf_P(PSTR("\nUPGRADE ERR %u\n"), ws_upgrade_reply_err);
                    // do reboot modem TODO
                }
        }
        break;

        case ws_send_ws_size:

            CONN_size_set(to_server);
            ws_state = ws_send_ws_content;
        break;

        case ws_send_ws_content:
            if (to_server == _Gsm_Modem.wait_schedule_size) {

                CONN_write_ofs();
                ws_state = ws_wait_data_accept;
            }
        break;

        case ws_wait_data_accept:
            if (to_server == _Gsm_Modem.data_accepted_size) {

                ws_state = ws_IDLE;
                to_server = 0;
            }
        break;

        case ws_IDLE:
            WsTx.tail = 0;

            to_server = 0;
            ws_stream_print = ws_stream_ws_masking;
            WS16_IDLE();
            if (WsTx.tail > 0) {
                ws_state = ws_send_ws_size;

                if (WsTx.tail < 126) {
                    to_server = 6;
                    WsTx.head.content[2] =  0x81;
                    WsTx.head.content[3] =  128;
                    WsTx.head.content[3] |= WsTx.tail;
                } else {
                    to_server = 8;
                    WsTx.head.content[0] =  0x81;
                    WsTx.head.content[1] =  128;
                    WsTx.head.content[1] |= 126;
                    WsTx.head.content[2] = (WsTx.tail >> 8) & 0xFF;
                    WsTx.head.content[3] = WsTx.tail & 0xFF;
                }
                to_server += WsTx.tail;
                printf_P(PSTR("\n enter in ws_send_ws_size\n"));
                __get_ip_status_flag = false;
            } else {

                __get_ip_status_flag = true; // TODO service GSM
            }
        break;
    }

}

// PONG 8a, 80

void ws_receive() {
    printf_P(PSTR("ws_receive\n"));

    uint8_t ws_receive_err = 1;

    if (WsRx.buff[0] == 0x81) {

        if ((WsRx.buff[1] & 0x80) == 0x00) {
            
            uint16_t payloadLen = (WsRx.buff[1] & 0x7F);
            uint8_t payload_offset = 2;
            if(payloadLen == 126) {
                payloadLen = WsRx.buff[2] << 8 | WsRx.buff[3];
                payload_offset += 2;
                printf_P(PSTR("payload_length=%u\n"), payloadLen);
            }
            if ((WsRx.buff[1] >> 7) & 0x01) {
                uint32_t mask = *((uint32_t *)(WsRx.buff + payload_offset));
                payload_offset += 4;
                char *c = (char *)WsRx.buff + payload_offset;
                for (uint16_t i = 0; i < payloadLen; i++) { // unmask payload:
                    c[i] = c[i] ^ ((char *)(&mask))[i % 4];
                }
            }
            ws_receive_err = 0; // OK
            WS16_CLIENT_RECEIVE((const char *)WsRx.buff + payload_offset, payloadLen);
            return;
    //      } else { ws_receive_err = 3; } // FRAME SIZE
        } else { ws_receive_err = 2; } // FRAME MASKED
    }
    
    if (ws_receive_err) {
        printf_P(PSTR("\nWsRx.buff[0] %u\n"), WsRx.buff[0]);
        printf_P(PSTR("\nWS_RECV ERR %u\n"), ws_receive_err);
    }
}

void ws_print_key_GET() {
    fprintf_P(modem, PSTR("GET "));
    fprintf_P(modem, http_path);
    fprintf_P(modem, PSTR("%s"), (const char *)&_Gsm_Modem.info.s_GSN);
    
    fprintf_P(modem, PSTR(" HTTP/1.1\r\nHost: "));
    fprintf_P(modem, http_host);
    // fprintf_P(modem, PSTR(":")); // dispensable
    // fprintf_P(modem, http_port); // dispensable
    fprintf_P(modem, PSTR("\r\n"));
    fprintf_P(modem, PSTR(
        "User-Agent: EVStation/1.0 (AVR8) KeyA/1001 KeyB/1.0 KeyC/1.2.3\r\n" // dispensable
        "Sec-WebSocket-Version: 13\r\n"
        "Origin: null\r\n" // dispensable
        "Connection: keep-alive, Upgrade\r\n"
        "Pragma: no-cache\r\n" // dispensable
        "Cache-Control: no-cache\r\n"
        "Upgrade: websocket\r\n"
        "Sec-WebSocket-Protocol: ocpp1.6\r\n"
        "Sec-WebSocket-Key: p/HZKoLI2P5DTZhVjOKzRw==\r\n\r\n"));
}





#ifdef LOG_ON
void print_unmasked() {
    if (WsTx.tail) { printf_P(PSTR("\nPrint Unmasked TAIL %u\n"), WsTx.tail); }
    for (size_t i = 0; i < WsTx.tail; ++i) {
        putchar(WsTx.buff[i] ^ WsTx.head.mask[i % sizeof(WsTx.head.mask)]);
    }
    if (WsTx.tail) putchar('\n');
}
#else
void print_unmasked() {}
#endif


