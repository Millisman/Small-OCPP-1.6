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

#include "ocpp_defines.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct __attribute__((__packed__)) {
	char c[20];
} c_string20_t;

typedef struct __attribute__((__packed__)) {
	char c[25];
} c_string25_t;

typedef struct __attribute__((__packed__)) {
	char c[50];
} c_string50_t;

typedef struct __attribute__((__packed__)) {
	char c[255];
} c_string255_t;

typedef struct __attribute__((__packed__)) {
    char c[37];
} c_id_string37_t;

// UUID structure defined as mentioned in RFC4122
struct uuid_RFC4122 {
    uint32_t     time_low;
    uint16_t     time_mid;
    uint16_t     time_hi_and_version;
    uint16_t     clock_seq;
    uint8_t      node[6];
};

typedef uint8_t uuid_bytes_t[16];


typedef enum {
    CP_Unknown,
    CP_Accepted,
    CP_Pending,
    CP_Rejected
} ChargePoint_RegistrationStatus_t;

typedef enum {
    Auth_Unknown,
    Auth_Accepted,
    Auth_Blocked,
    Auth_Expired,
    Auth_Invalid,
    Auth_ConcurrentTx
} Tag_AuthorizationStatus_t;
