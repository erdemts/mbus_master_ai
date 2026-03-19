/**
* \file            mbus_master_ll.h
* \brief           Low-level communication implementation
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
* This file is part of MBUS_MASTER - json communication protocol library.
*
* Author:          Erdem Tayfun Salman
* Version:         v1.0.1
* ----------------------------------------------------------------------------
*/

#ifndef MBUS_MASTER_HDR_LL_H
#define MBUS_MASTER_HDR_LL_H

#include "mbus_master/mbus_master_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        MBUS_MASTER_LL Low-level functions
 * \brief           Low-level communication functions
 * \{
 */
mbus_master_res_t mbus_master_ll_init(mbus_master_ll_t* ll);


/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBUS_MASTER_HDR_LL_H */
