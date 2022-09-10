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

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wformat-security"

volatile c_id_string37_t ID_Authorize;


// idTag String[20]  Required. IdToken is case insensitive.
// [3,"a2cdb118-94b1-46f4-8695-06b765cf77c9",{"idTagInfo":{"status":"Accepted","expiryDate":"2022-09-03T23:11:07.442Z"}}]
void Authorize(OCPP_CALL_ARGS) {

    if (call == ocpp_call_req) {
        MakeUUID((char *)&ID_Authorize);
        fprintf_P(ws_stream, PSTR_Req_Head, (char *)&ID_Authorize);
        fprintf_P(ws_stream, PSTR_Authorize);
        fprintf_P(ws_stream, PSTR_Obj_Start);
        
        ocpp_print_keyP_val(PSTR("idTag"),  true, buf, false);
        
        fprintf_P(ws_stream, PSTR_Obj_End);

    } else if (call == ocpp_call_reply) {


        if (strcmp(buf + tok[2].start, (const char *)&ID_Authorize) == 0) {

            for (uint8_t ax = 0; ax < tok[3].size; ++ax) {
                uint8_t ikey = ax*2 + 4;
                uint8_t ival = ikey + 1;
                set_zero(buf, &tok[ikey]);
                const char *key_ptr = buf + tok[ikey].start;
                if (tok[ival].type == JSMN_OBJECT && strcmp_P(key_ptr, PSTR("idTagInfo")) == 0) {
                    Tag_AuthorizationStatus_t   new_AuthStatus = Auth_Unknown;
                    uint32_t                    new_expiryDate = 0;
                    for (uint8_t nn = 0; nn < tok[ival].size; ++nn) {
                        uint8_t Zikey = nn*2 + ival + 1; //ival;
                        uint8_t Dival = Zikey + 1;
                        set_zero(buf, &tok[Zikey]);
                        set_zero(buf, &tok[Dival]);
                        const char *key_ptr = buf + tok[Zikey].start;
                        const char *val_ptr = buf + tok[Dival].start;
                        if (strcmp_P(key_ptr, PSTR("status")) == 0) {
                            if (strcmp_P(val_ptr, PSTR("Accepted")) == 0) {
                                new_AuthStatus = Auth_Accepted;
                            }
                            if (strcmp_P(val_ptr, PSTR("Blocked")) == 0) {
                                new_AuthStatus = Auth_Blocked;
                            }
                            if (strcmp_P(val_ptr, PSTR("Expired")) == 0) {
                                new_AuthStatus = Auth_Expired;
                            }
                            if (strcmp_P(val_ptr, PSTR("Invalid")) == 0) {
                                new_AuthStatus = Auth_Invalid;
                            }
                            if (strcmp_P(val_ptr, PSTR("ConcurrentTx")) == 0) {
                                new_AuthStatus = Auth_ConcurrentTx;
                            }
                        }
                        // parentIdTag // IdToken String[20] TODO
                        if (strcmp_P(key_ptr, PSTR("expiryDate")) == 0) {
                            new_expiryDate = Load_UnixTime(val_ptr);
                        }
                    }
                    if (new_expiryDate && new_AuthStatus != Auth_Unknown) {
                        printf_P(PSTR("new_expiryDate=%d new_AuthStatus=%d\n") , new_expiryDate , new_AuthStatus);
                    }
                }
            }
        }
    }
}
