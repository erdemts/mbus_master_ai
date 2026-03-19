/**
* \file            main.h
* \brief           Win32 main header file
*/

/*
* Copyright (c) 2023 Erdem Tayfun Salman
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* This file is part of AT25XE161D driver library
*
* Author:          Erdem Tayfun Salman
* Version:         v0.0.1
*/
#ifndef DEV_WIN32_MAIN_HDR_H
#define DEV_WIN32_MAIN_HDR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



// winsock.h dosyasýnýn dahil edilmesini engelle
#define _WINSOCKAPI_ 
#include "windows.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Gateway function type definitions                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
#define IEC62056                                1
#define MODBUS_RTU                              2
#define MBUS                                    3
/* Cihazýn hangi protokolü desteklediðini belirler. */
#define GATEWAY_TYPE                            MBUS

/*---------------------------------------------------------------------------------------------------------*/
/* Debug definitions                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
/* Debug logging configuration */
#ifndef DEBUG_LOG_ENABLE
#define DEBUG_LOG_ENABLE                       1 /* 1: enable logs, 0: disable */
#endif

#ifndef DEBUG_LOG_LEVEL
#define DEBUG_LOG_LEVEL                        LOG_DEBUG /* 0: none, 1: error, 2: warn, 3: info, 4: debug, 5: trace */
#endif

#if DEBUG_LOG_ENABLE
#define DBG_PRINTF(...)        printf(__VA_ARGS__)
#define DBG_LOG(level, ...)    do { if ((level) <= DEBUG_LOG_LEVEL) { printf(__VA_ARGS__); } } while (0)
#else
#define DBG_PRINTF(...)        ((void)0)
#define DBG_LOG(level, ...)    ((void)0)
#endif

/* Optional: predefined log level constants */
#define LOG_ERROR 1
#define LOG_WARN  2
#define LOG_INFO  3
#define LOG_DEBUG 4
#define LOG_TRACE 5


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INEDGE_WIN32_MAIN_HDR_H */

/************************ (C) COPYRIGHT ERDEM TAYFUN SALMAN *****END OF FILE****/