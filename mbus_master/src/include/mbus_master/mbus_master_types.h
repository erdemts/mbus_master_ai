/**
* \file            mbus_master.h
* \brief           mbus master library
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

#ifndef MBUS_MASTER_TYPES_HDR_H
#define MBUS_MASTER_TYPES_HDR_H

/* Get most important include files */
#include <stdint.h>
#include <stdlib.h>
#include "mbus_master/mbus_master_opt.h"

#if (MBUS_MASTER_CFG_NT750_ENABLED)
#include "nt750/mbus_aux.h"
#endif


/*****************************************************************************************************************************************
* MBUS based definitions
*****************************************************************************************************************************************/
#define MBUS_START                                                   0x68 
#define MBUS_STOP                                                    0x16
#define MBUS_ACK                                                     0xE5


/**
* \brief           MBUS master states
*/
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

/**
 * \brief           MBUS master sub states
 */
typedef enum {
	MBUS_MASTER_SUB_STATE_IDLE = 0,
	MBUS_MASTER_SUB_STATE_WAIT_FIRST_START,
	MBUS_MASTER_SUB_STATE_WAIT_FIRST_LEN,
	MBUS_MASTER_SUB_STATE_WAIT_SECOND_LEN,
	MBUS_MASTER_SUB_STATE_WAIT_SECOND_START,
	MBUS_MASTER_SUB_STATE_WAIT_DATA,
	MBUS_MASTER_SUB_STATE_WAIT_CHECKSUM,
	MBUS_MASTER_SUB_STATE_WAIT_STOP
} mbus_master_sub_state_t;

/**
* \brief           MBUS master state machine
 */
typedef enum {
	MBUS_MASTER_SND_NKE_TYPE_SND_NKE_NETWORK = 0,                    /*!< SND_NKE Write command */
	MBUS_MASTER_SND_NKE_TYPE_SND_NKE,                                /*!< SND_NKE Read command */
	MBUS_MASTER_SND_NKE_TYPE_NONE,                                   /*!< No SND_NKE command */
} mbus_master_snd_nke_type_t;

/**
 * \brief           MBUS master slave select types
 * \details         This enum defines the types of slave selection for the MBUS master.
 *                  It is used to specify which slave to select based on the slave ID.
 */
typedef enum {
	MBUS_MASTER_SLAVE_SELECT_TYPE_53h,                               /*!< Select slave with 0x53h */
	MBUS_MASTER_SLAVE_SELECT_TYPE_73h,                               /*!< Select slave with 0x73h */
} mbus_master_slave_select_type_t;

typedef enum {
	MBUS_MASTER_SLAVE_WAIT_ACK = 0,                                  /*!< Slave acknowledged the command */
	MBUS_MASTER_SLAVE_DONT_WAIT_ACK,                                 /*!< Slave did not acknowledge the command */
} mbus_master_ack_type_t;

/** 
* \brief           MBUS master response types
*/	
typedef enum {
	MBUS_MASTER_OK = 0,
	MBUS_MASTER_SLAVE_RESPONSE_SUCCESS = 0,                          /*!< Slave response success response type */
	MBUS_MASTER_ERR_E5_NOT_RECEIVED = -1,                            /*!< E5 not received response type */
	MBUS_MASTER_ERR_SLAVE_NOT_RESPONDING = -2,                       /*!< Slave not responding response type */
	MBUS_MASTER_ERR_INVALID_SLAVE_ID = -3,                           /*!< Invalid slave id */
	MBUS_MASTER_ERR_INVALID_COMMAND = -4,                            /*!< Invalid command response type */
	MBUS_MASTER_ERR_NOT_INITIALIZED = -5,                            /*!< Not initialized response type */
	MBUS_MASTER_ERR = -6,                                            /*!< Generic error response type */
} mbus_master_res_t;

/**
* \brief           MBUS master addressing modes
* \details         This enum defines the addressing modes for the MBUS master protocol.
*                  It is used to specify how the MBUS master addresses the slave devices.
*/
typedef enum {
	MBUS_MASTER_PRIMARY_ADDRESS_MODE = 0, /*!< Primary addressing mode. This mode is used for single device communication. */
	MBUS_MASTER_SECONDARY_ADDRESS_MODE,   /*!< Secondary addressing mode. This mode is used for multiple devices communication. */
}mbus_master_address_mode_t;

/**
* \brief           MBUS master modes
* \details         This enum defines the modes for the MBUS master protocol.
*                  It is used to specify the type of operation being performed on the MBUS slave device.
*/
typedef enum {
	MBUS_MASTER_MODE_READ = 0,                                       /*!< Standard MBUS read mode */
	MBUS_MASTER_MODE_GET,                                            /*!< MBUS get mode, used for reading raw data from the slave meter */
	MBUS_MASTER_MODE_SET,                                            /*!< MBUS set mode, used for writing raw data to the slave meter */
} mbus_master_mode_t;

/**
* \brief           Receive character structure to handle full line terminated with `\n` character
*/
typedef struct {
	char data[MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE];                  /*!< Received characters */
	size_t len;                                                      /*!< Length of valid characters */
} mbus_master_recv_t;

/* MBUS Master Buffer */
typedef struct {
	uint8_t data[MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE];
	uint16_t tail;
	uint16_t head;
} mbus_master_buffer_t;

/**
* \brief           MBUS Error codes
*/
typedef union {
	uint32_t all;
	struct {
		unsigned unknown : 1;                                        /*!< Unknown error code */
		unsigned short_circuit : 1;                                  /*!< Short circuit error code */
		unsigned over_current : 1;                                   /*!< Over current error code */
		unsigned uart_error : 1;                                     /*!< UART error code */
		unsigned converter_not_responding : 1;                       /*!< Converter not responding error code */
		unsigned ack_timeout : 1;                                    /*!< ACK timeout error code */
		unsigned device_not_responding : 1;                          /*!< Device not responding error code */
		unsigned invalid_slave_id : 1;                             /*!< Invalid slave ID error code */
		unsigned reserved : 24;                                      /*!< Reserved bits */
	};
}mbus_master_error_t;

/**
	 * \brief           Function prototype for serial output data
	 * \param[in]       data: Pointer to data to send. This parameter can be set to `NULL`
	 * \param[in]       len: Number of bytes to send.
	 * \return          Number of bytes sent
	 */
typedef size_t(*mbus_master_ll_send_fn)(const void* data, size_t len);

/**
 * \brief           Function prototype for configuring UART
 * \param[in]       baudrate: Baudrate value
 * \return          1 if success, 0 otherwise
 */
typedef uint8_t(*mbus_master_ll_configure_uart_fn)(uint32_t baudrate);

/**
* \ingroup         MBUS_MASTER_LL
* \brief           Low level user specific functions
*/
typedef struct {
	mbus_master_ll_send_fn send_fn;                                 /*!< Callback function to transmit data */
	mbus_master_ll_configure_uart_fn configure_uart_fn;             /*!< Callback function to configure UART */

	struct {
		uint32_t baudrate;                                       /*!< UART baudrate value */
	} uart;                                                      /*!< Flag indicating if low-level is initialized */

	union {
		struct {
			uint8_t initialized : 1;                             /*!< Flag indicating fota library is initialized */
		} f;                                                     /*!< Flags structure */
	} status;                                                    /*!< Status structure */
} mbus_master_ll_t;

/**
* \brief           MBUS_MASTER modules structure
* \details         This structure defines the MBUS master object for the master protocol.
*	
*    MBUS Read Type :
*    bit 7 - 4 : reserved for future use. All bits should be set to 0.
*    bit 3 - 2 : SND_NKE Type
*           00 : SND_NKE Network
*           01 : SND_NKE
*           10 : No SND_NKE
* 
*    bit 1     : Slave selection type
*            0 : Slave selection by 0x53 command
*            1 : Slave selection by 0x73 command
* 
*    bit 0     : ACK type
*            0 : ACK(wait for ACK)
*            1 : No ACK(don't wait for ACK)
* 
*/
typedef struct {
	uint32_t mbus_id;                                                /*!< MBUS ID of the slave meter */
	//uint32_t mbus_id_bcd;                                            /*!< MBUS ID in BCD format */
	uint8_t read_type;                                               /*!< MBUS read type */
	uint32_t read_period;                                            /*!< Read period */
	uint8_t primary_addr;
	mbus_master_error_t error;                                       /*!< MBUS error structure */
	mbus_master_address_mode_t address_mode;                         /*!< MBUS addressing mode */
	mbus_master_buffer_t receive_buffer;                             /*!< MBUS receive buffer */

	mbus_master_mode_t mode;                                         /*!< MBUS mode for NT750 commands */
	volatile mbus_master_state_t state;                              /*!< MBUS state machine state */
	mbus_master_sub_state_t sub_state;                               /*!< MBUS sub state machine state */
}mbus_master_t;

/**
 * \brief           MBUS library context structure
 * \details         This structure defines the context for the MBUS library, including the master object and low-level interface.
 */
typedef struct {
	mbus_master_t master;                                            /*!< MBUS master object */

#if(MBUS_MASTER_CFG_NT750_ENABLED)
	mbus_t nt750;                                                /*!< NT750 MBUS converter object */
#endif

#if(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_IN223_GATEWAY)
	master_rs485_uart_ll_t ll;                                   /*!< Low level object */
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_INEDGE_GATEWAY	)
	master_rs485_uart_ll_t ll;                                   /*!< Low level object */
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_CNV750IOT_CONVERTER)
    mbus_uart_ll_t ll;                                           /*!< Low level object */
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_WIN32_SIMULATOR)
	mbus_master_ll_t ll;                                           /*!< Low level object */
#endif
}mbus_lib_t;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBUS_SIMPLE_TYPES_HDR_H */

#endif /* GATEWAY_TYPE == MBUS */
