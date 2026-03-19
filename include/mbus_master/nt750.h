/**
* \file            nt750.h
* \brief           Cnv_Nt_750 M-Bus Converter Library Header File
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
//#include "mbus_master/mbus_master.h"
#if (MBUS_MASTER_CFG_NT750_ENABLED)

#ifndef NT750_HDR_H
#define NT750_HDR_H

/* Get most important include files */
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DEFAULT_ADMIN_PASSWORD                                        19960304
#define DEFAULT_PASSWORD                                              12345678
#define MANUFACTURER                                                  0x25CE//0x0613


	/**
	 * \brief           Get the primary address of the NT750
	 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
	 * \param[in,out]   nt750: Pointer to nt750 structure
	 * \note            Bu fonksiyon hatta sadece tek bir cihaz olduðunda kullanýlmalýdýr.
	 *
	 * \return          mbus_master_res_t type indicating the result of the operation
	 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
	 */
	mbus_master_res_t nt750_get_primary_address_pa(mbus_lib_t* mbus_master, mbus_t* nt750); 

	/**
	 * \brief           Get the secondary address of the NT750
	 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
	 * \param[in,out]   nt750: Pointer to nt750 structure
	 * \param[in]       primary_address: Primary address of the device
	 *
	 * \return          mbus_master_res_t type indicating the result of the operation
	 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
	 */
	mbus_master_res_t nt750_get_secondary_address_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address); 

	/**
	 * \brief           Get the baudrate of the NT750
	 * \bparam[in,out]   mbus_master: Pointer to mbus_master simple structure
	 * \param[in,out]   nt750: Pointer to nt750 structure
	 * \param[in]       device_primary_address: Primary address of the device
	 * \return          mbus_master_res_t type indicating the result of the operation
	 *
	 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
	 */
	mbus_master_res_t nt750_get_secondary_baudrate_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address); 

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
	mbus_master_res_t nt750_get_calibration_current(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer); 

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
	mbus_master_res_t nt750_get_customer(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer);

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
	mbus_master_res_t nt750_get_recovery_periode(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer);

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
	mbus_master_res_t nt750_get_over_current_limit(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer);

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
	mbus_master_res_t nt750_get_breaking_current_limit(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer); 

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
	mbus_master_res_t nt750_get_recovery_try_count(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer);

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
	mbus_master_res_t nt750_get_breaking_current_compare_match_count(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer);

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
	mbus_master_res_t nt750_get_mbus_receiving_current_treshold(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer);

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
	mbus_master_res_t nt750_get_fault_record(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint32_t password, uint16_t manufacturer, uint8_t* fault_group);

	/**
	 * \brief           Set the primary address of the NT750 via primary address mode
	 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
	 * \param[in,out]   nt750: Pointer to nt750 structure
	 * \param[in]       primary_address: Pointer to the current primary address
	 * \param[in]       new_primary_address: Pointer to the new primary address to be set
	 *
	 * \return          mbus_master_res_t type indicating the result of the operation
	 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
	 */
	mbus_master_res_t nt750_set_primary_address_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address, uint8_t* new_primary_address);

	/**
	 * \brief           Set the secondary address of the NT750 via primary address mode
	 * \param[in,out]   mbus_master: Pointer to mbus_master simple structure
	 * \param[in,out]   nt750: Pointer to nt750 structure
	 * \param[in]       primary_address: Pointer to the current primary address
	 * \param[in]       new_secondary_address: Pointer to the new secondary address to be set
	 *
	 * \return          mbus_master_res_t type indicating the result of the operation
	 * \Important Note: After calling this function, NT750's raw response stored in mbus_master.raw_data
	 */
	mbus_master_res_t nt750_set_secondary_address_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address, uint32_t* new_secondary_address);

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
	mbus_master_res_t nt750_set_baudrate_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address, uint8_t* baudrate);

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
	mbus_master_res_t nt750_set_full_identification_pa(mbus_lib_t* mbus_master, mbus_t* nt750, uint8_t* primary_address, uint32_t password, uint32_t secondary_address, uint16_t manufacturer, uint8_t* generation, uint8_t* medium); 

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
	mbus_master_res_t nt750_set_baudrate(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint8_t* baudrate);

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
	mbus_master_res_t nt750_set_primary_address(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint8_t* new_primary_address);

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
	mbus_master_res_t nt750_set_secondary_address(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t* new_secondary_address);

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
	mbus_master_res_t nt750_restart(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password);

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
	mbus_master_res_t nt750_set_mbus_power_status(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, mbus_power_status_t power_status);

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
	mbus_master_res_t nt750_set_change_password(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* new_password);

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
	mbus_master_res_t nt750_set_service_mode_status(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, mbus_service_mode_t* service_mode_status);

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
	mbus_master_res_t nt750_set_calibration_current(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint16_t* calibration_current_mA);

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
	mbus_master_res_t nt750_set_lock_device(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password);

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
	mbus_master_res_t nt750_set_unlock_device(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password);

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
	mbus_master_res_t nt750_set_customer(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* customer_1, uint32_t* customer_2, uint32_t* customer_3);

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
	mbus_master_res_t nt750_set_factory_reset(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password);

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
	mbus_master_res_t nt750_set_factory_reset_except_calibration(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password);

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
	mbus_master_res_t nt750_set_recovery_periode(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* recovery_periode, uint32_t* fast_recovery_periode);

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
	mbus_master_res_t nt750_set_recovery_try_count(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* recovery_try_count, uint32_t* fast_recovery_try_count);

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
	mbus_master_res_t nt750_set_over_current_limit(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* over_current_limit);

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
	mbus_master_res_t nt750_set_breaking_current_limit(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint32_t* breaking_current_limit);

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
	mbus_master_res_t nt750_set_reset_calibration(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password);

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
	mbus_master_res_t nt750_set_breaking_current_compare_match_count(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, uint8_t* breaking_current_compare_match_count);

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
	mbus_master_res_t nt750_set_mbus_receive_current_treshold(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password, float* f_mbus_receiving_current_treshold);

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
	mbus_master_res_t nt750_set_clean_all_fault_records(mbus_lib_t* mbus_master, mbus_t* nt750, uint32_t secondary_address, uint16_t manufacturer, uint32_t password); 


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NT750_HDR_H */

#endif /* MBUS_MASTER_CFG_NT750_ENABLED */

#endif /* GATEWAY_TYPE == MBUS */
