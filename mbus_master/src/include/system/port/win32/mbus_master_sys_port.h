/**
 * \file            mbus_master_sys_port.h
 * \brief           WIN32 based system file implementation
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
 * This file is part of MODBUSLIB - json communication protocol library.
 *
 * Author:          Erdem Tayfun Salman
 * Version:         v1.0.1
 * ----------------------------------------------------------------------------
 */
#ifndef MBUS_MASTER_HDR_SYSTEM_PORT_H
#define MBUS_MASTER_HDR_SYSTEM_PORT_H

#include <stdint.h>
#include <stdlib.h>
#include "windows.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef HANDLE                      mbus_master_sys_mutex_t;
typedef HANDLE                      mbus_master_sys_sem_t;
typedef HANDLE                      mbus_master_sys_mbox_t;
typedef HANDLE                      mbus_master_sys_thread_t;
typedef int                         mbus_master_sys_thread_prio_t;

#define MBUS_MASTER_SYS_MUTEX_NULL       ((HANDLE)0)
#define MBUS_MASTER_SYS_SEM_NULL         ((HANDLE)0)
#define MBUS_MASTER_SYS_MBOX_NULL        ((HANDLE)0)
#define MBUS_MASTER_SYS_TIMEOUT          (INFINITE)
#define MBUS_MASTER_SYS_THREAD_PRIO      (0)
#define MBUS_MASTER_SYS_THREAD_SS        (4096)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBUS_MASTER_HDR_SYSTEM_PORT_H */
