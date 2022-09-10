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

#include "port/port.h"
#include "ocpp_defines.h"
#include "ocpp_data.h"
#include "ocpp_types.h"
#include "ocpp.h"
#include "ocpp_strings.h"
#include <string.h>
#include <stdlib.h>
#include "ws16_gsm/conn_sim800.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wformat-security"

volatile c_id_string37_t ID_BootNotification;

extern SIM800_Man _Gsm_Modem;

void BootNotification(OCPP_CALL_ARGS) {

    if (call == ocpp_call_req) {
        MakeUUID((char *)&ID_BootNotification);
        fprintf_P(ws_stream, PSTR_Req_Head, (char *)&ID_BootNotification);
        fprintf_P(ws_stream, PSTR_BootNotification);
        fprintf_P(ws_stream, PSTR_Obj_Start);
#if defined(CHARGE_POINT_SERIAL_NUMBER)
        ocpp_print_keyP_val(PSTR_chargePointSerialNumber,  false, (const char *)&chargePointSerialNumber,     true);
#endif
#if defined(CHARGE_BOX_SERIAL_NUMBER)
        ocpp_print_keyP_val(PSTR_chargeBoxSerialNumber,  false, (const char *)&chargeBoxSerialNumber,     true);
#endif
#if defined(FIRMWARE_VERSION)
        ocpp_print_keyP_val(PSTR_firmwareVersion,  false, (const char *)&firmwareVersion,     true);
#endif
        
#if defined(ICCID)
        ocpp_print_keyP_val(PSTR_iccid,  false, (const char *)&iccid,     true);
#else
        ocpp_print_keyP_val((const uint8_t *)PSTR_iccid,  true, (const uint8_t *)&_Gsm_Modem.info.s_GSN, true);
#endif

#if defined(IMSI)
        ocpp_print_keyP_val(PSTR_imsi,  false, (const char *)&imsi,     true);
#else
        ocpp_print_keyP_val((const uint8_t *)PSTR_imsi,  true, (const uint8_t *)&_Gsm_Modem.info.s_CIMI, true);
#endif

#if defined(METER_SERIAL_NUMBER)
        ocpp_print_keyP_val(PSTR_meterSerialNumber,  false, (const char *)&meterSerialNumber,     true);
#endif
#if defined(METER_TYPE)
        ocpp_print_keyP_val(PSTR_meterType,  false, (const char *)&meterType,     true);
#endif
        ocpp_print_keyP_val(PSTR_chargePointVendor, false, (const char *)&chargePointVendor,    true);
        ocpp_print_keyP_val(PSTR_chargePointModel,  false, (const char *)&chargePointModel,     false);
        fprintf_P(ws_stream, PSTR_Obj_End);



    } else if (call == ocpp_call_reply) {

        if (strcmp(buf + tok[2].start, (const char *)&ID_BootNotification) == 0) {

            uint32_t new_time = 0;
            uint16_t new_interval = 0;
            ChargePoint_RegistrationStatus_t new_CP_RS = CP_Unknown;

            for (uint8_t ax = 0; ax < tok[3].size; ++ax) {
                uint8_t ikey = ax*2 + 4;
                uint8_t ival = ikey + 1;
                set_zero(buf, &tok[ikey]);
                set_zero(buf, &tok[ival]);
                const char *key_ptr = buf + tok[ikey].start;
                const char *val_ptr = buf + tok[ival].start;
                if (tok[ikey].type == JSMN_STRING) {
                    if (strcmp_P(key_ptr, PSTR("status")) == 0) {
                        new_CP_RS = CP_Unknown;
                        if (strcmp_P(val_ptr, PSTR("Accepted")) == 0) {
                            new_CP_RS = CP_Accepted;
                        } else if (strcmp_P(val_ptr, PSTR("Pending")) == 0) {
                            new_CP_RS = CP_Pending;
                        } else if (strcmp_P(val_ptr, PSTR("Rejected")) == 0) {
                            new_CP_RS = CP_Rejected;
                        }

                        printf_P(PSTR("found status=%s [%d]\n"), val_ptr, new_CP_RS);

                    }
                    if (strcmp_P(key_ptr, PSTR("currentTime")) == 0) {
                        new_time = Load_UnixTime(val_ptr);
                    }
                    if (strcmp_P(key_ptr, PSTR("interval")) == 0) {
                        new_interval = atoi(val_ptr);
                    }
                }
            }
            
// new_CP_RS
            if (new_CP_RS == CP_Accepted) {
                if (new_time)       { unixtime_set(new_time); }
                if (new_interval)   { Heartbeat_Interval = new_interval; }

                printf_P(PSTR("found interval=%d\n"), new_interval);

                printf_P(PSTR("found currentTime=%u\n"), new_time);

            }
            ChargePoint_RegistrationStatus = new_CP_RS;
        }
    }
}




// This contains the field definition of the BootNotification.req PDU sent by the Charge Point to the Central System
//
// BootNotification.req
//
// Field Name                   Field Type          Card.    Description
// chargeBoxSerialNumber        CiString25Type      0..1    Optional. This contains a value that identifies the serial number of the Charge
//                                                          Box inside the Charge Point. Deprecated, will be removed in future version
// chargePointModel             CiString20Type      1..1    Required. This contains a value that identifies the model of the ChargePoint.
// chargePointSerialNumber      CiString25Type      0..1    Optional. This contains a value that identifies the serial number of the Charge Point.
// chargePointVendor            CiString20Type      1..1    Required. This contains a value that identifies the vendor of the ChargePoint.
// firmwareVersion              CiString50Type      0..1    Optional. This contains the firmware version of the Charge Point.
// iccid                        CiString20Type      0..1    Optional. This contains the ICCID of the modem’s SIM card.
// imsi                         CiString20Type      0..1    Optional. This contains the IMSI of the modem’s SIM card.
// meterSerialNumber            CiString25Type      0..1    Optional. This contains the serial number of the main power meter of the Charge Point.
// meterType                    CiString25Type      0..1    Optional. This contains the type of the main power meter of the Charge Point.



// This contains the field definition of the BootNotification.conf PDU sent by the Central System
// to the Charge Point in response to a BootNotification.req PDU. See also Boot Notification
//
// BootNotification.conf
//
// currentTime  dateTime            1..1    Required. This contains the Central System’s current time.
// interval     integer             1..1    Required. When RegistrationStatus is Accepted, this contains the heartbeat interval in seconds.
//                                          If the Central System returns something other than Accepted, the value of the interval field
//                                          indicates the minimum wait time before sending a next BootNotification request.
// status       RegistrationStatus  1..1    Required. This contains whether the Charge Point has been registered within the System Central.
//                                          RegistrationStatus = Accepted | Pending | Rejected

