/**
 * @file        mbus_master.c
 * @brief       M-Bus master protocol — init, read, send commands
 *
 * No firmware headers, no HARDWARE_TYPE, no WIN32_DEBUG switches.
 * All OS/hardware operations via port function pointers.
 *
 * @author      Erdem Tayfun Salman
 * @version     v2.0.0
 * @date        2026
 * @copyright   Copyright (c) 2025-2026, Inno Technology. All Rights Reserved.
 */
#include "mbus_master/mbus_master.h"
#include "mbus_master/nt750.h"
#include <string.h>

/* MBUS protocol constants */
static const uint8_t snd_nke[5]         = { 0x10, 0x40, 0xFF, 0x3F, 0x16 };
static const uint8_t snd_nke_network[5] = { 0x10, 0x40, 0xFD, 0x3D, 0x16 };
static const uint8_t read_slave[5]      = { 0x10, 0x7B, 0xFD, 0x78, 0x16 };
static const uint8_t select_meter_53h[7] = { 0x68, 0x0B, 0x0B, 0x68, 0x53, 0xFD, 0x52 };
static const uint8_t select_meter_73h[7] = { 0x68, 0x0B, 0x0B, 0x68, 0x73, 0xFD, 0x52 };

/* Internal declarations */
extern void mbus_master_process(mbus_lib_t *ctx, const void *data, size_t len);
extern uint8_t mbus_master_get_snd_nke_type(const mbus_master_t *m);
extern uint8_t mbus_master_get_ack_type(const mbus_master_t *m);
extern uint8_t mbus_master_get_slave_select_type(const mbus_master_t *m);

mbus_master_res_t mbus_master_init(mbus_lib_t *ctx, const mbus_master_config_t *config) {
    if (ctx == NULL || config == NULL || config->port == NULL) {
        return MBUS_MASTER_ERR_NOT_INITIALIZED;
    }
    if (config->port->send == NULL || config->port->delay_ms == NULL ||
        config->port->get_tick_ms == NULL) {
        return MBUS_MASTER_ERR_NOT_INITIALIZED;
    }

    memset(ctx, 0, sizeof(mbus_lib_t));
    ctx->config = config;

    /* Set up receive buffer with firmware-owned memory */
    if (config->buf.rx_data != NULL && config->buf.rx_data_size > 0) {
        ctx->master.receive_buffer.data = config->buf.rx_data;
        ctx->master.receive_buffer.size = config->buf.rx_data_size;
    }

    /* Configure UART if port provides it */
    if (config->port->configure_uart != NULL) {
        uint32_t baud = config->baudrate ? config->baudrate : MBUS_MASTER_CFG_UART_BAUDRATE;
        config->port->configure_uart(baud);
    }

    ctx->flags.initialized = 1;
    return MBUS_MASTER_OK;
}

mbus_master_res_t mbus_master_read(mbus_lib_t *ctx) {
    if (ctx == NULL || !ctx->flags.initialized) {
        return MBUS_MASTER_ERR_NOT_INITIALIZED;
    }

    const mbus_master_port_t *port = ctx->config->port;

    mbus_master_reset_ctx(ctx);
    ctx->master.mode = MBUS_MASTER_MODE_READ;
    ctx->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE;
    ctx->master.state = MBUS_MASTER_STATE_START_READING;

    /* Switch baudrate if per-meter baudrate is set and port supports it */
    if (ctx->master.baudrate != 0 && port->configure_uart != NULL) {
        port->configure_uart(ctx->master.baudrate);
    }

    /* Send SND_NKE based on read_type */
    uint8_t snd_nke_type = mbus_master_get_snd_nke_type(&ctx->master);
    if (snd_nke_type == MBUS_MASTER_SND_NKE_TYPE_SND_NKE_NETWORK) {
        port->send(snd_nke_network, sizeof(snd_nke_network));
    } else if (snd_nke_type == MBUS_MASTER_SND_NKE_TYPE_SND_NKE) {
        port->send(snd_nke, sizeof(snd_nke));
    }

    port->delay_ms(MBUS_MASTER_CFG_COMMAND_SEND_DELAY);

    /* Send select device */
    uint8_t ack_type = mbus_master_get_ack_type(&ctx->master);
    ctx->master.state = MBUS_MASTER_STATE_WAIT_E5;
    mbus_master_send_select_device(ctx);

    /* Wait for ACK (E5) */
    if (ack_type == MBUS_MASTER_SLAVE_WAIT_ACK) {
        uint32_t start = port->get_tick_ms();
        while ((port->get_tick_ms() - start) < MBUS_MASTER_CFG_ACK_TIMEOUT) {
            if (ctx->master.state == MBUS_MASTER_STATE_E5_RECEIVED) {
                break;
            }
            port->delay_ms(10);
        }
        if (ctx->master.state != MBUS_MASTER_STATE_E5_RECEIVED) {
            ctx->master.error.ack_timeout = 1;
            return MBUS_MASTER_ERR_E5_NOT_RECEIVED;
        }
    } else {
        port->delay_ms(MBUS_MASTER_CFG_ACK_TIMEOUT);
        ctx->master.state = MBUS_MASTER_STATE_E5_RECEIVED;
    }

    /* Send read command */
    if (ctx->master.state == MBUS_MASTER_STATE_E5_RECEIVED) {
        port->delay_ms(MBUS_MASTER_CFG_COMMAND_SEND_DELAY);
        ctx->master.state = MBUS_MASTER_STATE_WAIT_SLAVE_RESPONSE;
        port->send(read_slave, sizeof(read_slave));

        uint32_t start = port->get_tick_ms();
        while ((port->get_tick_ms() - start) < MBUS_MASTER_CFG_SLAVE_RESPONSE_TIMEOUT) {
            if (ctx->master.state == MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS) {
                break;
            }
            port->delay_ms(10);
        }
    }

    if (ctx->master.state != MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS) {
        ctx->master.error.device_not_responding = 1;
        return MBUS_MASTER_ERR_SLAVE_NOT_RESPONDING;
    }

    return MBUS_MASTER_OK;
}

size_t mbus_master_send_select_device(mbus_lib_t *ctx) {
    uint8_t out_buffer[17];
    if (ctx == NULL || !ctx->flags.initialized) {
        return 0;
    }

    const uint8_t *tmpl = (mbus_master_get_slave_select_type(&ctx->master)
                           == MBUS_MASTER_SLAVE_SELECT_TYPE_53h)
                          ? select_meter_53h : select_meter_73h;

    memcpy(out_buffer, tmpl, 7);

    /* Serialize ID (little endian) */
    out_buffer[7]  = (uint8_t)(ctx->master.mbus_id & 0xFFu);
    out_buffer[8]  = (uint8_t)((ctx->master.mbus_id >> 8) & 0xFFu);
    out_buffer[9]  = (uint8_t)((ctx->master.mbus_id >> 16) & 0xFFu);
    out_buffer[10] = (uint8_t)((ctx->master.mbus_id >> 24) & 0xFFu);

    /* Wildcard */
    out_buffer[11] = 0xFFu;
    out_buffer[12] = 0xFFu;
    out_buffer[13] = 0xFFu;
    out_buffer[14] = 0xFFu;

    /* Checksum (bytes 4..14) */
    uint8_t cs = 0;
    for (uint8_t i = 4; i <= 14; ++i) {
        cs += out_buffer[i];
    }
    out_buffer[15] = cs;
    out_buffer[16] = MBUS_STOP;

    return ctx->config->port->send(out_buffer, sizeof(out_buffer));
}

mbus_master_res_t mbus_master_input_process(mbus_lib_t *ctx,
                                            const void *data, size_t len) {
    if (ctx == NULL || !ctx->flags.initialized) {
        return MBUS_MASTER_ERR_NOT_INITIALIZED;
    }
    mbus_master_process(ctx, data, len);
    return MBUS_MASTER_OK;
}

uint32_t mbus_master_is_idle(const mbus_lib_t *ctx) {
    if (ctx == NULL) return 1;
    return (ctx->master.state == MBUS_MASTER_STATE_IDLE) ? 1 : 0;
}

/* TODO: implement NT750 GET/SET command protocol */
#if MBUS_MASTER_CFG_NT750_ENABLED
mbus_master_res_t mbus_master_get(mbus_lib_t *ctx, mbus_t *nt750,
                                   nt750_cmd_t cmd) {
    (void)ctx; (void)nt750; (void)cmd;
    return MBUS_MASTER_ERR;
}

mbus_master_res_t mbus_master_set(mbus_lib_t *ctx, mbus_t *nt750,
                                   nt750_cmd_t cmd) {
    (void)ctx; (void)nt750; (void)cmd;
    return MBUS_MASTER_ERR;
}
#endif
