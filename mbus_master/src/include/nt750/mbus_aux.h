/**
 * \file            mbus.h
 * \brief           MBUS_lib - MBUS Library
 */

 /*
   * Copyright (c) 2022 Inno Teknoloji Elektrnoik San ve Tic Ltd Sti
   *
   *
   * This file is part of MBUS_library - MBUS library.
   *
   * Author:          Erdem Tayfun Salman <erdem@innoteknoloji.com>
   * Version:         v1.1.0
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

#if(MBUS_MASTER_CFG_NT750_ENABLED)

   //#define DLL_EXPORT


#ifdef DLL_EXPORT

#define MBUSLIBRARY_EXPORTS  

#ifdef MBUSLIBRARY_EXPORTS
#define MBUSLIBRARY_API __declspec(dllexport)
#else
#define MBUSLIBRARY_API __declspec(dllimport)
#endif

#else  

#define MBUSLIBRARY_API  

#endif // DLL_EXPORT


#ifndef _MBUS_AUX_H_
#define _MBUS_AUX_H_


#include "mbus_protocol.h"
#include "lwrb.h"
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \defgroup        MBUS library
	 * \brief           MBUS library Aux Functions.
	 * \{
	 */


	 /**
	  * \brief           MBUS identifications.
	  */
#define ACK													0xE5

	  /**
		* \brief           Service modes.
		*/


		/**
		* \brief           MBUS responses.
		*/
	typedef enum {
		MBUS_RSP_INVALID_SECONDARY_ADDRESS = (-20),
		MBUS_RSP_INVALID_PRIMARY_ADDRESS,
		MBUS_RSP_INVALID_MANUFACTURER_ID,
		MBUS_RSP_INVALID_DEVICE_GENERATION,
		MBUS_RSP_INVALID_DEVICE_MEDIUM,
		MBUS_ERROR = 0,
		MBUS_OK = 1,
		MBUS_RSP_NO_RESPONSE,
		MBUS_RSP_SEND_ACK,
		MBUS_RSP_SEND_RSP_UD,
		MBUS_RSP_SEND_PRIMARY_ADDRESS,
		MBUS_RSP_SEND_SECONDARY_ADDRESS,
		MBUS_RSP_SEND_BAUDRATE,
		MBUS_RSP_SEND_CALIBRATION_CURRENT,
		MBUS_RSP_SEND_CUSTOMER_LOCATION,
		MBUS_RSP_SEND_SUPER_PASSWORD,
		MBUS_RSP_SEND_RECOVERY_PERIODE,
		MBUS_RSP_SEND_RECOVERY_TRY_COUNT,
		MBUS_RSP_SEND_OVER_CURRENT_LIMIT,
		MBUS_RSP_SEND_BREAKING_CURRENT_LIMIT,
		MBUS_RSP_SEND_BREAKING_CURRENT_COMPARE_MATCH_COUNT,
		MBUS_RSP_SEND_MBUS_RECEIVING_CURRENT_TRESHOLD,
		MBUS_RSP_SEND_MBUS_FAULT_RECORDS,
		MBUS_RSP_SET_BAUDRATE,
		MBUS_RSP_SET_PRIMARY_ADDRESS,
		MBUS_RSP_SET_SECONDARY_ADDRESS,
		MBUS_RSP_SET_COMPLATE_IDENTIFICATION,
		MBUS_RSP_SET_SERVICE_MODE_STATUS,
		MBUS_RSP_SET_MBUS_POWER_STATUS,
		MBUS_RSP_SET_CUSTOMER_LOCATION,
		MBUS_RSP_SET_RECOVERY_PERIODE,
		MBUS_RSP_SET_RECOVERY_TRY_COUNT,
		MBUS_RSP_SET_OVER_CURRENT_LIMIT,
		MBUS_RSP_SET_BREAKING_CURRENT_LIMIT,
		MBUS_RSP_SET_BREAKING_CURRENT_COMPARE_MATCH_COUNT,
		MBUS_RSP_SET_MBUS_RECEIVING_CURRENT_TRESHOLD,
		MBUS_RSP_LOCK_DEVICE,
		MBUS_RSP_UNLOCK_DEVICE,
		MBUS_RSP_CALIBRATE_CURRENT_SENSOR,
		MBUS_RSP_CHANGE_PASSWORD,
		MBUS_RSP_FABRICATION_RESET,
		MBUS_RSP_FABRICATION_RESET_EXPECT_CALIBRATION,
		MBUS_RSP_RESET_CALIBRATION,
		MBUS_RSP_RESET_STORAGE,
		MBUS_RSP_RESTART_DEVICE,
		MBUS_RSP_CLEAN_ALL_FAULT_RECORDS
	}mbus_reponse_t;

	/**
	* \brief           MBUS selection statuses.
	*/
	typedef enum {
		MBUS_NOT_SELECTED,
		MBUS_SELECT_FOR_RSP_UD,
		MBUS_SELECT_FOR_READ_PRIMARY_ADDRESS,
		MBUS_SELECT_FOR_READ_SECONDARY_ADDRESS,
		MBUS_SELECT_FOR_READ_BAUDRATE,
		MBUS_SELECT_FOR_READ_SENSOR_CALIBRATION_CURRENT,
		MBUS_SELECT_FOR_READ_CUSTOMER_LOCATION,
		MBUS_SELECT_FOR_READ_SUPER_PASSWORD,
		MBUS_SELECT_FOR_READ_RECOVERY_PERIODE,
		MBUS_SELECT_FOR_READ_RECOVERY_TRY_COUNT,
		MBUS_SELECT_FOR_READ_OVER_CURRENT_LIMIT,
		MBUS_SELECT_FOR_READ_BREAKING_CURRENT_LIMIT,
		MBUS_SELECT_FOR_READ_BREAKING_CURRENT_COMPARE_MATCH_COUNT,
		MBUS_SELECT_FOR_READ_MBUS_RECEIVING_CURRENT_TRESHOLD,
		MBUS_SELECT_FOR_READ_FAULT_RECORDS
	}mbus_selection_t;

	/**
	* \brief           MBUS power status
	*/
	typedef enum {
		MBUS_POWER_DONT_CHANGE = 0,
		MBUS_POWER_ON,
		MBUS_POWER_OFF,
	} mbus_power_status_t;

	/**
	* \brief           cnv500 device lock status
	*/
	typedef enum {
		DEVICE_LOCKED = 1,
		DEVICE_UNLOCKED,
	}mbus_lock_status_t;

	/**
	* \brief           Service mode status (Cihaz ayarlarý servis mod = 1 ise yapabilir)
	*/
	typedef enum {
		SERVICE_MODE_ON = 1,
		SERVICE_MODE_OFF,
	}mbus_service_mode_t;

	/**
	* \brief           Calibration status
	*/
	typedef enum {
		DEVICE_CALIBRATED = 1,
		DEVICE_NOT_CALIBRATED,
	} mbus_calibration_status_t;


	/**
	 * \brief           MBUS structure forward declaration
	 */
	 //struct mbus;

	 /**
	 * \brief           MBUS structure.
	 */
	typedef struct {
		//MBUS General Values
		uint8_t                   primary_address;                         /*!< Primary address of the M-Bus device */
		uint32_t                  secondary_address;                       /*!< Secondary address of the M-Bus device */
		uint16_t                  manufacturer;                            /*!< Manufacturer ID */
		uint8_t                   generation;                              /*!< Generation of the M-Bus device */
		uint8_t                   medium;                                  /*!< Medium of the M-Bus device */
		uint8_t                   access_number;                           /*!< Access number of the M-Bus device */
		uint8_t                   status;                                  /*!< Status of the M-Bus device */
		uint16_t                  signature;                               /*<! Signature of the M-Bus device */
		mbus_selection_t          selection_status;                        /*<! Selection status of the M-Bus device */

		//RSP_UD Values
		uint32_t                  hardware_version;                        /*<! Hardware version of the M-Bus device */
		uint16_t                  model_version;                           /*<! Model version of the M-Bus device */
		uint16_t                  firmware_version;                        /*<! Firmware version of the M-Bus device */
		uint8_t                   current_app_state;                       /*<! Current application state of the M-Bus device */
		uint32_t                  error_flags;                             /*<! Error flags of the M-Bus device */
		mbus_power_status_t       mbus_power_status;                       /*!< M-Bus output power status */
		uint16_t                  mbus_current_mA;                         /*<! Mbus current in mA */
		uint16_t                  mbus_voltage_mV;                         /*<! Mbus voltage in mV */
		uint16_t                  dc24V_internal_voltage_mV;               /*<! Internal 24V dc/dc converter voltage in mV */
		uint16_t                  dc36V_internal_voltage_mV;               /*<! Internal 36V dc/dc converter voltage in mV */
		uint16_t                  power_supply_voltage_mV;                 /*<! External power supply voltage in mV */
		uint16_t                  calibration_current_mA;                  /*!< Current sensor calibration current in mA */
		mbus_service_mode_t       service_mode_status;                     /*!< Service mode status */
		mbus_calibration_status_t calibration_status;                      /*!< Calibration status */

		//Device Based Values
		uint8_t                   new_primary_address;                     /*<! New primary address of the M-Bus device */
		uint32_t                  new_secondary_address;                   /*<! New secondary address of the M-Bus device */
		uint32_t                  password;                                /*!< Password for device access */
		uint32_t                  new_password;                            /*!< New password for device access */
		uint32_t                  admin_password;                          /*!< Super password for device access */

		uint32_t                  new_admin_password;                      /*<! New super password for device access */
		uint32_t                  recovery_periode;                        /*<! Period for device over current or short circuit recover try periode */
		uint32_t                  fast_recovery_periode;                   /*<! Period for device over current or short circuit fast recover try periode */
		uint32_t                  recovery_try_count;                      /*<! Recover try count for device */
		uint32_t                  fast_recovery_try_count;                 /*<! Fast recover try count for device */
		uint32_t                  over_current_limit;                      /*!< Over current limit for device */
		uint32_t                  breaking_current_limit;                  /*!< Breaking current limit for device */
		uint32_t                  breaking_current_adc_count;              /*<! Breaking current ADC count */
		uint8_t                   breaking_current_compare_match_count;    /*<! Breaking current compare match count */
		mbus_lock_status_t        device_lock_status;                      /*!< Device lock status */
		uint32_t                  customer_location_1;                     /*!< Customer location 1 */
		uint32_t                  customer_location_2;                     /*!< Customer location 2 */
		uint32_t                  customer_location_3;                     /*!< Customer location 3 */
		uint8_t                   baudrate;                                /*<! Current baudrate setting */
		float                     f_mbus_receiving_current_treshold;       /*!< M-Bus receiving current threshold (mA Mbus slave okunurken hatta olusan akim degisim degeri) */
		uint8_t                   fault_records_group_number;              /*<! Fault records group number */
		uint16_t                  vdd_analog_mV;                           /*<! VDD analog voltage in mV */
		float                     f_calibrated_AV;                         /*<! Calibrated A/V value */

		mbus_reponse_t            response;                                /*<! Response of the M-Bus device */
		uint8_t                   current_fault_index;                     /*<! Current fault index of the M-Bus device */
		uint8_t                   fault_record_index;                      /*<! Fault record index of the M-Bus device */
		uint8_t                   wdt_reset_flag;                          /*<! Watchdog reset accurred flag */
	}mbus_t;


	void float_to_hex(float* source, uint32_t* dest);
	void hex_to_float(uint32_t* source, float* dest);

	MBUSLIBRARY_API const char* mbus_get_current_version(void);
	MBUSLIBRARY_API void mbus_init_for_slave(mbus_t* mbus);
	MBUSLIBRARY_API int32_t mbus_check_secondary_address(mbus_frame* frame, mbus_t* mbus);
	MBUSLIBRARY_API int32_t mbus_check_secondary_address_with_manufacturer(mbus_frame* frame, mbus_t* mbus);
	MBUSLIBRARY_API uint8_t mbus_check_password(uint32_t password, uint32_t device_password);
	MBUSLIBRARY_API uint8_t mbus_check_super_password(uint32_t super_password, uint32_t device_super_password);
	MBUSLIBRARY_API mbus_reponse_t mbus_process_frame(mbus_frame* frame, mbus_t* mbus, uint8_t* data_buffer, size_t data_size);

	MBUSLIBRARY_API void mbus_send_ack(uint8_t* data_buffer, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_primary_address(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_secondary_address(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_baudrate(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_calibration_current(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_customer_location(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_RSP_UD(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_super_password(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_recovery_periode(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_recovery_try_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_over_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_breaking_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_breaking_current_compare_match_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_mbus_receiving_current_treshold(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_send_fault_records(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size, volatile uint32_t* fault_buffer, volatile uint32_t* fault_mbus_current_and_voltage);
	MBUSLIBRARY_API void mbus_SND_NKE(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_REQ_UD2(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);

	//Primary Address Based Functions
	MBUSLIBRARY_API void mbus_read_primary_address_PA(uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_secondary_address_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_primary_address_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_secondary_address_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_complate_identification_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_baudrate_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_baudrate_PA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);

	//Secondary Address Based Functions
	MBUSLIBRARY_API void mbus_set_primary_address(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_secondary_address_SA(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_baudrate(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_service_mode(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_calibration_current(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_calibration_current(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_mbus_power(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_lock_device(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_unlock_device(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_customer_location(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_customer_location(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_change_password(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_fabrication_reset(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);;
	MBUSLIBRARY_API void mbus_fabrication_reset_except_calibration(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_select_slave(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_super_password(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_recovery_periode(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_recovery_periode(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_recovery_try_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_recovery_try_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_restart_device(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_over_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_over_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_breaking_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_breaking_current_limit(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_reset_calibration(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_reset_storage(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_breaking_current_compare_match_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_breaking_current_compare_match_count(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_set_mbus_receiving_current_treshold(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_mbus_receiving_current_treshold(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_read_fault_records(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);
	MBUSLIBRARY_API void mbus_clean_all_fault_records(mbus_t* mbus, uint8_t* data_buffer, size_t buffer_size, size_t* data_size);




#ifdef __cplusplus
}
#endif

#endif /* _MBUS_AUX_H_ */

/************************ (C) COPYRIGHT InnoTeknoloji  *****END OF FILE****/

#endif /* MBUS_MASTER_CFG_NT750_ENABLED */

#endif /* GATEWAY_TYPE == MBUS */


