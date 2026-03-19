/**
 * @file        mbus_master_utils.c
 * @brief       M-Bus master utility functions — no firmware dependencies
 * @author      Erdem Tayfun Salman
 * @version     v2.0.0
 * @copyright   Copyright (c) 2025-2026, Inno Technology. All Rights Reserved.
 */
#include "mbus_master/mbus_master.h"
#include <string.h>

void mbus_master_reset_ctx(mbus_lib_t *ctx) {
    if (ctx == NULL) return;
    if (ctx->master.receive_buffer.data != NULL) {
        memset(ctx->master.receive_buffer.data, 0,
               ctx->master.receive_buffer.size);
    }
    ctx->master.receive_buffer.tail = 0;
    ctx->master.receive_buffer.head = 0;
    ctx->master.error.all = 0;
    ctx->master.state = MBUS_MASTER_STATE_IDLE;
    ctx->master.sub_state = MBUS_MASTER_SUB_STATE_IDLE;
}

uint8_t mbus_master_get_snd_nke_type(const mbus_master_t *m) {
    if (m == NULL) return 0;
    return (m->read_type >> 2) & 0x03;
}

uint8_t mbus_master_get_slave_select_type(const mbus_master_t *m) {
    if (m == NULL) return 0;
    return (m->read_type >> 1) & 0x01;
}

uint8_t mbus_master_get_ack_type(const mbus_master_t *m) {
    if (m == NULL) return 0;
    return m->read_type & 0x01;
}

const char *mbus_master_result_to_str(mbus_master_res_t result) {
    switch (result) {
    case MBUS_MASTER_OK:                       return "OK";
    case MBUS_MASTER_ERR_E5_NOT_RECEIVED:      return "E5_NOT_RECEIVED";
    case MBUS_MASTER_ERR_SLAVE_NOT_RESPONDING: return "SLAVE_NOT_RESPONDING";
    case MBUS_MASTER_ERR_INVALID_SLAVE_ID:     return "INVALID_SLAVE_ID";
    case MBUS_MASTER_ERR_INVALID_COMMAND:      return "INVALID_COMMAND";
    case MBUS_MASTER_ERR_NOT_INITIALIZED:      return "NOT_INITIALIZED";
    case MBUS_MASTER_ERR:                      return "ERR";
    default:                                   return "UNKNOWN";
    }
}

unsigned int mbus_master_dec_to_bcd(unsigned int dec) {
    unsigned int bcd = 0;
    int shift = 0;
    while (dec > 0) {
        bcd |= (dec % 10) << (shift * 4);
        dec /= 10;
        shift++;
    }
    return bcd;
}

unsigned int mbus_master_bcd_to_dec(unsigned int bcd) {
    unsigned int dec = 0;
    unsigned int multiplier = 1;
    while (bcd > 0) {
        dec += (bcd & 0x0F) * multiplier;
        bcd >>= 4;
        multiplier *= 10;
    }
    return dec;
}

void mbus_master_add_as_char_to_buffer(mbus_master_buffer_t *buf, uint8_t byte) {
    static const char hex[] = "0123456789ABCDEF";
    if (buf == NULL || buf->data == NULL) return;

    if (buf->tail < buf->size - 1) {
        buf->data[buf->tail++] = (uint8_t)hex[byte >> 4];
    }
    if (buf->tail < buf->size - 1) {
        buf->data[buf->tail++] = (uint8_t)hex[byte & 0x0F];
    }
}

void mbus_master_hex_array_to_string(const mbus_master_buffer_t *buf,
                                     char *out, size_t out_size) {
    if (buf == NULL || out == NULL || out_size == 0) return;
    size_t copy_len = (buf->tail < out_size - 1) ? buf->tail : out_size - 1;
    memcpy(out, buf->data, copy_len);
    out[copy_len] = '\0';
}
