/* //device/libs/telephony/ril_commands.h
**
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
    {RIL_OEM_REQUEST_BASE, NULL},
    {RIL_REQUEST_DIAL_EMERGENCY_CALL, NULL}, // 10001
    {RIL_REQUEST_CALL_DEFLECTION, NULL}, // 10002
    {RIL_REQUEST_MODIFY_CALL_INITIATE, NULL}, // 10003
    {RIL_REQUEST_SET_IMS_CALL_LIST, NULL}, // 10004
    {RIL_REQUEST_SET_VOICE_DOMAIN_PREF, NULL}, // 10005
    {RIL_REQUEST_SAFE_MODE, NULL}, // 10006
    {RIL_REQUEST_SET_TRANSMIT_POWER, NULL}, // 10007
    {RIL_REQUEST_GET_CELL_BROADCAST_CONFIG, NULL}, // 10008
    {RIL_REQUEST_GET_PHONEBOOK_STORAGE_INFO, NULL}, // 10009
    {RIL_REQUEST_GET_PHONEBOOK_ENTRY, NULL}, // 10010
    {RIL_REQUEST_ACCESS_PHONEBOOK_ENTRY, NULL}, // 10011
    {RIL_REQUEST_USIM_PB_CAPA, NULL}, // 10012
    {RIL_REQUEST_LOCK_INFO, NULL}, // 10013
    {RIL_REQUEST_STK_SIM_INIT_EVENT, NULL}, // 10014
    {RIL_REQUEST_SET_PREFERRED_NETWORK_LIST, NULL}, // 10015
    {RIL_REQUEST_GET_PREFERRED_NETWORK_LIST, NULL}, // 10016
    {RIL_REQUEST_CHANGE_SIM_PERSO, NULL}, // 10017
    {RIL_REQUEST_ENTER_SIM_PERSO, NULL}, // 10018
    {RIL_REQUEST_SEND_ENCODED_USSD, NULL}, // 10019
    {RIL_REQUEST_CDMA_SEND_SMS_EXPECT_MORE, NULL}, // 10020
    {10021, NULL},
    {RIL_REQUEST_HOLD, NULL}, // 10022
    {RIL_REQUEST_SET_SIM_POWER, NULL}, // 10023
    {RIL_REQUEST_GET_ACB_INFO, NULL}, // 11024
    {RIL_REQUEST_UICC_GBA_AUTHENTICATE_BOOTSTRAP, NULL}, // 10025
    {RIL_REQUEST_UICC_GBA_AUTHENTICATE_NAF, NULL}, // 10026
    {RIL_REQUEST_GET_INCOMING_COMMUNICATION_BARRING, NULL}, // 10027
    {RIL_REQUEST_SET_INCOMING_COMMUNICATION_BARRING, NULL}, // 10028
    {RIL_REQUEST_QUERY_CNAP, NULL}, // 10029
    {RIL_REQUEST_SET_TRANSFER_CALL, NULL}, // 10030
    {RIL_REQUEST_GET_DISABLE_2G, NULL}, // 11031
    {RIL_REQUEST_SET_DISABLE_2G, NULL}, // 11032
    {RIL_REQUEST_REFRESH_NITZ_TIME, NULL}, // 11033
    {RIL_REQUEST_ENABLE_UNSOL_RESPONSE, NULL}, // 11034
    {RIL_REQUEST_CANCEL_TRANSFER_CALL, NULL}, // 11035
    {RIL_REQUEST_SIM_OPEN_CHANNEL_WITH_P2, NULL}, // 11036
    {RIL_REQUEST_ACKNOWLEDGE_RIL_CONNECTED, NULL}, // 11037
    {RIL_REQUEST_EMERGENCY_SEARCH, NULL}, // 11038
    {RIL_REQUEST_EMERGENCY_CONTROL, NULL}, // 11039
    {RIL_REQUEST_SIM_GET_ATR, NULL}, // 11040