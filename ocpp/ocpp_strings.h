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
#include "port/port.h"
#include "ocpp_defines.h"
#include "ocpp_types.h"

// CS -> CP
extern char const PSTR_CancelReservation       [];
extern char const PSTR_ChangeAvailability      [];
extern char const PSTR_ChangeConfiguration     [];
extern char const PSTR_ClearCache              [];
extern char const PSTR_ClearChargingProfile    [];
extern char const PSTR_DataTransfer            [];
extern char const PSTR_GetCompositeSchedule    [];
extern char const PSTR_GetConfiguration        [];
extern char const PSTR_GetDiagnostics          [];
extern char const PSTR_GetLocalListVersion     [];
extern char const PSTR_RemoteStartTransaction  [];
extern char const PSTR_RemoteStopTransaction   [];
extern char const PSTR_ReserveNow              [];
extern char const PSTR_Reset                   [];
extern char const PSTR_SendLocalList           [];
extern char const PSTR_SetChargingProfile      [];
extern char const PSTR_TriggerMessage          [];
extern char const PSTR_UnlockConnector         [];
extern char const PSTR_UpdateFirmware          [];

// CP -> CS
extern char const PSTR_Authorize               [];
extern char const PSTR_BootNotification        [];
extern char const PSTR_DiagnosticsStatusNotification   [];
extern char const PSTR_FirmwareStatusNotification      [];
extern char const PSTR_Heartbeat               [];
extern char const PSTR_MeterValues             [];
extern char const PSTR_StartTransaction        [];
extern char const PSTR_StatusNotification      [];
extern char const PSTR_StopTransaction         [];

// Required ID string
extern const char PSTR_chargePointVendor[];
extern const char PSTR_chargePointModel[];

// Required
#if !defined(CHARGE_POINT_VENDOR) || !defined(CHARGE_POINT_MODEL)
#error "CHARGE_POINT_VENDOR or CHARGE_POINT_MODEL not defined!"
#endif

extern const c_string20_t chargePointVendor;
extern const c_string20_t chargePointModel;

// Optional
#if defined(CHARGE_POINT_SERIAL_NUMBER)
extern char const PSTR_chargePointSerialNumber[];
extern const c_string25_t chargePointSerialNumber;
#endif

#if defined(CHARGE_BOX_SERIAL_NUMBER)
extern char const PSTR_chargeBoxSerialNumber[];
extern const c_string25_t chargeBoxSerialNumber;
#endif

#if defined(FIRMWARE_VERSION)
extern char const PSTR_firmwareVersion[];
extern const c_string50_t firmwareVersion;
#endif

extern char const PSTR_iccid[];

#if defined(ICCID)
extern const c_string20_t iccid;
#endif

extern char const PSTR_imsi[];

#if defined(IMSI)
extern const c_string20_t imsi;
#endif

#if defined(METER_SERIAL_NUMBER)
extern char const PSTR_meterSerialNumber[];
extern const c_string25_t meterSerialNumber;
#endif

#if defined(METER_TYPE)
extern char const PSTR_meterType[];
extern const c_string25_t meterType;
#endif

extern char const PSTR_Req_Head        [];
extern char const PSTR_Response_Head   [];
extern char const PSTR_Error_Head      [];
extern char const PSTR_Obj_Start       [];
extern char const PSTR_Obj_End         [];




