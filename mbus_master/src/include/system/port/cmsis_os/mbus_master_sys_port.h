 /**
 * \file            modbuslib_sys_port.h
 * \brief           System dependent functions for CMSIS-OS based operating system
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

#ifndef MODBUSLIB_HDR_SYSTEM_PORT_H
#define MODBUSLIB_HDR_SYSTEM_PORT_H

#include <stdint.h>
#include <stdlib.h>
#include "modbuslib/modbuslib_opt.h"

#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef osMutexId_t                 modbuslib_sys_mutex_t;
typedef osSemaphoreId_t             modbuslib_sys_sem_t;
typedef osMessageQueueId_t          modbuslib_sys_mbox_t;
typedef osThreadId_t                modbuslib_sys_thread_t;
typedef osPriority_t                modbuslib_sys_thread_prio_t;

#define MODBUSLIB_SYS_MUTEX_NULL          ((modbuslib_sys_mutex_t)0)
#define MODBUSLIB_SYS_SEM_NULL            ((modbuslib_sys_sem_t)0)
#define MODBUSLIB_SYS_MBOX_NULL           ((modbuslib_sys_mbox_t)0)
#define MODBUSLIB_SYS_TIMEOUT             ((uint32_t)osWaitForever)
#define MODBUSLIB_SYS_THREAD_PRIO         (osPriorityNormal)
#define MODBUSLIB_SYS_THREAD_SS           (384)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DOPLIB_HDR_SYSTEM_PORT_H */
