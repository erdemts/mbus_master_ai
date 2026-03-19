/**
* \file            mbus_master_ll_win32.c
* \brief           Low-level communication with uart device for WIN32
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
* Author:          Erdem Tayfun Salman
* Version:         v1.0.0
* ----------------------------------------------------------------------------
*/

#include "devlib/devlib.h"
#include "devlib/devlib_types.h"
#include "mbus_master/mbus_master_input.h"
#include "mbus_master/mbus_master_types.h"


#if !__DOXYGEN__

static uint8_t initialized = 0;
static HANDLE thread_handle;
static volatile HANDLE com_port;                /*!< COM port handle */
static uint8_t data_buffer[0x1000];             /*!< Received data array */

extern mbus_lib_t mbus_master;

static void mbus_master_uart_thread(void* param);

/**
* \brief           Send data to GSM device, function called from GSM stack when we have data to send
* \param[in]       data: Pointer to data to send
* \param[in]       len: Number of bytes to send
* \return          Number of bytes sent
*/
size_t
mbus_master_send_data(const void* data, size_t len) {
	DWORD written;
	if (com_port != NULL) {
#if MBUS_MASTER_CFG_UART_ECHO
		const uint8_t* d = data;
		HANDLE hConsole;

		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

		for (DWORD i = 0; i < len; ++i) {
			printf("%.2X", d[i]);
		}
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif /* !MBUS_MASTER_CFG_AT_ECHO */

		/* Write data to AT port */
		WriteFile(com_port, data, (DWORD)len, &written, NULL);
		FlushFileBuffers(com_port);
		return written;
	}
	return 0;
}

/**
* \brief           Configure UART (USB to UART)
*/
uint8_t
mbus_master_configure_uart(uint32_t baudrate) {
	DCB dcb = { 0 };
	dcb.DCBlength = sizeof(dcb);

	/*
	* On first call,
	* create virtual file on selected COM port and open it
	* as generic read and write
	*/
	if (!initialized) {
		static const char* com_ports[] = { "\\\\.\\COM20", "\\\\.\\COM12", "\\\\.\\COM9", "\\\\.\\COM8", "\\\\.\\COM30" };
		for (size_t i = 0; i < sizeof(com_ports) / sizeof(com_ports[0]); ++i) {
			com_port = CreateFileA(com_ports[i], GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL);
			if (GetCommState(com_port, &dcb)) {
				printf("COM PORT %s opened for MODBUS MASTER!\r\n", (const char*)com_ports[i]);
				break;
			}
		}
	}

	/* Configure COM port parameters */
	if (GetCommState(com_port, &dcb)) {
		COMMTIMEOUTS timeouts;

		dcb.BaudRate = baudrate;
		dcb.ByteSize = 8;
		dcb.Parity = EVENPARITY;
		dcb.StopBits = ONESTOPBIT;

		if (!SetCommState(com_port, &dcb)) {
			printf("Cannot set COM PORT info\r\n");
			return 0;
		}
		if (GetCommTimeouts(com_port, &timeouts)) {
			/* Set timeout to return immediately from ReadFile function */
			timeouts.ReadIntervalTimeout = MAXDWORD;
			timeouts.ReadTotalTimeoutConstant = 0;
			timeouts.ReadTotalTimeoutMultiplier = 0;
			if (!SetCommTimeouts(com_port, &timeouts)) {
				printf("Cannot set COM PORT timeouts\r\n");
			}
			GetCommTimeouts(com_port, &timeouts);
		}
		else {
			printf("Cannot get COM PORT timeouts\r\n");
			return 0;
		}
	}
	else {
		printf("Cannot get COM PORT info\r\n");
		return 0;
	}

	/* On first function call, create a thread to read data from COM port */
	if (!initialized) {
		devlib_sys_thread_create(&thread_handle, "mbus_master_ll_thread", mbus_master_uart_thread, NULL, 0, 0);
	}
	return 1;
}

/**
* \brief            UART thread
*/
static void
mbus_master_uart_thread(void* param) {
	DWORD bytes_read;
	devlib_sys_sem_t sem;
	FILE* file = NULL;

	DEVLIB_UNUSED(param);

	devlib_sys_sem_create(&sem, 0); /* Create semaphore for delay functions */

	while (com_port == NULL) {
		devlib_sys_sem_wait(&sem, 1); /* Add some delay with yield */
	}

	fopen_s(&file, "log_file.txt", "w+"); /* Open debug file in write mode */
	while (1) {
		/*
		 * Try to read data from COM port
		 * and send it to upper layer for processing
		 */
		do {
			ReadFile(com_port, data_buffer, sizeof(data_buffer), &bytes_read, NULL);
			if (bytes_read > 0) {
				HANDLE hConsole;
				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
				for (DWORD i = 0; i < bytes_read; ++i) {
					printf("%.2X", data_buffer[i]);
				}
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

				/* Send received data to input processing module */
				mbus_master_input_process(&mbus_master, data_buffer, (size_t)bytes_read);

				/* Write received data to output debug file */
				if (file != NULL) {
					fwrite(data_buffer, 1, bytes_read, file);
					fflush(file);
				}
			}
		} while (bytes_read == (DWORD)sizeof(data_buffer));

		/* Implement delay to allow other tasks processing */
		devlib_sys_sem_wait(&sem, 1);
	}
}

/**
* \brief           Callback function called from initialization process
*
* \note            This function may be called multiple times if AT baudrate is changed from application.
*                  It is important that every configuration except AT baudrate is configured only once!
*
* \note            This function may be called from different threads in GSM stack when using OS.
*                  When \ref MBUS_MASTER_CFG_INPUT_USE_PROCESS is set to 1, this function may be called from user UART thread.
*
* \param[in,out]   ll: Pointer to \ref mbus_master_ll_t structure to fill data for communication functions
* \return          \ref MBUS_MASTER_OK on success, member of \ref mbus_masterr_t enumeration otherwise
*/
mbus_master_res_t mbus_master_ll_init(mbus_master_ll_t* ll) {
//#if !MBUS_MASTER_CFG_MEM_CUSTOM
//	/* Step 1: Configure memory for dynamic allocations */
//	static uint8_t memory[0x10000]; /* Create memory for dynamic allocations with specific size */
//
//	/*
//	 * Create memory region(s) of memory.
//	 * If device has internal/external memory available,
//	 * multiple memories may be used
//	 */
//	mbus_master_mem_region_t mem_regions[] = { {memory, sizeof(memory)} };
//	if (!initialized) {
//		mbus_master_mem_assignmemory(mem_regions,
//			MBUS_MASTER_ARRAYSIZE(mem_regions)); /* Assign memory for allocations to GSM library */
//	}
//#endif /* !MBUS_MASTER_CFG_MEM_CUSTOM */

	/* Step 2: Set AT port send function to use when we have data to transmit */
	if (!initialized) {
		ll->send_fn = mbus_master_send_data; /* Set callback function to send data */
	}

	/* Step 3: Configure AT port to be able to send/receive data to/from GSM device */
	if (!mbus_master_configure_uart(ll->uart.baudrate)) { /* Initialize UART for communication */
		return MBUS_MASTER_ERR;
	}

	initialized = 1;
	ll->status.f.initialized = 1; /* We are initialized now */
	return MBUS_MASTER_OK;
}

#endif /* !__DOXYGEN__ */
