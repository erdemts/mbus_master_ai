/**
* \file            mbus_master_opt.h
* \brief           mbus_master options file
*/

/*
* ----------------------------------------------------------------------------
*  Copyright (c) 2025, Inno Technology.
*  All Rights Reserved.
*
*  This software and associated documentation files (the "Software") are
*  proprietary to Inno Technology and are provided solely under a
*  non-transferable, non-distributable license. No permission is granted
*  to any person to use, copy, modify, merge, publish, distribute, sublicense,
*  or sell copies of the Software, or to permit persons to whom the Software
*  is furnished, except as expressly authorized in writing by Inno Technology.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
*  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. IN NO EVENT SHALL
*  INNO TECHNOLOGY OR ITS AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*  FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
*  IN THE SOFTWARE.
*
*
*  This file is part of mbus master communication protocol library.
*
* Author:          Erdem Tayfun Salman
* Version:         v1.0.0
* ----------------------------------------------------------------------------
*/

#include "mbus_master/mbus_master_hardware_selection.h"

#if(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_IN223_GATEWAY)
#include "in223Gateway.h"
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_INEDGE_GATEWAY	)
#include "inEdgeGateway.h"
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_CNV750IOT_CONVERTER)
#include "cnv750iot.h"
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_WIN32_SIMULATOR)
#include "main.h"
#endif

#if(GATEWAY_TYPE == MBUS)

#ifndef MBUS_MASTER_OPT_HDR_H
#define MBUS_MASTER_OPT_HDR_H

/* Include application options */
#ifndef MBUS_MASTER_IGNORE_USER_OPTS
#include "mbus_master_opts.h"
#endif /* MBUS_MASTER_IGNORE_USER_OPTS */


/**
* \defgroup        MBUS_MASTER_OPT Configuration
* \brief           MBUS_MASTER options
* \{
*
*/

/* Debug logging configuration */
#ifndef MBUS_DEBUG_LOG_ENABLE
#define MBUS_DEBUG_LOG_ENABLE                                       0 /* 1: enable logs, 0: disable */
#endif

#ifndef GATEWAY_HARDWARE_TYPE
#define GATEWAY_HARDWARE_TYPE                                       HARDWARE_TYPE_WIN32_SIMULATOR
#endif

#ifndef MBUS_MASTER_CFG_NT750_ENABLED
#define MBUS_MASTER_CFG_NT750_ENABLED                               1UL
#endif

#ifndef MBUS_MASTER_CFG_UART_ECHO
#define MBUS_MASTER_CFG_UART_ECHO                                   1UL
#endif

/**
* \brief           Meter read try count
* \note
* 
*/
#ifndef MBUS_MASTER_CFG_READ_TRY_COUNT
#define MBUS_MASTER_CFG_READ_TRY_COUNT                               2UL
#endif

/**
* \brief           Buffer size for raw command
* \note
*
*/
#ifndef MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE
#define MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE                          528UL
#endif

/**
* \brief           Buffer size for raw command
* \note
*
*/
#ifndef MBUS_MASTER_CFG_RAW_CMD_BUFFER_SIZE
#define MBUS_MASTER_CFG_RAW_CMD_BUFFER_SIZE                          256UL
#endif

/**
* \brief           Meter E5 response wait timeout value
* \note
*
*/
#ifndef MBUS_MASTER_CFG_ACK_TIMEOUT
#define MBUS_MASTER_CFG_ACK_TIMEOUT                                     1500UL
#endif

/**
* \brief           Meter request command response timeout value
* \note
* 
*/
#ifndef MBUS_MASTER_CFG_SLAVE_RESPONSE_TIMEOUT
#define MBUS_MASTER_CFG_SLAVE_RESPONSE_TIMEOUT                           3000UL
#endif

/**
* \brief           SND_NKE command timeout value
* \note
* 
*/
#ifndef MBUS_MASTER_CFG_SND_NKE_TIMEOUT
#define MBUS_MASTER_CFG_SND_NKE_TIMEOUT                                  500UL /*!< SND_NKE command timeout in milliseconds */
#endif


/**
* \brief           command repeaat timeout value
* \note
* 
*/
#ifndef MBUS_MASTER_CFG_REPEAT_TIMEOUT
#define MBUS_MASTER_CFG_REPEAT_TIMEOUT                                  2000 /*!< Repeat command timeout in milliseconds */
#endif


/**
* \brief           Command send delay
* \note
* 
*/
#ifndef MBUS_MASTER_CFG_COMMAND_SEND_DELAY
#define MBUS_MASTER_CFG_COMMAND_SEND_DELAY                               500UL  /*!< Delay after sending a command in milliseconds */
#endif

/**
* \brief           UART baudrate for MBUS communication
* \note
* 
*/
#ifndef MBUS_MASTER_CFG_UART_BAUDRATE
#define MBUS_MASTER_CFG_UART_BAUDRATE                                    2400
#endif



/**
* \}
*/

#endif /* MBUS_MASTER_OPT_HDR_H */

#endif /* GATEWAY_TYPE == MBUS */
