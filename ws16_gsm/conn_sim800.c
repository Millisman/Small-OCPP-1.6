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

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "conn_sim800.h"
#include "port/port.h"
#include "port/uart.h"
#include "utils/cmd.h"
#include <string.h>
#include "conn.h"

volatile SIM800_Man _Gsm_Modem;

extern ws_rx_t WsRx;
extern ws_rx_t WsTx;
extern ws_stream_print_t ws_stream_print;

const char PSTR_AT[]  PROGMEM = "AT";
const char PSTR_EOL[] PROGMEM = "\r\n";

void modem_at_P(const char *c) {
    fprintf_P(modem, PSTR_AT);
    if (c) {
        fprintf_P(modem, c);
    }
    fprintf_P(modem, PSTR_EOL);
}

void modem_strace_P(const char *name, const char *str) {
    if (name && str) {
        printf_P(PSTR_AT);
        printf_P(name);
        printf_P(PSTR(":%s\n"), str);
    }
}


const char PSTR_NORMAL_POWER_DOWN[]         PROGMEM = "NORMAL POWER DOWN";
const char PSTR_UNDER_VOLTAGE_POWER_DOWN[]  PROGMEM = "UNDER-VOLTAGE POWER DOWN";
const char PSTR_UNDER_VOLTAGE_WARNNING[]    PROGMEM = "UNDER-VOLTAGE WARNNING";
const char PSTR_OVER_VOLTAGE_POWER_DOWN[]   PROGMEM = "OVER-VOLTAGE POWER DOWN";
const char PSTR_OVER_VOLTAGE_WARNNING[]     PROGMEM = "OVER-VOLTAGE WARNNING";

const char PSTR_CALL_READY[]    PROGMEM = "Call Ready";
const char PSTR_SMS_READY[]     PROGMEM = "SMS Ready";

const char PSTR_STATE[]             PROGMEM = "STATE";
const char PSTR_CONNECT_OK[]        PROGMEM ="CONNECT OK";
const char PSTR_CONNECTED[]         PROGMEM ="CONNECTED";
const char PSTR_IP_INITIAL[]        PROGMEM ="IP INITIAL";
const char PSTR_IP_START[]          PROGMEM ="IP START";
const char PSTR_IP_GPRSACT[]        PROGMEM ="IP GPRSACT";
const char PSTR_IP_STATUS[]         PROGMEM ="IP STATUS";
const char PSTR_INITIAL[]           PROGMEM ="INITIAL";
const char PSTR_REMOTE_CLOSING[]    PROGMEM ="REMOTE CLOSING";
const char PSTR_TCP_CLOSED[]        PROGMEM ="TCP CLOSED";
const char PSTR_UDP_CLOSED[]        PROGMEM ="UDP CLOSED";
const char PSTR_CLOSED[]            PROGMEM ="CLOSED";
const char PSTR_PDP_DEACT[]         PROGMEM ="PDP DEACT";
const char PSTR_TCP_CONNECTING[]    PROGMEM ="TCP CONNECTING";
const char PSTR_UDP_CONNECTING[]    PROGMEM ="UDP CONNECTING";
const char PSTR_CONNECTING[]        PROGMEM ="CONNECTING";
#if 0
const char PSTR_IP_PROCESSING[]     PROGMEM ="IP PROCESSING";
const char PSTR_IP_CONFIG[]         PROGMEM ="IP CONFIG";
const char PSTR_TCP_CLOSING[]       PROGMEM ="TCP CLOSING";
const char PSTR_UDP_CLOSING[]       PROGMEM ="UDP CLOSING";
const char PSTR_CLOSING[]           PROGMEM ="CLOSING";
const char PSTR_SERVER_LISTENING[]  PROGMEM ="SERVER LISTENING";
const char PSTR_LISTENING[]         PROGMEM ="LISTENING";
const char PSTR_OPENING[]           PROGMEM ="OPENING";

#endif



void gsm_handler_STATE() {
    SimConnStatus scs = CONN_UNKNOWN;
    if      (strstr_P(WsRx.buff, PSTR_INITIAL))         { scs = CONN_INITIAL; }
    else if (strstr_P(WsRx.buff, PSTR_IP_START))        { scs = CONN_IP_START; }
    else if (strstr_P(WsRx.buff, PSTR_IP_STATUS))       { scs = CONN_IP_STATUS; }
    else if (strstr_P(WsRx.buff, PSTR_IP_GPRSACT))      { scs = CONN_IP_GPRSACT; }
    else if (strstr_P(WsRx.buff, PSTR_TCP_CLOSED))      { scs = CONN_TCP_CLOSED; }
    else if (strstr_P(WsRx.buff, PSTR_UDP_CLOSED))      { scs = CONN_UDP_CLOSED; }
    else if (strstr_P(WsRx.buff, PSTR_CONNECT_OK))      { scs = CONN_CONNECT_OK; }
    else if (strstr_P(WsRx.buff, PSTR_REMOTE_CLOSING))  { scs = CONN_REMOTE_CLOSING; }
    else if (strstr_P(WsRx.buff, PSTR_CLOSED))          { scs = CONN_CLOSED; }
    else if (strstr_P(WsRx.buff, PSTR_CONNECTED))       { scs = CONN_CONNECTED; }
    else if (strstr_P(WsRx.buff, PSTR_IP_INITIAL))      { scs = CONN_IP_INITIAL; }
    else if (strstr_P(WsRx.buff, PSTR_PDP_DEACT))       { scs = CONN_PDP_DEACT; }
    else if (strstr_P(WsRx.buff, PSTR_TCP_CONNECTING))  { scs = CONN_TCP_CONNECTING; }
    else if (strstr_P(WsRx.buff, PSTR_UDP_CONNECTING))  { scs = CONN_UDP_CONNECTING; }
    else if (strstr_P(WsRx.buff, PSTR_CONNECTING))      { scs = CONN_CONNECTING; }
    //     { PSTR_IP_CONFIG,           CONN_IP_CONFIG},
    //     { PSTR_IP_PROCESSING,       CONN_IP_PROCESSING},
    //     { PSTR_TCP_CLOSING,         CONN_TCP_CLOSING},
    //     { PSTR_UDP_CLOSING,         CONN_UDP_CLOSING},
    //     { PSTR_CLOSING,             CONN_CLOSING},
    //     { PSTR_SERVER_LISTENING,    CONN_SERVER_LISTENING},
    //     { PSTR_LISTENING,           CONN_LISTENING},
    //     { PSTR_OPENING,             CONN_OPENING},
    if (scs != CONN_UNKNOWN) {
        if (_Gsm_Modem.ip_state != scs) {
            printf_P(PSTR("new IP STATE:%u\n"), scs);
            LiquidCrystal_setCursor(0,1);
            fprintf_P(LCD, PSTR("%s"), WsRx.buff + 6);
        }
        _Gsm_Modem.ip_state = scs;
    } else {
        printf_P(PSTR("IP STATE BAD\n"));
    }
}

void gsm_handler_waste(char *c) { printf_P(PSTR("WASTE [%s]\n"), c); }


// volatile sca __rxc1;
void gsm_proc_update() {
    CONN_update();
}



const char PSTR_E0[]  PROGMEM = "E0";       // ATE0 Echo off
const char PSTR_I[]   PROGMEM = "I";        // ATI
const char PSTR_pGCAP[]  PROGMEM = "+GCAP";    // Request complete TA capabilities list
const char PSTR_pGMI[]  PROGMEM = "+GMI";     // Request manufacturer identification
const char PSTR_pGMM[]  PROGMEM = "+GMM";     // Request TA model identification
const char PSTR_pGMR[]  PROGMEM = "+GMR";     // Request TA revision identification of software release
const char PSTR_pGOI[]  PROGMEM = "+GOI";     // Request global object identification
const char PSTR_pGSN[]  PROGMEM = "+GSN";     // Request TA serial number identification (IMEI)
const char PSTR_pCPINq[]  PROGMEM = "+CPIN?";
const char PSTR_pCREGq[]  PROGMEM = "+CREG?";
const char PSTR_pGSV[]  PROGMEM = "+GSV";
const char PSTR_pCIMI[]  PROGMEM = "+CIMI";

void gsm_proc_init() {
    _hal_modem_init_port();
    _Gsm_Modem.gsm_mode = 1; /// TODO
    if (_Gsm_Modem.gsm_mode) {
        printf_P(PSTR("gsm enabled: %u\n"), _Gsm_Modem.gsm_mode);
        gsm_proc_begin();
    }
}

void gsm_proc_begin() {
    __gsm_on_off(true);
    _Gsm_Modem.button                  = Pressed;
    _Gsm_Modem.counter_100ms           = 17; // pressing time >1500 ms
    _Gsm_Modem.mode                    = UNKNOWN;
    _Gsm_Modem.observe_GSM_OK          = false;
    _Gsm_Modem.observe_GSM_ERROR       = false;
    _Gsm_Modem.observe_GSM_PROMPT      = false;
    _Gsm_Modem.netreg_Event            = GSM_Unknown;
    _Gsm_Modem.ip_state                = CONN_UNKNOWN;
    _Gsm_Modem.boot_level              = 0;
    _Gsm_Modem.send_schedule_size = 0;
    _Gsm_Modem.wait_schedule_size = 0;
    _Gsm_Modem.data_accepted_size = 0;
    
    
}


volatile const char *_gsm_conn_host = NULL;
volatile const char *_gsm_conn_port = NULL;



typedef enum {
    load_buff,
    load_ipd_size,
    load_ipd_data
} dddd_t;


void gsm_proc_scan() {
    dddd_t s = load_buff;
    
    uint16_t ipd_len = 0;
    
    WsRx.count = 0;
    while (uart1_available()) {
        uint8_t __rxc1 = uart1_getc();
        
        uart0_putc(__rxc1);
        
        WsRx.buff[WsRx.count] = __rxc1;
        if (WsRx.count < sizeof(WsRx.buff)) {
            ++WsRx.count;
        } else {
            printf_P(PSTR("Scan ovf!\n"));
            WsRx.count = 0;
        }
        
        if ( WsRx.count == 5 && strstr_P((char *)&WsRx.buff, PSTR("+IPD,"))) {
            s = load_ipd_size;
            ipd_len = 0;
            WsRx.count = 0;
        }
        
        switch (s) {
            
            case load_buff:
                
                if (WsRx.count == 2 && strstr_P((char *)&WsRx.buff, PSTR("> "))) {
                    _Gsm_Modem.observe_GSM_PROMPT = true;
                    WsRx.count = 0;
                }
                
                if (WsRx.buff[WsRx.count-1] == '\n' && WsRx.buff[WsRx.count-2] == '\r') {
                    
                    if ( WsRx.count == 7 && strstr_P((char *)&WsRx.buff, PSTR("ERROR"))) {
                        _Gsm_Modem.observe_GSM_ERROR = true;
                        WsRx.count = 0;
                    }
                    
                    if ( WsRx.count == 14 && strstr_P((char *)&WsRx.buff, PSTR("CONNECT FAIL"))) {
                        ws_disconnect();
                        ws_stream_print = ws_stream_direct;
//                         _Gsm_Modem.boot_level = 0;
                        _Gsm_Modem.ip_state = CONN_IP_INITIAL;
                        gsm_proc_deactivate();
                        WsRx.count = 0;
                    }
                    
                    if ( WsRx.count == 12 && strstr_P((char *)&WsRx.buff, PSTR("CONNECT OK"))) {
                        _Gsm_Modem.ip_state = CONN_CONNECT_OK;
                        WsRx.count = 0;
                    }
                    
                    if ( WsRx.count == 8 && strstr_P((char *)&WsRx.buff, PSTR("CLOSED"))) {
                        ws_disconnect();
                        ws_stream_print = ws_stream_direct;
                        _Gsm_Modem.ip_state = CONN_IP_INITIAL;
                        gsm_proc_deactivate();
                        WsRx.count = 0;
                    }
                    

                    if (WsRx.count == 4 ) {
                        
                        if (strstr_P((char *)&WsRx.buff, PSTR("OK"))) {
                            _Gsm_Modem.observe_GSM_OK = true;
                            WsRx.count = 0;
                        }
                        
                        if (strstr_P((char *)&WsRx.buff, PSTR("AT"))) {
                            WsRx.count = 0;
                        }
                        
                    }
                    
                    if (WsRx.save_size && WsRx.save_ptr && WsRx.count > 8) {
                        WsRx.buff[WsRx.count-2] = '\0';
                        if (WsRx.save_size > WsRx.count) {
                            WsRx.save_size = WsRx.count;
                        }
                        strncpy(WsRx.save_ptr, (const char *)&WsRx.buff, WsRx.save_size);
                        WsRx.save_ptr = 0;
                        WsRx.save_size = 0;
                    } else {
                        if (WsRx.count > 2) {
                            WsRx.buff[WsRx.count-2] = '\0';
                            if (strstr_P((char *)&WsRx.buff, PSTR_CALL_READY)) {
                                gsm_handler_waste((char *)&WsRx.buff);
                            } else if (strstr_P((char *)&WsRx.buff, PSTR_SMS_READY)) {
                                gsm_handler_waste((char *)&WsRx.buff);
                            } else if (strstr_P((char *)&WsRx.buff, PSTR_NORMAL_POWER_DOWN)) {
                                gsm_handler_waste((char *)&WsRx.buff);
                            } else if (strstr_P((char *)&WsRx.buff, PSTR_UNDER_VOLTAGE_POWER_DOWN)) {
                                gsm_handler_waste((char *)&WsRx.buff);
                            } else if (strstr_P((char *)&WsRx.buff, PSTR_UNDER_VOLTAGE_WARNNING)) {
                                gsm_handler_waste((char *)&WsRx.buff);
                            } else if (strstr_P((char *)&WsRx.buff, PSTR_OVER_VOLTAGE_POWER_DOWN)) {
                                gsm_handler_waste((char *)&WsRx.buff);
                            } else if (strstr_P((char *)&WsRx.buff, PSTR_OVER_VOLTAGE_WARNNING)) {
                                gsm_handler_waste((char *)&WsRx.buff);
                            } else if (strstr_P((char *)&WsRx.buff, PSTR_STATE)) {
                                gsm_handler_STATE();
                            } else if (strstr_P((char *)&WsRx.buff, PSTR("DATA ACCEPT"))) {
                                
                                if (*(WsRx.buff + sizeof("DATA ACCEPT") - 1  ) == ':') {
                                    _Gsm_Modem.data_accepted_size = atoi(WsRx.buff + sizeof("DATA ACCEPT"));
                                    printf_P(PSTR("\ndata_accepted_size=%d\n"), _Gsm_Modem.data_accepted_size);
                                } else {
                                    _Gsm_Modem.data_accepted_size = 0;
                                    printf_P(PSTR("\nER data_accepted_size=0\n"), ipd_len);
                                }
                            } else {
                            }
                        }
                    }
                    memset(WsRx.buff, 0, sizeof(WsRx.buff));
                    WsRx.count = 0;
                }
                break;
                
                case load_ipd_size:
                    if (WsRx.buff[WsRx.count-1] == ':') {
                        ipd_len = atoi(WsRx.buff);
                        WsRx.count = 0;
                        s = load_ipd_data;
                    }
                    break;
                    
                case load_ipd_data:
                    if (ipd_len == WsRx.count) {
                        ws_receive();
                        s = load_buff;
                    }
                    break;
        }
    }
    
}

volatile bool scan_shed = false;


void gsm_proc_update_100ms() {
    
    if (scan_shed) {
        scan_shed = false;
        gsm_proc_scan();
    }
    
    if (uart1_available() > 1) { scan_shed = true;}
    if (_Gsm_Modem.counter_100ms) { --_Gsm_Modem.counter_100ms; }
    //     if (_Gsm_Modem.observe_GSM_OK)     { printf_P(PSTR("\n[OK]\n")); }
    //     if (_Gsm_Modem.observe_GSM_ERROR)  { printf_P(PSTR("\n[ERROR]\n")); }
    //     if (_Gsm_Modem.observe_GSM_PROMPT) { printf_P(PSTR("\n[PROMPT]\n")); }

    switch (_Gsm_Modem.button) {
        
        case Pressed:
            if (!_Gsm_Modem.counter_100ms) {
                __gsm_on_off(false);
                _Gsm_Modem.button = Booting;
                _Gsm_Modem.counter_100ms = 50;
            }
            break;
            
        case Booting:
            if (!_Gsm_Modem.counter_100ms) {
                _Gsm_Modem.button = NotPressed;
                _Gsm_Modem.counter_100ms = 40;
            }
            break;
            
        case NotPressed:
            
            switch (_Gsm_Modem.mode ) {
                
                case DISABLED:
                    break;
                    
                case UNKNOWN:
                    ws_stream_print = ws_stream_direct;
                    if (!_Gsm_Modem.counter_100ms) {
                        gsm_proc_begin();
                    } else if (_Gsm_Modem.observe_GSM_OK) {
                        _Gsm_Modem.boot_level = 0;
                        _Gsm_Modem.mode = AT_OK;
                    } else {
                        modem_at_P(0);
                    }
                    break;
                    
                case AT_OK: // found - init process

                        if (_Gsm_Modem.observe_GSM_OK) {
                            _Gsm_Modem.observe_GSM_OK = false;
                            ++_Gsm_Modem.boot_level;
                        }
                        
                        switch (_Gsm_Modem.boot_level) {
                            
                            case 0:
                                modem_at_P(PSTR_E0); // ATE0 Echo off
                                break;
                            case 1:
                                modem_at_P(PSTR_I); // ATI
                                WsRx.save_size = sizeof(_Gsm_Modem.info.ATI);
                                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.ATI;
                                break;
                                
                            case 2:
                                modem_at_P(PSTR_pGCAP); 
                                WsRx.save_size = sizeof(_Gsm_Modem.info.s_GCAP);
                                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.s_GCAP;
                                break;
                                
                            case 3:
                                modem_at_P(PSTR_pGMI); 
                                WsRx.save_size = sizeof(_Gsm_Modem.info.s_GMI);
                                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.s_GMI;
                                break;
                                
                            case 4:
                                modem_at_P(PSTR_pGMM); 
                                WsRx.save_size = sizeof(_Gsm_Modem.info.s_GMM);
                                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.s_GMM;
                                break;
                                
                            case 5:
                                modem_at_P(PSTR_pGMR); 
                                WsRx.save_size = sizeof(_Gsm_Modem.info.s_GMR);
                                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.s_GMR;
                                break;
                                
                            case 6:
                                modem_at_P(PSTR_pGOI); 
                                WsRx.save_size = sizeof(_Gsm_Modem.info.s_GOI);
                                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.s_GOI;
                                break;
                                
                            case 7:
                                modem_at_P(PSTR_pGSN); 
                                WsRx.save_size = sizeof(_Gsm_Modem.info.s_GSN);
                                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.s_GSN;
                                break;
                                
                            case 8:
                                modem_at_P(PSTR_pCPINq); // pin must be OK
                                break;
                                
                            case 9:
                                modem_at_P(PSTR_pCREGq); // reg must be OK
                                break;
                                
                            case 10:
                                modem_at_P(PSTR_pGSV);
                                WsRx.save_size = sizeof(_Gsm_Modem.info.s_GSV);
                                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.s_GSV;
                                break;
                                
                            case 11:
                                modem_at_P(PSTR_pCIMI);
                                WsRx.save_size = sizeof(_Gsm_Modem.info.s_CIMI);
                                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.s_CIMI;
                                break;
                                
                            case 12:


                                if (strlen((const char *)&_Gsm_Modem.info.s_GSN) > 14) {
                                    
                                    modem_strace_P(PSTR_I,      (const char *)&_Gsm_Modem.info.ATI);
                                    modem_strace_P(PSTR_pGCAP,  (const char *)&_Gsm_Modem.info.s_GCAP);  // Complete TA capabilities list
                                    modem_strace_P(PSTR_pGMI,   (const char *)&_Gsm_Modem.info.s_GMI);   // Manufacturer ID
                                    modem_strace_P(PSTR_pGMM,   (const char *)&_Gsm_Modem.info.s_GMM);   // TA model
                                    modem_strace_P(PSTR_pGMR,   (const char *)&_Gsm_Modem.info.s_GMR);   // TA revision of software release
                                    modem_strace_P(PSTR_pGOI,   (const char *)&_Gsm_Modem.info.s_GOI);   // Global object identification
                                    modem_strace_P(PSTR_pGSN,   (const char *)&_Gsm_Modem.info.s_GSN);   // Global serial number (IMEI)
                                    modem_strace_P(PSTR_pGSV,   (const char *)&_Gsm_Modem.info.s_GSV);
                                    modem_strace_P(PSTR_pCIMI,  (const char *)&_Gsm_Modem.info.s_CIMI);
                                    _Gsm_Modem.mode = ACTIVATING;
//                                     _Gsm_Modem.ip_state_prev = _Gsm_Modem.ip_state;
                                    
                                }
                                _Gsm_Modem.boot_level = 0;
                                break;
                        }
                    break;
                    
                    case ACTIVATING:

                    break;
            }
    }

    
    if (WsTx.count) {
        CONN_write_ofs();
        WsTx.count = 0;
    }
}



void gsm_proc_deactivate() {
    ws_stream_print = ws_stream_direct;
    modem_at_P(PSTR("+CIPSHUT"));
    CONN_write_ofs();
    _Gsm_Modem.ip_state = CONN_UNKNOWN;
    
}

// PARSE DATA [CLOSED]
void gsm_proc_update_1s() {

    if (_Gsm_Modem.mode == ACTIVATING ) {

        switch (_Gsm_Modem.ip_state) {
            
            case CONN_IP_INITIAL: // not actual
            case CONN_INITIAL:
                ws_stream_print = ws_stream_direct;
                modem_at_P(PSTR("+CGATT=1;+CSTT=\"internet\",\"gprs\",\"gprs\""));
                break;
                
            case CONN_IP_START:
                printf_P(PSTR("CONN_IP_START\n"));
                ws_stream_print = ws_stream_direct;
                modem_at_P(PSTR("+CIICR")); // 85sec max , traffic calculate!!
                break;
                
            case CONN_IP_GPRSACT:
                printf_P(PSTR("CONN_IP_GPRSACT\n"));
                ws_stream_print = ws_stream_direct;
                //  Get Local IP Address (AT+CIFSR ---> 100.91.34.225)
                modem_at_P(PSTR("+CIFSR"));
                WsRx.save_size = sizeof(_Gsm_Modem.info.s_IP);
                WsRx.save_ptr  = (char *)&_Gsm_Modem.info.s_IP;
                break;
                
            case CONN_IP_STATUS:
                
                if (_gsm_conn_host && _gsm_conn_port) {
                    modem_strace_P(PSTR("IP="), (const char *)&_Gsm_Modem.info.s_IP);
                    fprintf_P(modem, PSTR("AT+CIPSSL=1;+CIPQSEND=1;+CIPHEAD=1;+CIPSTART=\"TCP\",\"")); // solar-track.me\",\"543\""));
                    fprintf_P(modem, (const char *)_gsm_conn_host);
                    fprintf_P(modem, PSTR("\",\""));
                    fprintf_P(modem, (const char *)_gsm_conn_port);
                    fprintf_P(modem, PSTR("\""));
                    fprintf_P(modem, PSTR_EOL);
                    //                                     _gsm_conn_host = NULL;
                    //                                     _gsm_conn_port = NULL;
                }
                break;
                
            case CONN_CONNECTED: // not actual
            case CONN_CONNECT_OK: // << -- WORK STATE

                
                if (_Gsm_Modem.send_schedule_size && _Gsm_Modem.observe_GSM_PROMPT) {
                    _Gsm_Modem.observe_GSM_PROMPT = false;
                    _Gsm_Modem.wait_schedule_size = _Gsm_Modem.send_schedule_size;
                    _Gsm_Modem.send_schedule_size = 0;
                } else { _Gsm_Modem.wait_schedule_size = 0; }
                
                break;
                
            case CONN_CLOSED:           // not actual
            case CONN_REMOTE_CLOSING:   // not actual
            case CONN_TCP_CLOSED:
            case CONN_UDP_CLOSED:
                // scan 7 [CLOSED]
                gsm_proc_deactivate();
                break;
            case CONN_TCP_CONNECTING:
            case CONN_UDP_CONNECTING: // << -- wait
                printf_P(PSTR("Wait SSL...\n"));
                break;
                
                
            case CONN_PDP_DEACT:
                gsm_proc_deactivate();
                // DEACT - restart
                // scan 8 [SHUT OK]
                // GSM? [SHUT OK]
                break;
        }
    }
        
        

        
        
        if (_Gsm_Modem.ip_state != CONN_CONNECT_OK) {
            CONN_GET_IP_STATUS();
            printf_P(PSTR("gsm_proc_update_1s\n"));
            ws_stream_print = ws_stream_direct;
            modem_at_P(PSTR("+CIPSTATUS"));
        }

if (WsTx.count) {
    CONN_write_ofs();
    WsTx.count = 0;
}

}



