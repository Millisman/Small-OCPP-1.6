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

#include "ocpp_defines.h"
#include "port/port.h"
#include "ocpp_data.h"
#include "ocpp_types.h"
#include "ocpp.h"
#include "ocpp_strings.h"
#include <string.h>
#include <stdlib.h>
#include "utils/rtclib.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wformat-security"

volatile c_id_string37_t ID_StopTransaction;


// connectorId      integer     connectorId > 0 1..1 Required. This identifies which
//                                                   connector of the Charge Point is used. 
// idTag            IdToken     1..1  Required. This contains the identifier for which a transaction has to be started. 
// meterStart       integer     1..1  Required. This contains the meter value in Wh for the connector at start of the transaction. 
// reservationId    integer     0..1  Optional. This contains the id of the reservation that terminates as a result of this transaction. 
// timestamp        dateTime    1..1  Required. This contains the date and time on which the transaction is started. 

void StopTransaction(OCPP_CALL_ARGS) {
    
    if (call == ocpp_call_req) {
        MakeUUID((char *)&ID_StopTransaction);
        fprintf_P(ws_stream, PSTR_Req_Head, (char *)&ID_StopTransaction);
        fprintf_P(ws_stream, PSTR_StopTransaction);
        fprintf_P(ws_stream, PSTR_Obj_Start);
        
        fprintf_P(ws_stream, PSTR("\"transactionId\":%d,"), Last_transaction_id);
        fprintf_P(ws_stream, PSTR("\"reason\":\"%s\","), "Local");
       // fprintf_P(ws_stream, PSTR("\"transactionData\":{},"));  Optional.
        
        
        
        fprintf_P(ws_stream, PSTR("\"meterStop\":%d,"), 345); // W*h
        // fprintf_P(ws_stream, PSTR("\"reservationId\":%d,"), 777);
        ocpp_print_keyP_val(PSTR("idTag"), true, buf, true);
        
        uint32_t ut = unixtime_get();
        rtc_t rtc;
        TimeFromUnix(&ut, &rtc);
        //         20%02u-&u-%uT21:15:03.198Z
        // 20%02u-%02u-%02uT%02u:%02u:%02u.000Z
        fprintf_P(ws_stream, PSTR("\"timestamp\":\"20%02u-%02u-%02uT%02u:%02u:%02u.000Z\""),
                  rtc.yOff, rtc.m, rtc.d, rtc.hh, rtc.mm, rtc.ss);
        
        fprintf_P(ws_stream, PSTR_Obj_End);
    }
}
