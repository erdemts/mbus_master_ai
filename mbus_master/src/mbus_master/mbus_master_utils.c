/**
* \file            mbus_master_utils.c
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
#include <stdio.h>
#include <string.h>

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
uint8_t mbus_master_get_snd_nke_type(mbus_master_t* mbus_master) {
	if (mbus_master == NULL) {
		return 0; /* Default to SND_NKE Network */
	}
	uint8_t snd_nke_type = (mbus_master->read_type >> 2) & 0x03;
	return snd_nke_type;
}

/**
 * \brief           Get slave select type from mbus_master structure
 * \param[in]       mbus_master: Pointer to mbus_master structure
 * \return          Slave select type (0: 0x53 command, 1: 0x73 command)
 * \details         This function extracts the slave select type from the read_type field of the mbus_master structure.
 *                  It returns the corresponding slave select type as an integer value.
 */
uint8_t mbus_master_get_slave_select_type(mbus_master_t* mbus_master) {
	if (mbus_master == NULL) {
		return 0; /* Default to 0x53 command */
	}
	uint8_t slave_select_type = (mbus_master->read_type >> 1) & 0x01;
	return slave_select_type;
}

/**
 * \brief           Get ACK type from mbus_master structure
 * \param[in]       mbus_master: Pointer to mbus_master structure
 * \return          ACK type (0: ACK, 1: No ACK)
 * \details         This function extracts the ACK type from the read_type field of the mbus_master structure.
 *                  It returns the corresponding ACK type as an integer value.
 */
uint8_t mbus_master_get_ack_type(mbus_master_t* mbus_master) {
	if (mbus_master == NULL) {
		return 0; /* Default to ACK */
	}
	uint8_t ack_type = mbus_master->read_type & 0x01;
	return ack_type;
}

/**
* \brief 		 Verify MBUS frame integrity and optionally match ID
* \param[in] 	 rcv: Pointer to received data structure
* \param[in] 	 expected_mbus_id: Expected MBUS ID to match (0 to skip check)
* \return 		 Result of verification: MBUS_MASTER_SLAVE_RESPONSE_SUCCESS if valid, MBUS_MASTER_ERR otherwise
* \details		 This function verifies the integrity of the received MBUS frame by checking start/stop bytes, length consistency, and checksum.
* 				If an expected MBUS ID is provided (non-zero), it also checks if the ID in the frame matches the expected ID.
* 				The function is optimized for Cortex-M0 with early exits and minimal code size.
*/
mbus_master_res_t mbus_master_verify_frame(mbus_master_recv_t * rcv, uint32_t expected_mbus_id) {
	/* Cortex-M0 optimized: small code, early exits, pointer arithmetic */
	if (rcv == NULL) {
		return MBUS_MASTER_ERR;
	}
	/* Defensive: reject corrupted length larger than buffer capacity */
	if (rcv->len > sizeof(rcv->data)) {
		return MBUS_MASTER_ERR;
	}
	const uint8_t* d = (const uint8_t*)rcv->data;
	/* Quick minimal frame length check (short/long header + stop + checksum) */
	if (rcv->len < 9) {
		return MBUS_MASTER_ERR;
	}
	/* Start bytes */
	if (d[0] != 0x68 || d[3] != 0x68) {
		return MBUS_MASTER_ERR;
	}
	uint8_t len1 = d[1];
	if (len1 != d[2]) {
		return MBUS_MASTER_ERR;
	}
	if (len1 < 6) { /* avoid underflow / invalid */
		return MBUS_MASTER_ERR;
	}
	/* Total expected frame length = len1 data bytes + 6 overhead bytes */
	if ((uint16_t)len1 + 6u > rcv->len) {
		return MBUS_MASTER_ERR;
	}
	/* Index positions */
	uint16_t checksum_index = (uint16_t)len1 + 4u;      /* 4 header bytes before data */
	uint16_t stop_index = checksum_index + 1u;
	/* Bounds check against received buffer */
	if (stop_index >= rcv->len) {
		return MBUS_MASTER_ERR;
	}
	/* Compute checksum over len1 data bytes starting at offset 4 */
	uint8_t csum = 0;                       /* 8-bit arithmetic sum (natural wrap) */
	const uint8_t* p = d + 4;               /* data start */
	uint8_t remaining = len1;               /* number of bytes in data section */
	while (remaining--) {
		csum += *p++;
	}
	if (csum != d[checksum_index]) {
		return MBUS_MASTER_ERR;
	}
	if (d[stop_index] != 0x16) {
		return MBUS_MASTER_ERR;
	}
	/* Optional ID match check: only if caller provided a non-zero expected ID */
	if (expected_mbus_id != 0) {
		/* Data layout: after 4 header bytes -> C(0),A(1),CI(2), then variable data
		   ID is first 4 bytes of variable data -> positions 7..10 overall. */
		if (len1 >= 7) {
			uint32_t parsed_id = ((uint32_t)d[10] << 24) | ((uint32_t)d[9] << 16) | ((uint32_t)d[8] << 8) | d[7];
			if (parsed_id != 0 && parsed_id != expected_mbus_id) {
				return MBUS_MASTER_ERR;
			}
		} else {
			return MBUS_MASTER_ERR; /* Not enough bytes to contain an ID */
		}
	}
	return MBUS_MASTER_SLAVE_RESPONSE_SUCCESS;
}



/**
 * \brief           Convert hex array to uppercase ASCII hex string
 * \param[in]       rcv: Pointer to received data structure
 * \param[out]      str: Pointer to output string buffer
 * \details         This function converts a hex array in the received data structure to an uppercase ASCII hex string.
 *                  The caller must ensure that the output string buffer is large enough to hold the resulting string.
 *                  The output string will be null-terminated.
 */
void hex_array_to_string(mbus_master_recv_t* rcv, uint8_t* str) {
	/* Cortex-M0 optimized hex array -> uppercase ASCII hex string converter
	 * - Avoids heavy sprintf (reduces code size & stack usage)
	 * - Uses simple nibble lookup table, only shifts & masks
	 * - Caller must ensure 'str' buffer size >= rcv->len * 2 + 1
	 */
	if (!rcv || !str) {
		return;
	}
	uint32_t needed = (uint32_t)rcv->len * 2u + 1u;
	if (needed > (MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE * 2)) {
		/* Güvenlik: sýðmayacaksa mümkün olaný yaz (truncate) */
		uint32_t max_bytes = ((MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE * 2) - 1u) / 2u;
		if (rcv->len > max_bytes) {
			rcv->len = max_bytes; /* Lokal kopya degil, ama sadece cikti icin kýsýtlýyoruz */
		}
	}
	const uint8_t* in = (const uint8_t*)rcv->data;
	uint32_t len = (uint32_t)rcv->len;
	static const char hex[] = "0123456789ABCDEF"; /* Flash */
	uint8_t* out = str;
	while (len--) {
		uint8_t b = *in++;
		*out++ = (uint8_t)hex[b >> 4];
		*out++ = (uint8_t)hex[b & 0x0F];
	}
	*out = 0; /* Null-terminate */
}

/**
 * \brief           Convert decimal to BCD
 * \param[in]       dec: Decimal value to convert
 * \return          BCD representation of the decimal value
 * \details         This function converts a decimal value to its Binary-Coded Decimal (BCD) representation.
 */
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

/**
 * \brief           Convert BCD to decimal
 * \param[in]       bcd: BCD value to convert
 * \return          Decimal representation of the BCD value
 * \details         This function converts a Binary-Coded Decimal (BCD) value to its decimal representation.
 */
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
void mbus_master_add_as_char_to_mbus_data_buffer(mbus_master_buffer_t* buffer, unsigned char* data) {
	const char* hex = "0123456789ABCDEF";
	unsigned char a, b;

	a = hex[(*data >> 4) & 0xF];
	b = hex[*data & 0xF];

	buffer->data[buffer->tail++] = a;
	if (buffer->tail == MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE) {
		buffer->tail = 0;
	}
	buffer->data[buffer->tail++] = b;
	if (buffer->tail == MBUS_MASTER_CFG_RECEIVE_BUFFER_SIZE) {
		buffer->tail = 0;
	}
}

/**
 * \brief           Reset mbus_master context
 * \param[in]       ctx: Pointer to mbus_master structure
 * \details         This function resets the mbus_master context by clearing errors and setting the state to idle.
 */
void mbus_master_reset_ctx(mbus_lib_t* ctx) {
	memset(&ctx->master.receive_buffer, 0, sizeof(ctx->master.receive_buffer));
	ctx->master.error.all = 0;
	ctx->master.state = MBUS_MASTER_STATE_IDLE;
}

//typedef enum {
//	MBUS_MASTER_OK = 0,
//	MBUS_MASTER_SLAVE_RESPONSE_SUCCESS = 0,                          /*!< Slave response success response type */
//	MBUS_MASTER_ERR_E5_NOT_RECEIVED = -1,                            /*!< E5 not received response type */
//	MBUS_MASTER_ERR_SLAVE_NOT_RESPONDING = -2,                       /*!< Slave not responding response type */
//	MBUS_MASTER_ERR_INVALID_SLAVE_ID = -3,                           /*!< Invalid slave id */
//	MBUS_MASTER_ERR_INVALID_COMMAND = -4,                            /*!< Invalid command response type */
//	MBUS_MASTER_ERR_NOT_INITIALIZED = -5,                            /*!< Not initialized response type */
//	MBUS_MASTER_ERR = -6,                                            /*!< Generic error response type */
//} mbus_master_res_t;

/**
 * \brief           Get string representation of mbus_master response
 * \param[in]       r: Pointer to mbus_master response
 * \return          String representation of the response
 * \details         This function returns a string representation of the mbus_master response.
 *                  It is useful for logging and debugging purposes.
 */
const char* get_mbus_response_string(const mbus_master_res_t* r) {
	if (r == NULL) return "NULL";
	switch (*r) {
	case MBUS_MASTER_SLAVE_RESPONSE_SUCCESS: return "OK";
	case MBUS_MASTER_ERR_E5_NOT_RECEIVED:    return "E5_NOT_RECEIVED";
	case MBUS_MASTER_ERR_SLAVE_NOT_RESPONDING: return "SLAVE_NOT_RESPONDING";
	case MBUS_MASTER_ERR_INVALID_SLAVE_ID:   return "INVALID_SLAVE_ID";
	case MBUS_MASTER_ERR_INVALID_COMMAND:    return "INVALID_COMMAND";
	case MBUS_MASTER_ERR_NOT_INITIALIZED:    return "NOT_INITIALIZED";
	case MBUS_MASTER_ERR:                    return "ERR";
	default: return "UNKNOWN";
	}
}


#endif /* GATEWAY_TYPE == MBUS */

