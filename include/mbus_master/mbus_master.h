/**
 * @file        mbus_master.h
 * @brief       M-Bus master protocol library — public API
 *
 * Zero-copy architecture with firmware-owned buffers.
 * No firmware-specific headers, no HARDWARE_TYPE switches.
 * All hardware access via mbus_master_port_t function pointers.
 *
 * @author      Erdem Tayfun Salman
 * @version     v2.0.0
 * @date        2026
 * @copyright   Copyright (c) 2025-2026, Inno Technology. All Rights Reserved.
 */
#ifndef MBUS_MASTER_H
#define MBUS_MASTER_H

#define MBUS_MASTER_VERSION_MAJOR   2
#define MBUS_MASTER_VERSION_MINOR   0
#define MBUS_MASTER_VERSION_PATCH   0

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── Include options (user can override via mbus_master_opts.h) ───── */
#ifndef MBUS_MASTER_IGNORE_USER_OPTS
#if __has_include("mbus_master_opts.h")
#include "mbus_master_opts.h"
#endif
#endif

#include "mbus_master/mbus_master_opt.h"
#include "mbus_master/mbus_master_types.h"

/* ── Port interface ────────────────────────────────────────────────── */

/**
 * @brief   Hardware port operations.
 *          Declare as const in firmware → stored in Flash, zero RAM cost.
 */
typedef struct {
    size_t   (*send)(const void *data, size_t len);     /**< Send bytes via UART */
    int      (*configure_uart)(uint32_t baudrate);      /**< Configure UART (8E1 for MBUS) */
    void     (*delay_ms)(uint32_t ms);                  /**< Non-blocking delay */
    uint32_t (*get_tick_ms)(void);                      /**< Get monotonic tick (ms) */
} mbus_master_port_t;

/* ── Configuration ─────────────────────────────────────────────────── */

/**
 * @brief   Library configuration — firmware provides all resources.
 *          Declare as const where possible → stored in Flash.
 */
typedef struct {
    struct {
        uint8_t *rx_data;           /**< Receive data buffer (firmware-owned) */
        size_t   rx_data_size;      /**< Buffer size (default 528) */
    } buf;

    const mbus_master_port_t *port; /**< Hardware port (required) */
    uint32_t baudrate;              /**< UART baudrate (default 2400) */
} mbus_master_config_t;

/* ── Library context ───────────────────────────────────────────────── */

/**
 * @brief   M-Bus master protocol context.
 *          Firmware declares this as a variable — library manages state.
 */
typedef struct {
    mbus_master_t master;               /**< Protocol state machine */
    const mbus_master_config_t *config; /**< Config pointer (4 bytes RAM) */
    struct {
        uint8_t initialized : 1;
    } flags;
} mbus_lib_t;

/* ── Public API ────────────────────────────────────────────────────── */

/**
 * @brief   Initialize M-Bus master library.
 * @param   ctx     [in,out] Library context (firmware-owned)
 * @param   config  [in] Configuration with port and buffers
 * @return  MBUS_MASTER_OK on success, negative on error
 */
mbus_master_res_t mbus_master_init(mbus_lib_t *ctx, const mbus_master_config_t *config);

/**
 * @brief   Read meter data via M-Bus protocol.
 * @param   ctx     [in,out] Library context
 * @return  MBUS_MASTER_OK on success, negative on error
 */
mbus_master_res_t mbus_master_read(mbus_lib_t *ctx);

/**
 * @brief   Send select device command to M-Bus slave.
 * @param   ctx     [in,out] Library context
 * @return  Number of bytes sent, or negative error
 */
size_t mbus_master_send_select_device(mbus_lib_t *ctx);

/**
 * @brief   Process received data from UART (called from firmware RX thread).
 *
 * Firmware reads UART circular buffer and passes data here.
 * Zero-copy: library parses data in-place.
 *
 * @param   ctx     [in,out] Library context
 * @param   data    [in] Received data pointer
 * @param   len     [in] Data length
 * @return  MBUS_MASTER_OK on success
 */
mbus_master_res_t mbus_master_input_process(mbus_lib_t *ctx,
                                            const void *data, size_t len);

/**
 * @brief   Check if master is idle (ready for new command).
 * @param   ctx     [in] Library context
 * @return  1 if idle, 0 if busy
 */
uint32_t mbus_master_is_idle(const mbus_lib_t *ctx);

/**
 * @brief   Reset master context (clear errors, set idle state).
 * @param   ctx     [in,out] Library context
 */
void mbus_master_reset_ctx(mbus_lib_t *ctx);

/* ── Utility functions ─────────────────────────────────────────────── */

const char *mbus_master_result_to_str(mbus_master_res_t result);
unsigned int mbus_master_dec_to_bcd(unsigned int dec);
unsigned int mbus_master_bcd_to_dec(unsigned int bcd);
void mbus_master_hex_array_to_string(const mbus_master_buffer_t *buf,
                                     char *out, size_t out_size);

#if MBUS_MASTER_CFG_NT750_ENABLED
#include "mbus_master/nt750.h"

mbus_master_res_t mbus_master_get(mbus_lib_t *ctx, mbus_t *nt750, nt750_cmd_t cmd);
mbus_master_res_t mbus_master_set(mbus_lib_t *ctx, mbus_t *nt750, nt750_cmd_t cmd);
#endif

#ifdef __cplusplus
}
#endif

#endif /* MBUS_MASTER_H */
