/**
 * @file        mbus_master_types.h
 * @brief       M-Bus master protocol type definitions
 * @author      Erdem Tayfun Salman
 * @version     v2.0.0
 * @copyright   Copyright (c) 2025-2026, Inno Technology. All Rights Reserved.
 */
#ifndef MBUS_MASTER_TYPES_H
#define MBUS_MASTER_TYPES_H

#include <stdint.h>
#include <stddef.h>
#include "mbus_master/mbus_master_opt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── Protocol constants ────────────────────────────────────────────── */

#define MBUS_START      0x68
#define MBUS_STOP       0x16
#define MBUS_ACK        0xE5

/* ── Enumerations ──────────────────────────────────────────────────── */

typedef enum {
    MBUS_MASTER_STATE_IDLE = 0,
    MBUS_MASTER_STATE_START_READING,
    MBUS_MASTER_STATE_WAIT_E5,
    MBUS_MASTER_STATE_WAIT_SLAVE_RESPONSE,
    MBUS_MASTER_STATE_SLAVE_NOT_RESPONDING,
    MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS,
    MBUS_MASTER_STATE_E5_RECEIVED,
    MBUS_MASTER_STATE_INVALID_SLAVE_ID,
} mbus_master_state_t;

typedef enum {
    MBUS_MASTER_SUB_STATE_IDLE = 0,
    MBUS_MASTER_SUB_STATE_WAIT_FIRST_START,
    MBUS_MASTER_SUB_STATE_WAIT_FIRST_LEN,
    MBUS_MASTER_SUB_STATE_WAIT_SECOND_LEN,
    MBUS_MASTER_SUB_STATE_WAIT_SECOND_START,
    MBUS_MASTER_SUB_STATE_WAIT_DATA,
    MBUS_MASTER_SUB_STATE_WAIT_CHECKSUM,
    MBUS_MASTER_SUB_STATE_WAIT_STOP,
} mbus_master_sub_state_t;

typedef enum {
    MBUS_MASTER_SND_NKE_TYPE_SND_NKE_NETWORK = 0,
    MBUS_MASTER_SND_NKE_TYPE_SND_NKE,
    MBUS_MASTER_SND_NKE_TYPE_NONE,
} mbus_master_snd_nke_type_t;

typedef enum {
    MBUS_MASTER_SLAVE_SELECT_TYPE_53h,
    MBUS_MASTER_SLAVE_SELECT_TYPE_73h,
} mbus_master_slave_select_type_t;

typedef enum {
    MBUS_MASTER_SLAVE_WAIT_ACK = 0,
    MBUS_MASTER_SLAVE_DONT_WAIT_ACK,
} mbus_master_ack_type_t;

typedef enum {
    MBUS_MASTER_OK                       =  0,
    MBUS_MASTER_SLAVE_RESPONSE_SUCCESS   =  0,
    MBUS_MASTER_ERR_E5_NOT_RECEIVED      = -1,
    MBUS_MASTER_ERR_SLAVE_NOT_RESPONDING = -2,
    MBUS_MASTER_ERR_INVALID_SLAVE_ID     = -3,
    MBUS_MASTER_ERR_INVALID_COMMAND      = -4,
    MBUS_MASTER_ERR_NOT_INITIALIZED      = -5,
    MBUS_MASTER_ERR                      = -6,
} mbus_master_res_t;

typedef enum {
    MBUS_MASTER_PRIMARY_ADDRESS_MODE = 0,
    MBUS_MASTER_SECONDARY_ADDRESS_MODE,
} mbus_master_address_mode_t;

typedef enum {
    MBUS_MASTER_MODE_READ = 0,
    MBUS_MASTER_MODE_GET,
    MBUS_MASTER_MODE_SET,
} mbus_master_mode_t;

/* ── Error structure ───────────────────────────────────────────────── */

typedef union {
    uint32_t all;
    struct {
        unsigned unknown                 : 1;
        unsigned short_circuit           : 1;
        unsigned over_current            : 1;
        unsigned uart_error              : 1;
        unsigned converter_not_responding: 1;
        unsigned ack_timeout             : 1;
        unsigned device_not_responding   : 1;
        unsigned invalid_slave_id        : 1;
        unsigned reserved                : 24;
    };
} mbus_master_error_t;

/* ── Data buffer (firmware-owned data pointer) ─────────────────────── */

/**
 * @brief   Receive data buffer with head/tail indices.
 *          data pointer is set from config->buf.rx_data (firmware-owned).
 */
typedef struct {
    uint8_t *data;          /**< Pointer to firmware-owned buffer */
    size_t   size;          /**< Buffer capacity */
    uint16_t tail;          /**< Write position */
    uint16_t head;          /**< Read position */
} mbus_master_buffer_t;

/* ── Receive buffer (for frame verification) ───────────────────────── */

typedef struct {
    char   *data;           /**< Pointer to firmware-owned buffer */
    size_t  size;           /**< Buffer capacity */
    size_t  len;            /**< Valid data length */
} mbus_master_recv_t;

/* ── Master protocol state ─────────────────────────────────────────── */

typedef struct {
    uint32_t                    mbus_id;            /**< MBUS ID of slave meter */
    uint8_t                     read_type;          /**< MBUS read type (SND_NKE, ACK bits) */
    uint32_t                    read_period;        /**< Read period (minutes) */
    uint8_t                     primary_addr;       /**< Primary address */
    mbus_master_error_t         error;              /**< Error flags */
    mbus_master_address_mode_t  address_mode;       /**< Addressing mode */
    mbus_master_buffer_t        receive_buffer;     /**< RX data buffer (pointer to firmware memory) */
    mbus_master_mode_t          mode;               /**< Current operation mode */
    volatile mbus_master_state_t state;             /**< Protocol state machine */
    mbus_master_sub_state_t     sub_state;          /**< Sub-state for frame parsing */
} mbus_master_t;

#ifdef __cplusplus
}
#endif

#endif /* MBUS_MASTER_TYPES_H */
