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

volatile c_id_string37_t ID_StatusNotification;

// connectorId   integer connectorId >= 0   1..1    Required. The id of the connector for which the status is reported.
//                                                  Id '0' (zero) is used if the status is for the Charge Point main controller.
// errorCode        ChargePointErrorCode    1..1    Required. This contains the error code reported by the Charge Point.
// info             CiString50Type          0..1    Optional. Additional free format information related to the error.
// status           ChargePointStatus       1..1    Required. This contains the current status of the Charge Point.
// timestamp        dateTime                0..1    Optional. The time for which the status is reported.
//                                                            If absent time of receipt of the message will be assumed.
// vendorId         CiString255Type         0..1    Optional. This identifies the vendor-specific implementation.
// vendorErrorCode  CiString50Type          0..1    Optional. This contains the vendor-specific error code.

// * * * ChargePointStatus
// Available        When a Connector becomes available for a new user (Operative) 
// Preparing        When a Connector becomes no longer available for a new user but no charging session is active.
//                  Typically a Connector is occupied when a user presents a tag, inserts a cable or a vehicle occupies the parking bay (Operative) 
// Charging         When the contactor of a Connector closes, allowing the vehicle to charge (Operative) 
// SuspendedEVSE    When the contactor of a Connector opens upon request of the EVSE, e.g. due to a smart charging restriction or
//                  as the result of StartTransaction.conf indicating that charging is not allowed (Operative) 
// SuspendedEV      When the EVSE is ready to deliver energy but contactor is open, e.g. the EV is not ready. 
// Finishing        When a charging session has stopped at a Connector, but the Connector is not yet available for a new user,
//                  e.g. the cable has not been removed or the vehicle has not left the parking bay (Operative) 
// Reserved         When a Connector becomes reserved as a result of a Reserve Now command (Operative) 
// Unavailable      When a Connector becomes unavailable as the result of a Change Availability command or an event upon
//                  which the Charge Point transitions to unavailable at its discretion. Upon receipt of a Change Availability command,
//                  the status MAY change immediately or the change MAY be scheduled. When scheduled, the Status Notification shall be send
//                  when the availability change becomes effective (Inoperative) 
// Faulted          When a Charge Point or connector has reported an error and is not available for energy delivery . (Inoperative). 

// * * * ChargePointErrorCode
// ConnectorLockFailure     Failure to lock or unlock connector. 
// EVCommunicationError     Communication failure with the vehicle, might be Mode 3 or other communication protocol problem.
//                          This is not a real error in the sense that the Charge Point doesn’t need to go to the faulted state.
//                          Instead, it should go to the SuspendedEVSE state. 
// GroundFailure            Ground fault circuit interrupter has been activated. 
// HighTemperature          Temperature inside Charge Point is too high. 
// InternalError            Error in internal hard- or software component. 
// LocalListConflict        The authorization information received from the Central System is in conflict with the LocalAuthorizationList. 
// NoError                  No error to report. 
// OtherError               Other type of error. More information in vendorErrorCode. 
// OverCurrentFailure       Over current protection device has tripped. 
// OverVoltage              Voltage has risen above an acceptable level. 
// PowerMeterFailure        Failure to read power meter. 
// PowerSwitchFailure       Failure to control power switch. 
// ReaderFailure            Failure with idTag reader. 
// ResetFailure             Unable to perform a reset. 
// UnderVoltage             Voltage has dropped below an acceptable level. 
// WeakSignal               Wireless communication device reports a weak signal. 


void StatusNotification(OCPP_CALL_ARGS) {
    
    if (call == ocpp_call_req) {
        MakeUUID((char *)&ID_StatusNotification);
        fprintf_P(ws_stream, PSTR_Req_Head, (char *)&ID_StatusNotification);
        fprintf_P(ws_stream, PSTR_StatusNotification);
        fprintf_P(ws_stream, PSTR_Obj_Start);
        

        fprintf_P(ws_stream, PSTR("\"connectorId\":%d,"), 0);
        fprintf_P(ws_stream, PSTR("\"errorCode\":\"%s\","),         "NoError");
        fprintf_P(ws_stream, PSTR("\"info\":\"%s\","),              "NoInfo");
        fprintf_P(ws_stream, PSTR("\"status\":\"%s\","),            "Available");
        // fprintf_P(ws_stream, PSTR("\"transactionData\":{},"));  Optional.
        fprintf_P(ws_stream, PSTR("\"vendorId\":\"%s\","),          "vendorId"); // 255 max
        fprintf_P(ws_stream, PSTR("\"vendorErrorCode\":\"%s\","),   "vendorErrorCode"); // 50 max


        uint32_t ut = unixtime_get();
        rtc_t rtc;
        TimeFromUnix(&ut, &rtc);
        fprintf_P(ws_stream, PSTR("\"timestamp\":\"20%02u-%02u-%02uT%02u:%02u:%02u.000Z\""),
                  rtc.yOff, rtc.m, rtc.d, rtc.hh, rtc.mm, rtc.ss);

        fprintf_P(ws_stream, PSTR_Obj_End);
    }
}
