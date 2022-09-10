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

#include "../port/port.h"

#include "conn.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stddef.h>
#include <sys/time.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include "ws16.h"

volatile uint16_t send_schedule_size = 0;
volatile int sock_fd = 0;

conn_progress_t CONN_start_connect(const char *host, const char *port) {
    if (sock_fd > 0) { close(sock_fd); }
    send_schedule_size = 0;
    sock_fd = 0;
    printf("CONN_start_connect %s:%s\n", host, port);
    int rv;
    struct addrinfo hints, *servinfo, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }
        if (connect(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sock_fd);
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 1000; //500000;
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    //-----------------------------------------
    printf("connected!\n");
}


extern ws_tx_t WsTx;
extern ws_rx_t WsRx;
extern ws_stream_print_t ws_stream_print;

void CONN_write_ofs() {
    printf("CONN_write_ofs\n");

    const char *__buf = NULL;
    size_t __n = 0;

    switch (ws_stream_print) {

        case ws_stream_direct:  // plain text
            __buf = (char *)&WsTx.buff;
            __n   = WsTx.tail;
            break;

        case ws_stream_ws_masking:
            if (WsTx.tail < 126) {
                __buf = (char *)&WsTx.buff - 6;
                __n = WsTx.tail + 6;
            } else {
                __buf = (char *)&WsTx.buff - 8;
                __n = WsTx.tail + 8;
            }
            break;

        case ws_stream_counter:
            return;
            break;

    }

    //     printf("\n---------------------------\n");
    //     for (ssize_t xx = 0; xx < __n; ++xx) {
    //         printf(" %02x", *(uint8_t*)(__buf + xx));
    //         if ((xx+1) % 16 == 0) printf("\n");
    //     }
    //     printf("\n++++++++++++++++++++++++++\n");


    ssize_t var = send(sock_fd, __buf, __n, 0);
    if (var == -1) {
        printf("\n *** send FAIL -1 ***\n");
    } else {
        if (send_schedule_size == var) {
            printf("\n *** send OK %ld ***\n", var);
        } else {
            printf("\n *** send PROBLEM var(%ld) != size(%hu) ***\n", var, send_schedule_size);
        }
    }

    send_schedule_size = 0;
}

void  CONN_size_set(uint16_t size) {
    printf("CONN_size_set = %d ", size);
    if (send_schedule_size == 0) {
        if (size <= sizeof(WsTx.buff)) {
            send_schedule_size = size;
            printf("[OK]\n");
        }
    } else { printf("[ERROR] (send_schedule_size !=0)\n"); }
}

void CONN_update () {
    memset((void *)WsRx.buff, 0, sizeof(WsRx.buff));
    ssize_t r = recv(sock_fd, (void *)WsRx.buff, sizeof(WsRx.buff), 0);
    if (1 > r) { return; }
    for (size_t a = 0; a < r; ++a) {
        printf("%c", WsRx.buff[a]);
    }
    printf("\n");
    if (r <= sizeof(WsRx.buff)) {
        WsRx.count = r;
        WsRx.buff[WsRx.count] = '\0';
    }
}




volatile ModemRxBuff_t __mdm_rx;

void _hal_modem_init_port() {
    memset((void *)&__mdm_rx, 0, sizeof(__mdm_rx));
    printf("\n%s:%d %s():\n", __FILE__, __LINE__, __func__);
}

extern SIM800_Man _Gsm_Modem;

void recv_char(const char __rxc1) {
    if ( __rxc1 == '\n' && __mdm_rx.rx_len > 0 && __mdm_rx.rx_buff[__mdm_rx.rx_len-1] == '\r') {

        if (__mdm_rx.rx_len == 3 &&
            __mdm_rx.rx_buff[0] == 'O' &&
            __mdm_rx.rx_buff[1] == 'K'
        ) {
            _Gsm_Modem.observe_GSM_OK = true;
            //                 __mdm_rx.trip_ok = true;
        } else if (__mdm_rx.rx_len == 6 &&
            __mdm_rx.rx_buff[0] == 'E' &&
            __mdm_rx.rx_buff[1] == 'R' &&
            __mdm_rx.rx_buff[2] == 'R' &&
            __mdm_rx.rx_buff[3] == 'O' &&
            __mdm_rx.rx_buff[4] == 'R'
        ) {
            _Gsm_Modem.observe_GSM_ERROR = true;
            //                 __mdm_rx.trip_err = true;
        } else {
            __mdm_rx.rx_buff[__mdm_rx.rx_len] = 0;
            if (__mdm_rx.rx_len > 2) {
                if (__mdm_rx.save_size && __mdm_rx.save_ptr) {
                    if (__mdm_rx.save_size > __mdm_rx.rx_len) { __mdm_rx.save_size = __mdm_rx.rx_len; }
                    strncpy(__mdm_rx.save_ptr, (const void *)&__mdm_rx.rx_buff, __mdm_rx.save_size);
                    __mdm_rx.save_ptr = 0;
                    __mdm_rx.save_size = 0;
                } else {
                    _Gsm_Modem.scheduled_scan = true;
                    __mdm_rx.rx_len_scan = __mdm_rx.rx_len;
                }
            }
        }
        __mdm_rx.rx_len = 0;
    } else {
        __mdm_rx.rx_buff[__mdm_rx.rx_len] = __rxc1;
        if (__mdm_rx.rx_len < sizeof(WsRx.buff)) { ++__mdm_rx.rx_len; }
    }
    if (__mdm_rx.rx_len == 2 &&
        __mdm_rx.rx_buff[0] == '>' &&
        __mdm_rx.rx_buff[1] == ' '
    ) {
        _Gsm_Modem.observe_GSM_PROMPT = true;
        __mdm_rx.rx_len = 0;
    }
}



#endif
