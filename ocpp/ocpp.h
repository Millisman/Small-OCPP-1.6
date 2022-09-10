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
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "ocpp_defines.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../utils/jsmn.h"
#include "ocpp_types.h"


// void Ocpp_init();

typedef enum {
    ocpp_call_none,
    ocpp_call_req,
    ocpp_call_conf,
    ocpp_call_reply
} ocpp_call_t;



typedef enum {
    task_BootNotification,
    task_Heartbeat,
    task_Authorize,
    task_MeterValues,
    task_StartTransaction,
    task_StatusNotification,
    task_StopTransaction,
    task_DiagnosticsStatusNotification,
    task_FirmwareStatusNotification,
} ocpp_call_task;

typedef struct __attribute__((__packed__)) {
    const uint8_t *key;
    bool        ram;
    uint8_t     max;
    const uint8_t *val;
} ocpp_string_pair_t;


//  jsmntok_t *tok,
#define OCPP_CALL_ARGS ocpp_call_t call, const uint8_t *buf

typedef struct __attribute__((__packed__)) {
    const uint8_t *cmd;
    void (*func)(OCPP_CALL_ARGS);
} ocpp_conf_pair_t;

void CancelReservation(OCPP_CALL_ARGS);
void ChangeAvailability(OCPP_CALL_ARGS);
void ChangeConfiguration(OCPP_CALL_ARGS);
void ClearCache(OCPP_CALL_ARGS);
void ClearChargingProfile(OCPP_CALL_ARGS);
void DataTransfer(OCPP_CALL_ARGS);
void GetCompositeSchedule(OCPP_CALL_ARGS);
void GetConfiguration(OCPP_CALL_ARGS);
void GetDiagnostics(OCPP_CALL_ARGS);
void GetLocalListVersion(OCPP_CALL_ARGS);
void RemoteStartTransaction(OCPP_CALL_ARGS);
void RemoteStopTransaction(OCPP_CALL_ARGS);
void ReserveNow(OCPP_CALL_ARGS);
void Reset(OCPP_CALL_ARGS);
void SendLocalList(OCPP_CALL_ARGS);
void SetChargingProfile(OCPP_CALL_ARGS);
void TriggerMessage(OCPP_CALL_ARGS);
void UnlockConnector(OCPP_CALL_ARGS);
void UpdateFirmware(OCPP_CALL_ARGS);
///////////////////////////////////////////////////////////////
void Authorize(OCPP_CALL_ARGS);
void BootNotification(OCPP_CALL_ARGS);
void DataTransfer(OCPP_CALL_ARGS);
void DiagnosticsStatusNotification(OCPP_CALL_ARGS);
void FirmwareStatusNotification(OCPP_CALL_ARGS);
void Heartbeat(OCPP_CALL_ARGS);
void MeterValues(OCPP_CALL_ARGS);
void StartTransaction(OCPP_CALL_ARGS);
void StatusNotification(OCPP_CALL_ARGS);
void StopTransaction(OCPP_CALL_ARGS);

void __uuid_generate_random(uuid_bytes_t out, uint16_t* num);
void uuid_unpack(const uuid_bytes_t in, struct uuid_RFC4122 *uu);
void uuid_pack(const struct uuid_RFC4122 *uu, uuid_bytes_t ptr);
void MakeUUID(uint8_t* buf);

// void ocpp_print_string_pair(const ocpp_string_pair_t *p);
void ocpp_print_keyP_val(const uint8_t *key, bool ram, const uint8_t *val, bool comma);
void set_zero(const uint8_t *js, jsmntok_t *tok);
uint32_t Load_UnixTime(const uint8_t *tmp_c);




void Ocpp_CLIENT_CONNECTION_ERROR();
void Ocpp_CLIENT_RECEIVE(const uint8_t* buf, const uint16_t len);
void Ocpp_ESTABLISHED();
void Ocpp_IDLE();
