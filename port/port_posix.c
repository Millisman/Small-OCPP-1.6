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

#include "port.h"

#if defined (__linux) || defined (__linux__) || defined (__gnu_linux__) \
|| defined (__unix) || defined (__unix__) \
|| defined (__x86_64) || defined (__x86_64__)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

char pgm_read_byte(const char * _address_short) {
    return *_address_short;
}

uint16_t pgm_read_word(const uint16_t * _address_short) {
    return *_address_short;
}

uint32_t pgm_read_dword(const uint32_t * _address_short) {
    return *_address_short;
}

void eeprom_write_block( const void * src, void* dst, size_t n ) {
    (void)src;
    (void)dst;
    (void)n;
    printf("\n%s:%d %s():\n", __FILE__, __LINE__, __func__);
}

void eeprom_read_block(  void * dest, const void * source, size_t n ) {
    (void)source;
    (void)dest;
    (void)n;
    printf("\n%s:%d %s():\n", __FILE__, __LINE__, __func__);
}

static uint64_t app_start_ms = 0;
static uint64_t last_onesec = 0;
static uint64_t last_100ms = 0;

uint64_t _internal_current_ms_stamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    return te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds;
}

void init() {
    printf("\n%s:%d %s():\n", __FILE__, __LINE__, __func__);
    app_start_ms = _internal_current_ms_stamp();
    last_onesec = app_start_ms;
    last_100ms = app_start_ms;
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    srand(time(NULL));
}


uint32_t millis() {
    return _internal_current_ms_stamp() - app_start_ms;
}

uint32_t uptime_sec() {
    return (_internal_current_ms_stamp() - app_start_ms) / 1000;
}

static int unix_time_offset = 0;

uint32_t unixtime_get() {
    uint32_t ret = time(NULL) + unix_time_offset;
    printf("\n%s:%d %s(): %u\n", __FILE__, __LINE__, __func__, ret);
    return ret;
}

void unixtime_set(uint32_t unixtime) {
    unix_time_offset = unixtime - time(NULL);
    printf("\n%s:%d %s(): %u %d\n", __FILE__, __LINE__, __func__, unixtime, unix_time_offset);
}

bool elapsed_1sec() {
    if (_internal_current_ms_stamp() - last_onesec > 1000) {
        last_onesec = _internal_current_ms_stamp();
        return true;
    }
    return false;
}

bool elapsed_100ms() {
    if (_internal_current_ms_stamp() - last_100ms > 100) {
        last_100ms = _internal_current_ms_stamp();
        return true;
    }
    return false;
}


#endif
