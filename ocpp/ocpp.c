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

#include "../port/port.h"
#include "../utils/jsmn.h"
#include "ws16_gsm/ws16.h"
#include "ocpp_types.h"
#include "ocpp_strings.h"
#include "ocpp_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ocpp.h"

void set_zero(const uint8_t *js, jsmntok_t *tok) {
    *((uint8_t *)js + tok->end - tok->start + tok->start) = '\0';
}

void Ocpp_CLIENT_CONNECTION_ERROR() {
    printf_P(PSTR("\nOcpp_CLIENT_CONNECTION_ERROR\n"));
}

// Authorize
// BootNotification
// DataTransfer
// DiagnosticsStatusNotification
// FirmwareStatusNotification
// Heartbeat
// MeterValues
// StartTransaction
// StatusNotification
// StopTransaction

void CancelReservation(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf CancelReservation\n")); )
    }
}


// [2,"f7d37671-a7cf-488c-a449-657b83af0c7f","ChangeAvailability",{"connectorId":0,"type":"Operative"}]
// [2,"9cf6a258-b5a8-46db-a28e-603a102a9669","ChangeAvailability",{"connectorId":0,"type":"Inoperative"}]
void ChangeAvailability(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf ChangeAvailability\n")); )
    }
}

// [2,"ef39ae51-f802-4ab4-823a-2d9c301052f9","ChangeConfiguration",{"key":"AllowOfflineTxForUnknownId","value":""}]
// [2,"9e2b9cda-5baa-4cde-840b-2c310eac273d","ChangeConfiguration",{"key":"CustomConfigurationKey","value":"Value"}]
void ChangeConfiguration(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf ChangeConfiguration\n")); )
    }
}

// [2,"f57c8d7e-cf6b-4f68-8797-88b2f34cd687","ClearCache",{}]
void ClearCache(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("handler ClearCache\n")); )
    }
}

// [2,"1a9321ca-5b2a-4a43-a308-03c9f27c9664","ClearChargingProfile",{"stackLevel":0}]
void ClearChargingProfile(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf ClearChargingProfile\n")); )
    }
}

// [2,"feeed34d-0943-4e9a-bc27-d3bc326e0d24","DataTransfer",{"vendorId":"Vendor ID","messageId":"Message ID","data":"Data TEXT"}]
void DataTransfer(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf DataTransfer\n")); )
    }
}

// [2,"a842a770-4c98-46bb-8994-9ca9f4f73ee9","GetCompositeSchedule",{"connectorId":0,"duration":123,"chargingRateUnit":"W"}]
// [2,"796618b4-3435-4fb1-918a-426fec5d1df5","GetCompositeSchedule",{"connectorId":0,"duration":789,"chargingRateUnit":"A"}]
void GetCompositeSchedule(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf GetCompositeSchedule\n")); )
    }
}

// [2,"1324400b-28ff-40ac-99ee-f57af8ad725f","GetConfiguration",{"key":["BlinkRepeat"]}]
// [2,"7d589fb2-a71f-42a6-ad64-12e3eb94a010","GetConfiguration",{"key":[
//     "AllowOfflineTxForUnknownId",   "AuthorizationCacheEnabled",
//     "AuthorizeRemoteTxRequests",    "BlinkRepeat",
//     "ChargeProfileMaxStackLevel",   "ChargingScheduleAllowedChargingRateUnit",
//     "ChargingScheduleMaxPeriods",   "ClockAlignedDataInterval",
//     "ConnectionTimeOut",            "ConnectorPhaseRotation",
//     "ConnectorPhaseRotationMaxLength",  "ConnectorSwitch3to1PhaseSupported",
//     "GetConfigurationMaxKeys",      "HeartbeatInterval",
//     "LightIntensity",   "LocalAuthListEnabled", "LocalAuthListMaxLength",
//     "LocalAuthorizeOffline",    "LocalPreAuthorize",    "MaxChargingProfilesInstalled",
//     "MaxEnergyOnInvalidId",     "MeterValueSampleInterval", "MeterValuesAlignedData",
//     "MeterValuesAlignedDataMaxLength",  "MeterValuesSampledData",   "MeterValuesSampledDataMaxLength",
//     "MeterValuesSignatureContexts", "MinimumStatusDuration",    "NumberOfConnectors",
//     "ReserveConnectorZeroSupported",    "ResetRetries", "SendLocalListMaxLength",
//     "StopTransactionOnEVSideDisconnect",    "StopTransactionOnInvalidId",
//     "StopTransactionSignatureContexts", "StopTransactionSignatureFormat",
//     "StopTxnAlignedData",   "StopTxnAlignedDataMaxLength",
//     "StopTxnSampledData",   "StopTxnSampled",   "MaxChargingProfilesInstalled",
//     "MaxDataMaxLength",     "SupportedFeatureProfiles", "SupportedFeatureProfilesMaxLength",
//     "SupportedFileTransferProtocols",   "TransactionMessageAttempts",
//     "TransactionMessageRetryInterval",  "UnlockConnectorOnEVSideDisconnect",
//     "WebSocketPingInterval",    "CUSTOM_KEY"
// ]}]
void GetConfiguration(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf GetConfiguration\n")); )
    }
}

void GetDiagnostics(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf GetDiagnostics\n")); )
    }
}

// [2,"42966b41-c6ab-4523-a659-4c239ee63eb5","GetLocalListVersion",{}]
void GetLocalListVersion(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf GetLocalListVersion\n")); )
    }
}

// [2,"da8ed36a-554d-4e5a-ae62-678ef464cc6b","RemoteStartTransaction",{"idTag":"B4A63CDF"}]
void RemoteStartTransaction(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf RemoteStartTransaction\n")); )
    }
}

void RemoteStopTransaction(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf RemoteStopTransaction\n")); )
    }
}

void ReserveNow(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf ReserveNow\n")); )
    }
}

// [2,"e481beb2-6560-4243-8d17-9b372792ef71","Reset",{"type":"Hard"}]
// [2,"bbe4e5e0-b75d-42e1-a394-9f4e3edb5312","Reset",{"type":"Soft"}]
void Reset(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf Reset\n")); )
    }
}

// [2,"8cc74b49-932c-4b62-9a93-f4cc77644d25","SendLocalList",{"listVersion":0,"localAuthorizationList":[],"updateType":"Full"}]
void SendLocalList(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf SendLocalList\n")); )
    }
}

void SetChargingProfile(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf SetChargingProfile\n")); )
    }
}

// [2,"0655d76a-e575-446c-865f-b1f822b139e8","TriggerMessage",{"requestedMessage":"MeterValues"}]
// [2,"51a74dd4-5c43-4e36-94b8-f4bdae7529a2","TriggerMessage",{"requestedMessage":"FirmwareStatusNotification","connectorId":1}]
void TriggerMessage(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf TriggerMessage\n")); )
    }
}

void UnlockConnector(OCPP_CALL_ARGS) {
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR("\nconf UnlockConnector\n")); )
    }
}

void UpdateFirmware(OCPP_CALL_ARGS) {
    LOGGING( printf_P(PSTR("\nUpdateFirmware")); )
    if (call == ocpp_call_conf) {
        LOGGING( printf_P(PSTR(".conf\n")); )
    }
    else if (call == ocpp_call_reply) {
        LOGGING( printf_P(PSTR("[reply]\n")); )
    }

}

// ------------------------------------------------------------------------
// Authorize
// BootNotification
// DataTransfer
// DiagnosticsStatusNotification
// FirmwareStatusNotification
// Heartbeat
// MeterValues
// StartTransaction
// StatusNotification
// StopTransaction

void DiagnosticsStatusNotification(OCPP_CALL_ARGS) {
    LOGGING( printf_P(PSTR("\nDiagnosticsStatusNotification\n")); )
    if (call == ocpp_call_req) {
        
    } else if (call == ocpp_call_reply) {
        
    }
}

void FirmwareStatusNotification(OCPP_CALL_ARGS) {
    LOGGING( printf_P(PSTR("\nFirmwareStatusNotification\n")); )
    if (call == ocpp_call_req) {
        
    } else if (call == ocpp_call_reply) {
        
    }
}

void MeterValues(OCPP_CALL_ARGS) {
    LOGGING( printf_P(PSTR("\nMeterValues\n")); )
    if (call == ocpp_call_conf) {
    }
}

//-------------------------------------------------------------------------

// ocpp_conf_pair_t procs[] = {
//     { PSTR_CancelReservation,      CancelReservation },
//     { PSTR_ChangeAvailability,     ChangeAvailability },
//     { PSTR_ChangeConfiguration,    ChangeConfiguration },
//     { PSTR_ClearCache,             ClearCache },
//     { PSTR_ClearChargingProfile,   ClearChargingProfile },
//     { PSTR_DataTransfer,           DataTransfer },
//     { PSTR_GetCompositeSchedule,   GetCompositeSchedule },
//     { PSTR_GetConfiguration,       GetConfiguration },
//     { PSTR_GetDiagnostics,         GetDiagnostics },
//     { PSTR_GetLocalListVersion,    GetLocalListVersion },
//     { PSTR_RemoteStartTransaction, RemoteStartTransaction },
//     { PSTR_RemoteStopTransaction,  RemoteStopTransaction },
//     { PSTR_ReserveNow,             ReserveNow },
//     { PSTR_Reset,                  Reset },
//     { PSTR_SendLocalList,          SendLocalList },
//     { PSTR_SetChargingProfile,     SetChargingProfile },
//     { PSTR_TriggerMessage,         TriggerMessage },
//     { PSTR_UnlockConnector,        UnlockConnector },
//     { PSTR_UpdateFirmware,         UpdateFirmware },
//     // -----------------------------------------------
//     { PSTR_Authorize               , Authorize },
//     { PSTR_BootNotification        , BootNotification },
//     { PSTR_DiagnosticsStatusNotification ,DiagnosticsStatusNotification },
//     { PSTR_FirmwareStatusNotification    ,FirmwareStatusNotification },
//     { PSTR_Heartbeat               , Heartbeat },
//     { PSTR_MeterValues             , MeterValues },
//     { PSTR_StartTransaction        , StartTransaction },
//     { PSTR_StatusNotification      , StatusNotification },
//     { PSTR_StopTransaction         , StopTransaction },
//     {0, 0}
// };

// [4,"7b437670-e231-4a82-bab7-b6042bf2d896","FormationViolation","The payload for action could not be deserialized",{"errorMsg":"Cannot deserialize value of type `java.util.ArrayList<ocpp.cs._2015._10.MeterValue>` from Object val..."}]

void Ocpp_CLIENT_RECEIVE(const uint8_t *buf, const uint16_t len) {
    LOGGING(
        printf_P(PSTR("\nOcpp_CLIENT_RECEIVE(%u)\n"), len);
        printf("%.*s\n\n", len, buf);
    )

    HexDump(buf, len);
    
    
    
    jsmn_init(&p);
    int r = jsmn_parse(&p, buf, len, tok, TOKENS);

    if (r < 0) {
        printf_P(PSTR("JSON_ERROR\n"));
    } else {

        if (tok[0].type == JSMN_ARRAY) {

            int8_t      Call_Type   = 0;

            if (tok[1].type == JSMN_PRIMITIVE) {
                set_zero(buf, &tok[1]);
                Call_Type = atoi(buf + tok[1].start);
            }

            const uint8_t *Call_ID     = NULL;
            if (tok[2].type == JSMN_STRING) {
                    set_zero(buf, &tok[2]);
                    Call_ID = buf + tok[2].start;
            }

            if (Call_Type == 2) { // call from CS

                if (tok[3].type == JSMN_STRING) {
                    set_zero(buf, &tok[3]);
                    
                    const char *str_in = (const char *)buf + tok[3].start;
                    if (strcmp_P( str_in, PSTR_CancelReservation) == 0) {
                        CancelReservation(ocpp_call_conf, buf);
                    } else if (strcmp_P( str_in, PSTR_ChangeAvailability) == 0) {
                        ChangeAvailability(ocpp_call_conf, buf);
                    } else if (strcmp_P( str_in, PSTR_ChangeConfiguration) == 0) { 
                        ChangeConfiguration(ocpp_call_conf, buf);
                    } else if (strcmp_P( str_in, PSTR_ClearCache) == 0) { 
                        ClearCache(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_ClearChargingProfile) == 0) { 
                        ClearChargingProfile(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_DataTransfer) == 0) { 
                        DataTransfer(ocpp_call_conf, buf);
                    } else if (strcmp_P( str_in, PSTR_GetCompositeSchedule) == 0) { 
                        GetCompositeSchedule(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_GetConfiguration) == 0) { 
                        GetConfiguration(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_GetDiagnostics) == 0) { 
                        GetDiagnostics(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_GetLocalListVersion) == 0) { 
                        GetLocalListVersion(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_RemoteStartTransaction) == 0) { 
                        RemoteStartTransaction(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_RemoteStopTransaction) == 0) { 
                        RemoteStopTransaction(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_ReserveNow) == 0) { 
                        ReserveNow(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_Reset) == 0) { 
                        Reset(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_SendLocalList) == 0) { 
                        SendLocalList(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_SetChargingProfile) == 0) { 
                        SetChargingProfile(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_TriggerMessage) == 0) { 
                        TriggerMessage(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_UnlockConnector) == 0) { 
                        UnlockConnector(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_UpdateFirmware) == 0) { 
                        UpdateFirmware(ocpp_call_conf, buf); 
                    /////////////////////////
                    } else if (strcmp_P( str_in, PSTR_Authorize ) == 0) {  
                        Authorize(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_BootNotification ) == 0) {  
                        BootNotification(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_DiagnosticsStatusNotification) == 0) { 
                        DiagnosticsStatusNotification(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_FirmwareStatusNotification ) == 0) { 
                        FirmwareStatusNotification(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_Heartbeat ) == 0) {
                        Heartbeat(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_MeterValues ) == 0) {
                        MeterValues(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_StartTransaction ) == 0) {
                        StartTransaction(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_StatusNotification ) == 0) {
                        StatusNotification(ocpp_call_conf, buf); 
                    } else if (strcmp_P( str_in, PSTR_StopTransaction ) == 0) {
                        StopTransaction(ocpp_call_conf, buf);
                    }
                }
            } else if (Call_Type == 3) { // req result
                // if (tok[3].type == JSMN_OBJECT) {
                StartTransaction(ocpp_call_reply, buf);
                BootNotification(ocpp_call_reply, buf); // tok, buf + tok[2].start
                Heartbeat(ocpp_call_reply, buf);
                Authorize(ocpp_call_reply, buf);
                StatusNotification(ocpp_call_reply, buf);
            } else if (Call_Type == 4) { // error
                set_zero(buf, &tok[3]);
                set_zero(buf, &tok[4]);
                set_zero(buf, &tok[7]);
                const uint8_t *m1 = buf + tok[3].start;
                const uint8_t *m2 = buf + tok[4].start;
                const uint8_t *m3 = buf + tok[7].start;
                printf_P(PSTR("\n[%s] ERROR %s: %s\n%s\n\n"), Call_ID, m1, m2, m3);
                // [4,"7b437670-e231-4a82-bab7-b6042bf2d896",
                // "FormationViolation",
                // "The payload for action could not be deserialized",
                // {"errorMsg":"Cannot deserialize value of type `java.util.ArrayList<ocpp.cs._2015._10.MeterValue>` from Object val..."}]
            }

        } else { printf_P(PSTR("JSON_NO_ARRAY\n")); }
    }
}

volatile uint16_t ocpp_task = 0;

void Ocpp_ESTABLISHED() {
    printf_P(PSTR("\nOcpp_ESTABLISHED\n"));
    ocpp_task |= (1 << task_BootNotification);
    ocpp_task |= (1 << task_StatusNotification);
}

void ___Ocpp_IDLE_timer();

// void Ocpp_IDLE() {}

void Ocpp_IDLE() {
//    printf("~");
    if (ocpp_task & 1 << (task_BootNotification)) {
        ocpp_task &= ~(1 << task_BootNotification);

        BootNotification(ocpp_call_req, NULL);

    } else if (ocpp_task & 1 << (task_Heartbeat)) {
        ocpp_task &= ~(1 << task_Heartbeat);

        Heartbeat(ocpp_call_req, NULL);

    } else if (ocpp_task & 1 << (task_Authorize)) {
        ocpp_task &= ~(1 << task_Authorize);

        Authorize(ocpp_call_req, "AAADDFFF");

    } else if (ocpp_task & 1 << (task_MeterValues)) {
        ocpp_task &= ~(1 << task_MeterValues);

    } else if (ocpp_task & 1 << (task_StartTransaction)) {
        ocpp_task &= ~(1 << task_StartTransaction);

        StartTransaction(ocpp_call_req, "AAADDFFF");


    } else if (ocpp_task & 1 << (task_StatusNotification)) {
        ocpp_task &= ~(1 << task_StatusNotification);

        StatusNotification(ocpp_call_req, NULL);

    } else if (ocpp_task & 1 << (task_StopTransaction)) {
        ocpp_task &= ~(1 << task_StopTransaction);

        StopTransaction(ocpp_call_req, "AAADDFFF");

    } else if (ocpp_task & 1 << (task_DiagnosticsStatusNotification)) {
        ocpp_task &= ~(1 << task_DiagnosticsStatusNotification);

    } else if (ocpp_task & 1 << (task_FirmwareStatusNotification)) {
        ocpp_task &= ~(1 << task_FirmwareStatusNotification);

    } else {
        ___Ocpp_IDLE_timer();
    }
}

static uint32_t ts_StatusNotification = 0;
static uint32_t ts_Authorize = 0;
static uint32_t prev_ut = 0;

void ___Ocpp_IDLE_timer() {
    
    if (uptime_sec() % 30 == 0) {
        ocpp_task |= (1 << task_Heartbeat);
    }

    if (uptime_sec() - ts_StatusNotification > 240) {
        ts_StatusNotification = uptime_sec();
        ocpp_task |= (1 << task_StatusNotification);
    }

    if (uptime_sec() - ts_Authorize > 120) {
        ts_Authorize = uptime_sec();
        if (Last_transaction_id == 0) {
            ocpp_task |= (1 << task_StartTransaction);
        } else {
            ocpp_task |= (1 << task_StopTransaction);
        }
    }
    if (prev_ut != uptime_sec()) {
        prev_ut = uptime_sec();
        // printf_P(PSTR(" uptime_sec=%lu\n"), prev_ut);
    }

}










// void Ocpp_init() {
// }



/*
 * 
 * 
 * 
 * req_BootNotification();
 * 
 * printf("ocpp_req_message_tail=%d\n", ocpp_req_message_tail);
 * 
 * printf("\n%s\n", ocpp_req_message);
 * 
 * ocpp_req_message_tail = 0;
 * 
 * 
 * 
 * return 0;
 * 
 * //For reset purposes, we store original argc and argv values & pointers.
 * original_argc = argc;
 * original_argv = argv;
 * 
 * int error = CP_initialize();
 * if (error > 0) {
 *    currentChargePointState         =   _CP_STATUS_FAULTED;
 *    currentChargePointErrorStatus   =   _CP_ERROR_INTERNALERROR;
 *    return error;
 * }
 * 
 * lwsl_user("LWS minimal ws client\n");
 * signal(SIGINT, sigint_handler);
 * 
 * struct lws_context_creation_info info;
 * memset(&info, 0, sizeof info);
 * lws_cmdline_option_handle_builtin(argc, argv, &info);
 * 
 * 
 * info.fd_limit_per_thread = 1 + 1 + 1;
 * info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
 * info.port = CONTEXT_PORT_NO_LISTEN; // we do not run any server
 // info.protocols = protocols;
 // 
 // ssl_connection &= ~LCCSCF_USE_SSL;
 // ssl_connection |= LCCSCF_ALLOW_SELFSIGNED;
 // ssl_connection |= LCCSCF_ALLOW_INSECURE;
 // ssl_connection |= LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
 // ssl_connection |= LCCSCF_ALLOW_EXPIRED;
 // 
 // context = lws_create_context(&info);
 // if (!context) {
 //     lwsl_err("lws init failed\n");
 //     return 1;
 // }
 // 
 //     Creamos un thread que manda un heartbeat cada 'heartbeat' microsegundos
 // pthread_t pidheartbeat;
 // struct pthread_routine_tool tool;
 // tool.wsi = mco.wsi;
 // tool.context = context;
 // 
 // pthread_create(&pidheartbeat, NULL, sendHeartbeat, &tool);
 // pthread_detach(pidheartbeat);
 // 
 // sendToDisplay("Initializing...");
 // 
 // 
 // 
 // int n = 0;
 // schedule the first client connection attempt to happen immediately
 // lws_sul_schedule(context, 0, &mco.sul, connect_client, 1);
 // while (n >= 0 && !interrupted) { n = lws_service(context, 0); }
 // lws_context_destroy(context);
 // lwsl_user("Completed\n");
 // return 0;*/
 
 
