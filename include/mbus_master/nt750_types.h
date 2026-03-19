/**
* \file            nt750_types.h
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


#include "mbus_master/mbus_master_opt.h"


#if (MBUS_MASTER_CFG_NT750_ENABLED)

#ifndef NT750_TYPES_HDR_H
#define NT750_TYPES_HDR_H

/* Get most important include files */



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */




	/**
	* \brief M-Bus Converter NT750 Command Definitions.
	* \ingroup nt750_cmds
	* \note Must be service mode on to use some commands
	* \note These commands are used to communicate with the NT750 M-Bus Converter.
	*/
	typedef enum {
		// NT750 MBUS Konvertor komutlari. Primari adresleme (PA) ile çalýþýr.
		NT750_GET_PRI_ADDR_PA = 0,                                    /**!< Server to Client. Read primary address of M-BUS slave device. (PA: Primary addressing. Bu komut hatta tek cihaz olduðunda kullanýlabilir.) */
		NT750_GET_SEC_ADDR_PA,                                        /**!< Server to Client. Read secondary address of M-BUS slave device. (PA: Primary addressing.) */
		NT750_GET_BAUD_PA,                                            /**!< Server to Client. Read baudrate of M-BUS slave device. (PA: Primary addressing.) */

		NT750_SET_PRI_ADDR_PA,                                        /**!< Server to Client. Set primary address of M-BUS slave device. (PA: Primary addressing.) */
		NT750_SET_SEC_ADDR_PA,                                        /**!< Server to Client. Set secondary address of M-BUS slave device. (PA: Primary addressing.) */
		NT750_SET_BAUD_PA,                                            /**!< Server to Client. Set baudrate of M-BUS slave device. (PA: Primary addressing.) */

		// !!! MUST BE SERVICE MODE ON !!! to use NT750_SET_FULL_IDENT_PA command.
		NT750_SET_FULL_IDENT_PA,                                      /**!< Server to Client. Set full identification of M-BUS slave device. (PA: Primary addressing.) */


		// NT750 MBUS Konvertor komutlari. Sekondary adresleme (SA) ile çalýþýr.
		NT750_GET_CALIBRATION_CURRENT,                                /**!< Server to Client. Get current sensor calibration value of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_GET_CUSTOMER,                                           /**!< Server to Client. Get customer data of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_GET_RECOVERY_PERIODE,                                   /**!< Server to Client. Get recovery period of M-BUS slave device (in seconds). (SA: Secondary addressing.) */
		NT750_GET_OVERCURR_LIM,                                       /**!< Server to Client. Get overcurrent limit of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_GET_BREAKCURR_LIM,                                      /**!< Server to Client. Get breaking current limit of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_GET_RECOVERY_TRY_CNT,                                   /**!< Server to Client. Get recovery try count of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_GET_BREAKCURR_COMPARE_MATCH_CNT,                        /**!< Server to Client. Get breaking current compare match count of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_GET_MBUS_RECCURR_TRESHOLD,                              /**!< Server to Client. Get M-BUS receiving current threshold of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_GET_FAULT_RECORDS,                                      /**!< Server to Client. Get fault records of M-BUS slave device. (SA: Secondary addressing.) */

		NT750_SET_BAUD,                                               /**!< Server to Client. Set baudrate of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_PRI_ADDR,                                           /**!< Server to Client. Set primary address of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_SEC_ADDR,                                           /**!< Server to Client. Set secondary address of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_RESTART_DEV,                                        /**!< Server to Client. Restart M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_MBUS_PWR,                                           /**!< Server to Client. Set M-BUS power of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_CHNGE_PASS,                                         /**!< Server to Client. Change password of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_SERVICE_MODE,                                       /**!< Server to Client. Set service mode of M-BUS slave device. (SA: Secondary addressing.) */

		// !!! MUST BE SERVICE MODE ON !!! to use below commands.
		NT750_SET_CALIBRATION_CURRENT,                                /**!< Server to Client. Set current sensor calibration value of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_LOCK_DEV,                                           /**!< Server to Client. Lock M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_UNLOCK_DEV,                                         /**!< Server to Client. Unlock M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_CUSTOMER,                                           /**!< Server to Client. Set customer data of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_FCTRY_RST,                                          /**!< Server to Client. Reset M-BUS slave device to factory settings. (SA: Secondary addressing.) */
		NT750_SET_FCTRY_RST_EXC_CAL,                                  /**!< Server to Client. Reset M-BUS slave device to factory settings except calibration values. (SA: Secondary addressing.) */
		NT750_SET_RECOVERY_PERIODE,                                   /**!< Server to Client. Set recovery period of M-BUS slave device (in seconds). (SA: Secondary addressing.) */
		NT750_SET_RECOVERY_TRY_COUNT,                                 /**!< Server to Client. Set recovery try count of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_OVERCURR_LIM,                                       /**!< Server to Client. Set overcurrent limit of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_BREAKCURR_LIM,                                      /**!< Server to Client. Set breaking current limit of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_RESET_CALIBRATION,                                  /**!< Server to Client. Reset calibration values of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_BREAKCURR_COMPARE_MATCH_CNT,                        /**!< Server to Client. Set breaking current compare match count of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_MBUS_RECEIVECURR_TRESHOLD,                          /**!< Server to Client. Set M-BUS receiving current threshold of M-BUS slave device. (SA: Secondary addressing.) */
		NT750_SET_CLEAN_ALL_FAULT_RECORDS,                            /**!< Server to Client. Clean all fault records of M-BUS slave device. (SA: Secondary addressing.) */

	} nt750_cmd_t;



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBUS_SIMPLE_HDR_H */

#endif /* MBUS_MASTER_CFG_NT750_ENABLED */

