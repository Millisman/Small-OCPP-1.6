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
#include "ocpp_types.h"

#pragma GCC diagnostic ignored "-Wunused-const-variable"

// CS -> CP
char const PSTR_CancelReservation       []  PROGMEM = "CancelReservation";
char const PSTR_ChangeAvailability      []  PROGMEM = "ChangeAvailability";
char const PSTR_ChangeConfiguration     []  PROGMEM = "ChangeConfiguration";
char const PSTR_ClearCache              []  PROGMEM = "ClearCache";
char const PSTR_ClearChargingProfile    []  PROGMEM = "ClearChargingProfile";
char const PSTR_DataTransfer            []  PROGMEM = "DataTransfer";
char const PSTR_GetCompositeSchedule    []  PROGMEM = "GetCompositeSchedule";
char const PSTR_GetConfiguration        []  PROGMEM = "GetConfiguration";
char const PSTR_GetDiagnostics          []  PROGMEM = "GetDiagnostics";
char const PSTR_GetLocalListVersion     []  PROGMEM = "GetLocalListVersion";
char const PSTR_RemoteStartTransaction  []  PROGMEM = "RemoteStartTransaction";
char const PSTR_RemoteStopTransaction   []  PROGMEM = "RemoteStopTransaction";
char const PSTR_ReserveNow              []  PROGMEM = "ReserveNow";
char const PSTR_Reset                   []  PROGMEM = "Reset";
char const PSTR_SendLocalList           []  PROGMEM = "SendLocalList";
char const PSTR_SetChargingProfile      []  PROGMEM = "SetChargingProfile";
char const PSTR_TriggerMessage          []  PROGMEM = "TriggerMessage";
char const PSTR_UnlockConnector         []  PROGMEM = "UnlockConnector";
char const PSTR_UpdateFirmware          []  PROGMEM = "UpdateFirmware";

// CP -> CS
char const PSTR_Authorize               []  PROGMEM = "Authorize";
char const PSTR_BootNotification        []  PROGMEM = "BootNotification";
char const PSTR_DiagnosticsStatusNotification   []  PROGMEM = "DiagnosticsStatusNotification";
char const PSTR_FirmwareStatusNotification      []  PROGMEM = "FirmwareStatusNotification";
char const PSTR_Heartbeat               []  PROGMEM = "Heartbeat";
char const PSTR_MeterValues             []  PROGMEM = "MeterValues";
char const PSTR_StartTransaction        []  PROGMEM = "StartTransaction";
char const PSTR_StatusNotification      []  PROGMEM = "StatusNotification";
char const PSTR_StopTransaction         []  PROGMEM = "StopTransaction";

// Required ID string
const char PSTR_chargePointVendor[] PROGMEM = "chargePointVendor";
const char PSTR_chargePointModel[]  PROGMEM = "chargePointModel";

// Required
#if !defined(CHARGE_POINT_VENDOR) || !defined(CHARGE_POINT_MODEL)
#error "CHARGE_POINT_VENDOR or CHARGE_POINT_MODEL not defined!"
#endif

const c_string20_t chargePointVendor PROGMEM = { CHARGE_POINT_VENDOR "\0" };
const c_string20_t chargePointModel PROGMEM =  { CHARGE_POINT_MODEL "\0" };

#if defined(CHARGE_POINT_SERIAL_NUMBER)
char const PSTR_chargePointSerialNumber []  PROGMEM = "chargePointSerialNumber";
const c_string25_t chargePointSerialNumber  PROGMEM = { CHARGE_POINT_SERIAL_NUMBER "\0" };
#endif

#if defined(CHARGE_BOX_SERIAL_NUMBER)
char const PSTR_chargeBoxSerialNumber   []  PROGMEM = "chargeBoxSerialNumber";
const c_string25_t chargeBoxSerialNumber    PROGMEM = { CHARGE_BOX_SERIAL_NUMBER "\0" };
#endif

#if defined(FIRMWARE_VERSION)
char const PSTR_firmwareVersion []  PROGMEM = "firmwareVersion";
const c_string50_t firmwareVersion  PROGMEM = { FIRMWARE_VERSION "\0" };
#endif

char const PSTR_iccid   []  PROGMEM = "iccid";

#if defined(ICCID)
const c_string20_t  iccid   PROGMEM = { ICCID "\0" };
#endif

char const PSTR_imsi    []  PROGMEM = "imsi";

#if defined(IMSI)
const c_string20_t imsi     PROGMEM = { IMSI "\0" };
#endif

#if defined(METER_SERIAL_NUMBER)
char const PSTR_meterSerialNumber [] PROGMEM = "meterSerialNumber";
const c_string25_t meterSerialNumber PROGMEM = { METER_SERIAL_NUMBER "\0" };
#endif

#if defined(METER_TYPE)
char const PSTR_meterType   []  PROGMEM = "meterType";
const c_string25_t meterType    PROGMEM = { METER_TYPE "\0" };
#endif


char const PSTR_Req_Head        []  PROGMEM = "[2,\"%s\",\"";
char const PSTR_Response_Head   []  PROGMEM = "[3,\"%s\",\"";
char const PSTR_Error_Head      []  PROGMEM = "[4,\"%s\",\"";

char const PSTR_Obj_Start       []  PROGMEM = "\",{";
char const PSTR_Obj_End         []  PROGMEM = "}]";










