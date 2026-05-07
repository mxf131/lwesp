/**
 * \file            lwesp_ble.c
 * \brief           Bluetooth Low Energy
 */

/*
 * Copyright (c) 2024 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LwESP - Lightweight ESP-AT parser library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v1.1.2-dev
 */
#include "lwesp/lwesp_ble.h"
#include "lwesp/lwesp_private.h"

#if LWESP_CFG_BLE || __DOXYGEN__

/* ============================================================================ */
/* Phase 1: Core BLE                                                            */
/* ============================================================================ */

lwespr_t
lwesp_ble_init(lwesp_ble_role_t role, const lwesp_api_cmd_evt_fn evt_fn,
               void* const evt_arg, const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_ASSERT(role <= LWESP_BLE_ROLE_SERVER);
#if LWESP_CFG_ESP8266
    LWESP_ASSERT(esp.m.device != LWESP_DEVICE_ESP8266); /* ESP8266 does not support BLE */
#endif /* LWESP_CFG_ESP8266 */

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEINIT_SET;
    LWESP_MSG_VAR_REF(msg).msg.ble_init.role = role;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

lwespr_t
lwesp_ble_deinit(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                 const uint32_t blocking) {
    return lwesp_ble_init(LWESP_BLE_ROLE_DEINIT, evt_fn, evt_arg, blocking);
}

lwespr_t
lwesp_ble_set_name(const char* name, const lwesp_api_cmd_evt_fn evt_fn,
                   void* const evt_arg, const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_ASSERT(name != NULL);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLENAME_SET;
    LWESP_MSG_VAR_REF(msg).msg.ble_name.name = name;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 2000);
}

lwespr_t
lwesp_ble_scan_start(int32_t duration, const lwesp_api_cmd_evt_fn evt_fn,
                     void* const evt_arg, const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLESCAN;
    LWESP_MSG_VAR_REF(msg).msg.ble_scan.enable = 1;
    LWESP_MSG_VAR_REF(msg).msg.ble_scan.duration = duration;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 30000);
}

lwespr_t
lwesp_ble_scan_stop(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                    const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLESCAN;
    LWESP_MSG_VAR_REF(msg).msg.ble_scan.enable = 0;
    LWESP_MSG_VAR_REF(msg).msg.ble_scan.duration = 0;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 2000);
}

lwespr_t
lwesp_ble_adv_set_data(const char* data, const lwesp_api_cmd_evt_fn evt_fn,
                       void* const evt_arg, const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_ASSERT(data != NULL);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEADVDATA_SET;
    LWESP_MSG_VAR_REF(msg).msg.ble_adv_data.data = data;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 2000);
}

lwespr_t
lwesp_ble_adv_start(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                    const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEADVSTART;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 2000);
}

lwespr_t
lwesp_ble_adv_stop(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                   const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEADVSTOP;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 2000);
}

lwespr_t
lwesp_ble_conn(uint8_t conn_index, const lwesp_mac_t* addr, uint8_t addr_type,
               uint32_t timeout, const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
               const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_ASSERT(addr != NULL);
    LWESP_ASSERT(conn_index < LWESP_CFG_BLE_MAX_CONNS);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLECONN;
    LWESP_MSG_VAR_REF(msg).msg.ble_conn.conn_index = conn_index;
    LWESP_MEMCPY(&LWESP_MSG_VAR_REF(msg).msg.ble_conn.remote_addr, addr, sizeof(*addr));
    LWESP_MSG_VAR_REF(msg).msg.ble_conn.remote_addr_type = addr_type;
    LWESP_MSG_VAR_REF(msg).msg.ble_conn.timeout = timeout;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 10000);
}

lwespr_t
lwesp_ble_disconn(uint8_t conn_index, const lwesp_api_cmd_evt_fn evt_fn,
                  void* const evt_arg, const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_ASSERT(conn_index < LWESP_CFG_BLE_MAX_CONNS);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEDISCONN;
    LWESP_MSG_VAR_REF(msg).msg.ble_disconn.conn_index = conn_index;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

/* ============================================================================ */
/* Phase 2: GATT Server                                                         */
/* ============================================================================ */

lwespr_t
lwesp_ble_gatts_create_service(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                               const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTSSRVCRE;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

lwespr_t
lwesp_ble_gatts_start_service(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                              const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTSSRVSTART;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

lwespr_t
lwesp_ble_gatts_stop_service(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                             const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTSSRVSTOP;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

lwespr_t
lwesp_ble_gatts_notify(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                       const uint8_t* data, size_t len,
                       const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                       const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_ASSERT(data != NULL && len > 0);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTSNTFY;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.conn_index = conn_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.srv_index = srv_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.char_index = char_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.data = data;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.len = len;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

lwespr_t
lwesp_ble_gatts_indicate(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                         const uint8_t* data, size_t len,
                         const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                         const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_ASSERT(data != NULL && len > 0);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTSIND;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.conn_index = conn_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.srv_index = srv_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.char_index = char_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.data = data;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_ntfy_ind.len = len;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

lwespr_t
lwesp_ble_gatts_set_attr(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                         const uint8_t* data, size_t len,
                         const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                         const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_ASSERT(data != NULL && len > 0);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTSSETATTR;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_set_attr.conn_index = conn_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_set_attr.srv_index = srv_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_set_attr.char_index = char_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_set_attr.data = data;
    LWESP_MSG_VAR_REF(msg).msg.ble_gatts_set_attr.len = len;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

/* ============================================================================ */
/* Phase 2: GATT Client                                                         */
/* ============================================================================ */

lwespr_t
lwesp_ble_gattc_discover_primary_services(uint8_t conn_index,
                                          const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                          const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTCPRIMSRV;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_prim_srv.conn_index = conn_index;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 10000);
}

lwespr_t
lwesp_ble_gattc_discover_chars(uint8_t conn_index, uint16_t srv_index,
                               const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                               const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTCCHAR;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_char.conn_index = conn_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_char.srv_index = srv_index;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 10000);
}

lwespr_t
lwesp_ble_gattc_read(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                     const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                     const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTCRD;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_rd.conn_index = conn_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_rd.srv_index = srv_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_rd.char_index = char_index;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

lwespr_t
lwesp_ble_gattc_write(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                      const uint8_t* data, size_t len,
                      const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                      const uint32_t blocking) {
    LWESP_MSG_VAR_DEFINE(msg);

    LWESP_ASSERT(data != NULL && len > 0);

    LWESP_MSG_VAR_ALLOC(msg, blocking);
    LWESP_MSG_VAR_SET_EVT(msg, evt_fn, evt_arg);
    LWESP_MSG_VAR_REF(msg).cmd_def = LWESP_CMD_BLEGATTCWR;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_wr.conn_index = conn_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_wr.srv_index = srv_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_wr.char_index = char_index;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_wr.data = data;
    LWESP_MSG_VAR_REF(msg).msg.ble_gattc_wr.len = len;

    return lwespi_send_msg_to_producer_mbox(&LWESP_MSG_VAR_REF(msg), lwespi_initiate_cmd, 5000);
}

#endif /* LWESP_CFG_BLE || __DOXYGEN__ */
