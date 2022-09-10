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

#include "../port/port.h"
#include "../port/uart.h"
#include "conn.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ws16.h"
#include "conn_sim800.h"

extern const char *_gsm_conn_host;
extern const char *_gsm_conn_port;
extern SIM800_Man _Gsm_Modem;

conn_progress_t CONN_start_connect(const char *host, const char *port) {
    if (_Gsm_Modem.ip_state == CONN_CONNECT_OK) {
        return conn_Connected;
    }
    if (host && port) {
        _gsm_conn_host = host;
        _gsm_conn_port = port;
        return conn_Connecting;
    }
}

volatile uint16_t isr_cursor;
volatile const char *__buf = NULL;
volatile uint16_t __n = 0;

ISR(USART1_UDRE_vect) {
    if (isr_cursor < __n) {
        UDR1 = __buf[isr_cursor];
        ++isr_cursor;
    } else {
        UCSR1B &= ~( 1 << UDRIE1 );
        __buf = NULL;
        isr_cursor = 0;
        __n = 0;
    }
}

void __start_tx_isr() {
//     HexDump((void *)__buf, __n);
    if (__n > 0) {
        isr_cursor = 0;
        UCSR1B |= ( 1 << UDRIE1 );
    }
}

extern ws_tx_t WsTx;
volatile char __CIPSEND_buf[20];

void  CONN_GET_IP_STATUS() {
    sprintf_P((char *)&__CIPSEND_buf, PSTR("AT+CIPSTATUS\r\n"));
    __buf = (char *)&__CIPSEND_buf;
    __n = strlen((const char *)__buf);
    __start_tx_isr();
}


void  CONN_size_set(uint16_t size) {
    if (size  && size <= sizeof(WsTx.buff)) {
        _Gsm_Modem.send_schedule_size = size;
        _Gsm_Modem.wait_schedule_size = 0;
        sprintf_P((char *)&__CIPSEND_buf, PSTR("AT+CIPSEND=%u\r\n"), _Gsm_Modem.send_schedule_size);
        __buf = (char *)&__CIPSEND_buf;
        __n = strlen((const char *)__buf);
        __start_tx_isr();
    }
}

extern ws_stream_print_t ws_stream_print;

void CONN_write_ofs() {
    printf_P(PSTR("CONN_write_ofs(mode %u), tail %u\n"), ws_stream_print, WsTx.tail);

    if (WsTx.tail == 0) {
        printf_P(PSTR("CONN_write_ofs WsTx.tail==0!\n"));
        return;
    }

    switch (ws_stream_print) {

        case ws_stream_direct:
            if (WsTx.tail) {
                __buf = (char *)&WsTx.buff;
                __n   = WsTx.tail;
                __start_tx_isr();
            }
            break;

        case ws_stream_ws_masking:
            if (WsTx.tail < 126) {
                __buf = (char *)&WsTx.buff - 6;
                __n = WsTx.tail + 6;
            } else {
                __buf = (char *)&WsTx.buff - 8;
                __n = WsTx.tail + 8;
            }
            __start_tx_isr();
            break;

        case ws_stream_counter:
            break;
    }
    WsTx.tail = 0;
}



void CONN_update () {
}

extern ws_rx_t WsRx;

void _hal_modem_init_port() {
    memset((void *)&WsRx, 0, sizeof(WsRx));
    memset((void *)&WsTx, 0, sizeof(WsTx));
    UCSR1A = 1 << U2X1;
    UBRR1H = ((F_CPU / 4 / MODEM_BAUD_RATE - 1) / 2) >> 8;
    UBRR1L = ((F_CPU / 4 / MODEM_BAUD_RATE - 1) / 2) & 0xFF;
    UCSR1B = ((1<<TXEN1) | (1<<RXEN1) | (1<<RXCIE1) /*| (1<<TXCIE1)*/);
}

// volatile uint8_t __rxc1;
extern SIM800_Man _Gsm_Modem;

#define UART_RX1_BUFFER_SIZE 512
#define UART_RX1_BUFFER_MASK (UART_RX1_BUFFER_SIZE - 1)

#define UART1_STATUS   UCSR1A
#define UART1_CONTROL  UCSR1B
#define UART1_DATA     UDR1
#define UART1_UDRIE    UDRIE1

static volatile uint8_t UART1_RxBuf[UART_RX1_BUFFER_SIZE];
static volatile uint16_t UART1_RxHead = 0;
static volatile uint16_t UART1_RxTail = 0;

static volatile uint8_t  __tmp8;
static volatile uint16_t __tmp16;

ISR(USART1_RX_vect) {
    __tmp8 = UDR1;
    __tmp16 = (UART1_RxHead + 1) & UART_RX1_BUFFER_MASK;
    if (__tmp16 == UART1_RxTail) {
        // buffer overflow
    } else {
        UART1_RxHead = __tmp16;
        UART1_RxBuf[__tmp16] = __tmp8;
    }
}

char uart1_getc() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (UART1_RxHead == UART1_RxTail) {
            // no data
            return 0;
        }
        __tmp16 = (UART1_RxTail + 1) & UART_RX1_BUFFER_MASK;
        UART1_RxTail = __tmp16;
    }
    __tmp8 = UART1_RxBuf[__tmp16];
    return __tmp8;

}

uint16_t uart1_available() {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        __tmp16 = (UART_RX1_BUFFER_SIZE + UART1_RxHead - UART1_RxTail) & UART_RX1_BUFFER_MASK;
    }
    return __tmp16;
}


#endif
