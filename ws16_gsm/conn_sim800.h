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

#include <stdint.h>
#include <stdbool.h>
#include "ws16.h"
#include "conn_sim800.h"


#define MODEM_BAUD_RATE     115200

typedef enum {
    NORMAL_POWER_DOWN,
    UNDER_VOLTAGE_POWER_DOWN,
    UNDER_VOLTAGE_WARNNING,
    OVER_VOLTAGE_POWER_DOWN,
    OVER_VOLTAGE_WARNNING
} SIM800_PowerEvent;

typedef enum {
    CPIN_UNKNOWN,
    CPIN_READY,       //MT is not pending for any password
    CPIN_SIM_PIN,     //MT is waiting SIM PIN to be given
    CPIN_SIM_PUK,     //MT is waiting for SIM PUK to be given
    CPIN_PH_SIM_PIN,  //ME is waiting for phone to SIM card (antitheft)
    CPIN_PH_SIM_PUK,  //ME is waiting for SIM PUK (antitheft)
    CPIN_SIM_PIN2,
    CPIN_SIM_PUK2
} SimCPIN;

typedef enum {
    GSM_NotReg = 0,     // GSM: Not reg
    GSM_RegInHomeNet,   // GSM: Reg in home net
    GSM_NetSearching,   // GSM: Net searching...
    GSM_RegDenied,      // GSM: Reg denied
    GSM_Unknown,        // GSM: Unknown
    GSM_RegInRoaming    // GSM: Reg in Roaming
} SimRegGSM;

// Query Current Connection Status
typedef enum {
    CONN_UNKNOWN,
    CONN_IP_INITIAL,
    CONN_INITIAL,
    CONN_IP_START,
    CONN_IP_CONFIG,
    CONN_IP_GPRSACT,
    CONN_IP_STATUS,
    CONN_TCP_CONNECTING,
    CONN_UDP_CONNECTING,
    CONN_CONNECTING,
    CONN_SERVER_LISTENING,
    CONN_LISTENING,
    CONN_CONNECT_OK,
    CONN_TCP_CLOSING,
    CONN_UDP_CLOSING,
    CONN_CLOSING,
    CONN_TCP_CLOSED,
    CONN_UDP_CLOSED,
    CONN_CLOSED,
    CONN_PDP_DEACT,
    CONN_CONNECTED,
    CONN_IP_PROCESSING,
    CONN_REMOTE_CLOSING,
    CONN_OPENING,
//     NOT_CONN_TRAP
} SimConnStatus;

typedef enum {
    NotPressed,
    Pressed,
    Booting
} PowerKey;

typedef enum {
    UNKNOWN,
    DISABLED,
    AT_OK,
    ACTIVATING
} ModemStatus;

typedef struct __attribute__((__packed__)) {
    char ATI[24];
    char s_GCAP[24];
    char s_GMI[24];
    char s_GMM[24];
    char s_GMR[32];
    char s_GOI[24];
    char s_GSN[20];
    char s_GSV[20];
    char s_CIMI[20];
    char s_IP[16];
} gsm_init_info_t;

// typedef void (*GsmPowerONOFF)(bool on);

typedef struct __attribute__((__packed__)) {
    gsm_init_info_t info;
//     GsmPowerONOFF   Gsm_Power;
    PowerKey button;
    ModemStatus mode;
    bool observe_GSM_OK;
    bool observe_GSM_ERROR;
    bool observe_GSM_PROMPT;
//     bool scheduled_scan;
    uint8_t gsm_mode;
    SimConnStatus ip_state;
//     SimConnStatus ip_state_prev;
    SimRegGSM netreg_Event;
    uint8_t counter_100ms;
    uint8_t boot_level;
    uint16_t send_schedule_size;
    uint16_t wait_schedule_size;
    uint16_t data_accepted_size;
} SIM800_Man;



typedef struct __attribute__((__packed__)) {
    const char   *str;
    SimConnStatus ip_status;
} gsm_ipstates_t;

// typedef struct __attribute__((__packed__)) {
//     const char *cmd;
//     void (*func)(char *, SIM800_Man *);
// } gsm_reply_t;

typedef struct __attribute__((__packed__)) {
    const char *cmd;
    char *result;
    uint8_t     size;
} gsm_first_init_t;



