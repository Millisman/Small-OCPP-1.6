/**************************************************************************/
/*!
 *  @file     RTClib.h
 *
 *  @mainpage Adafruit RTClib
 *
 *  @section intro Introduction
 *
 *  This is a fork of JeeLab's fantastic real time clock library for Arduino.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing
 *  products from Adafruit!
 *
 *  @section classes Available classes
 *
 *  This library provides the following classes:
 *
 *  - Classes for manipulating dates, times and durations:
 *    - DateTime represents a specific point in time; this is the data
 *      type used for setting and reading the supported RTCs
 *  - RTC emulated in software; do not expect much accuracy out of these:
 *    - RTC_Millis is based on `millis()`
 *
 *  @section license License
 *
 *  Original library by JeeLabs https://jeelabs.org/pub/docs/rtclib/, released to
 *  the public domain.
 *
 *  This version: MIT (see LICENSE)
 */
/**************************************************************************/

#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdint.h>


// static uint8_t conv2d(const char *p);

#define SECONDS_PER_DAY 86400L ///< 60 * 60 * 24

// Unixtime for 2000-01-01 00:00:00, useful for initialization
#define SECONDS_FROM_1970_TO_2000   946684800


/**************************************************************************/
// utility code, some of this could be exposed in the DateTime API if needed
/**************************************************************************/

/**
  Number of days in each month, from January to November. December is not
  needed. Omitting it avoids an incompatibility with Paul Stoffregen's Time
  library. C.f. https://github.com/adafruit/RTClib/issues/114
*/
extern const uint8_t daysInMonth[];

/**************************************************************************/
/*!
    @brief  Given a date, return number of days since 2000/01/01,
            valid for 2000--2099
    @param y Year
    @param m Month
    @param d Day
    @return Number of days
*/
/**************************************************************************/
extern uint16_t date2days(uint16_t y, uint8_t m, uint8_t d);

/**************************************************************************/
/*!
    @brief  Given a number of days, hours, minutes, and seconds, return the
   total seconds
    @param days Days
    @param h Hours
    @param m Minutes
    @param s Seconds
    @return Number of seconds total
*/
/**************************************************************************/
extern uint32_t time2ulong(uint16_t days, uint8_t h, uint8_t m, uint8_t s);

typedef struct __attribute__((__packed__)) {
    uint8_t yOff; ///< Year offset from 2000
    uint8_t m;    ///< Month 1-12
    uint8_t d;    ///< Day 1-31
    uint8_t hh;   ///< Hours 0-23
    uint8_t mm;   ///< Minutes 0-59
    uint8_t ss;   ///< Seconds 0-59
} rtc_t;

extern uint32_t UnixTime(const rtc_t *rtc);

extern void TimeFromUnix(uint32_t *ut, rtc_t *rtc);
