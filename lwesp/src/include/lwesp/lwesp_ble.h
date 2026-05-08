/**
 * \file            lwesp_ble.h
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
#ifndef LWESP_BLE_HDR_H
#define LWESP_BLE_HDR_H

#include "lwesp/lwesp_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \ingroup         LWESP
 * \defgroup        LWESP_BLE Bluetooth Low Energy
 * \brief           Bluetooth Low Energy
 * \{
 *
 * Functions to manage BLE protocol on some of ESP devices (if hardware supports it)
 *
 * \ref LWESP_CFG_BLE must be enabled to use this feature.
 */

#if LWESP_CFG_BLE || __DOXYGEN__

/* ============================================================================ */
/* Phase 1: Core BLE - Init, Name, Scan, Advertising, Connection               */
/* ============================================================================ */

/**
 * \brief           Initialize BLE with specified role
 * \param[in]       role: BLE role. Member of \ref lwesp_ble_role_t enumeration
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_init(lwesp_ble_role_t role,
                        const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                        const uint32_t blocking);

/**
 * \brief           Deinitialize BLE
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_deinit(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                          const uint32_t blocking);

/**
 * \brief           Set BLE device name
 * \param[in]       name: Device name string (max \ref LWESP_CFG_BLE_MAX_NAME_LEN bytes)
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_set_name(const char* name,
                            const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                            const uint32_t blocking);

/**
 * \brief           Start BLE scanning
 * \param[in]       duration: Scan duration in seconds. Use 0 for continuous scan
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_scan_start(int32_t duration,
                              const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                              const uint32_t blocking);

/**
 * \brief           Stop BLE scanning
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_scan_stop(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                             const uint32_t blocking);

/**
 * \brief           Set BLE advertising data
 * \param[in]       data: Hex string of advertising data
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_adv_set_data(const char* data,
                                const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                const uint32_t blocking);

/**
 * \brief           Start BLE advertising
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_adv_start(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                             const uint32_t blocking);

/**
 * \brief           Stop BLE advertising
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_adv_stop(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                            const uint32_t blocking);

/**
 * \brief           Establish BLE connection
 * \param[in]       conn_index: Connection index (0 to \ref LWESP_CFG_BLE_MAX_CONNS - 1)
 * \param[in]       addr: Remote device MAC address
 * \param[in]       addr_type: Remote address type (0=public, 1=random)
 * \param[in]       timeout: Connection timeout in milliseconds
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_conn(uint8_t conn_index, const lwesp_mac_t* addr,
                        uint8_t addr_type, uint32_t timeout,
                        const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                        const uint32_t blocking);

/**
 * \brief           Disconnect BLE connection
 * \param[in]       conn_index: Connection index to disconnect
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_disconn(uint8_t conn_index,
                           const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                           const uint32_t blocking);

/* ============================================================================ */
/* Phase 2: GATT Server                                                         */
/* ============================================================================ */

/**
 * \brief           Create GATTS services (from service bin in flash)
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gatts_create_service(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                        const uint32_t blocking);

/**
 * \brief           Start GATTS services
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gatts_start_service(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                       const uint32_t blocking);

/**
 * \brief           Stop GATTS services
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gatts_stop_service(const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                      const uint32_t blocking);

/**
 * \brief           Send GATTS notification to client
 * \param[in]       conn_index: Connection index
 * \param[in]       srv_index: Service index
 * \param[in]       char_index: Characteristic index
 * \param[in]       data: Data to notify
 * \param[in]       len: Data length
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gatts_notify(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                                const uint8_t* data, size_t len,
                                const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                const uint32_t blocking);

/**
 * \brief           Send GATTS indication to client
 * \param[in]       conn_index: Connection index
 * \param[in]       srv_index: Service index
 * \param[in]       char_index: Characteristic index
 * \param[in]       data: Data to indicate
 * \param[in]       len: Data length
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gatts_indicate(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                                  const uint8_t* data, size_t len,
                                  const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                  const uint32_t blocking);

/**
 * \brief           Set GATTS characteristic attribute value
 * \param[in]       conn_index: Connection index
 * \param[in]       srv_index: Service index
 * \param[in]       char_index: Characteristic index
 * \param[in]       data: Data to set
 * \param[in]       len: Data length
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gatts_set_attr(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                                  const uint8_t* data, size_t len,
                                  const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                  const uint32_t blocking);

/* ============================================================================ */
/* Phase 2: GATT Client                                                         */
/* ============================================================================ */

/**
 * \brief           Discover GATTC primary services
 * \param[in]       conn_index: Connection index
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gattc_discover_primary_services(uint8_t conn_index,
                                                   const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                                   const uint32_t blocking);

/**
 * \brief           Discover GATTC characteristics
 * \param[in]       conn_index: Connection index
 * \param[in]       srv_index: Service index to discover characteristics for
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gattc_discover_chars(uint8_t conn_index, uint16_t srv_index,
                                        const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                                        const uint32_t blocking);

/**
 * \brief           Read GATTC characteristic value
 * \param[in]       conn_index: Connection index
 * \param[in]       srv_index: Service index
 * \param[in]       char_index: Characteristic index
 * \param[out]      data: Pointer to buffer to save read data
 * \param[in]       btr: Number of bytes to read
 * \param[out]      actual_len: Pointer to variable to save actual length read
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gattc_read(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                              void* data, size_t btr, size_t* actual_len,
                              const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                              const uint32_t blocking);

/**
 * \brief           Write GATTC characteristic value
 * \param[in]       conn_index: Connection index
 * \param[in]       srv_index: Service index
 * \param[in]       char_index: Characteristic index
 * \param[in]       data: Data to write
 * \param[in]       len: Data length
 * \param[in]       evt_fn: Callback function called when command has finished. Set to `NULL` when not used
 * \param[in]       evt_arg: Custom argument for event callback function
 * \param[in]       blocking: Status whether command should be blocking or not
 * \return          \ref lwespOK on success, member of \ref lwespr_t enumeration otherwise
 */
lwespr_t lwesp_ble_gattc_write(uint8_t conn_index, uint16_t srv_index, uint16_t char_index,
                               const uint8_t* data, size_t len,
                               const lwesp_api_cmd_evt_fn evt_fn, void* const evt_arg,
                               const uint32_t blocking);

#endif /* LWESP_CFG_BLE || __DOXYGEN__ */

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LWESP_BLE_HDR_H */
