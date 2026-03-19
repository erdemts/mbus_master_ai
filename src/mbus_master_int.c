/**
 * @file        mbus_master_int.c
 * @brief       M-Bus frame parser — internal protocol processing
 * @author      Erdem Tayfun Salman
 * @version     v2.0.0
 * @copyright   Copyright (c) 2025-2026, Inno Technology. All Rights Reserved.
 */
#include "mbus_master/mbus_master.h"

/* Internal helper — declared in mbus_master_utils.c */
extern void mbus_master_add_as_char_to_buffer(mbus_master_buffer_t *buf, uint8_t byte);

void mbus_master_process(mbus_lib_t *ctx, const void *data, size_t data_len) {
    const uint8_t *d = (const uint8_t *)data;

    while (data_len > 0) {
        uint8_t ch = *d++;
        --data_len;

        if (ctx->master.state == MBUS_MASTER_STATE_WAIT_E5) {
            if (ch == MBUS_ACK) {
                ctx->master.state = MBUS_MASTER_STATE_E5_RECEIVED;
                ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
            }
        } else if (ctx->master.state == MBUS_MASTER_STATE_WAIT_SLAVE_RESPONSE) {
            switch (ctx->master.sub_state) {
            case MBUS_MASTER_SUB_STATE_WAIT_FIRST_START:
                ctx->master._parser_checksum = 0;
                if (ch == MBUS_START) {
                    mbus_master_add_as_char_to_buffer(&ctx->master.receive_buffer, ch);
                    ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_LEN;
                }
                break;

            case MBUS_MASTER_SUB_STATE_WAIT_FIRST_LEN:
                ctx->master._parser_msg_len = ch;
                mbus_master_add_as_char_to_buffer(&ctx->master.receive_buffer, ch);
                ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_SECOND_LEN;
                break;

            case MBUS_MASTER_SUB_STATE_WAIT_SECOND_LEN:
                if (ctx->master._parser_msg_len == ch) {
                    mbus_master_add_as_char_to_buffer(&ctx->master.receive_buffer, ch);
                    ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_SECOND_START;
                } else {
                    ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
                }
                break;

            case MBUS_MASTER_SUB_STATE_WAIT_SECOND_START:
                if (ch == 0x68) {
                    ctx->master._parser_msg_len = ctx->master._parser_msg_len + 6 - 4;
                    mbus_master_add_as_char_to_buffer(&ctx->master.receive_buffer, ch);
                    ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_DATA;
                } else {
                    ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
                }
                break;

            case MBUS_MASTER_SUB_STATE_WAIT_DATA:
                if (ctx->master._parser_msg_len > 2) {
                    mbus_master_add_as_char_to_buffer(&ctx->master.receive_buffer, ch);
                    ctx->master._parser_checksum += ch;
                    ctx->master._parser_msg_len--;
                } else {
                    if (ch == ctx->master._parser_checksum) {
                        mbus_master_add_as_char_to_buffer(&ctx->master.receive_buffer, ch);
                        ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_STOP;
                    } else {
                        ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
                    }
                }
                break;

            case MBUS_MASTER_SUB_STATE_WAIT_STOP:
                if (ch == MBUS_STOP) {
                    mbus_master_add_as_char_to_buffer(&ctx->master.receive_buffer, ch);
                    /* Null-terminate */
                    if (ctx->master.receive_buffer.tail < ctx->master.receive_buffer.size) {
                        ctx->master.receive_buffer.data[ctx->master.receive_buffer.tail] = 0;
                    }

                    if (ctx->master.address_mode == MBUS_MASTER_SECONDARY_ADDRESS_MODE) {
                        /* Verify slave ID from response */
                        uint32_t rid = 0;
                        uint8_t *bd = ctx->master.receive_buffer.data;
                        rid  = (uint32_t)(bd[20] - '0') << 28;
                        rid |= (uint32_t)(bd[21] - '0') << 24;
                        rid |= (uint32_t)(bd[18] - '0') << 20;
                        rid |= (uint32_t)(bd[19] - '0') << 16;
                        rid |= (uint32_t)(bd[16] - '0') << 12;
                        rid |= (uint32_t)(bd[17] - '0') << 8;
                        rid |= (uint32_t)(bd[14] - '0') << 4;
                        rid |= (uint32_t)(bd[15] - '0');

                        if (rid == ctx->master.mbus_id) {
                            ctx->master.state = MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS;
                        } else {
                            ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
                            ctx->master.state = MBUS_MASTER_STATE_SLAVE_NOT_RESPONDING;
                            ctx->master.error.invalid_slave_id = 1;
                        }
                    } else {
                        ctx->master.state = MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS;
                    }
                } else {
                    ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
                }
                break;

            default:
                ctx->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
                break;
            }
        }
    }
}
