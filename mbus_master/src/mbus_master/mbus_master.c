/**
* \file            mbus_master.c
* \brief           mbus_master simple library
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

#include <string.h>
#include <stdio.h>
#include "mbus_master/mbus_master.h"

#if(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_IN223_GATEWAY)
#include "in233_master_rs485_uart.h"
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_INEDGE_GATEWAY	)
#include "inEdge_master_rs485_uart.h"
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_CNV750IOT_CONVERTER)
#include "mbus_uart.h"
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_WIN32_SIMULATOR)
#include "system/mbus_master_ll.h"
#endif





/* MBUS commands */
const uint8_t snd_nke[5] = { 0x10, 0x40, 0xFF, 0x3F, 0x16 };
const uint8_t snd_nke_network[5] = { 0x10, 0x40, 0xFD, 0x3D, 0x16 };
const uint8_t read_slave[5] = { 0x10, 0x7B, 0xFD, 0x78, 0x16 };
const uint8_t select_meter_53h[7] = { 0x68, 0x0B, 0x0B, 0x68, 0x53, 0xFD, 0x52 };
const uint8_t select_meter_73h[7] = { 0x68, 0x0B, 0x0B, 0x68, 0x73, 0xFD, 0x52 };




/**
 * \brief           mbus_master master initialization
 * \param[in,out]   mbus: Pointer to mbus_master simple structure
 *
 * \details         This function initializes the mbus_master simple structure.
 *
 */
void mbus_master_init(mbus_lib_t* master) {
	memset(master, 0, sizeof(mbus_lib_t)); // Clear master structure
	master->ll.status.f.initialized = 0; /* Clear possible init flag */
	master->ll.uart.baudrate = MBUS_MASTER_CFG_UART_BAUDRATE;
#if(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_IN223_GATEWAY)
	master_rs485_uart_ll_init(&master->ll); /* Init low-level communication */
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_INEDGE_GATEWAY	)
	master_rs485_uart_ll_init(&master->ll); /* Init low-level communication */
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_CNV750IOT_CONVERTER)
    mbus_uart_ll_init(&master->ll); /* Init low-level communication */
#elif(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_WIN32_SIMULATOR)
	mbus_master_ll_init(&master->ll); /* Init low-level communication */
#endif
}


/**
 * \brief           mbus_master read meter
 * \param[in]       mbus_master: Pointer to mbus_master structure
 * \details         This function reads the meter data from the mbus_master.
 * \return          Result of the read operation
 */
mbus_master_res_t mbus_master_read(mbus_lib_t* mbus_master) {
	size_t len = 0;
	uint32_t current_time = 0;
	uint8_t snd_nke_type = 0;
	uint8_t ack_type = 0;

	if (!mbus_master) {
		return MBUS_MASTER_ERR_NOT_INITIALIZED;
	}
	if (!mbus_master->ll.send_fn) {
		return MBUS_MASTER_ERR_NOT_INITIALIZED;
	}

	mbus_master_reset_ctx(mbus_master); /* Reset context */
	mbus_master->master.mode = MBUS_MASTER_MODE_READ;

	mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; /* Use secondary addressing mode */
	mbus_master->master.state = MBUS_MASTER_STATE_START_READING;

	snd_nke_type = mbus_master_get_snd_nke_type(&mbus_master->master);

	if (snd_nke_type == MBUS_MASTER_SND_NKE_TYPE_SND_NKE_NETWORK) {
		mbus_master->ll.send_fn(snd_nke_network, sizeof(snd_nke_network));
	}
	else if (snd_nke_type == MBUS_MASTER_SND_NKE_TYPE_SND_NKE) {
		mbus_master->ll.send_fn(snd_nke, sizeof(snd_nke));
	}

#if (!WIN32_DEBUG) 
	osDelay(MBUS_MASTER_CFG_COMMAND_SEND_DELAY);
#else 
	Sleep(MBUS_MASTER_CFG_COMMAND_SEND_DELAY);
#endif

	ack_type = mbus_master_get_ack_type(&mbus_master->master);
	mbus_master->master.state = MBUS_MASTER_STATE_WAIT_E5;
	len = mbus_master_send_select_device(mbus_master);

	if (ack_type == MBUS_MASTER_SLAVE_WAIT_ACK) {
#if (!WIN32_DEBUG) 
		current_time = osKernelGetTickCount();
#else 
		current_time = GetTickCount();
#endif

#if (!WIN32_DEBUG) 
		while ((osKernelGetTickCount() - current_time) < MBUS_MASTER_CFG_ACK_TIMEOUT) {
#else 
		while ((GetTickCount() - current_time) < MBUS_MASTER_CFG_ACK_TIMEOUT) {
#endif	
			if (mbus_master->master.state == MBUS_MASTER_STATE_E5_RECEIVED) {
				break;
			}
#if (!WIN32_DEBUG) 
			osDelay(10);
#else 
			Sleep(10);
#endif
		}
		if (mbus_master->master.state != MBUS_MASTER_STATE_E5_RECEIVED) {
			mbus_master->master.error.ack_timeout = 1;
			return MBUS_MASTER_ERR_E5_NOT_RECEIVED;
		}
	}
	else {
#if (!WIN32_DEBUG) 
		osDelay(MBUS_MASTER_CFG_ACK_TIMEOUT);
#else 
		Sleep(MBUS_MASTER_CFG_ACK_TIMEOUT);
#endif
		mbus_master->master.state = MBUS_MASTER_STATE_E5_RECEIVED;
	}

	if (mbus_master->master.state == MBUS_MASTER_STATE_E5_RECEIVED) {
		// Send read meter command
#if (!WIN32_DEBUG) 
		osDelay(MBUS_MASTER_CFG_COMMAND_SEND_DELAY);
#else 
		Sleep(MBUS_MASTER_CFG_COMMAND_SEND_DELAY);
#endif
		mbus_master->master.state = MBUS_MASTER_STATE_WAIT_SLAVE_RESPONSE;
		mbus_master->ll.send_fn(read_slave, sizeof(read_slave));

#if (!WIN32_DEBUG) 
		current_time = osKernelGetTickCount();
#else 
		current_time = GetTickCount();
#endif
#if (!WIN32_DEBUG) 
		while ((osKernelGetTickCount() - current_time) < MBUS_MASTER_CFG_SLAVE_RESPONSE_TIMEOUT) {
#else 
		while ((GetTickCount() - current_time) < MBUS_MASTER_CFG_SLAVE_RESPONSE_TIMEOUT) {
#endif	
			if (mbus_master->master.state == MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS) {
				break;
			}
#if (!WIN32_DEBUG) 
			osDelay(10);
#else 
			Sleep(10);
#endif
		}
	}

	if (mbus_master->master.state != MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS) {
		mbus_master->master.error.device_not_responding = 1; // Set device not responding error
		return MBUS_MASTER_ERR_SLAVE_NOT_RESPONDING;
	}

	return MBUS_MASTER_SLAVE_RESPONSE_SUCCESS;
}

/** \brief           Send select device command to MBUS slave
 * \param[in]       master: Pointer to mbus_master structure
 * \return          Result of sending command: number of bytes sent or error code
 * \details         This function constructs and sends a select device command to the MBUS slave.
 *                  It uses the slave select type from the mbus_master structure to determine the command format.
 *                  The function computes the checksum and sends the command using the low-level send function.
 */
size_t mbus_master_send_select_device(mbus_lib_t * mbus_master) {
	uint8_t out_buffer[17];
	if (mbus_master == NULL) {
		return MBUS_MASTER_ERR_NOT_INITIALIZED;
	}

	/* Pick template (7 bytes) */
	const uint8_t* tmpl = (mbus_master_get_slave_select_type(&mbus_master->master) == MBUS_MASTER_SLAVE_SELECT_TYPE_53h) ? select_meter_53h : select_meter_73h;

	/* Copy fixed 7-byte header */
	out_buffer[0] = tmpl[0];
	out_buffer[1] = tmpl[1];
	out_buffer[2] = tmpl[2];
	out_buffer[3] = tmpl[3];
	out_buffer[4] = tmpl[4];
	out_buffer[5] = tmpl[5];
	out_buffer[6] = tmpl[6];

	/* Serialize ID (little endian) */
	out_buffer[7] = (uint8_t)(mbus_master->master.mbus_id & 0xFFu);
	out_buffer[8] = (uint8_t)((mbus_master->master.mbus_id >> 8) & 0xFFu);
	out_buffer[9] = (uint8_t)((mbus_master->master.mbus_id >> 16) & 0xFFu);
	out_buffer[10] = (uint8_t)((mbus_master->master.mbus_id >> 24) & 0xFFu);

	/* 4 bytes wildcard 0xFF */
	out_buffer[11] = 0xFFu;
	out_buffer[12] = 0xFFu;
	out_buffer[13] = 0xFFu;
	out_buffer[14] = 0xFFu;

	/* Compute checksum (bytes 4..14 inclusive) */
	uint8_t cs = 0;
	for (uint8_t i = 4; i <= 14; ++i) {
		cs += out_buffer[i];
	}
	out_buffer[15] = cs;
	out_buffer[16] = 0x16u; /* Stop */

	return(mbus_master->ll.send_fn(out_buffer, sizeof(out_buffer)));
}


#if (MBUS_MASTER_CFG_NT750_ENABLED)
/**
 * \brief           Get data from the MBUS master
 * \param[in]       mbus_master: Pointer to mbus_master structure
 * \param[in]       nt750: Pointer to nt750 structure
 * \param[in]       cmd: Command to execute
 * \return          Result of the operation
 * \details         This function retrieves data from the NT750 MBUS converter using the MBUS protocol.
 *                  It handles both reading and setting commands for the NT750 device.
 */
mbus_master_res_t mbus_master_get(mbus_lib_t* mbus_master, mbus_t* nt750, nt750_cmd_t cmd) {
 	uint32_t current_time = 0;
 	uint8_t msg_buffer[512];
 	size_t msg_len = 0;
 
 	if (!mbus_master || !nt750) {
 		return MBUS_MASTER_ERR_NOT_INITIALIZED;
 	}
 	if (!mbus_master->ll.send_fn) {
 		return MBUS_MASTER_ERR_NOT_INITIALIZED;
 	}
 	if (cmd < NT750_GET_PRI_ADDR_PA || cmd > NT750_SET_CLEAN_ALL_FAULT_RECORDS) {
 		return MBUS_MASTER_ERR_INVALID_COMMAND;
 	}
 
    mbus_master_reset_ctx(mbus_master);
 	mbus_master->master.mode = MBUS_MASTER_MODE_GET;
 	mbus_master->master.mbus_id = dec_to_bcd(nt750->secondary_address);
 	msg_len = 0;
 	/* Build command */
 	switch (cmd) {
 	case NT750_GET_PRI_ADDR_PA: mbus_master->master.address_mode = MBUS_MASTER_PRIMARY_ADDRESS_MODE; mbus_read_primary_address_PA(msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_SEC_ADDR_PA: mbus_master->master.address_mode = MBUS_MASTER_PRIMARY_ADDRESS_MODE; mbus_read_secondary_address_PA(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_BAUD_PA: mbus_master->master.address_mode = MBUS_MASTER_PRIMARY_ADDRESS_MODE; mbus_read_baudrate_PA(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_CALIBRATION_CURRENT: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_calibration_current(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_CUSTOMER: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_customer_location(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_RECOVERY_PERIODE: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_recovery_periode(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_OVERCURR_LIM: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_over_current_limit(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_BREAKCURR_LIM: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_breaking_current_limit(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_RECOVERY_TRY_CNT: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_recovery_try_count(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_BREAKCURR_COMPARE_MATCH_CNT: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_breaking_current_compare_match_count(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_MBUS_RECCURR_TRESHOLD: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_mbus_receiving_current_treshold(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	case NT750_GET_FAULT_RECORDS: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_fault_records(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
 	default: return MBUS_MASTER_ERR_INVALID_COMMAND;
 	}
 	if (msg_len > sizeof(msg_buffer)) { /* Builder overrun protection */
 		return MBUS_MASTER_ERR;
 	}
 
 	mbus_master->master.state = MBUS_MASTER_STATE_START_READING;
 	mbus_master->ll.send_fn(snd_nke, sizeof(snd_nke_network));
 	mbus_master_delay(MBUS_MASTER_CFG_COMMAND_SEND_DELAY);
 	mbus_master->ll.send_fn(msg_buffer, msg_len);
 
 	mbus_master->master.state = MBUS_MASTER_STATE_WAIT_E5;

#if (!WIN32_DEBUG) 
        current_time = osKernelGetTickCount();
        while ((osKernelGetTickCount() - current_time) < MBUS_MASTER_CFG_SND_NKE_TIMEOUT) {
#else 
        current_time = GetTickCount();
		while ((GetTickCount() - current_time) < MBUS_MASTER_CFG_SND_NKE_TIMEOUT) {
#endif

 		if (mbus_master->master.state == MBUS_MASTER_STATE_E5_RECEIVED) {
 			break;
 		}
#if (!WIN32_DEBUG) 
        osDelay(10);
#else 
        Sleep(10);
#endif
 	}
 	if (mbus_master->master.state != MBUS_MASTER_STATE_E5_RECEIVED) {
 		return MBUS_MASTER_ERR_E5_NOT_RECEIVED;
 	}
 
 	mbus_master->ll.send_fn(read_slave, sizeof(read_slave));
 	mbus_master->master.state = MBUS_MASTER_STATE_WAIT_SLAVE_RESPONSE;

#if (!WIN32_DEBUG) 
    current_time = osKernelGetTickCount();
    while ((osKernelGetTickCount() - current_time) < MBUS_MASTER_CFG_SLAVE_RESPONSE_TIMEOUT) {
#else 
    current_time = GetTickCount();
	while ((GetTickCount() - current_time) < MBUS_MASTER_CFG_SLAVE_RESPONSE_TIMEOUT) {
#endif
 		if (mbus_master->master.state == MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS) {
 			break;
 		}
#if (!WIN32_DEBUG) 
        osDelay(10);
#else 
        Sleep(10);
#endif
 	}
 	if (mbus_master->master.state != MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS) {
 		return MBUS_MASTER_ERR_SLAVE_NOT_RESPONDING;
 	}
 	mbus_master->master.state = MBUS_MASTER_STATE_IDLE;
 	return MBUS_MASTER_SLAVE_RESPONSE_SUCCESS;
 }


 /**
  * \brief           Set data to the MBUS master simple
  * \param[in]       mbus_master: Pointer to mbus_master structure
  * \param[in]       nt750: Pointer to nt750 structure
  * \param[in]       cmd: Command to execute
  * \return          Result of the operation
  * \details         This function sets data to the NT750 MBUS converter using the MBUS simple protocol.
  *                  It handles various setting commands for the NT750 device.
  */
mbus_master_res_t mbus_master_set(mbus_lib_t* mbus_master, mbus_t* nt750, nt750_cmd_t cmd) {
  	uint32_t current_time = 0;
  	uint8_t msg_buffer[512];
  	size_t msg_len = 0;
  	if (!mbus_master || !nt750) {
  		return MBUS_MASTER_ERR_NOT_INITIALIZED;
  	}
  	if (!mbus_master->ll.send_fn) {
  		return MBUS_MASTER_ERR_NOT_INITIALIZED;
  	}
  	if (cmd < NT750_GET_PRI_ADDR_PA || cmd > NT750_SET_CLEAN_ALL_FAULT_RECORDS) {
  		return MBUS_MASTER_ERR_INVALID_COMMAND;
  	}
    mbus_master_reset_ctx(mbus_master);
  	mbus_master->master.mode = MBUS_MASTER_MODE_SET;
  	mbus_master->master.mbus_id = dec_to_bcd(nt750->secondary_address);
  	msg_len = 0;
  	switch (cmd) {
  	case NT750_SET_PRI_ADDR_PA: mbus_master->master.address_mode = MBUS_MASTER_PRIMARY_ADDRESS_MODE; mbus_set_primary_address_PA(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_SEC_ADDR_PA: mbus_master->master.address_mode = MBUS_MASTER_PRIMARY_ADDRESS_MODE; mbus_set_secondary_address_PA(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_BAUD_PA: mbus_master->master.address_mode = MBUS_MASTER_PRIMARY_ADDRESS_MODE; mbus_set_baudrate_PA(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_FULL_IDENT_PA: mbus_master->master.address_mode = MBUS_MASTER_PRIMARY_ADDRESS_MODE; mbus_set_complate_identification_PA(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_BAUD: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_baudrate(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_PRI_ADDR: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_primary_address(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_SEC_ADDR: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_secondary_address_SA(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_RESTART_DEV: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_restart_device(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_MBUS_PWR: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_mbus_power(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_CHNGE_PASS: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_change_password(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_SERVICE_MODE: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_service_mode(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_CALIBRATION_CURRENT: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_calibration_current(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_LOCK_DEV: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_lock_device(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_UNLOCK_DEV: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_unlock_device(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_CUSTOMER: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_customer_location(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_FCTRY_RST: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_fabrication_reset(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_FCTRY_RST_EXC_CAL: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_fabrication_reset_except_calibration(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_RECOVERY_PERIODE: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_recovery_periode(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_RECOVERY_TRY_COUNT: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_recovery_try_count(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_OVERCURR_LIM: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_over_current_limit(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_BREAKCURR_LIM: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_breaking_current_limit(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_RESET_CALIBRATION: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_reset_calibration(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_BREAKCURR_COMPARE_MATCH_CNT: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_set_breaking_current_compare_match_count(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	case NT750_SET_MBUS_RECEIVECURR_TRESHOLD: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_read_mbus_receiving_current_treshold(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break; /* TODO: should be a SET variant? */
  	case NT750_SET_CLEAN_ALL_FAULT_RECORDS: mbus_master->master.address_mode = MBUS_MASTER_SECONDARY_ADDRESS_MODE; mbus_clean_all_fault_records(nt750, msg_buffer, sizeof(msg_buffer), &msg_len); break;
  	default: return MBUS_MASTER_ERR_INVALID_COMMAND;
  	}
  	if (msg_len > sizeof(msg_buffer)) {
  		return MBUS_MASTER_ERR;
  	}
  	mbus_master->master.state = MBUS_MASTER_STATE_START_READING;
  	mbus_master->ll.send_fn(snd_nke_network, sizeof(snd_nke_network));
  	mbus_master_delay(MBUS_MASTER_CFG_COMMAND_SEND_DELAY);
  	mbus_master->ll.send_fn(msg_buffer, msg_len);
  	mbus_master->master.state = MBUS_MASTER_STATE_WAIT_E5;


#if (!WIN32_DEBUG) 
    current_time = osKernelGetTickCount();
    while ((osKernelGetTickCount() - current_time) < MBUS_MASTER_CFG_SND_NKE_TIMEOUT) {
#else 
    current_time = GetTickCount();
    while (((GetTickCount() - current_time) < MBUS_MASTER_CFG_SND_NKE_TIMEOUT)) {
#endif
  		if (mbus_master->master.state == MBUS_MASTER_STATE_E5_RECEIVED) {
  			break;
  		}
#if (!WIN32_DEBUG) 
        osDelay(10);
#else 
        Sleep(10);
#endif
  	}
  	if (mbus_master->master.state != MBUS_MASTER_STATE_E5_RECEIVED) {
  		return MBUS_MASTER_ERR_E5_NOT_RECEIVED;
  	}
  	
  	if (mbus_master->master.state == MBUS_MASTER_STATE_E5_RECEIVED) {
  		return MBUS_MASTER_SLAVE_RESPONSE_SUCCESS;
  	}
  	mbus_master->master.state = MBUS_MASTER_STATE_IDLE;
  	return MBUS_MASTER_SLAVE_RESPONSE_SUCCESS;
  }
#endif /* MBUS_MASTER_CFG_NT750_ENABLED */

  /**
  * \brief           Delay function
  *
  * This function creates a delay for a specified number of milliseconds
  * without blocking the execution of other tasks.
  *
  * \param[in]       ms: Number of milliseconds to delay
  */
  void mbus_master_delay(uint32_t ms) {
      uint32_t start_time = 0;
#if (!WIN32_DEBUG) 
      start_time = osKernelGetTickCount();
      while ((osKernelGetTickCount() - start_time) < ms) {
#else 
      start_time = GetTickCount();
      while ((GetTickCount() - start_time) < ms) {
#endif

#if (!WIN32_DEBUG) 
          osDelay(10);
#else 
          Sleep(10);
#endif
  	}
  }

  /**
   * \brief           Check if mbus_master is idle
   * \param[in]       mbus_master: Pointer to mbus_master structure
   * \return          1 if mbus_master is idle, 0 otherwise
   * \details         This function checks the state of the mbus_master to determine if it is idle.
   */
   uint32_t is_mbus_master_idle(mbus_lib_t* mbus_master) {
   	/* Check if mbus_master is idle */
   	if (mbus_master->master.state == MBUS_MASTER_STATE_IDLE) {
   		return 1; // Idle
   	}
   	return 0; // Not idle
   }

   /**
	* \brief           Sleep function
	* \param[in]       ms: Number of milliseconds to sleep
	* \details         This function puts the current task to sleep for a specified number of milliseconds.
	*                  It uses the osDelay function from the CMSIS-RTOS API.
	*/
	void mbus_master_sleep(uint32_t ms) {
#if (!WIN32_DEBUG) 
        osDelay(ms);
#else 
        Sleep(ms);
#endif
	}

	/**
	 * \brief           Reset mbus_master receive buffer
	 * \param[in]       mbus_master: Pointer to mbus_master structure
	 * \details         This function resets the mbus_master receive buffer by clearing its contents
	 *                  and resetting the head and tail pointers to zero.
	 */
	 //void reset_mbus_receive_buffer(mbus_lib_t* mbus_master) {
	 //	memset(&mbus_master->master.raw_data, 0, sizeof(mbus_master->master.raw_data));
	 //}



#endif /* GATEWAY_TYPE == MBUS */