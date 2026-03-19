/**
* \file            mbus_master_includes.h
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

#ifndef MBUS_MASTER_INCLUDES_H
#define MBUS_MASTER_INCLUDES_H


/* Get most important include files */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mbus_master/mbus_master_types.h"
#include "mbus_master/mbus_master_utils.h"
#include "mbus_master/mbus_master_input.h"
#include "mbus_master/mbus_master_opt.h"
#include "mbus_master/mbus_master_int.h"

#if (MBUS_MASTER_CFG_NT750_ENABLED)
#include "nt750/lwrb.h"
#include "nt750/mbus_aux.h"
#include "nt750/mbus_protocol.h"
#include "nt750/nt750_types.h"
#include "nt750/nt750.h"

#endif /* MBUS_MASTER_CFG_NT750_ENABLED */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBUS_MASTER_INCLUDE_H */

#endif /* GATEWAY_TYPE == MBUS */