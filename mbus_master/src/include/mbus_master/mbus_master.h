/**
* \file            mbus_master.h
* \brief           mbus master library header
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
* This file is part of MBUS_MASTER communication protocol library.
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

#ifndef MBUS_MASTER_HDR_H
#define MBUS_MASTER_HDR_H

/* Get most important include files */
#include <stdint.h>
#include "mbus_master/mbus_master_includes.h"


//extern mbus_simple_t mbus_master;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


	/**
	 * \brief           master master initialization
	 * \param[in,out]   mbus: Pointer to mbus_master simple structure
	 *
	 * \details         This function initializes the mbus_master simple structure.
	 *
	 */
	void mbus_master_init(mbus_lib_t* master);

	/**
	 * \brief           mbus_master read meter
	 * \param[in]       mbus_master: Pointer to mbus_master structure
	 * \details         This function reads the meter data from the mbus_master.
	 * \return          Result of the read operation
	 */
	mbus_master_res_t mbus_master_read(mbus_lib_t* mbus_master);

	/** \brief           Send select device command to MBUS slave
	 * \param[in]       master: Pointer to mbus_master structure
	 * \return          Result of sending command: number of bytes sent or error code
	 * \details         This function constructs and sends a select device command to the MBUS slave.
	 *                  It uses the slave select type from the mbus_master structure to determine the command format.
	 *                  The function computes the checksum and sends the command using the low-level send function.
	 */
	size_t mbus_master_send_select_device(mbus_lib_t* mbus_master);

#if (MBUS_MASTER_CFG_NT750_ENABLED)
	/**
	 * \brief           Get data from the MBUS master
	 * \param[in]       mbus_master: Pointer to mbus_master structure
	 * \param[in]       nt750: Pointer to nt750 structure
	 * \param[in]       cmd: Command to execute
	 * \return          Result of the operation
	 * \details         This function retrieves data from the NT750 MBUS converter using the MBUS protocol.
	 *                  It handles both reading and setting commands for the NT750 device.
	 */
	mbus_master_res_t mbus_master_get(mbus_lib_t* mbus_master, mbus_t* nt750, nt750_cmd_t cmd);

	/**
	 * \brief           Set data to the MBUS master simple
	 * \param[in]       mbus_master: Pointer to mbus_master structure
	 * \param[in]       nt750: Pointer to nt750 structure
	 * \param[in]       cmd: Command to execute
	 * \return          Result of the operation
	 * \details         This function sets data to the NT750 MBUS converter using the MBUS simple protocol.
	 *                  It handles various setting commands for the NT750 device.
	 */
	mbus_master_res_t mbus_master_set(mbus_lib_t* mbus_master, mbus_t* nt750, nt750_cmd_t cmd);
#endif

	/**
	* \brief           Delay function
	*
	* This function creates a delay for a specified number of milliseconds
	* without blocking the execution of other tasks.
	*
	* \param[in]       ms: Number of milliseconds to delay
	*/
	void mbus_master_delay(uint32_t ms);

	/**
	 * \brief           Check if mbus_master is idle
	 * \param[in]       mbus_master: Pointer to mbus_master structure
	 * \return          1 if mbus_master is idle, 0 otherwise
	 * \details         This function checks the state of the mbus_master to determine if it is idle.
	 */
	uint32_t is_mbus_master_idle(mbus_lib_t* mbus_master);

	/**
	 * \brief           Sleep function
	 * \param[in]       ms: Number of milliseconds to sleep
	 * \details         This function puts the current task to sleep for a specified number of milliseconds.
	 *                  It uses the osDelay function from the CMSIS-RTOS API.
	 */
	void mbus_master_sleep(uint32_t ms);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBUS_SIMPLE_HDR_H */

#endif /* GATEWAY_TYPE == MBUS */
