//------------------------------------------------------------------------------
// Copyright (C) 2010-2011, Robert Johansson, Raditex AB
// All rights reserved.
//
// rSCADA
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------

/**
 * @file   mbus-protocol.h
 *
 * @brief  Functions and data structures for M-Bus protocol parsing.
 *
 */

#ifndef _MBUS_PROTOCOL_H_
#define _MBUS_PROTOCOL_H_

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "mbus_master/lwrb.h"

#ifdef __cplusplus
extern "C" {
#endif

	//
	// Packet formats:
	//
	// ACK: size = 1 byte
	//
	//   byte1: ack   = 0xE5
	//
	// SHORT: size = 5 byte
	//
	//   byte1: start   = 0x10
	//   byte2: control = ...
	//   byte3: address = ...
	//   byte4: chksum  = ...
	//   byte5: stop    = 0x16
	//
	// CONTROL: size = 9 byte
	//
	//   byte1: start1  = 0x68
	//   byte2: length1 = ...
	//   byte3: length2 = ...
	//   byte4: start2  = 0x68
	//   byte5: control = ...
	//   byte6: address = ...
	//   byte7: ctl.info= ...
	//   byte8: chksum  = ...
	//   byte9: stop    = 0x16
	//
	// LONG: size = N >= 9 byte
	//
	//   byte1: start1  = 0x68
	//   byte2: length1 = ...
	//   byte3: length2 = ...
	//   byte4: start2  = 0x68
	//   byte5: control = ...
	//   byte6: address = ...
	//   byte7: ctl.info= ...
	//   byte8: data    = ...
	//          ...     = ...
	//   byteN-1: chksum  = ...
	//   byteN: stop    = 0x16
	//
	//
	//

#define MBUS_FRAME_DATA_LENGTH 252

	typedef struct _mbus_frame {

		unsigned char start1;
		unsigned char length1;
		unsigned char length2;
		unsigned char start2;
		unsigned char control;
		unsigned char address;
		unsigned char control_information;
		// variable data field
		unsigned char checksum;
		unsigned char stop;

		unsigned char   data[MBUS_FRAME_DATA_LENGTH];
		size_t data_size;
		int type;
	} mbus_frame;

	//------------------------------------------------------------------------------
	// MBUS FRAME DATA FORMATS
	//

	// DATA RECORDS
#define MBUS_DIB_DIF_WITHOUT_EXTENSION                          0x7F
#define MBUS_DIB_DIF_EXTENSION_BIT                              0x80
#define MBUS_DIB_VIF_WITHOUT_EXTENSION                          0x7F
#define MBUS_DIB_VIF_EXTENSION_BIT                              0x80
#define MBUS_DIB_DIF_MANUFACTURER_SPECIFIC                      0x0F
#define MBUS_DIB_DIF_MORE_RECORDS_FOLLOW                        0x1F
#define MBUS_DIB_DIF_IDLE_FILLER                                0x2F


#define MBUS_DATA_INFO_BLOCK_DIFE_SIZE                          10
#define MBUS_VALUE_INFO_BLOCK_VIFE_SIZE                         10
#define MBUS_VALUE_INFO_BLOCK_CUSTOM_VIF_SIZE                   128


//
// HEADER FOR SECONDARY ADDRESSING
//
	typedef struct _mbus_data_secondary_address {

		//Ident.Nr. Manufr. Version Medium
		//4 Byte    2 Byte  1 Byte  1 Byte

		// ex
		// 14 49 10 01 10 57 01 06

		unsigned char id_bcd[4];         // 14 49 10 01
		unsigned char manufacturer[2];   // 10 57
		unsigned char version;           // 01
		unsigned char medium;            // 06

	} mbus_data_secondary_address;


	//------------------------------------------------------------------------------
	// FRAME types
	//
#define MBUS_FRAME_TYPE_ANY                                     0x00
#define MBUS_FRAME_TYPE_ACK                                     0x01
#define MBUS_FRAME_TYPE_SHORT                                   0x02
#define MBUS_FRAME_TYPE_CONTROL                                 0x03
#define MBUS_FRAME_TYPE_LONG                                    0x04

#define MBUS_FRAME_ACK_BASE_SIZE                                1
#define MBUS_FRAME_SHORT_BASE_SIZE                              5
#define MBUS_FRAME_CONTROL_BASE_SIZE                            9
#define MBUS_FRAME_LONG_BASE_SIZE                               9

#define MBUS_FRAME_BASE_SIZE_ACK                                1
#define MBUS_FRAME_BASE_SIZE_SHORT                              5
#define MBUS_FRAME_BASE_SIZE_CONTROL                            9
#define MBUS_FRAME_BASE_SIZE_LONG                               9

#define MBUS_FRAME_FIXED_SIZE_ACK                               1
#define MBUS_FRAME_FIXED_SIZE_SHORT                             5
#define MBUS_FRAME_FIXED_SIZE_CONTROL                           6
#define MBUS_FRAME_FIXED_SIZE_LONG                              6

//
// Frame start/stop bits
//
#define MBUS_FRAME_ACK_START                                    0xE5
#define MBUS_FRAME_SHORT_START                                  0x10
#define MBUS_FRAME_CONTROL_START                                0x68
#define MBUS_FRAME_LONG_START                                   0x68
#define MBUS_FRAME_STOP                                         0x16

//
//
//
#define MBUS_MAX_PRIMARY_SLAVES                                 250

//
// Control field
//
#define MBUS_CONTROL_FIELD_DIRECTION                            0x07
#define MBUS_CONTROL_FIELD_FCB                                  0x06
#define MBUS_CONTROL_FIELD_ACD                                  0x06
#define MBUS_CONTROL_FIELD_FCV                                  0x05
#define MBUS_CONTROL_FIELD_DFC                                  0x05
#define MBUS_CONTROL_FIELD_F3                                   0x04
#define MBUS_CONTROL_FIELD_F2                                   0x03
#define MBUS_CONTROL_FIELD_F1                                   0x02
#define MBUS_CONTROL_FIELD_F0                                   0x01

#define MBUS_CONTROL_MASK_SND_NKE                               0x40
#define MBUS_CONTROL_MASK_SND_UD                                0x53
#define MBUS_CONTROL_MASK_REQ_UD2                               0x5B
#define MBUS_CONTROL_MASK_REQ_UD1                               0x5A
#define MBUS_CONTROL_MASK_RSP_UD                                0x08

#define MBUS_CONTROL_MASK_FCB                                   0x20
#define MBUS_CONTROL_MASK_FCV                                   0x10

#define MBUS_CONTROL_MASK_ACD                                   0x20
#define MBUS_CONTROL_MASK_DFC                                   0x10

#define MBUS_CONTROL_MASK_DIR                                   0x40
#define MBUS_CONTROL_MASK_DIR_M2S                               0x40
#define MBUS_CONTROL_MASK_DIR_S2M                               0x00

//
// Address field
//
#define MBUS_ADDRESS_BROADCAST_REPLY                            0xFE
#define MBUS_ADDRESS_BROADCAST_NOREPLY                          0xFF
#define MBUS_ADDRESS_NETWORK_LAYER                              0xFD

//
// Control Information field
//
//Mode 1 Mode 2                   Application                   Definition in
// 51h    55h                       data send                    EN1434-3
// 52h    56h                  selection of slaves           Usergroup July  ́93
// 50h                          application reset           Usergroup March  ́94
// 54h                          synronize action                 suggestion
// B8h                     set baudrate to 300 baud          Usergroup July  ́93
// B9h                     set baudrate to 600 baud          Usergroup July  ́93
// BAh                    set baudrate to 1200 baud          Usergroup July  ́93
// BBh                    set baudrate to 2400 baud          Usergroup July  ́93
// BCh                    set baudrate to 4800 baud          Usergroup July  ́93
// BDh                    set baudrate to 9600 baud          Usergroup July  ́93
// BEh                   set baudrate to 19200 baud              suggestion
// BFh                   set baudrate to 38400 baud              suggestion
// B1h           request readout of complete RAM content     Techem suggestion
// B2h          send user data (not standardized RAM write) Techem suggestion
// B3h                 initialize test calibration mode      Usergroup July  ́93
// B4h                           EEPROM read                 Techem suggestion
// B6h                         start software test           Techem suggestion
// 90h to 97h              codes used for hashing           longer recommended

#define MBUS_CONTROL_INFO_DATA_SEND                             0x51
#define MBUS_CONTROL_INFO_DATA_SEND_MSB                         0x55
#define MBUS_CONTROL_INFO_SELECT_SLAVE                          0x52
#define MBUS_CONTROL_INFO_SELECT_SLAVE_MSB                      0x56
#define MBUS_CONTROL_INFO_APPLICATION_RESET                     0x50
#define MBUS_CONTROL_INFO_SYNC_ACTION                           0x54
#define MBUS_CONTROL_INFO_SET_BAUDRATE_300                      0xB8
#define MBUS_CONTROL_INFO_SET_BAUDRATE_600                      0xB9
#define MBUS_CONTROL_INFO_SET_BAUDRATE_1200                     0xBA
#define MBUS_CONTROL_INFO_SET_BAUDRATE_2400                     0xBB
#define MBUS_CONTROL_INFO_SET_BAUDRATE_4800                     0xBC
#define MBUS_CONTROL_INFO_SET_BAUDRATE_9600                     0xBD
#define MBUS_CONTROL_INFO_SET_BAUDRATE_19200                    0xBE
#define MBUS_CONTROL_INFO_SET_BAUDRATE_38400                    0xBF
#define MBUS_CONTROL_INFO_REQUEST_RAM_READ                      0xB1
#define MBUS_CONTROL_INFO_SEND_USER_DATA                        0xB2
#define MBUS_CONTROL_INFO_INIT_TEST_CALIB                       0xB3
#define MBUS_CONTROL_INFO_EEPROM_READ                           0xB4
#define MBUS_CONTROL_INFO_SW_TEST_START                         0xB6

//Mode 1 Mode 2                   Application                   Definition in
// 70h             report of general application errors     Usergroup March 94
// 71h                      report of alarm status          Usergroup March 94
// 72h   76h                variable data respond                EN1434-3
// 73h   77h                 fixed data respond                  EN1434-3
#define MBUS_CONTROL_INFO_ERROR_GENERAL                         0x70
#define MBUS_CONTROL_INFO_STATUS_ALARM                          0x71

#define MBUS_CONTROL_INFO_RESP_FIXED                            0x73
#define MBUS_CONTROL_INFO_RESP_FIXED_MSB                        0x77

#define MBUS_CONTROL_INFO_RESP_VARIABLE                         0x72
#define MBUS_CONTROL_INFO_RESP_VARIABLE_MSB                     0x76

//
// DATA BITS
//
#define MBUS_DATA_FIXED_STATUS_FORMAT_MASK                      0x80
#define MBUS_DATA_FIXED_STATUS_FORMAT_BCD                       0x00
#define MBUS_DATA_FIXED_STATUS_FORMAT_INT                       0x80
#define MBUS_DATA_FIXED_STATUS_DATE_MASK                        0x40
#define MBUS_DATA_FIXED_STATUS_DATE_STORED                      0x40
#define MBUS_DATA_FIXED_STATUS_DATE_CURRENT                     0x00


//
// data record fields
//
#define MBUS_DATA_RECORD_DIF_MASK_INST                          0x00
#define MBUS_DATA_RECORD_DIF_MASK_MIN                           0x10

#define MBUS_DATA_RECORD_DIF_MASK_TYPE_INT32                    0x04
#define MBUS_DATA_RECORD_DIF_MASK_DATA                          0x0F
#define MBUS_DATA_RECORD_DIF_MASK_FUNCTION                      0x30
#define MBUS_DATA_RECORD_DIF_MASK_STORAGE_NO                    0x40
#define MBUS_DATA_RECORD_DIF_MASK_EXTENTION                     0x80
#define MBUS_DATA_RECORD_DIF_MASK_NON_DATA                      0xF0

#define MBUS_DATA_RECORD_DIFE_MASK_STORAGE_NO                   0x0F
#define MBUS_DATA_RECORD_DIFE_MASK_TARIFF                       0x30
#define MBUS_DATA_RECORD_DIFE_MASK_DEVICE                       0x40
#define MBUS_DATA_RECORD_DIFE_MASK_EXTENSION                    0x80

//
// GENERAL APPLICATION ERRORS
//
#define MBUS_ERROR_DATA_UNSPECIFIED                             0x00
#define MBUS_ERROR_DATA_UNIMPLEMENTED_CI                        0x01
#define MBUS_ERROR_DATA_BUFFER_TOO_LONG                         0x02
#define MBUS_ERROR_DATA_TOO_MANY_RECORDS                        0x03
#define MBUS_ERROR_DATA_PREMATURE_END                           0x04
#define MBUS_ERROR_DATA_TOO_MANY_DIFES                          0x05
#define MBUS_ERROR_DATA_TOO_MANY_VIFES                          0x06
#define MBUS_ERROR_DATA_RESERVED                                0x07
#define MBUS_ERROR_DATA_APPLICATION_BUSY                        0x08
#define MBUS_ERROR_DATA_TOO_MANY_READOUTS                       0x09

//
// FIXED DATA FLAGS
//

//
// VARIABLE DATA FLAGS
//
#define MBUS_VARIABLE_DATA_MEDIUM_OTHER                         0x00
#define MBUS_VARIABLE_DATA_MEDIUM_OIL                           0x01
#define MBUS_VARIABLE_DATA_MEDIUM_ELECTRICITY                   0x02
#define MBUS_VARIABLE_DATA_MEDIUM_GAS                           0x03
#define MBUS_VARIABLE_DATA_MEDIUM_HEAT_OUT                      0x04
#define MBUS_VARIABLE_DATA_MEDIUM_STEAM                         0x05
#define MBUS_VARIABLE_DATA_MEDIUM_HOT_WATER                     0x06
#define MBUS_VARIABLE_DATA_MEDIUM_WATER                         0x07
#define MBUS_VARIABLE_DATA_MEDIUM_HEAT_COST                     0x08
#define MBUS_VARIABLE_DATA_MEDIUM_COMPR_AIR                     0x09
#define MBUS_VARIABLE_DATA_MEDIUM_COOL_OUT                      0x0A
#define MBUS_VARIABLE_DATA_MEDIUM_COOL_IN                       0x0B
#define MBUS_VARIABLE_DATA_MEDIUM_HEAT_IN                       0x0C
#define MBUS_VARIABLE_DATA_MEDIUM_HEAT_COOL                     0x0D
#define MBUS_VARIABLE_DATA_MEDIUM_BUS                           0x0E
#define MBUS_VARIABLE_DATA_MEDIUM_UNKNOWN                       0x0F
#define MBUS_VARIABLE_DATA_MEDIUM_IRRIGATION                    0x10
#define MBUS_VARIABLE_DATA_MEDIUM_WATER_LOGGER                  0x11
#define MBUS_VARIABLE_DATA_MEDIUM_GAS_LOGGER                    0x12
#define MBUS_VARIABLE_DATA_MEDIUM_GAS_CONV                      0x13
#define MBUS_VARIABLE_DATA_MEDIUM_COLORIFIC                     0x14
#define MBUS_VARIABLE_DATA_MEDIUM_BOIL_WATER                    0x15
#define MBUS_VARIABLE_DATA_MEDIUM_COLD_WATER                    0x16
#define MBUS_VARIABLE_DATA_MEDIUM_DUAL_WATER                    0x17
#define MBUS_VARIABLE_DATA_MEDIUM_PRESSURE                      0x18
#define MBUS_VARIABLE_DATA_MEDIUM_ADC                           0x19
#define MBUS_VARIABLE_DATA_MEDIUM_SMOKE                         0x1A
#define MBUS_VARIABLE_DATA_MEDIUM_ROOM_SENSOR                   0x1B
#define MBUS_VARIABLE_DATA_MEDIUM_GAS_DETECTOR                  0x1C
#define MBUS_VARIABLE_DATA_MEDIUM_BREAKER_E                     0x20
#define MBUS_VARIABLE_DATA_MEDIUM_VALVE                         0x21
#define MBUS_VARIABLE_DATA_MEDIUM_CUSTOMER_UNIT                 0x25
#define MBUS_VARIABLE_DATA_MEDIUM_WASTE_WATER                   0x28
#define MBUS_VARIABLE_DATA_MEDIUM_GARBAGE                       0x29
#define MBUS_VARIABLE_DATA_MEDIUM_SERVICE_UNIT                  0x30
#define MBUS_VARIABLE_DATA_MEDIUM_RC_SYSTEM                     0x36
#define MBUS_VARIABLE_DATA_MEDIUM_RC_METER                      0x37



//
// Standard Value Information Field (VIF) Used
//
#define MBUS_VIF_PRIMARY_ADDRESS_RECORD                         0x7A
#define MBUS_VIF_SECONDARY_ADDRESS_RECORD                       0x79
#define MBUS_VIF_EXTENSION_VIFE_FALLOW                          0xFD
#define MBUS_VIF_MAN_SPECIFIC_VIFE_FALLOW                       0xFF
#define MBUS_VIF_MAN_SPECIFIC_VIF                               0x7F

//
// Standard Value Information Extension (VIFE) Used
//
#define MBUS_VIFE_EXTENSION_HARDWARE_VERSION                    0x0D
#define MBUS_VIFE_EXTENSION_MODEL_VERSION                       0x0C
#define MBUS_VIFE_EXTENSION_FIRMWARE_VERSION                    0x0E
#define MBUS_VIFE_EXTENSION_STATE_OF_PARAMETER_ACTIVATION       0x66 
#define MBUS_VIFE_EXTENSION_ERROR_FLAGS                         0x17
#define MBUS_VIFE_EXTENSION_MILLI_AMPERE                        0x59
#define MBUS_VIFE_EXTENSION_MILLI_VOLT                          0x47
#define MBUS_VIFE_EXTENSION_MILLI_VOLT_2                        0x46
#define MBUS_VIFE_EXTENSION_SPECIAL_SUPPLIER_INFORMATION        0x67

#define MBUS_VIFE_EXTENSION_BAUDRATE                            0x1C
#define MBUS_VIFE_EXTENSION_CUSTOMER_LOCATION                   0x10
#define MBUS_VIFE_EXTENSION_DEVICE_TYPE                         0x09
#define MBUS_VIFE_EXTENSION_PARAMETER_SET_IDENTIFICATION        0x0B      
#define MBUS_VIFE_EXTENSION_DIGITAL_OUTPUT                      0x1A      
#define MBUS_VIFE_EXTENSION_CONTROL_SIGNAL                      0x62      




//
// Manufacturer Specific Value Information Field Extension (VIFE) Used
//
	typedef enum {
		MBUS_VIFE_SET_SERVICE_MODE = 0x71,
		MBUS_VIFE_CALIBRATE_CURRENT_SENSOR,
		MBUS_VIFE_READ_SERVICE_MODE_STATUS,
		MBUS_VIFE_READ_CALIBRATION_STATUS,
		MBUS_VIFE_READ_CALIBRATION_CURRENT,
		MBUS_VIFE_SET_MBUS_POWER,
		MBUS_VIFE_READ_MBUS_POWER,
		MBUS_VIFE_LOCK_DEVICE,
		MBUS_VIFE_UNLOCK_DEVICE,
		MBUS_VIFE_READ_DEVICE_LOCK_STATUS,
		MBUS_VIFE_SET_CUSTOMER_LOCATION,
		MBUS_VIFE_READ_CUSTOMER_LOCATION,
		MBUS_VIFE_ENTER_PASSWORD,
		MBUS_VIFE_CHANGE_PASSWORD,
		MBUS_VIFE_READ_BAUDRATE,
		MBUS_VIFE_DEVICE_RESTART,
		MBUS_VIFE_ENTER_SUPER_PASSWORD,
		MBUS_VIFE_SET_SUPER_PASSWORD,
		MBUS_VIFE_READ_SUPER_PASSWORD,
		MBUS_VIFE_SET_RECOVERY_PERIODE,
		MBUS_VIFE_SET_FAST_RECOVERY_PERIODE,
		MBUS_VIFE_READ_RECOVERY_PERIODE,
		MBUS_VIFE_SET_RECOVERY_TRY_COUNT,
		MBUS_VIFE_SET_FAST_RECOVERY_TRY_COUNT,
		MBUS_VIFE_READ_RECOVERY_TRY_COUNT,
		MBUS_VIFE_FABRICATION_RESET,
		MBUS_VIFE_SET_OVER_CURRENT_LIMIT,
		MBUS_VIFE_READ_OVER_CURRENT_LIMIT,
		MBUS_VIFE_SET_BREAKING_CURRENT_LIMIT,
		MBUS_VIFE_READ_BREAKING_CURRENT_LIMIT,
		MBUS_VIFE_READ_DEVICE_STATUS,
		MBUS_VIFE_FABRICATION_RESET_EXCEPT_CALIBRATION,
		MBUS_VIFE_RESET_CALIBRATION,
		MBUS_VIFE_RESET_STORAGE,
		MBUS_VIFE_SET_BREAKING_CURRENT_COMPARE_MATCH_COUNT,
		MBUS_VIFE_READ_BREAKING_CURRENT_COMPARE_MATCH_COUNT,
		MBUS_VIFE_SET_MBUS_RECEIVING_CURRENT_TRESHOLD,
		MBUS_VIFE_READ_MBUS_RECEIVING_CURRENT_TRESHOLD,
		MBUS_VIFE_READ_FAULT_RECORDS,
		MBUS_VIFE_FAULT_RECORDS_GROUP_NUMBER,
		MBUS_VIFE_CLEAN_ALL_FAULT_RECORDS
	}MANUFACTURER_SPECIFIC_VIFES_t;


	//
	// Standard DIF Used
	//
#define MBUS_DIF_NO_DATA                                        0x00
#define MBUS_DIF_8_BIT_INT                                      0x01
#define MBUS_DIF_16_BIT_INT                                     0x02
#define MBUS_DIF_24_BIT_INT                                     0x03
#define MBUS_DIF_32_BIT_INT                                     0x04
#define MBUS_DIF_32_BIT_REAL                                    0x05
#define MBUS_DIF_48_BIT_INT                                     0x06
#define MBUS_DIF_64_BIT_INT                                     0x07
#define MBUS_DIF_SELECTION_FOR_READOUT                          0x08
#define MBUS_DIF_2_DIG_BCD                                      0x09
#define MBUS_DIF_4_DIG_BCD                                      0x0A
#define MBUS_DIF_6_DIG_BCD                                      0x0B
#define MBUS_DIF_8_DIG_BCD                                      0x0C
#define MBUS_DIF_VAR_LENGHT                                     0x0D
#define MBUS_DIF_12_DIG_BCD                                     0x0E
#define MBUS_DIF_SPECIAL_FUNC                                   0x0F



//
// Returns the manufacturer ID or zero if the given
// string could not be converted into an ID
//
	unsigned int mbus_manufacturer_id(char* manufacturer);
	int mbus_frame_calc_checksum(mbus_frame* frame);
	int mbus_frame_calc_length(mbus_frame* frame);

	//
	// Parse/Pack to bin
	//
	int mbus_parse(mbus_frame* frame, const void* data_buf, size_t data_size);
	int mbus_frame_pack(mbus_frame* frame, unsigned char* data, size_t data_size);
	int mbus_frame_verify(mbus_frame* frame);

	//
	// M-Bus frame data struct access/write functions
	//
	int mbus_frame_type(mbus_frame* frame);
	int mbus_frame_direction(mbus_frame* frame);


	char* mbus_error_str(void);
	void  mbus_error_str_set(char* message);
	void  mbus_error_reset(void);


	//
	// data encode/decode functions
	//
	int mbus_data_manufacturer_encode(unsigned char* m_data, unsigned char* m_code);
	const char* mbus_decode_manufacturer(unsigned char byte1, unsigned char byte2);
	int mbus_data_bcd_encode(unsigned char* bcd_data, size_t bcd_data_size, int value);
	int mbus_data_int_encode(unsigned char* int_data, size_t int_data_size, int value);

	long long mbus_data_bcd_decode(unsigned char* bcd_data, size_t bcd_data_size);
	long long mbus_data_bcd_decode_hex(unsigned char* bcd_data, size_t bcd_data_size);
	int mbus_data_int_decode(unsigned char* int_data, size_t int_data_size, int* value);
	int mbus_data_long_decode(unsigned char* int_data, size_t int_data_size, long* value);
	int mbus_data_long_long_decode(unsigned char* int_data, size_t int_data_size, long long* value);

	float mbus_data_float_decode(unsigned char* float_data);

	void mbus_data_tm_decode(struct tm* t, unsigned char* t_data, size_t t_data_size);

	void mbus_data_str_decode(unsigned char* dst, const unsigned char* src, size_t len);

	void mbus_data_bin_decode(unsigned char* dst, const unsigned char* src, size_t len, size_t max_len);

	/**
	 * @brief Converts a decimal integer to its BCD representation.
	 *
	 * For example, calling the function with the value 12345678
	 * will return the value 0x12345678.
	 *
	 * @param dec The decimal integer to convert.
	 * @return The BCD representation of the integer.
	 */
	unsigned int dec_to_bcd(unsigned int dec);

	/**
	 * @brief Converts a BCD integer to its decimal representation.
	 *
	 * For example, calling the function with the value 0x12345678
	 * will return the value 12345678.
	 *
	 * @param bcd The BCD integer to convert.
	 * @return The decimal representation of the integer.
	 */
	unsigned int bcd_to_dec(unsigned int bcd);


#ifdef __cplusplus
}
#endif

#endif /* _MBUS_PROTOCOL_H_ */
