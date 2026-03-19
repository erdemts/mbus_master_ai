/**
* \file            mbus_master_input.c
* \brief           Wrapper for passing input data to stack
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

#include "mbus_master/mbus_master_int.h"

static uint32_t mbus_recv_total_len;
static uint32_t mbus_recv_calls;

/**
 * \brief           Pass input data to mbus_master stack
 * \param[in]       master: Pointer to mbus_master structure
 * \param[in]       data: Pointer to input data buffer
 * \param[in]       len: Length of input data buffer
 * \return          Result of the operation
 * \note            This function should be called when new data is received from the serial port.
 *                  It processes the input data and updates the mbus_master state accordingly.
 */
mbus_master_res_t mbus_master_input_process(mbus_lib_t* mbus_master, const void* data, size_t data_len) {
    mbus_master_res_t res = 0;

	if (!mbus_master->ll.status.f.initialized) {
        return MBUS_MASTER_ERR_NOT_INITIALIZED;
    }
	
	mbus_recv_total_len += (uint32_t)data_len; 	/* Update total number of received bytes */
    ++mbus_recv_calls;          				/* Update number of calls */
	
    mbus_master_process(mbus_master, data, data_len); /* Process input data */
	
    return res;
}


#endif /* GATEWAY_TYPE == MBUS */

