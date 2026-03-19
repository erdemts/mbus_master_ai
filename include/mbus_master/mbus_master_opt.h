/**
 * @file        mbus_master_opt.h
 * @brief       M-Bus master default configuration options
 * @author      Erdem Tayfun Salman
 * @version     v2.0.0
 * @copyright   Copyright (c) 2025-2026, Inno Technology. All Rights Reserved.
 */
#ifndef MBUS_MASTER_OPT_H
#define MBUS_MASTER_OPT_H

#ifndef MBUS_MASTER_CFG_NT750_ENABLED
#define MBUS_MASTER_CFG_NT750_ENABLED           0
#endif

#ifndef MBUS_MASTER_CFG_READ_TRY_COUNT
#define MBUS_MASTER_CFG_READ_TRY_COUNT          2
#endif

#ifndef MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE
#define MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE     528
#endif

#ifndef MBUS_MASTER_CFG_RAW_CMD_BUFFER_SIZE
#define MBUS_MASTER_CFG_RAW_CMD_BUFFER_SIZE     256
#endif

#ifndef MBUS_MASTER_CFG_ACK_TIMEOUT
#define MBUS_MASTER_CFG_ACK_TIMEOUT             1500
#endif

#ifndef MBUS_MASTER_CFG_SLAVE_RESPONSE_TIMEOUT
#define MBUS_MASTER_CFG_SLAVE_RESPONSE_TIMEOUT  3000
#endif

#ifndef MBUS_MASTER_CFG_SND_NKE_TIMEOUT
#define MBUS_MASTER_CFG_SND_NKE_TIMEOUT         500
#endif

#ifndef MBUS_MASTER_CFG_REPEAT_TIMEOUT
#define MBUS_MASTER_CFG_REPEAT_TIMEOUT          2000
#endif

#ifndef MBUS_MASTER_CFG_COMMAND_SEND_DELAY
#define MBUS_MASTER_CFG_COMMAND_SEND_DELAY      500
#endif

#ifndef MBUS_MASTER_CFG_UART_BAUDRATE
#define MBUS_MASTER_CFG_UART_BAUDRATE           2400
#endif

#endif /* MBUS_MASTER_OPT_H */
