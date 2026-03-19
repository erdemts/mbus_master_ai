/**
* \file            mbus_master_int.c
* \brief           mbus_master internal functions
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

#include "mbus_master/mbus_master_types.h"
#include "mbus_master/mbus_master_utils.h"


//mbus_master_recv_t mbus_master_recv;



///* Receive character macros */
//#define RECV_ADD(ch)                                                        \
//    do {                                                                    \
//        if (mbus_master_recv.len < (sizeof(mbus_master_recv.data)) - 1) {   \
//            mbus_master_recv.data[mbus_master_recv.len++] = ch;             \
//            mbus_master_recv.data[mbus_master_recv.len] = 0;                \
//        }                                                                   \
//    } while (0)
//#define RECV_RESET()                                                        \
//    do {                                                                    \
//        mbus_master_recv.len = 0;                                           \
//        mbus_master_recv.data[0] = 0;                                       \
//    } while (0)
//#define RECV_LEN()                  ((size_t)mbus_master_recv.len)
//#define RECV_IDX(index)             mbus_master_recv.data[index]



/**
 * \brief           Parse received MBUS data based on current state
 * \param[in]       master: Pointer to mbus_master structure
 * \param[in]       recv: Pointer to received data structure
 * \details         This function parses the received MBUS data based on the current state of the mbus_master.
 *                  It handles different states such as waiting for E5 and waiting for slave response.
 *                  The function updates the state of the mbus_master accordingly.
 */
 //void mbus_master_parse_received(mbus_master_t* master, mbus_master_recv_t* recv) {
 //	uint8_t ch;
 //	const uint8_t* d = (const uint8_t*)recv->data;
 //	size_t d_len = recv->len;


 //	if (master->state == MBUS_MASTER_STATE_WAIT_E5) {
 //		if (recv->len == 1 && recv->data[0] == MBUS_ACK) {
 //			master->state = MBUS_MASTER_STATE_E5_RECEIVED;
 //		}
 //	}
 //	else if (master->state == MBUS_MASTER_STATE_WAIT_SLAVE_RESPONSE) {
 //		master->state = MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS;
 //		if (mbus_master_verify_frame(recv, master->mbus_id_bcd) == MBUS_MASTER_SLAVE_RESPONSE_SUCCESS) {
 //			master->state = MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS;
 //		}
 //		else {
 //			// Invalid frame, reset state
 //			master->state = MBUS_MASTER_STATE_SLAVE_NOT_RESPONDING;
 //		}
 //	}
 //}

 /**
  * \brief           Process incoming MBUS data
  * \param[in]       master: Pointer to mbus_master structure
  * \param[in]       data: Pointer to incoming data buffer
  * \param[in]       data_len: Length of incoming data buffer
  * \return
  * \details         This function processes incoming MBUS data based on the current state of the mbus_master.
  *                  It handles different states such as waiting for E5 and waiting for slave response.
  *                  The function updates the state of the mbus_master accordingly and parses the received data.
  */
void mbus_master_process(mbus_lib_t* mbus_master, const void* data, size_t data_len) {
	uint8_t ch;
	const uint8_t* d = data;
	size_t d_len = data_len;
	static uint32_t mbus_msg_len = 0;
	static uint8_t checksum = 0;
	uint32_t received_mbus_id = 0;
	uint32_t i = 0;

	while (d_len > 0) {
		ch = *d++;
		--d_len;
		if (mbus_master->master.state == MBUS_MASTER_STATE_WAIT_E5) {
			if (ch == MBUS_ACK) {
				mbus_master->master.state = MBUS_MASTER_STATE_E5_RECEIVED;
				mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
			}
		}
		else if (mbus_master->master.state == MBUS_MASTER_STATE_WAIT_SLAVE_RESPONSE) {
			if (mbus_master->master.sub_state == MBUS_MASTER_SUB_STATE_WAIT_FIRST_START) {
				checksum = 0;

				if (ch == MBUS_START) {
					mbus_master_add_as_char_to_mbus_data_buffer(&mbus_master->master.receive_buffer, &ch);
					mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_LEN;
				}
			}
			else if (mbus_master->master.sub_state == MBUS_MASTER_SUB_STATE_WAIT_FIRST_LEN) {
				mbus_msg_len = ch;
				mbus_master_add_as_char_to_mbus_data_buffer(&mbus_master->master.receive_buffer, &ch);
				mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_SECOND_LEN;
			}
			else if (mbus_master->master.sub_state == MBUS_MASTER_SUB_STATE_WAIT_SECOND_LEN) {
				if (mbus_msg_len == ch) {
					mbus_master_add_as_char_to_mbus_data_buffer(&mbus_master->master.receive_buffer, &ch);
					mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_SECOND_START;
				}
				else {
					mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
				}
			}
			else if (mbus_master->master.sub_state == MBUS_MASTER_SUB_STATE_WAIT_SECOND_START) {
				if (ch == 0x68) {
					mbus_msg_len = mbus_msg_len + 6 - 4;
					mbus_master_add_as_char_to_mbus_data_buffer(&mbus_master->master.receive_buffer, &ch);
					mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_DATA;
				}
				else {
					mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
				}
			}
			else if (mbus_master->master.sub_state == MBUS_MASTER_SUB_STATE_WAIT_DATA) {
				if (mbus_msg_len > 2) {
					mbus_master_add_as_char_to_mbus_data_buffer(&mbus_master->master.receive_buffer, &ch);
					checksum += ch;
					mbus_msg_len--;
				}
				else {
					if (ch == checksum) {
						mbus_master_add_as_char_to_mbus_data_buffer(&mbus_master->master.receive_buffer, &ch);
						mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_STOP;
					}
					else {
						mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
					}
				}
			}
			else if (mbus_master->master.sub_state == MBUS_MASTER_SUB_STATE_WAIT_STOP) {
				if (ch == MBUS_STOP) {
					mbus_master_add_as_char_to_mbus_data_buffer(&mbus_master->master.receive_buffer, &ch);
					mbus_master->master.receive_buffer.data[mbus_master->master.receive_buffer.tail++] = 0;

					if (mbus_master->master.address_mode == MBUS_MASTER_SECONDARY_ADDRESS_MODE) {
						received_mbus_id = mbus_master->master.receive_buffer.data[20] - 48;
						received_mbus_id <<= 4;
						received_mbus_id |= mbus_master->master.receive_buffer.data[21] - 48;
						received_mbus_id <<= 4;
						received_mbus_id |= mbus_master->master.receive_buffer.data[18] - 48;
						received_mbus_id <<= 4;
						received_mbus_id |= mbus_master->master.receive_buffer.data[19] - 48;
						received_mbus_id <<= 4;
						received_mbus_id |= mbus_master->master.receive_buffer.data[16] - 48;
						received_mbus_id <<= 4;
						received_mbus_id |= mbus_master->master.receive_buffer.data[17] - 48;
						received_mbus_id <<= 4;
						received_mbus_id |= mbus_master->master.receive_buffer.data[14] - 48;
						received_mbus_id <<= 4;
						received_mbus_id |= mbus_master->master.receive_buffer.data[15] - 48;

						if (received_mbus_id == mbus_master->master.mbus_id) {
							mbus_master->master.state = MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS;
						}
						else {
							mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
							mbus_master->master.state = MBUS_MASTER_STATE_SLAVE_NOT_RESPONDING;
							mbus_master->master.error.invalid_slave_id = 1; // Set invalid slave id error
						}
					}
					if (mbus_master->master.address_mode == MBUS_MASTER_PRIMARY_ADDRESS_MODE) {
						mbus_master->master.state = MBUS_MASTER_STATE_SLAVE_RESPONSE_SUCCESS;
					}
				}
				else {
					mbus_master->master.sub_state = MBUS_MASTER_SUB_STATE_WAIT_FIRST_START;
				}
			}
		}
	}
}

#endif /* GATEWAY_TYPE == MBUS */

