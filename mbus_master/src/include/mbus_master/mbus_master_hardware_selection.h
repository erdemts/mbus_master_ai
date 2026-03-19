/**
* \file            mbus_master_hardware_selection.h
* \brief           mbus_master hardware selection
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
* This file is part of MBUS_MASTER communication protocol library.
*
* Author:          Erdem Tayfun Salman
* Version:         v1.0.0
* ----------------------------------------------------------------------------
*/

#ifndef MBUS_MASTER_HW_H
#define MBUS_MASTER_HW_H

#include <stdint.h>
#include "mbus_master_opts.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



	/*---------------------------------------------------------------------------------------------------------*/
	/* Gateway hardware type definitions                                                                       */
	/*---------------------------------------------------------------------------------------------------------*/
#define HARDWARE_TYPE_INEDGE_GATEWAY           1
#define HARDWARE_TYPE_IN223_GATEWAY            2
#define HARDWARE_TYPE_WIN32_SIMULATOR          3
#define HARDWARE_TYPE_CNV750IOT_CONVERTER      4
	/*---------------------------------------------------------------------------------------------------------*/
	/* Select gateway hardware type here                                                                      */
    /*---------------------------------------------------------------------------------------------------------*/


/* Kullanýlan donanýma uygun uart dosyalarý kullanýlmalýdýr */
//#define GATEWAY_HARDWARE_TYPE                 HARDWARE_TYPE_WIN32_SIMULATOR

#if(GATEWAY_HARDWARE_TYPE == HARDWARE_TYPE_WIN32_SIMULATOR)
#define WIN32_DEBUG                            1
#endif



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBUS_MASTER_HW_H */

