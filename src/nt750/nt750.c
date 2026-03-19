/**
* \file            mbus_simple.c
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
* This file is part of DEVLIB - development library.
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
#include "mbus_master/mbus_master.h"

#if (MBUS_MASTER_CFG_NT750_ENABLED)



/**
 * \brief           Get the primary address of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \note            Bu fonksiyon hatta sadece tek bir cihaz olduðunda kullanýlmalýdýr.
 * 
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_primary_address_pa(mbus_lib_t* mbus_master, mbus_t* nt750) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_PRI_ADDR_PA);
	return res;
}

/**
 * \brief           Get the secondary address of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       primary_address: Primary address of the device
 * 
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_secondary_address_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the primary address of the nt750 structure
	nt750->primary_address = *primary_address;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_SEC_ADDR_PA);
	return res;
}

/**
 * \brief           Get the baudrate of the NT750
 * \bparam[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       device_primary_address: Primary address of the device
 * \return          mbus_master_res_t type indicating the result of the operation
 * 
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_baudrate_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the primary address of the nt750 structure
	nt750->primary_address = *primary_address;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_BAUD_PA);
	return res;
}

/**
 * \brief           Get the firmware version of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       Device secondary_address: Secondary address of the device
 * \param[in]       Device password: Password of the device
 * \param[in]       Device manufacturer: Manufacturer of the device
 * 
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_calibration_current(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the secondary address of the nt750 structure
	nt750->secondary_address = secondary_address;
	nt750->password = password;
	nt750->manufacturer = manufacturer;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_CALIBRATION_CURRENT);
	return res;
}

/**
 * \brief           Get the customer information of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       Device secondary_address: Secondary address of the device
 * \param[in]       Device password: Password of the device
 * \param[in]       Device manufacturer: Manufacturer of the device
 * 
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_customer(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the secondary address of the nt750 structure
	nt750->secondary_address = secondary_address;
	nt750->password = password;
	nt750->manufacturer = manufacturer;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_CUSTOMER);
	return res;
}

/**
 * \brief           Get the recovery period of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       Device secondary_address: Secondary address of the device
 * \param[in]       Device password: Password of the device
 * \param[in]       Device manufacturer: Manufacturer of the device
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_recovery_periode(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the secondary address of the nt750 structure
	nt750->secondary_address = secondary_address;
	nt750->password = password;
	nt750->manufacturer = manufacturer;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_RECOVERY_PERIODE);
	return res;
}

/**
 * \brief           Get the over current limit of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       Device secondary_address: Secondary address of the device
 * \param[in]       Device password: Password of the device
 * \param[in]       Device manufacturer: Manufacturer of the device
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_over_current_limit(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the secondary address of the nt750 structure
	nt750->secondary_address = secondary_address;
	nt750->password = password;
	nt750->manufacturer = manufacturer;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_OVERCURR_LIM);
	return res;
}

/**
 * \brief           Get the breaking current limit of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       Device secondary_address: Secondary address of the device
 * \param[in]       Device password: Password of the device
 * \param[in]       Device manufacturer: Manufacturer of the device
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_breaking_current_limit(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the secondary address of the nt750 structure
	nt750->secondary_address = secondary_address;
	nt750->password = password;
	nt750->manufacturer = manufacturer;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_BREAKCURR_LIM);
	return res;
}

/**
 * \brief           Get the recovery try count of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       Device secondary_address: Secondary address of the device
 * \param[in]       Device password: Password of the device
 * \param[in]       Device manufacturer: Manufacturer of the device
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_recovery_try_count(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the secondary address of the nt750 structure
	nt750->secondary_address = secondary_address;
	nt750->password = password;
	nt750->manufacturer = manufacturer;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_RECOVERY_TRY_CNT);
	return res;
}

/**
 * \brief           Get the breaking current compare match count of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       Device secondary_address: Secondary address of the device
 * \param[in]       Device password: Password of the device
 * \param[in]       Device manufacturer: Manufacturer of the device
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_breaking_current_compare_match_count(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the secondary address of the nt750 structure
	nt750->secondary_address = secondary_address;
	nt750->password = password;
	nt750->manufacturer = manufacturer;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_BREAKCURR_COMPARE_MATCH_CNT);
	return res;
}

/**
 * \brief           Get the mbus receiving current treshold of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       Device secondary_address: Secondary address of the device
 * \param[in]       Device password: Password of the device
 * \param[in]       Device manufacturer: Manufacturer of the device
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_mbus_receiving_current_treshold(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the secondary address of the nt750 structure
	nt750->secondary_address = secondary_address;
	nt750->password = password;
	nt750->manufacturer = manufacturer;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_MBUS_RECCURR_TRESHOLD);
	return res;
}

/**
 * \brief           Get the fault record of the NT750
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       Device secondary_address: Secondary address of the device
 * \param[in]       Device password: Password of the device
 * \param[in]       Device manufacturer: Manufacturer of the device
 * \param[in]       Fault group: Fault group to be read (0 - 8)
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_get_fault_record(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer, uint8_t* fault_group) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the secondary address of the nt750 structure
	nt750->secondary_address = secondary_address;
	nt750->password = password;
	nt750->manufacturer = manufacturer;
	nt750->fault_records_group_number = *fault_group;
	res = mbus_master_get(mbus_master, nt750, NT750_GET_FAULT_RECORDS);
	return res;
}

/**
 * \brief           Set the primary address of the NT750 via primary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       primary_address: Pointer to the current primary address
 * \param[in]       new_primary_address: Pointer to the new primary address to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 * \Important Note: Before calling this function, service mode must be set to on
 */
mbus_master_res_t nt750_set_primary_address_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address, uint8_t* new_primary_address) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the primary address of the nt750 structure
	nt750->primary_address = *primary_address;
	nt750->new_primary_address = *new_primary_address;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_PRI_ADDR_PA);
	return res;
}

/**
 * \brief           Set the secondary address of the NT750 via primary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       primary_address: Pointer to the current primary address
 * \param[in]       new_secondary_address: Pointer to the new secondary address to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 * \Important Note: Before calling this function, service mode must be set to on
 */
mbus_master_res_t nt750_set_secondary_address_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address, uint32_t* new_secondary_address) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the primary address of the nt750 structure
	nt750->primary_address = *primary_address;
	nt750->new_secondary_address = *new_secondary_address;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_SEC_ADDR_PA);
	return res;
}

/**
 * \brief           Set the baudrate of the NT750 via primary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       primary_address: Pointer to the current primary address
 * \param[in]       baudrate: Pointer to the new baudrate to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_baudrate_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address, uint8_t* baudrate) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the primary address of the nt750 structure
	nt750->primary_address = *primary_address;
	nt750->baudrate = *baudrate;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_BAUD_PA);
	return res;
}

/**
 * \brief           Set the full identification of the NT750 via primary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       primary_address: Pointer to the current primary address
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       secondary_address: Pointer to the new secondary address to be set
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       generation: Pointer to the generation ID to be set
 * \param[in]       medium: Pointer to the medium type to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 * \Important Note: Before calling this function, service mode must be set to on
 */
mbus_master_res_t nt750_set_full_identification_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address, uint32_t password, uint32_t secondary_address, uint16_t manufacturer, uint8_t* generation, uint8_t* medium) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	// Set the primary address of the nt750 structure
	nt750->primary_address = *primary_address;
	nt750->password = password;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->generation = *generation;
	nt750->medium = *medium;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_FULL_IDENT_PA);
	return res;
}

/**
 * \brief           Set baudrate of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       baudrate: Pointer to the new baudrate to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_baudrate(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint8_t* baudrate) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->baudrate = *baudrate;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_BAUD);
	return res;
}

/**
 * \brief           Set primary address of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       new_primary_address: Pointer to the new primary address to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_primary_address(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint8_t* new_primary_address) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->new_primary_address = *new_primary_address;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_PRI_ADDR);
	return res;
}

/**
 * \brief           Set secondary address of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       new_secondary_address: Pointer to the new secondary address to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_secondary_address(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t* new_secondary_address) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->new_secondary_address = *new_secondary_address;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_SEC_ADDR);
	return res;
}

/**
 * \brief           Set the full identification of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       generation: Pointer to the generation ID to be set
 * \param[in]       medium: Pointer to the medium type to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_restart(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_RESTART_DEV);
	return res;
}

/** 
 * \brief           Set the power status of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       power_status: Pointer to the power status to be set (0 - off, 1 - on)
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_mbus_power_status(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, mbus_power_status_t power_status) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->mbus_power_status = power_status;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_MBUS_PWR);
	return res;
}

/** 
 * \brief           Set the change password of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the current password
 * \param[in]       new_password: Pointer to the new password to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_change_password(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* new_password) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->new_password = *new_password;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_CHNGE_PASS);
	return res;
}

/** 
 * \brief           Set the service mode status of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       service_mode_status: Pointer to the service mode status to be set (0 - off, 1 - on)
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_service_mode_status(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, mbus_service_mode_t* service_mode_status) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->service_mode_status = *service_mode_status;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_SERVICE_MODE);
	return res;
}

/** 
 * \brief           Set the calibration current of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       calibration_current_mA: Pointer to the calibration current in mA to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 * \Important Note: Before calling this function, service mode must be set to on
 */
mbus_master_res_t nt750_set_calibration_current(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint16_t* calibration_current_mA) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->calibration_current_mA = *calibration_current_mA;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_CALIBRATION_CURRENT);
	return res;
}

/** 
 * \brief           Set the lock status of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       device_lock_status: Pointer to the device lock status to be set (0 - unlock, 1 - lock)
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 * \Important Note: Before calling this function, service mode must be set to on
 */
mbus_master_res_t nt750_set_lock_device(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->device_lock_status = 1;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_LOCK_DEV);
	return res;
}

/** 
 * \brief           Set the unlock status of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       device_lock_status: Pointer to the device lock status to be set (0 - unlock, 1 - lock)
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 * \Important Note: Before calling this function, service mode must be set to on
 */
mbus_master_res_t nt750_set_unlock_device(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->device_lock_status = 0;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_UNLOCK_DEV);
	return res;
}

/** 
 * \brief           Set the customer information of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       customer_1: Pointer to the first customer location to be set
 * \param[in]       customer_2: Pointer to the second customer location to be set
 * \param[in]       customer_3: Pointer to the third customer location to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_customer(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* customer_1, uint32_t* customer_2, uint32_t* customer_3) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->customer_location_1 = *customer_1;
	nt750->customer_location_2 = *customer_2;
	nt750->customer_location_3 = *customer_3;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_CUSTOMER);
	return res;
}

/** 
 * \brief           Set the factory reset of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_factory_reset(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_FCTRY_RST);
	return res;
}

/** 
 * \brief           Set the factory reset of the NT750 except calibration via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_factory_reset_except_calibration(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_FCTRY_RST_EXC_CAL);
	return res;
}

/** 
 * \brief           Set the recovery period of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       recovery_periode: Pointer to the recovery period in seconds to be set
 * \param[in]       fast_recovery_periode: Pointer to the fast recovery period in seconds to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_recovery_periode(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* recovery_periode, uint32_t* fast_recovery_periode) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->recovery_periode = *recovery_periode;
	nt750->fast_recovery_periode = *fast_recovery_periode;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_RECOVERY_PERIODE);
	return res;
}

/** 
 * \brief           Set the recovery try count of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       recovery_try_count: Pointer to the recovery try count to be set
 * \param[in]       fast_recovery_try_count: Pointer to the fast recovery try count to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_recovery_try_count(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* recovery_try_count, uint32_t* fast_recovery_try_count) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->recovery_try_count = *recovery_try_count;
	nt750->fast_recovery_try_count = *fast_recovery_try_count;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_RECOVERY_TRY_COUNT);
	return res;
}

/** 
 * \brief           Set the over current limit of the NT750 via secondary address mode
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       over_current_limit: Pointer to the over current limit in mA to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_over_current_limit(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* over_current_limit) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->over_current_limit = *over_current_limit;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_OVERCURR_LIM);
	return res;
}

/** 
 * \brief           Set the breaking current limit of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       breaking_current_limit: Pointer to the breaking current limit in mA to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_breaking_current_limit(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* breaking_current_limit) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->breaking_current_limit = *breaking_current_limit;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_BREAKCURR_LIM);
	return res;
}

/** 
 * \brief           Set the reset calibration of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_reset_calibration(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_RESET_CALIBRATION);
	return res;
}

/** 
 * \brief           Set the breaking current compare match count of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       breaking_current_compare_match_count: Pointer to the breaking current compare match count to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_breaking_current_compare_match_count(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint8_t* breaking_current_compare_match_count) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->breaking_current_compare_match_count = *breaking_current_compare_match_count;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_BREAKCURR_COMPARE_MATCH_CNT);
	return res;
}

/** 
 * \brief           Set the mbus receiving current treshold of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 * \param[in]       mbus_receiving_current_treshold: Pointer to the mbus receiving current treshold in mA to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_mbus_receive_current_treshold(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, float* f_mbus_receiving_current_treshold) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	nt750->f_mbus_receiving_current_treshold = *f_mbus_receiving_current_treshold;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_MBUS_RECEIVECURR_TRESHOLD);
	return res;
}

/** 
 * \brief           Set the clean all fault records of the NT750 via secondary address mode
 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
 * \param[in,out]   nt750: Pointer to nt750 structure
 * \param[in]       secondary_address: Pointer to the current secondary address
 * \param[in]       manufacturer: Pointer to the manufacturer ID to be set
 * \param[in]       password: Pointer to the password to be set
 *
 * \return          mbus_master_res_t type indicating the result of the operation
 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
 */
mbus_master_res_t nt750_set_clean_all_fault_records(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password) {
	mbus_master_res_t res = MBUS_MASTER_ERR;
	nt750->secondary_address = secondary_address;
	nt750->manufacturer = manufacturer;
	nt750->password = password;
	res = mbus_master_set(mbus_master, nt750, NT750_SET_CLEAN_ALL_FAULT_RECORDS);
	return res;
}

#endif /* MBUS_MASTER_CFG_NT750_ENABLED */

#endif /* GATEWAY_TYPE == MBUS */
