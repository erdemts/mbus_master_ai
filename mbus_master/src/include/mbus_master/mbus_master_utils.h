/**
* \file            mbus_master_utils.h
* \brief           mbus_master utils functions
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

#ifndef MBUS_MASTER_UTILS_H
#define MBUS_MASTER_UTILS_H

#include <stdint.h>
#include "mbus_master/mbus_master_types.h"



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MBUS_UNUSED(x)                   ((void)(x))

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

	/**
	 * \brief           Get SND_NKE type from mbus_master structure
	 * \param[in]       mbus_master: Pointer to mbus_master structure
	 * \return          SND_NKE type (0: SND_NKE Network, 1: SND_NKE, 2: No SND_NKE)
	 * \details         This function extracts the SND_NKE type from the read_type field of the mbus_master structure.
	 *                  It returns the corresponding SND_NKE type as an integer value.
	 */
	uint8_t mbus_master_get_snd_nke_type(mbus_master_t* mbus_master);

	/**
	 * \brief           Get slave select type from mbus_master structure
	 * \param[in]       mbus_master: Pointer to mbus_master structure
	 * \return          Slave select type (0: 0x53 command, 1: 0x73 command)
	 * \details         This function extracts the slave select type from the read_type field of the mbus_master structure.
	 *                  It returns the corresponding slave select type as an integer value.
	 */
	uint8_t mbus_master_get_slave_select_type(mbus_master_t* mbus_master);

	/**
	 * \brief           Get ACK type from mbus_master structure
	 * \param[in]       mbus_master: Pointer to mbus_master structure
	 * \return          ACK type (0: ACK, 1: No ACK)
	 * \details         This function extracts the ACK type from the read_type field of the mbus_master structure.
	 *                  It returns the corresponding ACK type as an integer value.
	 */
	uint8_t mbus_master_get_ack_type(mbus_master_t* mbus_master);

	/**
	* \brief 		 Verify MBUS frame integrity and optionally match ID
	* \param[in] 	 rcv: Pointer to received data structure
	* \param[in] 	 expected_mbus_id: Expected MBUS ID to match (0 to skip check)
	* \return 		 Result of verification: MBUS_MASTER_SLAVE_RESPONSE_SUCCESS if valid, MBUS_MASTER_ERR otherwise
	* \details		 This function verifies the integrity of the received MBUS frame by checking start/stop bytes, length consistency, and checksum.
	* 				If an expected MBUS ID is provided (non-zero), it also checks if the ID in the frame matches the expected ID.
	* 				The function is optimized for Cortex-M0 with early exits and minimal code size.
	*/
	mbus_master_res_t mbus_master_verify_frame(mbus_master_recv_t* rcv, uint32_t expected_mbus_id);

	/**
	 * \brief           Convert hex array to uppercase ASCII hex string
	 * \param[in]       rcv: Pointer to received data structure
	 * \param[out]      str: Pointer to output string buffer
	 * \details         This function converts a hex array in the received data structure to an uppercase ASCII hex string.
	 *                  The caller must ensure that the output string buffer is large enough to hold the resulting string.
	 *                  The output string will be null-terminated.
	 */
	void hex_array_to_string(mbus_master_recv_t* rcv, uint8_t* str);

	/**
	 * \brief           Convert decimal to BCD
	 * \param[in]       dec: Decimal value to convert
	 * \return          BCD representation of the decimal value
	 * \details         This function converts a decimal value to its Binary-Coded Decimal (BCD) representation.
	 */
	unsigned int mbus_master_dec_to_bcd(unsigned int dec);

	/**
	 * \brief           Convert BCD to decimal
	 * \param[in]       bcd: BCD value to convert
	 * \return          Decimal representation of the BCD value
	 * \details         This function converts a Binary-Coded Decimal (BCD) value to its decimal representation.
	 */
	unsigned int mbus_master_bcd_to_dec(unsigned int bcd);

	/**
	* \brief           Add data to mbus_master simple buffer as char
	* \param[in,out]   buffer: Pointer to mbus_master simple buffer
	* \param[in]       data: Pointer to data to be added
	* \details         This function adds data to the mbus_master simple buffer as characters.
	* *                  It converts each byte of data into two hexadecimal characters
	* *                  and adds them to the buffer.
	* * \note            The buffer must have enough space to accommodate the new data.
	* * \warning         If the buffer is full, the tail pointer will wrap around to the beginning.
	*/
	void mbus_master_add_as_char_to_mbus_data_buffer(mbus_master_buffer_t* buffer, unsigned char* data);

	/**
	 * \brief           Reset mbus_master context
	 * \param[in]       ctx: Pointer to mbus_master structure
	 * \details         This function resets the mbus_master context by clearing errors and setting the state to idle.
	 */
	void mbus_master_reset_ctx(mbus_lib_t* ctx);
	
	/**
	 * \brief           Get string representation of mbus_master response
	 * \param[in]       r: Pointer to mbus_master response
	 * \return          String representation of the response
	 * \details         This function returns a string representation of the mbus_master response.
	 *                  It is useful for logging and debugging purposes.
	 */
	const char* get_mbus_response_string(const mbus_master_res_t* r);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBUS_MASTER_UTILS_H */

#endif /* GATEWAY_TYPE == MBUS */
