//------------------------------------------------------------------------------
// Copyright (C) 2010-2011, Robert Johansson, Raditex AB
// All rights reserved.
//
// rSCADA
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------
//#include "pch.h"



#include "mbus_master/mbus_master.h"

#if (MBUS_MASTER_CFG_NT750_ENABLED)

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static int parse_debug = 0;
static char error_str[512];

#define NITEMS(x) (sizeof(x)/sizeof(x[0]))

//------------------------------------------------------------------------------
// Returns the manufacturer ID according to the manufacturer's 3 byte ASCII code
// or zero when there was an error.
//------------------------------------------------------------------------------
unsigned int
mbus_manufacturer_id(char* manufacturer)
{
	unsigned int id;

	/*
	 * manufacturer must consist of at least 3 alphabetic characters,
	 * additional chars are silently ignored.
	 */

	if (!manufacturer || strlen(manufacturer) < 3)
		return 0;

	if (!isalpha(manufacturer[0]) ||
		!isalpha(manufacturer[1]) ||
		!isalpha(manufacturer[2]))
		return 0;

	id = (toupper(manufacturer[0]) - 64) * 32 * 32 +
		(toupper(manufacturer[1]) - 64) * 32 +
		(toupper(manufacturer[2]) - 64);

	/*
	 * Valid input data should be in the range of 'AAA' to 'ZZZ' according to
	 * the FLAG Association (http://www.dlms.com/flag/), thus resulting in
	 * an ID from 0x0421 to 0x6b5a. If the conversion results in anything not
	 * in this range, simply discard it and return 0 instead.
	 */
	return (0x0421 <= id && id <= 0x6b5a) ? id : 0;
}


//------------------------------------------------------------------------------
/// Return a string that contains an the latest error message.
//------------------------------------------------------------------------------
char*
mbus_error_str(void)
{
	return error_str;
}

void
mbus_error_str_set(char* message)
{
	if (message)
	{
		snprintf(error_str, sizeof(error_str), "%s", message);
	}
}

void
mbus_error_reset(void)
{
	snprintf(error_str, sizeof(error_str), "no errors");
}


//------------------------------------------------------------------------------
//
// M-Bus FRAME RELATED FUNCTIONS
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/// Caclulate the checksum of the M-Bus frame. Internal.
//------------------------------------------------------------------------------
unsigned char
calc_checksum(mbus_frame* frame)
{
	size_t i;
	unsigned char cksum;

	//    assert(frame != NULL);
	if (frame == NULL) {
		return 0;
	}

	switch (frame->type)
	{
	case MBUS_FRAME_TYPE_SHORT:

		cksum = frame->control;
		cksum += frame->address;

		break;

	case MBUS_FRAME_TYPE_CONTROL:

		cksum = frame->control;
		cksum += frame->address;
		cksum += frame->control_information;

		break;

	case MBUS_FRAME_TYPE_LONG:

		cksum = frame->control;
		cksum += frame->address;
		cksum += frame->control_information;

		for (i = 0; i < frame->data_size; i++)
		{
			cksum += frame->data[i];
		}

		break;

	case MBUS_FRAME_TYPE_ACK:
	default:
		cksum = 0;
	}

	return cksum;
}

//------------------------------------------------------------------------------
/// Caclulate the checksum of the M-Bus frame. The checksum algorithm is the
/// arithmetic sum of the frame content, without using carry. Which content
/// that is included in the checksum calculation depends on the frame type.
//------------------------------------------------------------------------------
int
mbus_frame_calc_checksum(mbus_frame* frame)
{
	if (frame)
	{
		switch (frame->type)
		{
		case MBUS_FRAME_TYPE_ACK:
		case MBUS_FRAME_TYPE_SHORT:
		case MBUS_FRAME_TYPE_CONTROL:
		case MBUS_FRAME_TYPE_LONG:
			frame->checksum = calc_checksum(frame);

			break;

		default:
			return -1;
		}
	}

	return 0;
}

///
/// Calculate the values of the lengths fields in the M-Bus frame. Internal.
///
unsigned char
calc_length(const mbus_frame* frame)
{
	//    assert(frame != NULL);
	if (frame == NULL) {
		return 0;
	}
	switch (frame->type)
	{
	case MBUS_FRAME_TYPE_CONTROL:
		return 3;
	case MBUS_FRAME_TYPE_LONG:
		return (unsigned char)frame->data_size + 3;
	default:
		return 0;
	}
}

//------------------------------------------------------------------------------
/// Calculate the values of the lengths fields in the M-Bus frame.
//------------------------------------------------------------------------------
int
mbus_frame_calc_length(mbus_frame* frame)
{
	if (frame)
	{
		frame->length1 = frame->length2 = calc_length(frame);
	}

	return 0;
}

//------------------------------------------------------------------------------
/// Return the M-Bus frame type
//------------------------------------------------------------------------------
int
mbus_frame_type(mbus_frame* frame)
{
	if (frame)
	{
		return frame->type;
	}
	return -1;
}

//------------------------------------------------------------------------------
/// Return the M-Bus frame direction
//------------------------------------------------------------------------------
int
mbus_frame_direction(mbus_frame* frame)
{
	if (frame)
	{
		if (frame->type == MBUS_FRAME_TYPE_ACK)
		{
			return MBUS_CONTROL_MASK_DIR_S2M;
		}
		else
		{
			return (frame->control & MBUS_CONTROL_MASK_DIR);
		}
	}
	return -1;
}

//------------------------------------------------------------------------------
/// Verify that parsed frame is a valid M-bus frame.
//
// Possible checks:
//
// 1) frame type
// 2) Start/stop bytes
// 3) control field
// 4) length field and actual data size
// 5) checksum
//
//------------------------------------------------------------------------------
int
mbus_frame_verify(mbus_frame* frame)
{
	unsigned char checksum;

	if (frame)
	{
		switch (frame->type)
		{
		case MBUS_FRAME_TYPE_ACK:
			return frame->start1 == MBUS_FRAME_ACK_START;

		case MBUS_FRAME_TYPE_SHORT:
			if (frame->start1 != MBUS_FRAME_SHORT_START)
			{
				snprintf(error_str, sizeof(error_str), "No frame start");

				return -1;
			}

			if ((frame->control != MBUS_CONTROL_MASK_SND_NKE) &&
				(frame->control != MBUS_CONTROL_MASK_REQ_UD1) &&
				(frame->control != (MBUS_CONTROL_MASK_REQ_UD1 | MBUS_CONTROL_MASK_FCB)) &&
				(frame->control != MBUS_CONTROL_MASK_REQ_UD2) &&
				(frame->control != (MBUS_CONTROL_MASK_REQ_UD2 | MBUS_CONTROL_MASK_FCB)))
			{
				snprintf(error_str, sizeof(error_str), "Unknown Control Code 0x%.2x", frame->control);

				return -1;
			}

			break;

		case MBUS_FRAME_TYPE_CONTROL:
		case MBUS_FRAME_TYPE_LONG:
			if (frame->start1 != MBUS_FRAME_CONTROL_START ||
				frame->start2 != MBUS_FRAME_CONTROL_START)
			{
				snprintf(error_str, sizeof(error_str), "No frame start");

				return -1;
			}

			if ((frame->control != MBUS_CONTROL_MASK_SND_UD) &&
				(frame->control != (MBUS_CONTROL_MASK_SND_UD | MBUS_CONTROL_MASK_FCB)) &&
				(frame->control != MBUS_CONTROL_MASK_RSP_UD) &&
				(frame->control != (MBUS_CONTROL_MASK_RSP_UD | MBUS_CONTROL_MASK_DFC)) &&
				(frame->control != (MBUS_CONTROL_MASK_RSP_UD | MBUS_CONTROL_MASK_ACD)) &&
				(frame->control != (MBUS_CONTROL_MASK_RSP_UD | MBUS_CONTROL_MASK_DFC | MBUS_CONTROL_MASK_ACD)))
			{
				snprintf(error_str, sizeof(error_str), "Unknown Control Code 0x%.2x", frame->control);

				return -1;
			}

			if (frame->length1 != frame->length2)
			{
				snprintf(error_str, sizeof(error_str), "Frame length 1 != 2");

				return -1;
			}

			if (frame->length1 != calc_length(frame))
			{
				snprintf(error_str, sizeof(error_str), "Frame length 1 != calc length");

				return -1;
			}

			break;

		default:
			snprintf(error_str, sizeof(error_str), "Unknown frame type 0x%.2x", frame->type);

			return -1;
		}

		if (frame->stop != MBUS_FRAME_STOP)
		{
			snprintf(error_str, sizeof(error_str), "No frame stop");

			return -1;
		}

		checksum = calc_checksum(frame);

		if (frame->checksum != checksum)
		{
			snprintf(error_str, sizeof(error_str), "Invalid checksum (0x%.2x != 0x%.2x)", frame->checksum, checksum);

			return -1;
		}

		return 0;
	}

	snprintf(error_str, sizeof(error_str), "Got null pointer to frame.");

	return -1;
}



//------------------------------------------------------------------------------
//
// FIXED-LENGTH DATA RECORD FUNCTIONS
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
// PARSER FUNCTIONS
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// PARSE M-BUS frame data structures from binary data.
//------------------------------------------------------------------------------
int mbus_parse(mbus_frame* frame, const void* data_buf, size_t data_size)
{
	const uint8_t* data = data_buf;
	size_t i, len;

	if (frame && data && data_size > 0)
	{
		//frame->next = NULL;   //TODO:gerek olmadýðý için kaldýrdým.

#if MBUS_DEBUG_LOG_ENABLE
		DBG_LOG(LOG_TRACE, "%s: Attempting to parse binary data [size = %zu]\n", "mbus_parse", data_size);
		DBG_LOG(LOG_TRACE, "%s: ", "mbus_parse");

		for (i = 0; i < data_size && parse_debug; i++) {
			DBG_LOG(LOG_TRACE, "%.2X ", data[i] & 0xFF);
		}

		DBG_LOG(LOG_TRACE, "\n%s: done.\n", "mbus_parse");
#endif

		switch (data[0])
		{
		case MBUS_FRAME_ACK_START:

			// OK, got a valid ack frame, require no more data
			frame->start1 = data[0];
			frame->type = MBUS_FRAME_TYPE_ACK;
			return 1;
			//return MBUS_FRAME_BASE_SIZE_ACK - 1; // == 0

		case MBUS_FRAME_SHORT_START:

			if (data_size < MBUS_FRAME_BASE_SIZE_SHORT)
			{
				// OK, got a valid short packet start, but we need more data
				return (int)(MBUS_FRAME_BASE_SIZE_SHORT - data_size);
			}

			if (data_size != MBUS_FRAME_BASE_SIZE_SHORT)
			{
				snprintf(error_str, sizeof(error_str), "Too much data in frame.");

				// too much data... ?
				return -2;
			}

			// init frame data structure
			frame->start1 = data[0];
			frame->control = data[1];
			frame->address = data[2];
			frame->checksum = data[3];
			frame->stop = data[4];

			frame->type = MBUS_FRAME_TYPE_SHORT;

			// verify the frame
			if (mbus_frame_verify(frame) != 0)
			{
				return -3;
			}

			// successfully parsed data
			return 1;

		case MBUS_FRAME_LONG_START: // (also CONTROL)

			if (data_size < 3)
			{
				// OK, got a valid long/control packet start, but we need
				// more data to determine the length
				return (int)(3 - data_size);
			}

			// init frame data structure
			frame->start1 = data[0];
			frame->length1 = data[1];
			frame->length2 = data[2];

			if (frame->length1 < 3)
			{
				snprintf(error_str, sizeof(error_str), "Invalid M-Bus frame length.");

				// not a valid M-bus frame
				return -2;
			}

			if (frame->length1 != frame->length2)
			{
				snprintf(error_str, sizeof(error_str), "Invalid M-Bus frame length.");

				// not a valid M-bus frame
				return -2;
			}

			// check length of packet:
			len = frame->length1;

			if (data_size < (size_t)(MBUS_FRAME_FIXED_SIZE_LONG + len))
			{
				// OK, but we need more data
				return (int)(MBUS_FRAME_FIXED_SIZE_LONG + len - data_size);
			}

			if (data_size > (size_t)(MBUS_FRAME_FIXED_SIZE_LONG + len))
			{
				snprintf(error_str, sizeof(error_str), "Too much data in frame.");

				// too much data... ?
				return -2;
			}

			// we got the whole packet, continue parsing
			frame->start2 = data[3];
			frame->control = data[4];
			frame->address = data[5];
			frame->control_information = data[6];

			frame->data_size = len - 3;
			for (i = 0; i < frame->data_size; i++)
			{
				frame->data[i] = data[7 + i];
			}

			frame->checksum = data[data_size - 2]; // data[6 + frame->data_size + 1]
			frame->stop = data[data_size - 1]; // data[6 + frame->data_size + 2]

			if (frame->data_size == 0)
			{
				frame->type = MBUS_FRAME_TYPE_CONTROL;
			}
			else
			{
				frame->type = MBUS_FRAME_TYPE_LONG;
			}

			// verify the frame
			if (mbus_frame_verify(frame) != 0)
			{
				return -3;
			}

			/*frame->direction = frame->control & 0x40;*/

			// successfully parsed data
			return 1;
		default:
			snprintf(error_str, sizeof(error_str), "Invalid M-Bus frame start.");

			// not a valid M-Bus frame header (start byte)
			return -4;
		}

	}

	snprintf(error_str, sizeof(error_str), "Got null pointer to frame, data or zero data_size.");

	return -1;
}


//------------------------------------------------------------------------------
/// Pack the M-bus frame into a binary string representation that can be sent
/// on the bus. The binary packet format is different for the different types
/// of M-bus frames.
//------------------------------------------------------------------------------
int
mbus_frame_pack(mbus_frame* frame, unsigned char* data, size_t data_size)
{
	size_t i, offset = 0;

	if (frame && data)
	{
		if (mbus_frame_calc_length(frame) == -1)
		{
			return -2;
		}

		if (mbus_frame_calc_checksum(frame) == -1)
		{
			return -3;
		}

		switch (frame->type)
		{
		case MBUS_FRAME_TYPE_ACK:

			if (data_size < MBUS_FRAME_ACK_BASE_SIZE)
			{
				return -4;
			}

			data[offset++] = frame->start1;

			return (int)offset;

		case MBUS_FRAME_TYPE_SHORT:

			if (data_size < MBUS_FRAME_SHORT_BASE_SIZE)
			{
				return -4;
			}

			data[offset++] = frame->start1;
			data[offset++] = frame->control;
			data[offset++] = frame->address;
			data[offset++] = frame->checksum;
			data[offset++] = frame->stop;

			return (int)offset;

		case MBUS_FRAME_TYPE_CONTROL:

			if (data_size < MBUS_FRAME_CONTROL_BASE_SIZE)
			{
				return -4;
			}

			data[offset++] = frame->start1;
			data[offset++] = frame->length1;
			data[offset++] = frame->length2;
			data[offset++] = frame->start2;

			data[offset++] = frame->control;
			data[offset++] = frame->address;
			data[offset++] = frame->control_information;

			data[offset++] = frame->checksum;
			data[offset++] = frame->stop;

			return (int)offset;

		case MBUS_FRAME_TYPE_LONG:

			if (data_size < frame->data_size + MBUS_FRAME_LONG_BASE_SIZE)
			{
				return -4;
			}

			data[offset++] = frame->start1;
			data[offset++] = frame->length1;
			data[offset++] = frame->length2;
			data[offset++] = frame->start2;

			data[offset++] = frame->control;
			data[offset++] = frame->address;
			data[offset++] = frame->control_information;

			for (i = 0; i < frame->data_size; i++)
			{
				data[offset++] = frame->data[i];
			}

			data[offset++] = frame->checksum;
			data[offset++] = frame->stop;

			return (int)offset;

		default:
			return -5;
		}
	}

	return -1;
}

//------------------------------------------------------------------------------
//
// DATA ENCODING, DECODING, AND CONVERSION FUNCTIONS
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
///
/// Encode BCD data
///
//------------------------------------------------------------------------------
int
mbus_data_bcd_encode(unsigned char* bcd_data, size_t bcd_data_size, int value)
{
	int v0, v1, v2, x1, x2;
	size_t i;

	if (bcd_data && bcd_data_size)
	{
		v2 = abs(value);

		for (i = 0; i < bcd_data_size; i++)
		{
			v0 = v2;
			v1 = (int)(v0 / 10);
			v2 = (int)(v1 / 10);

			x1 = v0 - v1 * 10;
			x2 = v1 - v2 * 10;

			bcd_data[bcd_data_size - 1 - i] = (x2 << 4) | x1;
		}

		if (value < 0)
		{
			bcd_data[bcd_data_size - 1] |= 0xF0;
		}

		return 0;
	}

	return -1;
}

//------------------------------------------------------------------------------
///
/// Decode BCD data (decimal)
///
//------------------------------------------------------------------------------
long long
mbus_data_bcd_decode(unsigned char* bcd_data, size_t bcd_data_size)
{
	long long val = 0;
	size_t i;

	if (bcd_data)
	{
		for (i = bcd_data_size; i > 0; i--)
		{
			val = (val * 10);

			if (bcd_data[i - 1] >> 4 < 0xA)
			{
				val += ((bcd_data[i - 1] >> 4) & 0xF);
			}

			val = (val * 10) + (bcd_data[i - 1] & 0xF);
		}

		// hex code Fh in the MSD position signals a negative BCD number
		if (bcd_data[bcd_data_size - 1] >> 4 == 0xF)
		{
			val *= -1;
		}

		return val;
	}

	return -1;
}

//------------------------------------------------------------------------------
///
/// Decode BCD data (hexadecimal)
///
//------------------------------------------------------------------------------
long long
mbus_data_bcd_decode_hex(unsigned char* bcd_data, size_t bcd_data_size)
{
	long long val = 0;
	size_t i;

	if (bcd_data)
	{
		for (i = bcd_data_size; i > 0; i--)
		{
			val = (val << 8) | bcd_data[i - 1];
		}

		return val;
	}

	return -1;
}

//------------------------------------------------------------------------------
///
/// Decode INTEGER data
///
//------------------------------------------------------------------------------
int
mbus_data_int_decode(unsigned char* int_data, size_t int_data_size, int* value)
{
	size_t i;
	int neg;
	*value = 0;

	if (!int_data || (int_data_size < 1))
	{
		return -1;
	}

	neg = int_data[int_data_size - 1] & 0x80;

	for (i = int_data_size; i > 0; i--)
	{
		if (neg)
		{
			*value = (*value << 8) + (int_data[i - 1] ^ 0xFF);
		}
		else
		{
			*value = (*value << 8) + int_data[i - 1];
		}
	}

	if (neg)
	{
		*value = *value * -1 - 1;
	}

	return 0;
}

int
mbus_data_long_decode(unsigned char* int_data, size_t int_data_size, long* value)
{
	size_t i;
	int neg;
	*value = 0;

	if (!int_data || (int_data_size < 1))
	{
		return -1;
	}

	neg = int_data[int_data_size - 1] & 0x80;

	for (i = int_data_size; i > 0; i--)
	{
		if (neg)
		{
			*value = (*value << 8) + (int_data[i - 1] ^ 0xFF);
		}
		else
		{
			*value = (*value << 8) + int_data[i - 1];
		}
	}

	if (neg)
	{
		*value = *value * -1 - 1;
	}

	return 0;
}

int
mbus_data_long_long_decode(unsigned char* int_data, size_t int_data_size, long long* value)
{
	size_t i;
	int neg;
	*value = 0;

	if (!int_data || (int_data_size < 1))
	{
		return -1;
	}

	neg = int_data[int_data_size - 1] & 0x80;

	for (i = int_data_size; i > 0; i--)
	{
		if (neg)
		{
			*value = (*value << 8) + (int_data[i - 1] ^ 0xFF);
		}
		else
		{
			*value = (*value << 8) + int_data[i - 1];
		}
	}

	if (neg)
	{
		*value = *value * -1 - 1;
	}

	return 0;
}

//------------------------------------------------------------------------------
///
/// Encode INTEGER data (into 'int_data_size' bytes)
///
//------------------------------------------------------------------------------
int
mbus_data_int_encode(unsigned char* int_data, size_t int_data_size, int value)
{
	int i;

	if (int_data)
	{
		for (i = 0; i < int_data_size; i++)
		{
			int_data[i] = (value >> (i * 8)) & 0xFF;
		}

		return 0;
	}

	return -1;
}

//------------------------------------------------------------------------------
///
/// Decode float data
///
/// see also http://en.wikipedia.org/wiki/Single-precision_floating-point_format
///
//------------------------------------------------------------------------------
float
mbus_data_float_decode(unsigned char* float_data)
{
#ifdef _HAS_NON_IEEE754_FLOAT
	float val = 0.0f;
	long temp = 0, fraction;
	int sign, exponent;
	size_t i;

	if (float_data)
	{
		for (i = 4; i > 0; i--)
		{
			temp = (temp << 8) + float_data[i - 1];
		}

		// first bit = sign bit
		sign = (temp >> 31) ? -1 : 1;

		// decode 8 bit exponent
		exponent = ((temp & 0x7F800000) >> 23) - 127;

		// decode explicit 23 bit fraction
		fraction = temp & 0x007FFFFF;

		if ((exponent != -127) &&
			(exponent != 128))
		{
			// normalized value, add bit 24
			fraction |= 0x800000;
		}

		// calculate float value
		val = (float)sign * fraction * pow(2.0f, -23.0f + exponent);

		return val;
	}
#else
	if (float_data)
	{
		union {
			uint32_t u32;
			float f;
		} data;
		memcpy(&(data.u32), float_data, sizeof(uint32_t));
		return data.f;
	}
#endif

	return -1.0f;
}

//------------------------------------------------------------------------------
///
/// Decode string data.
///
//------------------------------------------------------------------------------
void
mbus_data_str_decode(unsigned char* dst, const unsigned char* src, size_t len)
{
	size_t i;

	i = 0;

	if (src && dst)
	{
		dst[len] = '\0';
		while (len > 0) {
			dst[i++] = src[--len];
		}
	}
}

//------------------------------------------------------------------------------
///
/// Decode binary data.
///
//------------------------------------------------------------------------------
//void
//mbus_data_bin_decode(unsigned char* dst, const unsigned char* src, size_t len, size_t max_len)
//{
//    size_t i, pos;

//    i = 0;
//    pos = 0;

//    if (src && dst)
//    {
//        while ((i < len) && ((pos + 3) < max_len)) {
//            pos += snprintf(&dst[pos], max_len - pos, "%.2X ", src[i]);
//            i++;
//        }

//        if (pos > 0)
//        {
//            // remove last space
//            pos--;
//        }

//        dst[pos] = '\0';
//    }
//}

//------------------------------------------------------------------------------
///
/// Decode time data
///
/// Usable for the following types:
///   I = 6 bytes (Date and time)
///   F = 4 bytes (Date and time)
///   G = 2 bytes (Date)
///
/// TODO:
///   J = 3 bytes (Time)
///
//------------------------------------------------------------------------------
void
mbus_data_tm_decode(struct tm* t, unsigned char* t_data, size_t t_data_size)
{
	if (t == NULL)
	{
		return;
	}

	t->tm_sec = 0;
	t->tm_min = 0;
	t->tm_hour = 0;
	t->tm_mday = 0;
	t->tm_mon = 0;
	t->tm_year = 0;
	t->tm_wday = 0;
	t->tm_yday = 0;
	t->tm_isdst = 0;

	if (t_data)
	{
		if (t_data_size == 6)                // Type I = Compound CP48: Date and Time
		{
			if ((t_data[1] & 0x80) == 0)     // Time valid ?
			{
				t->tm_sec = t_data[0] & 0x3F;
				t->tm_min = t_data[1] & 0x3F;
				t->tm_hour = t_data[2] & 0x1F;
				t->tm_mday = t_data[3] & 0x1F;
				t->tm_mon = (t_data[4] & 0x0F) - 1;
				t->tm_year = 100 + (((t_data[3] & 0xE0) >> 5) |
					((t_data[4] & 0xF0) >> 1));
				t->tm_isdst = (t_data[0] & 0x40) ? 1 : 0;  // day saving time
			}
		}
		else if (t_data_size == 4)           // Type F = Compound CP32: Date and Time
		{
			if ((t_data[0] & 0x80) == 0)     // Time valid ?
			{
				t->tm_min = t_data[0] & 0x3F;
				t->tm_hour = t_data[1] & 0x1F;
				t->tm_mday = t_data[2] & 0x1F;
				t->tm_mon = (t_data[3] & 0x0F) - 1;
				t->tm_year = 100 + (((t_data[2] & 0xE0) >> 5) |
					((t_data[3] & 0xF0) >> 1));
				t->tm_isdst = (t_data[1] & 0x80) ? 1 : 0;  // day saving time
			}
		}
		else if (t_data_size == 2)           // Type G: Compound CP16: Date
		{
			t->tm_mday = t_data[0] & 0x1F;
			t->tm_mon = (t_data[1] & 0x0F) - 1;
			t->tm_year = 100 + (((t_data[0] & 0xE0) >> 5) |
				((t_data[1] & 0xF0) >> 1));
		}
	}
}

//------------------------------------------------------------------------------
///
/// Generate manufacturer code from 2-byte encoded data
///
//------------------------------------------------------------------------------
int
mbus_data_manufacturer_encode(unsigned char* m_data, unsigned char* m_code)
{
	int m_val;

	if (m_data == NULL || m_code == NULL)
		return -1;

	m_val = ((((int)m_code[0] - 64) & 0x001F) << 10) +
		((((int)m_code[1] - 64) & 0x001F) << 5) +
		((((int)m_code[2] - 64) & 0x001F));

	mbus_data_int_encode(m_data, 2, m_val);

	return 0;
}

//------------------------------------------------------------------------------
///
/// Generate manufacturer code from 2-byte encoded data
///
//------------------------------------------------------------------------------
const char*
mbus_decode_manufacturer(unsigned char byte1, unsigned char byte2)
{
	static char m_str[4];

	int m_id;

	m_str[0] = byte1;
	m_str[1] = byte2;

	mbus_data_int_decode((unsigned char*)m_str, 2, &m_id);

	m_str[0] = (char)(((m_id >> 10) & 0x001F) + 64);
	m_str[1] = (char)(((m_id >> 5) & 0x001F) + 64);
	m_str[2] = (char)(((m_id) & 0x001F) + 64);
	m_str[3] = 0;

	return m_str;
}

/**
 * @brief Converts a decimal integer to its BCD representation.
 *
 * For example, calling the function with the value 12345678
 * will return the value 0x12345678.
 *
 * @param dec The decimal integer to convert.
 * @return The BCD representation of the integer.
 */
unsigned int dec_to_bcd(unsigned int dec) {
	unsigned int bcd = 0;
	int shift = 0;
	while (dec > 0) {
		bcd |= (dec % 10) << (shift * 4);
		dec /= 10;
		shift++;
	}
	return bcd;
}

unsigned int bcd_to_dec(unsigned int bcd) {
	unsigned int dec = 0;
	unsigned int multiplier = 1;
	while (bcd > 0) {
		dec += (bcd & 0x0F) * multiplier;
		bcd >>= 4;
		multiplier *= 10;
	}
	return dec;
}

#endif /* MBUS_MASTER_CFG_NT750_ENABLED */


