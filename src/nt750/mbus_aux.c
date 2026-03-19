/******************************************************************************
 * @file           : mbus_aux.c
 * @brief          :
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 Inno Teknoloji.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by Inno Teknoloji
 *
 ******************************************************************************
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
#include "mbus_master/mbus_master.h"

#if (MBUS_MASTER_CFG_NT750_ENABLED)

 /* Includes -----------------------------------------------------------------*/

#include "nt750/mbus_aux.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define VERSION "1.1.0"

/* Private macro -------------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/




/**
* \brief           Return current version of the library
* \return          MBUS library version
*/
const char* mbus_get_current_version(void) { return VERSION; }

/**
* \brief           Init of the library
*/
void mbus_init_for_slave(mbus_t* mbus) {
	mbus->access_number = 0;
	mbus->selection_status = MBUS_NOT_SELECTED;
	mbus->status = 0;
	mbus->signature = 0;
}

/**
* \brief           Check secondary address
* \param[in]       frame: Frame for checking the secondary address
* \param[out]      mbus: Buffer for device variables.
* \return          MBUS responses
*/
int32_t mbus_check_secondary_address(mbus_frame* frame, mbus_t* mbus) {
	uint8_t buff[4];
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);

	if (((frame->data[0] == buff[3]) || (frame->data[0] == (buff[3] | 0xF0)) || (frame->data[0] == (buff[3] | 0x0F)) || (frame->data[0] == (buff[3] | 0xFF))) &&
		((frame->data[1] == buff[2]) || (frame->data[1] == (buff[2] | 0xF0)) || (frame->data[1] == (buff[2] | 0x0F)) || (frame->data[1] == (buff[2] | 0xFF))) &&
		((frame->data[2] == buff[1]) || (frame->data[2] == (buff[1] | 0xF0)) || (frame->data[2] == (buff[1] | 0x0F)) || (frame->data[2] == (buff[1] | 0xFF))) &&
		((frame->data[3] == buff[0]) || (frame->data[3] == (buff[0] | 0xF0)) || (frame->data[3] == (buff[0] | 0x0F)) || (frame->data[3] == (buff[0] | 0xFF)))) {
		if (((frame->data[4] == (mbus->manufacturer & 0x00FF)) || (frame->data[4] == ((mbus->manufacturer & 0x00FF) | 0xFF))) &&
			((frame->data[5] == ((mbus->manufacturer >> 8) & 0x00FF)) || (frame->data[5] == (((mbus->manufacturer >> 8) & 0x00FF) | 0xFF)))) {
			if ((frame->data[6] == mbus->generation) || (frame->data[6] == (mbus->generation | 0xFF))) {
				if ((frame->data[7] == mbus->medium) || (frame->data[7] == (mbus->medium | 0xFF))) {
					return MBUS_OK;
				}
				else {
					return MBUS_RSP_INVALID_DEVICE_MEDIUM;
				}
			}
			else {
				return MBUS_RSP_INVALID_DEVICE_GENERATION;
			}
		}
		else {
			return MBUS_RSP_INVALID_MANUFACTURER_ID;
		}
	}
	else {
		return MBUS_RSP_INVALID_SECONDARY_ADDRESS;
	}
}

/**
* \brief           Check secondary address with manufacturer
* \param[in]       frame: Frame for checking the secondary address
* \param[out]      mbus: Buffer for device variables.
* \return          MBUS responses
*/
int32_t mbus_check_secondary_address_with_manufacturer(mbus_frame* frame, mbus_t* mbus) {
	uint8_t buff[4];
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);

	if (((frame->data[0] == buff[3]) || (frame->data[0] == (buff[3] | 0xF0)) || (frame->data[0] == (buff[3] | 0x0F)) || (frame->data[0] == (buff[3] | 0xFF))) &&
		((frame->data[1] == buff[2]) || (frame->data[1] == (buff[2] | 0xF0)) || (frame->data[1] == (buff[2] | 0x0F)) || (frame->data[1] == (buff[2] | 0xFF))) &&
		((frame->data[2] == buff[1]) || (frame->data[2] == (buff[1] | 0xF0)) || (frame->data[2] == (buff[1] | 0x0F)) || (frame->data[2] == (buff[1] | 0xFF))) &&
		((frame->data[3] == buff[0]) || (frame->data[3] == (buff[0] | 0xF0)) || (frame->data[3] == (buff[0] | 0x0F)) || (frame->data[3] == (buff[0] | 0xFF)))) {
		if ((frame->data[4] == (mbus->manufacturer & 0x00FF)) && (frame->data[5] == ((mbus->manufacturer >> 8) & 0x00FF))) {
			if ((frame->data[6] == mbus->generation) || (frame->data[6] == (mbus->generation | 0xFF))) {
				if ((frame->data[7] == mbus->medium) || (frame->data[7] == (mbus->medium | 0xFF))) {
					return MBUS_OK;
				}
				else {
					return MBUS_RSP_INVALID_DEVICE_MEDIUM;
				}
			}
			else {
				return MBUS_RSP_INVALID_DEVICE_GENERATION;
			}
		}
		else {
			return MBUS_RSP_INVALID_MANUFACTURER_ID;
		}
	}
	else {
		return MBUS_RSP_INVALID_SECONDARY_ADDRESS;
	}
}

/**
* \brief           Check password
* \param[in]       password:
* \param[in]	   device_password:
* \return          `1` on success, `0` otherwise
*/
uint8_t mbus_check_password(uint32_t password, uint32_t device_password) {
	if (password == device_password) {
		return 1;
	}
	return 0;
}

/**
* \brief           Check super password
* \param[in]       super_password:
* \param[out]      device_super_password: Buffer for device variables.
* \return          `1` on success, `0` otherwise
*/
uint8_t mbus_check_super_password(uint32_t super_password, uint32_t device_super_password) {
	if (super_password == device_super_password) {
		return 1;
	}
	return 0;
}

/**
* \brief           Process frame and return action
* \param[in]       frame:
* \param[in/out]   mbus:
* \param[out]      ring_buffer:
* \return          MBUS responses
*/
mbus_reponse_t mbus_process_frame(mbus_frame* frame, mbus_t* mbus, uint8_t* data_buffer, size_t data_size) {
	mbus_reponse_t res = MBUS_ERROR;
	uint32_t uint32_temp = 0;

	mbus->response = MBUS_RSP_NO_RESPONSE;
	res = (mbus_reponse_t)mbus_parse(frame, data_buffer, data_size);

	if (res != MBUS_OK) {
#if MBUS_DEBUG_LOG_ENABLE
		DBG_LOG(LOG_TRACE, "result:%s\n", mbus_error_str());
		mbus_error_reset();
		DBG_LOG(LOG_TRACE, "Bozuk frame.....................\n");
#endif
		return MBUS_ERROR;
	}

	if (frame->type == MBUS_FRAME_TYPE_ANY) {
#if MBUS_DEBUG_LOG_ENABLE
		DBG_LOG(LOG_TRACE, "MBUS_FRAME_TYPE_ANY_RECEIVED\n");
#endif
		return MBUS_ERROR;
	}
	else if (frame->type == MBUS_FRAME_TYPE_ACK) {
#if MBUS_DEBUG_LOG_ENABLE
		DBG_LOG(LOG_TRACE, "ACK_FRAME_RECEIVED\n");
#endif
		return MBUS_ERROR;
	}

	if (!mbus_frame_direction(frame)) {
#if MBUS_DEBUG_LOG_ENABLE
		DBG_LOG(LOG_TRACE, "FAULT slave to master direction.\n");
#endif
		return MBUS_ERROR;
	}

	switch (frame->type) {
	case MBUS_FRAME_TYPE_SHORT: {
		//1. SND_NKE	[Short Frame]: (10 40 FD 3D 16) or (10 40 FE 3E 16) or (10 40 FF 3F 16)  
		if (frame->control == MBUS_CONTROL_MASK_SND_NKE) {
			if (frame->address == MBUS_ADDRESS_BROADCAST_REPLY) {
#if MBUS_DEBUG_LOG_ENABLE
				DBG_LOG(LOG_TRACE, "Received Command: SND_NKE - MBUS_ADDRESS_BROADCAST_REPLY FE\n");
#endif
				mbus->selection_status = MBUS_NOT_SELECTED;
				mbus->response = MBUS_RSP_SEND_ACK;
				res = MBUS_OK;
			}
			else if (frame->address == MBUS_ADDRESS_BROADCAST_NOREPLY) {
#if MBUS_DEBUG_LOG_ENABLE
				DBG_LOG(LOG_TRACE, "Received Command: SND_NKE - MBUS_ADDRESS_BROADCAST_NOREPLY FF\n");
#endif
				mbus->selection_status = MBUS_NOT_SELECTED;
				mbus->response = MBUS_RSP_NO_RESPONSE;
				res = MBUS_OK;
			}
			else if (frame->address == MBUS_ADDRESS_NETWORK_LAYER) {
#if MBUS_DEBUG_LOG_ENABLE
				DBG_LOG(LOG_TRACE, "Received Command: SND_NKE - MBUS_ADDRESS_NETWORK_LAYER FD\n");
#endif
				mbus->selection_status = MBUS_NOT_SELECTED;
				mbus->response = MBUS_RSP_NO_RESPONSE;
				res = MBUS_OK;
			}
		}
		else if (frame->control == (MBUS_CONTROL_MASK_REQ_UD2 | MBUS_CONTROL_MASK_FCB)) {
			if (frame->address == MBUS_ADDRESS_NETWORK_LAYER) {
				//2. REQ_UD2	[Short Frame]:	(10 7B FD 78 16) 
				if (mbus->selection_status == MBUS_SELECT_FOR_RSP_UD) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Send User Data\n");
#endif
					mbus->response = MBUS_RSP_SEND_RSP_UD;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_PRIMARY_ADDRESS) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Primary Address\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_PRIMARY_ADDRESS;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_SECONDARY_ADDRESS) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Secondary Address\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_SECONDARY_ADDRESS;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_BAUDRATE) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Baudrate\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_BAUDRATE;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_SENSOR_CALIBRATION_CURRENT) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Sensor Calibration Value\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_CALIBRATION_CURRENT;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_CUSTOMER_LOCATION) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Customer Location\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_CUSTOMER_LOCATION;
					res = MBUS_OK;
				}
				//else if (mbus->selection_status == MBUS_STATUS_SELECT_FOR_READ_SUPER_PASSWORD) {
				//	if (mbus_debug) {
				//		DBG_LOG(LOG_TRACE,"Received Command: REQ_UD2 - Read Super Password\n");
				//	}
				//	mbus->selection_status = MBUS_STATUS_NOT_SELECTED;
				//	mbus->response = MBUS_RSP_SEND_SUPER_PASSWORD;
				//	res = MBUS_OK;
				//}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_RECOVERY_PERIODE) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Read Short Cicuit Recovery Periode\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_RECOVERY_PERIODE;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_RECOVERY_TRY_COUNT) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Read Short Cicuit Recovery Try Count\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_RECOVERY_TRY_COUNT;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_OVER_CURRENT_LIMIT) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Read Over Current Limit\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_OVER_CURRENT_LIMIT;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_BREAKING_CURRENT_LIMIT)	{
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Read Breaking Current Limit\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_BREAKING_CURRENT_LIMIT;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_BREAKING_CURRENT_COMPARE_MATCH_COUNT){
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Read Breaking Current Compare Match Count\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_BREAKING_CURRENT_COMPARE_MATCH_COUNT;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_MBUS_RECEIVING_CURRENT_TRESHOLD) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Read Mbus Receiving Current Treshold\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_MBUS_RECEIVING_CURRENT_TRESHOLD;
					res = MBUS_OK;
				}
				else if (mbus->selection_status == MBUS_SELECT_FOR_READ_FAULT_RECORDS) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: REQ_UD2 - Read Fault Records\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SEND_MBUS_FAULT_RECORDS;
					res = MBUS_OK;
				}
			}
		}
		break;
	}
	case MBUS_FRAME_TYPE_CONTROL: {
		if (frame->control == (MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB)) {
			if (frame->address == mbus->primary_address) {
				//8. SET BAUDRATE PA [Control Frame]: (68 03 03 68 73 01 BB 2F 16) (E5)
				if ((frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_300) || (frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_600) ||
					(frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_1200) || (frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_2400) ||
					(frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_4800) || (frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_9600)) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: Set Baudrate PA\n");
#endif
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->baudrate = frame->control_information;
					mbus->response = MBUS_RSP_SET_BAUDRATE;
					res = MBUS_OK;
				}
			}
		}
		break;
	}
	case MBUS_FRAME_TYPE_LONG: {
		//3. READ THE PRIMARY ADDRESS [Long Frame]:  (broadcast) (68 05 05 68 73 FE 51 08 7A 44 16) (E5) (10 7B FE 79 16) (answer)
		if ((frame->control == (MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB)) && (frame->control_information == MBUS_CONTROL_INFO_DATA_SEND) && (frame->address == MBUS_ADDRESS_BROADCAST_REPLY) && (frame->data_size == 2)) {
#if MBUS_DEBUG_LOG_ENABLE
				DBG_LOG(LOG_TRACE, "Received Command: Read primary address\n");
#endif
			mbus->selection_status = MBUS_SELECT_FOR_READ_PRIMARY_ADDRESS;
			mbus->response = MBUS_RSP_SEND_ACK;
			res = MBUS_OK;
		}
		else if ((frame->control == (MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB)) && (frame->control_information == MBUS_CONTROL_INFO_DATA_SEND) && frame->address == mbus->primary_address) {
			//4. READ THE SECONDARY ADDRESS [Long Frame]:   (68 05 05 68 73 01 51 08 79 46 16) (E5) (10 7B FD 78 16) (answer)
			if ((frame->data_size == 2) && (frame->data[0] == MBUS_DIF_SELECTION_FOR_READOUT) && (frame->data[1] == MBUS_VIF_SECONDARY_ADDRESS_RECORD)) {
#if MBUS_DEBUG_LOG_ENABLE
				DBG_LOG(LOG_TRACE, "Received Command: Read secondary address\n");
#endif
				mbus->selection_status = MBUS_SELECT_FOR_READ_SECONDARY_ADDRESS;
				mbus->response = MBUS_RSP_SEND_ACK;
				res = MBUS_OK;
			}
			//5. SET PRIMARY ADDRESS PA [Long Frame]:	 (68 06 06 68 73 01 51 01 7A 02 42 16) (E5) 3
			else if ((frame->data_size == 3) && (frame->data[0] == MBUS_DIF_8_BIT_INT) && (frame->data[1] == MBUS_VIF_PRIMARY_ADDRESS_RECORD)) {
#if MBUS_DEBUG_LOG_ENABLE
				DBG_LOG(LOG_TRACE, "Received Command: Set primary address PA\n");
#endif
				mbus->new_primary_address = frame->data[2];
				mbus->selection_status = MBUS_NOT_SELECTED;
				mbus->response = MBUS_RSP_SET_PRIMARY_ADDRESS;
				res = MBUS_OK;
			}
			//6. SET SECONDARY ADDRESS PA[Long Frame]: (adres) (68 09 09 68 73 01 51 04 79 66 77 88 99 40 16) (E5) 6
			else if ((frame->data_size == 6) && (frame->data[0] == MBUS_DIF_32_BIT_INT) && (frame->data[1] == MBUS_VIF_SECONDARY_ADDRESS_RECORD)) {
#if MBUS_DEBUG_LOG_ENABLE
				DBG_LOG(LOG_TRACE, "Received Command: Set Secondary Address PA\n");
#endif
				mbus->new_secondary_address = (uint32_t)mbus_data_bcd_decode(&frame->data[2], 4);
				mbus->selection_status = MBUS_NOT_SELECTED;
				mbus->response = MBUS_RSP_SET_SECONDARY_ADDRESS;
				res = MBUS_OK;
			}
			//7. SET COMPLATE IDENTIFICATION [Long Frame]: (ID, Man, Gen, Med)
			//(ID=55667788, Man=1234h (PAD), Gen=99, Med=0E (Heat)(68 0D 0D 68 73 01 51 07 79 88 77 66 55 34 12 99 0E EC 16) (E5) 10
			else if ((frame->data_size == 17) && (frame->data[0] == MBUS_DIF_64_BIT_INT) && (frame->data[1] == MBUS_VIF_SECONDARY_ADDRESS_RECORD)) {
#if MBUS_DEBUG_LOG_ENABLE
				DBG_LOG(LOG_TRACE, "Received Command: Set Complate Identification\n");
#endif
				uint32_temp = frame->data[16];
				uint32_temp = uint32_temp << 8;
				uint32_temp |= frame->data[15];
				uint32_temp = uint32_temp << 8;
				uint32_temp |= frame->data[14];
				uint32_temp = uint32_temp << 8;
				uint32_temp |= frame->data[13];
				//TODO:düzeltilecek secondary address
				if (mbus_check_password(uint32_temp, mbus->password)) {
					mbus->secondary_address = (uint32_t)mbus_data_bcd_decode(&frame->data[2], 4);
					mbus->manufacturer = frame->data[7];			//MAN
					mbus->manufacturer = mbus->manufacturer << 8;
					mbus->manufacturer |= frame->data[6];
					mbus->generation = frame->data[8];			//GEN
					mbus->medium = frame->data[9];				//MED
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SET_COMPLATE_IDENTIFICATION;
					res = MBUS_OK;
				}
			}
			//9.READ BAUDRATE [Long Frame]: (68 06 06 68 73 01 51 08 FF 91 5D 16) (E5) (10 7B FD 78 16) (answer) 3
			else if ((frame->data_size == 3) && (frame->data[0] == MBUS_DIF_SELECTION_FOR_READOUT) && (frame->data[1] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[2] == MBUS_VIFE_READ_BAUDRATE)) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: Read Baudrate\n");
#endif
				mbus->selection_status = MBUS_SELECT_FOR_READ_BAUDRATE;
				mbus->response = MBUS_RSP_SEND_ACK;
				res = MBUS_OK;
			}
		}
		//C. SECONDARY ADDRESSING
		//10. SELECT_SLAVE [Long Frame]: (68 0B 0B 68 53 FD 52 78 56 34 12 FF FF FF FF B2 16) 8
		else if ((frame->data_size == 8) && ((frame->control == MBUS_CONTROL_MASK_SND_UD) || (frame->control == (MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB))) && (frame->address == MBUS_ADDRESS_NETWORK_LAYER) && (frame->control_information == MBUS_CONTROL_INFO_SELECT_SLAVE)) {
			if (mbus_check_secondary_address(frame, mbus) == MBUS_OK) {
#if MBUS_DEBUG_LOG_ENABLE
				DBG_LOG(LOG_TRACE, "Received Command: Select Slave\n");
#endif
				mbus->selection_status = MBUS_SELECT_FOR_RSP_UD;
				mbus->response = MBUS_RSP_SEND_ACK;
				res = MBUS_OK;
			}
			else {
				mbus->selection_status = MBUS_NOT_SELECTED;
				mbus->response = MBUS_RSP_NO_RESPONSE;
			}
		}
		else if (mbus_check_secondary_address_with_manufacturer(frame, mbus) == MBUS_OK) {
			//11. SET BAUDRATE SA[Long Frame]: (68 0B 0B 68 73 FD BB 78 56 34 12 13 06 FF FF 56 16) (E5) 8
			if ((frame->data_size == 8) && (frame->control == (MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB)) && (frame->address == MBUS_ADDRESS_NETWORK_LAYER)) {
				if ((frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_300) || (frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_600) ||
					(frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_1200) || (frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_2400) ||
					(frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_4800) || (frame->control_information == MBUS_CONTROL_INFO_SET_BAUDRATE_9600)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Set Baudrate SA\n");
#endif
					mbus->baudrate = frame->control_information;
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SET_BAUDRATE;
					res = MBUS_OK;
				}
			}
			else if ((frame->control == (MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB)) && (frame->address == MBUS_ADDRESS_NETWORK_LAYER) && (frame->control_information == MBUS_CONTROL_INFO_DATA_SEND)) {
				//12. SET PRIMARY ADDRESS SA [Long Frame]: (68 0E 0E 68 73 FD 51 78 56 34 12 13 06 FF FF 01 7A 02 69 16) (E5) 11
				if ((frame->data_size == 11) && (frame->data[8] == MBUS_DIF_8_BIT_INT) && (frame->data[9] == MBUS_VIF_PRIMARY_ADDRESS_RECORD)) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: Set Primary Address SA\n");
#endif
					mbus->new_primary_address = frame->data[10];
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SET_PRIMARY_ADDRESS;
					res = MBUS_OK;
				}
				//13. SET SECONDARY ADDRESS SA [Long Frame]: (68 11 11 68 73 FD 51 78 56 34 12 13 06 FF FF 04 79 88 77 66 55 23 16) (E5) 14
				else if ((frame->data_size == 14) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_SECONDARY_ADDRESS_RECORD)) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: Set Secondary Address SA\n");
#endif
					mbus->new_secondary_address = (uint32_t)mbus_data_bcd_decode(&frame->data[10], 4);
					mbus->selection_status = MBUS_NOT_SELECTED;
					mbus->response = MBUS_RSP_SET_SECONDARY_ADDRESS;
					res = MBUS_OK;
				}
				//14. SET CALIBRATION MODE STATUS(ON/OFF) (manufacturer specific) [Long Frame]: (68 16 16 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 01 FF 81 01 92 16) (E5) 19
				else if ((frame->data_size == 19) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_8_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_SET_SERVICE_MODE)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Set Calibration Mode\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->service_mode_status = frame->data[18];
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_SET_SERVICE_MODE_STATUS;
						res = MBUS_OK;
					}
				}
				//15. CALIBRATE CURRENT SENSOR (manufacturer specific) [Long Frame]: (68 17 17 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 02 FF 82 09 05 A1 16) (E5) 20
				else if ((frame->data_size == 20) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_16_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_CALIBRATE_CURRENT_SENSOR)) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: Calibrate Current Sensor\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->calibration_current_mA = frame->data[19];
						mbus->calibration_current_mA = mbus->calibration_current_mA << 8;
						mbus->calibration_current_mA |= frame->data[18];
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_CALIBRATE_CURRENT_SENSOR;
						res = MBUS_OK;
					}
				}
				//16. READ CURRENT SENSOR CALIBRATION VALUE (manufacturer specific) [Long Frame]: (68 15 15 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 00 FF 83 92 16) (E5) (10 7B 01 5C 16) (answer) 18
				else if ((frame->data_size == 18) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_CALIBRATION_CURRENT)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Read Calibration Value\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_SELECT_FOR_READ_SENSOR_CALIBRATION_CURRENT;
						mbus->response = MBUS_RSP_SEND_ACK;
						res = MBUS_OK;
					}
				}
				//17. SET M-BUS POWER(on/off) (manufacturer specific) [Long Frame]: (68 0F 0F 68 73 FD 51 78 56 34 12 13 06 FF FF 01 FF 84 01 71 16) (E5) 12
				else if ((frame->data_size == 19) && (frame->data[15] == MBUS_DIF_8_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_SET_MBUS_POWER)) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: Set M-BUS Power\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->mbus_power_status = frame->data[18];
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_SET_MBUS_POWER_STATUS;
						res = MBUS_OK;
					}
				}
				//18. LOCK DEVICE(manufacturer specific)[Long Frame]:  (68 15 15 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 00 FF 85 94 16) (E5) 18
				else if ((frame->data_size == 25) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_ENTER_SUPER_PASSWORD) &&
					(frame->data[22] == MBUS_DIF_NO_DATA) && (frame->data[23] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[24] == MBUS_VIFE_LOCK_DEVICE)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Lock Device\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						uint32_temp = frame->data[21];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[20];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[19];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[18];
						if (mbus_check_super_password(uint32_temp, mbus->admin_password)) {
							mbus->selection_status = MBUS_NOT_SELECTED;
							mbus->response = MBUS_RSP_LOCK_DEVICE;
							res = MBUS_OK;
						}
					}
				}
				//19.	UNLOCK DEVICE (manufacturer specific) [Long Frame]: (68 15 15 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 00 FF 86 95 16) (E5) 18
				else if ((frame->data_size == 25) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_ENTER_SUPER_PASSWORD) &&
					(frame->data[22] == MBUS_DIF_NO_DATA) && (frame->data[23] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[24] == MBUS_VIFE_UNLOCK_DEVICE)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Unlock Device\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						uint32_temp = frame->data[21];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[20];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[19];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[18];
						if (mbus_check_super_password(uint32_temp, mbus->admin_password)) {
							mbus->selection_status = MBUS_NOT_SELECTED;
							mbus->response = MBUS_RSP_UNLOCK_DEVICE;
							res = MBUS_OK;
						}
					}
				}
				//20. SET CUSTOMER LOCATION (manufacturer specific) [Long Frame]: (68 19 19 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 04 FF 87 88 66 44 22 EE 16) (E5) 22
				else if ((frame->data_size == 36) &&
					(frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_SET_CUSTOMER_LOCATION) &&
					(frame->data[22] == MBUS_DIF_32_BIT_INT) && (frame->data[23] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[24] == MBUS_VIFE_SET_CUSTOMER_LOCATION) &&
					(frame->data[29] == MBUS_DIF_32_BIT_INT) && (frame->data[30] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[31] == MBUS_VIFE_SET_CUSTOMER_LOCATION)) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: Set Customer Location\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->customer_location_1 = frame->data[21];
						mbus->customer_location_1 = mbus->customer_location_1 << 8;
						mbus->customer_location_1 |= frame->data[20];
						mbus->customer_location_1 = mbus->customer_location_1 << 8;
						mbus->customer_location_1 |= frame->data[19];
						mbus->customer_location_1 = mbus->customer_location_1 << 8;
						mbus->customer_location_1 |= frame->data[18];

						mbus->customer_location_2 = frame->data[28];
						mbus->customer_location_2 = mbus->customer_location_2 << 8;
						mbus->customer_location_2 |= frame->data[27];
						mbus->customer_location_2 = mbus->customer_location_2 << 8;
						mbus->customer_location_2 |= frame->data[26];
						mbus->customer_location_2 = mbus->customer_location_2 << 8;
						mbus->customer_location_2 |= frame->data[25];

						mbus->customer_location_3 = frame->data[35];
						mbus->customer_location_3 = mbus->customer_location_3 << 8;
						mbus->customer_location_3 |= frame->data[34];
						mbus->customer_location_3 = mbus->customer_location_3 << 8;
						mbus->customer_location_3 |= frame->data[33];
						mbus->customer_location_3 = mbus->customer_location_3 << 8;
						mbus->customer_location_3 |= frame->data[32];

						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_SET_CUSTOMER_LOCATION;
						res = MBUS_OK;
					}
				}
				//21.	READ CUSTOMER LOCATION (manufacturer specific) [Long Frame]: (68 15 15 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 00 FF 88 97 16) (E5) (10 7B FD 78 16) (answer) 18
				else if ((frame->data_size == 18) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_CUSTOMER_LOCATION)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Read Customer Location\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_SELECT_FOR_READ_CUSTOMER_LOCATION;
						mbus->response = MBUS_RSP_SEND_ACK;
						res = MBUS_OK;
					}
				}
				//22. CHANGE PASSWORD (manufacturer specific) [Long Frame]: (68 1C 1C 68 73 FD 51 78 56 34 12 13 06 FF FF 04 FF 89 00 99 88 77 04 FF 93 96 19 04 03 00 FF 92 ED 16) (E5) 22
				else if ((frame->data_size == 22) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_CHANGE_PASSWORD)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Change Password\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->password = frame->data[21];
						mbus->password = mbus->password << 8;
						mbus->password |= frame->data[20];
						mbus->password = mbus->password << 8;
						mbus->password |= frame->data[19];
						mbus->password = mbus->password << 8;
						mbus->password |= frame->data[18];
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_CHANGE_PASSWORD;
						res = MBUS_OK;
					}
				}
				//23. FABRICATION RESET(manufacturer specific) [Long Frame]: () (E5) 25
				else if ((frame->data_size == 18) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_FABRICATION_RESET)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Fabrication Reset\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_FABRICATION_RESET;
						res = MBUS_OK;
					}
				}
				//24.	READ SUPER PASSWORD SA (manufacturer specific) [Long Frame]: () (E5) (10 7B FD 78 16) (answer) data size:18
	//				else if ((frame->data_size == 18) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_SUPER_PASSWORD)) {
	//#if MBUS_DEBUG_LOG_ENABLE	
	//						DBG_LOG(LOG_TRACE,"Received Command: Read Super Password\n");
	//#endif
	//					temp_password = frame->data[14];
	//					temp_password = temp_password << 8;
	//					temp_password |= frame->data[13];
	//					temp_password = temp_password << 8;
	//					temp_password |= frame->data[12];
	//					temp_password = temp_password << 8;
	//					temp_password |= frame->data[11];
	//					if (check_password(temp_password, app_data.password)) {
	//						mbus->selection_status = MBUS_STATUS_SELECT_FOR_READ_SUPER_PASSWORD;
	//						mbus_send_ack(uart);
	//						res = MBUS_OK;
	//					}
	//				}
					//25.	SET RECOVERY PERIODE SECONDS SA (manufacturer specific) [Long Frame]: () (E5) data size:22
				else if ((frame->data_size == 29) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_SET_RECOVERY_PERIODE) &&
					(frame->data[22] == MBUS_DIF_32_BIT_INT) && (frame->data[23] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[24] == MBUS_VIFE_SET_FAST_RECOVERY_PERIODE)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Set Recovery Periode In Second\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						uint32_temp = frame->data[21];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[20];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[19];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[18];
						mbus->recovery_periode = uint32_temp;
						uint32_temp = frame->data[28];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[27];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[26];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[25];
						mbus->fast_recovery_periode = uint32_temp;

						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_SET_RECOVERY_PERIODE;
						res = MBUS_OK;
					}
				}
				//26.	READ RECOVERY PERIODE IN SECONDS SA(manufacturer specific)[Long Frame]: () (E5) (10 7B FD 78 16) (answer) data size : 18
				else if ((frame->data_size == 18) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_RECOVERY_PERIODE)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Read Recovery Time In Second\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_SELECT_FOR_READ_RECOVERY_PERIODE;
						mbus->response = MBUS_RSP_SEND_ACK;
						res = MBUS_OK;
					}
				}
				//27.	SET RECOVERY TRY COUNT SA (manufacturer specific) [Long Frame]:() (E5) data size:22
				else if ((frame->data_size == 29) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_SET_RECOVERY_TRY_COUNT) &&
					(frame->data[22] == MBUS_DIF_32_BIT_INT) && (frame->data[23] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[24] == MBUS_VIFE_SET_FAST_RECOVERY_TRY_COUNT)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Set Recovery Try Count\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						uint32_temp = frame->data[21];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[20];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[19];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[18];
						mbus->recovery_try_count = uint32_temp;
						uint32_temp = frame->data[28];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[27];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[26];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[25];
						mbus->fast_recovery_try_count = uint32_temp;
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_SET_RECOVERY_TRY_COUNT;
						res = MBUS_OK;
					}
				}
				//28.	READ RECOVERY TRY COUNT SA (manufacturer specific) [Long Frame]:() (E5) (10 7B FD 78 16) (answer) data size:18
				else if ((frame->data_size == 18) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_RECOVERY_TRY_COUNT)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Read Recovery Try Count\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_SELECT_FOR_READ_RECOVERY_TRY_COUNT;
						mbus->response = MBUS_RSP_SEND_ACK;
						res = MBUS_OK;
					}
				}
				//29.	DEVICE RESTART SA (manufacturer specific) [Long Frame]: () (E5) data size:18
				else if ((frame->data_size == 18) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_DEVICE_RESTART)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Restart Device\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_RESTART_DEVICE;
						res = MBUS_OK;
					}
				}
				//30.	SET OVER CURRENT LIMIT SA (manufacturer specific) [Long Frame]:() (E5) data size:22
				else if ((frame->data_size == 22) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_SET_OVER_CURRENT_LIMIT)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Set Over Current Limit\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						uint32_temp = frame->data[21];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[20];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[19];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[18];
						mbus->over_current_limit = uint32_temp;
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_SET_OVER_CURRENT_LIMIT;
						res = MBUS_OK;
					}
				}
				//31.	READ OVER CURRENT LIMIT SA (manufacturer specific) [Long Frame]:() (E5) (10 7B FD 78 16) (answer) data size:18
				else if ((frame->data_size == 18) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_OVER_CURRENT_LIMIT)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Read Over Current Limit\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_SELECT_FOR_READ_OVER_CURRENT_LIMIT;
						mbus->response = MBUS_RSP_SEND_ACK;
						res = MBUS_OK;
					}
				}
				//32.	SET BREAKING CURRENT LIMIT SA (manufacturer specific) [Long Frame]:() (E5) data size:22
				else if ((frame->data_size == 22) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_SET_BREAKING_CURRENT_LIMIT)) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: Set Breaking Current Limit\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						uint32_temp = frame->data[21];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[20];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[19];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[18];
						mbus->breaking_current_limit = uint32_temp;
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_SET_BREAKING_CURRENT_LIMIT;
						res = MBUS_OK;
					}
				}
				//33.	READ BREAKING CURRENT LIMIT SA (manufacturer specific) [Long Frame]:() (E5) (10 7B FD 78 16) (answer) data size:18
				else if ((frame->data_size == 18) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_BREAKING_CURRENT_LIMIT)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Read Breaking Current Limit\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_SELECT_FOR_READ_BREAKING_CURRENT_LIMIT;
						mbus->response = MBUS_RSP_SEND_ACK;
						res = MBUS_OK;
					}
				}
				//34. FABRICATION RESET EXPECT CALIBRATION(manufacturer specific) [Long Frame]: () (E5) 25
				else if ((frame->data_size == 18) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_FABRICATION_RESET_EXCEPT_CALIBRATION)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Fabrication Reset Except Calibration\n");
#endif	
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_FABRICATION_RESET_EXPECT_CALIBRATION;
						res = MBUS_OK;
					}
				}
				//35. RESET CALIBRATION(manufacturer specific) [Long Frame]: () (E5) 18
				else if ((frame->data_size == 18) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_RESET_CALIBRATION)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Reset Calibration \n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_RESET_CALIBRATION;
						res = MBUS_OK;
					}
				}
				//36. RESET STORAGE(manufacturer specific) [Long Frame]: () (E5) 18
				else if ((frame->data_size == 18) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_RESET_STORAGE)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Reset Calibration \n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_RESET_STORAGE;
						res = MBUS_OK;
					}
				}
				//37.	SET BREAKING CURRENT COMPARE MATCH COUNT (manufacturer specific) [Long Frame]:() (E5) data size:19
				else if ((frame->data_size == 19) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_SET_BREAKING_CURRENT_COMPARE_MATCH_COUNT)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Set Breaking Current Compare Match Count\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->breaking_current_compare_match_count = frame->data[18];
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_SET_BREAKING_CURRENT_COMPARE_MATCH_COUNT;
						res = MBUS_OK;
					}
				}
				//38.	READ BREAKING CURRENT COMPARE MATCH COUNT (manufacturer specific) [Long Frame]:() (E5) (10 7B FD 78 16) (answer) data size:18
				else if ((frame->data_size == 18) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_BREAKING_CURRENT_COMPARE_MATCH_COUNT)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Read Breaking Current Limit\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_SELECT_FOR_READ_BREAKING_CURRENT_COMPARE_MATCH_COUNT;
						mbus->response = MBUS_RSP_SEND_ACK;
						res = MBUS_OK;
					}
				}
				//39.	SET MBUS RECEIVING CURRENT TRESHOLD (manufacturer specific) [Long Frame]:() (E5) data size:22
				else if ((frame->data_size == 22) && (frame->data[8] == MBUS_DIF_32_BIT_INT) && (frame->data[9] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[10] == MBUS_VIFE_ENTER_PASSWORD) &&
					(frame->data[15] == MBUS_DIF_32_BIT_INT) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_SET_MBUS_RECEIVING_CURRENT_TRESHOLD)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Set Breaking Current Compare Match Count\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						uint32_temp = frame->data[21];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[20];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[19];
						uint32_temp = uint32_temp << 8;
						uint32_temp |= frame->data[18];
						hex_to_float(&uint32_temp, &mbus->f_mbus_receiving_current_treshold);
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_SET_MBUS_RECEIVING_CURRENT_TRESHOLD;
						res = MBUS_OK;
					}
				}
				//40.	READ MBUS RECEIVING CURRENT TRESHOLD (manufacturer specific) [Long Frame]:() (E5) (10 7B FD 78 16) (answer) data size:18
				else if ((frame->data_size == 18) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_MBUS_RECEIVING_CURRENT_TRESHOLD)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Read Breaking Current Limit\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_SELECT_FOR_READ_MBUS_RECEIVING_CURRENT_TRESHOLD;
						mbus->response = MBUS_RSP_SEND_ACK;
						res = MBUS_OK;
					}
				}
				//41.	READ FAULT RECORDS (manufacturer specific) [Long Frame]:() (E5) (10 7B FD 78 16) (answer) 
				else if ((frame->data_size == 22) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_READ_FAULT_RECORDS) &&
					(frame->data[18] == MBUS_DIF_8_BIT_INT) && (frame->data[20] == MBUS_VIFE_FAULT_RECORDS_GROUP_NUMBER)) {
#if MBUS_DEBUG_LOG_ENABLE
						DBG_LOG(LOG_TRACE, "Received Command: Read Fault Records\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->fault_records_group_number = frame->data[21];
						mbus->selection_status = MBUS_SELECT_FOR_READ_FAULT_RECORDS;
						mbus->response = MBUS_RSP_SEND_ACK;
						res = MBUS_OK;
					}
				}
				//42.	CLEAN ALL FAULT RECORDS (manufacturer specific) [Long Frame]:() (E5) 
				else if ((frame->data_size == 18) && (frame->data[15] == MBUS_DIF_NO_DATA) && (frame->data[16] == MBUS_VIF_EXTENSION_VIFE_FALLOW) && (frame->data[17] == MBUS_VIFE_CLEAN_ALL_FAULT_RECORDS)) {
#if MBUS_DEBUG_LOG_ENABLE
					DBG_LOG(LOG_TRACE, "Received Command: Read Fault Records\n");
#endif
					uint32_temp = frame->data[14];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[13];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[12];
					uint32_temp = uint32_temp << 8;
					uint32_temp |= frame->data[11];
					if (mbus_check_password(uint32_temp, mbus->password)) {
						mbus->selection_status = MBUS_NOT_SELECTED;
						mbus->response = MBUS_RSP_CLEAN_ALL_FAULT_RECORDS;
						res = MBUS_OK;
					}
				}
			}
		}
		break;
	}
	default:
		res = MBUS_ERROR;
		break;
	}
	return res;
}

/**
* \brief           Write ACK to buffer for serial sending
* \param[out]      ring_buffer:
*/
void mbus_send_ack(uint8_t* data_buffer, size_t* data_size) {
	data_buffer[0] = 0xE5;
	*data_size = 1;
}

/**
* \brief           Write primary address sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_primary_address(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_2_DIG_BCD;
	frame.data[frame.data_size++] = MBUS_VIF_PRIMARY_ADDRESS_RECORD;
	frame.data[frame.data_size++] = mbus->primary_address;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write secondary address sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_secondary_address(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_DIG_BCD;
	frame.data[frame.data_size++] = MBUS_VIF_SECONDARY_ADDRESS_RECORD;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write baudrate sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_baudrate(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_BAUDRATE;
	frame.data[frame.data_size++] = mbus->baudrate;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write calibration value sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_calibration_current(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = mbus->calibration_current_mA & 0x00FF;
	frame.data[frame.data_size++] = (mbus->calibration_current_mA >> 8) & 0x00FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write customer location sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_customer_location(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_CUSTOMER_LOCATION;
	frame.data[frame.data_size++] = mbus->customer_location_1 & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_1 >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_1 >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_1 >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x40;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_CUSTOMER_LOCATION;
	frame.data[frame.data_size++] = mbus->customer_location_2 & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_2 >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_2 >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_2 >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_CUSTOMER_LOCATION;
	frame.data[frame.data_size++] = mbus->customer_location_3 & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_3 >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_3 >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_3 >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write RSP_UD sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_RSP_UD(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];
	uint32_t temp = 0;

	frame.data_size = 0;
	float_to_hex(&mbus->f_mbus_receiving_current_treshold, &temp);
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;								//Hardware version
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_HARDWARE_VERSION;
	frame.data[frame.data_size++] = mbus->hardware_version & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->hardware_version >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->hardware_version >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->hardware_version >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT;								//Model version
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MODEL_VERSION;
	frame.data[frame.data_size++] = mbus->model_version & 0x00FF;
	frame.data[frame.data_size++] = (mbus->model_version >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT;								//Firmware version
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_FIRMWARE_VERSION;
	frame.data[frame.data_size++] = mbus->firmware_version & 0x00FF;
	frame.data[frame.data_size++] = (mbus->firmware_version >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;									//App state
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_STATE_OF_PARAMETER_ACTIVATION;
	frame.data[frame.data_size++] = mbus->current_app_state;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;								//Error flags
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_ERROR_FLAGS;
	frame.data[frame.data_size++] = mbus->error_flags & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->error_flags >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->error_flags >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->error_flags >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;									//Mbus power status
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_DIGITAL_OUTPUT;
	frame.data[frame.data_size++] = mbus->mbus_power_status;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT;								//Mbus current
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = mbus->mbus_current_mA & 0x00FF;
	frame.data[frame.data_size++] = (mbus->mbus_current_mA >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT;								//Mbus voltage (ST:0)
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = mbus->mbus_voltage_mV & 0x00FF;
	frame.data[frame.data_size++] = (mbus->mbus_voltage_mV >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x40;							//24VDC internal supply (ST:1)
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = mbus->dc24V_internal_voltage_mV & 0x00FF;
	frame.data[frame.data_size++] = (mbus->dc24V_internal_voltage_mV >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80;							//36VDC internal supply (ST:2)
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = mbus->dc36V_internal_voltage_mV & 0x00FF;
	frame.data[frame.data_size++] = (mbus->dc36V_internal_voltage_mV >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80 | 0x40;					//External power supply (ST:3)
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = mbus->power_supply_voltage_mV & 0x00FF;
	frame.data[frame.data_size++] = (mbus->power_supply_voltage_mV >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80;							//Vdd analog (ST:4)
	frame.data[frame.data_size++] = 0x02;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT_2;
	frame.data[frame.data_size++] = mbus->vdd_analog_mV & 0x00FF;
	frame.data[frame.data_size++] = (mbus->vdd_analog_mV >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x40;							//Calibrating current (ST:1)
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = mbus->calibration_current_mA & 0x00FF;
	frame.data[frame.data_size++] = (mbus->calibration_current_mA >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80;							//Breaking current limit(ST:2)
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = mbus->breaking_current_limit & 0x00FF;
	frame.data[frame.data_size++] = (mbus->breaking_current_limit >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80 | 0x40;					//Over current limit(ST:3)
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = mbus->over_current_limit & 0x00FF;
	frame.data[frame.data_size++] = (mbus->over_current_limit >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;									//Lock status (ST:0)
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->device_lock_status;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT | 0x40;							//Service mode status (ST:1)
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->service_mode_status;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT | 0x80;							//Calibration status (ST:2)
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->calibration_status;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80 | 0x40;					//Receiving current treshold(ST:3)	
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	temp = 0;
	float_to_hex(&mbus->f_mbus_receiving_current_treshold, &temp);
	frame.data[frame.data_size++] = temp & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80;							//Recovery periode(ST:4)	
	frame.data[frame.data_size++] = 0x02;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->recovery_periode & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_periode >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_periode >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_periode >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80 | 0x40;					//Fast recovery periode(ST:5)	
	frame.data[frame.data_size++] = 0x02;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->fast_recovery_periode & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_periode >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_periode >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_periode >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80;							//Recovery try count(ST:6)	
	frame.data[frame.data_size++] = 0x03;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->fast_recovery_try_count & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_try_count >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_try_count >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_try_count >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80 | 0x40;					//Fast recovery try count(ST:7)	
	frame.data[frame.data_size++] = 0x03;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->recovery_try_count & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_try_count >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_try_count >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_try_count >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80;							//Calibrated AV(ST:8)	
	frame.data[frame.data_size++] = 0x04;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	temp = 0;
	float_to_hex(&mbus->f_calibrated_AV, &temp);
	frame.data[frame.data_size++] = temp & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT | 0x80 | 0x40;					//Current fault index(ST:9)	
	frame.data[frame.data_size++] = 0x04;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->current_fault_index;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT | 0x80;							//Fault record index(ST:10)	
	frame.data[frame.data_size++] = 0x05;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->fault_record_index;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT | 0x80 | 0x40;					//Breaking current compare match count(ST:11)	
	frame.data[frame.data_size++] = 0x05;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->breaking_current_compare_match_count;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT | 0x80;							//wdt reset flag(ST:12)	
	frame.data[frame.data_size++] = 0x06;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->wdt_reset_flag;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;								//Customer location 1 (ST:0)
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_CUSTOMER_LOCATION;
	frame.data[frame.data_size++] = mbus->customer_location_1 & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_1 >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_1 >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_1 >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x40;							//Customer location 2 (ST:1)
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_CUSTOMER_LOCATION;
	frame.data[frame.data_size++] = mbus->customer_location_2 & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_2 >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_2 >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_2 >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80;							//Customer location 3 (ST:2)
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_CUSTOMER_LOCATION;
	frame.data[frame.data_size++] = mbus->customer_location_3 & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_3 >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_3 >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_3 >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}


/**
* \brief           Write super password sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_super_password(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->admin_password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->admin_password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->admin_password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->admin_password >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write recovery periode sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_recovery_periode(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->recovery_periode & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_periode >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_periode >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_periode >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x40;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->fast_recovery_periode & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_periode >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_periode >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_periode >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write recovery try count sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_recovery_try_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->recovery_try_count & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_try_count >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_try_count >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_try_count >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x40;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->fast_recovery_try_count & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_try_count >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_try_count >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_try_count >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write over current limit sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_over_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = mbus->over_current_limit & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->over_current_limit >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->over_current_limit >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->over_current_limit >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write breaking current limit sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_breaking_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = mbus->breaking_current_limit & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->breaking_current_limit >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->breaking_current_limit >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->breaking_current_limit >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write breaking current compare match count sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_breaking_current_compare_match_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->breaking_current_compare_match_count;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write breaking current limit sending MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_mbus_receiving_current_treshold(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint32_t temp = 0;
	uint8_t buff[4];

	frame.data_size = 0;
	float_to_hex(&mbus->f_mbus_receiving_current_treshold, &temp);
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = temp & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}

/**
* \brief           Write read fault record groups MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_send_fault_records(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size, volatile uint32_t* fault_buffer, volatile uint32_t* fault_mbus_current_and_voltage) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_RSP_UD;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_RESP_VARIABLE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = mbus->access_number;
	frame.data[frame.data_size++] = mbus->status;
	frame.data[frame.data_size++] = mbus->signature & 0x00FF;
	frame.data[frame.data_size++] = (mbus->signature >> 8) & 0x00FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;						//Fault records group number
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION;
	frame.data[frame.data_size++] = mbus->fault_records_group_number;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_ERROR_FLAGS;
	frame.data[frame.data_size++] = fault_buffer[0] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[0] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[0] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[0] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[0] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[0] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = fault_mbus_current_and_voltage[0] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[0] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x40;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_ERROR_FLAGS;
	frame.data[frame.data_size++] = fault_buffer[1] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[1] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[1] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[1] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x40;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[1] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[1] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x40;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = fault_mbus_current_and_voltage[1] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[1] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_ERROR_FLAGS;
	frame.data[frame.data_size++] = fault_buffer[2] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[2] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[2] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[2] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[2] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[2] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = fault_mbus_current_and_voltage[2] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[2] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80 | 0x40;
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_ERROR_FLAGS;
	frame.data[frame.data_size++] = fault_buffer[3] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[3] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[3] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[3] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80 | 0x40;
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[3] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[3] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80 | 0x40;
	frame.data[frame.data_size++] = 0x01;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = fault_mbus_current_and_voltage[3] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[3] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x02;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_ERROR_FLAGS;
	frame.data[frame.data_size++] = fault_buffer[4] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[4] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[4] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[4] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x02;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[4] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[4] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x02;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = fault_mbus_current_and_voltage[4] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[4] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80 | 0x40;
	frame.data[frame.data_size++] = 0x02;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_ERROR_FLAGS;
	frame.data[frame.data_size++] = fault_buffer[5] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[5] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[5] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[5] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80 | 0x40;
	frame.data[frame.data_size++] = 0x02;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[5] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[5] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80 | 0x40;
	frame.data[frame.data_size++] = 0x02;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = fault_mbus_current_and_voltage[5] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[5] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x03;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_ERROR_FLAGS;
	frame.data[frame.data_size++] = fault_buffer[6] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[6] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[6] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[6] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x03;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[6] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[6] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80;
	frame.data[frame.data_size++] = 0x03;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = fault_mbus_current_and_voltage[6] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[6] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT | 0x80 | 0x40;
	frame.data[frame.data_size++] = 0x03;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_ERROR_FLAGS;
	frame.data[frame.data_size++] = fault_buffer[7] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[7] >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[7] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_buffer[7] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80 | 0x40;
	frame.data[frame.data_size++] = 0x03;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_AMPERE;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[7] >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[7] >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT | 0x80 | 0x40;
	frame.data[frame.data_size++] = 0x03;//DIFE
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_EXTENSION_MILLI_VOLT;
	frame.data[frame.data_size++] = fault_mbus_current_and_voltage[7] & 0x000000FF;
	frame.data[frame.data_size++] = (fault_mbus_current_and_voltage[7] >> 8) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
	mbus->access_number++;
}


/**
* \brief           Write SND_NKE sending MBUS frame to buffer for serial sending
* \param[in]       address:
* \param[out]      ring_buffer:
*/
void mbus_SND_NKE(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_SHORT;
	frame.start1 = MBUS_FRAME_SHORT_START;
	frame.control = MBUS_CONTROL_MASK_SND_NKE;
	frame.address = mbus->primary_address;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write REQ_UD2 sending MBUS frame to buffer for serial sending
* \param[in]       address:
* \param[out]      ring_buffer:
*/
void mbus_REQ_UD2(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_SHORT;
	frame.start1 = MBUS_FRAME_SHORT_START;
	frame.control = MBUS_CONTROL_MASK_REQ_UD2 | MBUS_CONTROL_MASK_FCB;
	frame.address = mbus->primary_address;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

//A.	PRIMARY ADDRESSING
//--------------------------

/**
* \brief           Write primary address reading (primary address based)MBUS frame to buffer for serial sending
* \param[out]      ring_buffer:
*/
void mbus_read_primary_address_PA(uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame = { 0 };

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_BROADCAST_REPLY;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	frame.data[frame.data_size++] = MBUS_DIF_SELECTION_FOR_READOUT;
	frame.data[frame.data_size++] = MBUS_VIF_PRIMARY_ADDRESS_RECORD;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write secondary address reading (primary address based)MBUS frame to buffer for serial sending
* \param[in]       address:
* \param[out]      ring_buffer:
*/
void mbus_read_secondary_address_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	frame.data[frame.data_size++] = MBUS_DIF_SELECTION_FOR_READOUT;
	frame.data[frame.data_size++] = MBUS_VIF_SECONDARY_ADDRESS_RECORD;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write primary address setting (primary address based)MBUS frame to buffer for serial sending
* \param[in]       primary_address:
* \param[in]	   new_primary_address:
* \param[out]      ring_buffer:
*/
void mbus_set_primary_address_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_PRIMARY_ADDRESS_RECORD;
	frame.data[frame.data_size++] = mbus->new_primary_address;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write secondary address setting (primary address based)MBUS frame to buffer for serial sending
* \param[in]       primary_address:
* \param[in]       secondary_address:
* \param[out]      ring_buffer:
*/
void mbus_set_secondary_address_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_SECONDARY_ADDRESS_RECORD;
	mbus_data_bcd_encode(buff, 4, mbus->new_secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write complate identification setting (primary address based)MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_complate_identification_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	frame.data[frame.data_size++] = MBUS_DIF_64_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_SECONDARY_ADDRESS_RECORD;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = mbus->generation;
	frame.data[frame.data_size++] = mbus->medium;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write baudrate (primary address based)MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_baudrate_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	frame.data_size = 0;

	frame.type = MBUS_FRAME_TYPE_CONTROL;
	frame.start1 = MBUS_FRAME_CONTROL_START;
	frame.start2 = MBUS_FRAME_CONTROL_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = mbus->primary_address;
	if ((mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_300) || (mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_600) ||
		(mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_1200) || (mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_2400) ||
		(mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_4800) || (mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_9600)) {
		frame.control_information = mbus->baudrate;
	}
	else {
		frame.control_information = MBUS_CONTROL_INFO_SET_BAUDRATE_2400;
	}
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read baudrate (primary address based)MBUS frame to buffer for serial sending
* \param[in]	   mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_baudrate_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = mbus->primary_address;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	frame.data[frame.data_size++] = MBUS_DIF_SELECTION_FOR_READOUT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_BAUDRATE;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}


//B.	SECONDARY ADDRESSING
//--------------------------

/**
* \brief           Write change primary address MBUS frame to buffer for serial sending
* \param[in]       primary_address:
* \param[in]	   new_primary_address:
* \param[out]      ring_buffer:
*/
void mbus_set_primary_address(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_PRIMARY_ADDRESS_RECORD;
	frame.data[frame.data_size++] = mbus->new_primary_address;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write change secondary address MBUS frame to buffer for serial sending
* \param[in]       primary_address:
* \param[in]	   new_primary_address:
* \param[out]      ring_buffer:
*/
void mbus_set_secondary_address_SA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_SECONDARY_ADDRESS_RECORD;
	mbus_data_bcd_encode(buff, 4, mbus->new_secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set baudrate MBUS frame to buffer for serial sending
* \param[in]       primary_address:
* \param[in]	   baudrate:
* \param[out]      ring_buffer:
*/
void mbus_set_baudrate(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;

	if ((mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_300) || (mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_600) ||
		(mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_1200) || (mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_2400) ||
		(mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_4800) || (mbus->baudrate == MBUS_CONTROL_INFO_SET_BAUDRATE_9600)) {
		frame.control_information = mbus->baudrate;
	}
	else {
		frame.control_information = MBUS_CONTROL_INFO_SET_BAUDRATE_2400;
	}

	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}


/**
* \brief           Write set calibration mode MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_service_mode(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_SERVICE_MODE;
	frame.data[frame.data_size++] = mbus->service_mode_status;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set calibration current MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_calibration_current(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_16_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_CALIBRATE_CURRENT_SENSOR;
	frame.data[frame.data_size++] = mbus->calibration_current_mA & 0x00FF;
	frame.data[frame.data_size++] = (mbus->calibration_current_mA >> 8) & 0x00FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read calibration current MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_calibration_current(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_CALIBRATION_CURRENT;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set MBUS power output(on, off) MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_mbus_power(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_MBUS_POWER;
	frame.data[frame.data_size++] = mbus->mbus_power_status;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write lock device MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_lock_device(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_SUPER_PASSWORD;
	frame.data[frame.data_size++] = mbus->admin_password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->admin_password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->admin_password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->admin_password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_LOCK_DEVICE;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write unlock device MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_unlock_device(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_SUPER_PASSWORD;
	frame.data[frame.data_size++] = mbus->admin_password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->admin_password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->admin_password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->admin_password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_UNLOCK_DEVICE;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set customer location MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_customer_location(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_CUSTOMER_LOCATION;
	frame.data[frame.data_size++] = mbus->customer_location_1 & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_1 >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_1 >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_1 >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_CUSTOMER_LOCATION;
	frame.data[frame.data_size++] = mbus->customer_location_2 & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_2 >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_2 >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_2 >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_CUSTOMER_LOCATION;
	frame.data[frame.data_size++] = mbus->customer_location_3 & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_3 >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_3 >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->customer_location_3 >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read customer location MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_customer_location(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_CUSTOMER_LOCATION;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write change password MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_change_password(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_CHANGE_PASSWORD;
	frame.data[frame.data_size++] = mbus->new_password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->new_password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->new_password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->new_password >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write fabrication reset MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_fabrication_reset(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_FABRICATION_RESET;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write fabrication resetexcept calibration MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_fabrication_reset_except_calibration(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;

	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_FABRICATION_RESET_EXCEPT_CALIBRATION;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write select slave MBUS frame to buffer for serial sending
* \param[in]       secondary_address:
* \param[out]      ring_buffer:
*/
void mbus_select_slave(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_SELECT_SLAVE;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read super password MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_super_password(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_SUPER_PASSWORD;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set short or over current recovery periode MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_recovery_periode(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_RECOVERY_PERIODE;
	frame.data[frame.data_size++] = mbus->recovery_periode & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_periode >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_periode >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_periode >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_FAST_RECOVERY_PERIODE;
	frame.data[frame.data_size++] = mbus->fast_recovery_periode & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_periode >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_periode >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_periode >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read short or over current recovery periode MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_recovery_periode(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_RECOVERY_PERIODE;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set short or over current recovery try count MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_recovery_try_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_RECOVERY_TRY_COUNT;
	frame.data[frame.data_size++] = mbus->recovery_try_count & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_try_count >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_try_count >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->recovery_try_count >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_FAST_RECOVERY_TRY_COUNT;
	frame.data[frame.data_size++] = mbus->fast_recovery_try_count & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_try_count >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_try_count >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->fast_recovery_try_count >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read short or over current recovery try count MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_recovery_try_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_RECOVERY_TRY_COUNT;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write restart device MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_restart_device(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_DEVICE_RESTART;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set over current limit MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_over_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_OVER_CURRENT_LIMIT;
	frame.data[frame.data_size++] = mbus->over_current_limit & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->over_current_limit >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->over_current_limit >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->over_current_limit >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read over current limit MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_over_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_OVER_CURRENT_LIMIT;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set breaking current limit MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_breaking_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_BREAKING_CURRENT_LIMIT;
	frame.data[frame.data_size++] = mbus->breaking_current_limit & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->breaking_current_limit >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->breaking_current_limit >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->breaking_current_limit >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read breaking current limit MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_breaking_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_BREAKING_CURRENT_LIMIT;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write reset calibration MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_reset_calibration(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_RESET_CALIBRATION;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write reset storage MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_reset_storage(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_RESET_STORAGE;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set breaking current compare match count MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_breaking_current_compare_match_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_BREAKING_CURRENT_COMPARE_MATCH_COUNT;
	frame.data[frame.data_size++] = mbus->breaking_current_compare_match_count;

	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read breaking current compare match count MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_breaking_current_compare_match_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_BREAKING_CURRENT_COMPARE_MATCH_COUNT;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write set mbus receiving current treshold MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_set_mbus_receiving_current_treshold(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint32_t temp = 0;
	uint8_t buff[4];

	frame.data_size = 0;
	float_to_hex(&mbus->f_mbus_receiving_current_treshold, &temp);
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_SET_MBUS_RECEIVING_CURRENT_TRESHOLD;
	frame.data[frame.data_size++] = temp & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (temp >> 24) & 0x000000FF;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read breaking current compare match count MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_mbus_receiving_current_treshold(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_MBUS_RECEIVING_CURRENT_TRESHOLD;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write read fault records MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_read_fault_records(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_READ_FAULT_RECORDS;
	frame.data[frame.data_size++] = MBUS_DIF_8_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_FAULT_RECORDS_GROUP_NUMBER;
	frame.data[frame.data_size++] = mbus->fault_records_group_number;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}

/**
* \brief           Write clean all fault records MBUS frame to buffer for serial sending
* \param[in]       mbus:
* \param[out]      ring_buffer:
*/
void mbus_clean_all_fault_records(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size) {
	mbus_frame frame;
	uint8_t buff[4];

	frame.data_size = 0;
	frame.type = MBUS_FRAME_TYPE_LONG;
	frame.start1 = MBUS_FRAME_LONG_START;
	frame.start2 = MBUS_FRAME_LONG_START;
	frame.control = MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB;
	frame.address = MBUS_ADDRESS_NETWORK_LAYER;
	frame.control_information = MBUS_CONTROL_INFO_DATA_SEND;
	mbus_data_bcd_encode(buff, 4, mbus->secondary_address);
	frame.data[frame.data_size++] = buff[3];
	frame.data[frame.data_size++] = buff[2];
	frame.data[frame.data_size++] = buff[1];
	frame.data[frame.data_size++] = buff[0];
	frame.data[frame.data_size++] = mbus->manufacturer & 0x00FF;
	frame.data[frame.data_size++] = (mbus->manufacturer >> 8) & 0x00FF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = 0xFF;
	frame.data[frame.data_size++] = MBUS_DIF_32_BIT_INT;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_ENTER_PASSWORD;
	frame.data[frame.data_size++] = mbus->password & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 8) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 16) & 0x000000FF;
	frame.data[frame.data_size++] = (mbus->password >> 24) & 0x000000FF;
	frame.data[frame.data_size++] = MBUS_DIF_NO_DATA;
	frame.data[frame.data_size++] = MBUS_VIF_EXTENSION_VIFE_FALLOW;
	frame.data[frame.data_size++] = MBUS_VIFE_CLEAN_ALL_FAULT_RECORDS;
	frame.stop = MBUS_FRAME_STOP;

	mbus_frame_calc_length(&frame);
	mbus_frame_calc_checksum(&frame);
	*data_size = mbus_frame_pack(&frame, data_buffer, buffer_size);
}




void float_to_hex(float* source, uint32_t* dest) {
	uint8_t* charPtr;
	uint8_t i;

	charPtr = (uint8_t*)source;

	for (i = 0; i < 4; i++)
	{
		*dest |= ((*(charPtr + i)) << (i * 8));
	}
}


void hex_to_float(uint32_t* source, float* dest) {
	*dest = *((float*)(source));
}

#endif /* MBUS_MASTER_CFG_NT750_ENABLED */

#endif /* GATEWAY_TYPE == MBUS */


