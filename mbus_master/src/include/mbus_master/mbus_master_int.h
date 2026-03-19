/**
* \file            mbus_master_int.h
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

#ifndef MBUS_MASTER_INT_H
#define MBUS_MASTER_INT_H

#include <stdint.h>
#include "mbus_master/mbus_master_types.h"



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


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
	void mbus_master_process(mbus_lib_t* mbus_master, const void* data, size_t data_len);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBUS_MASTER_INT_H */

#endif /* GATEWAY_TYPE == MBUS */
