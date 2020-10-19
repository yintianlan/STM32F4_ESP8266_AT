#include <stdarg.h>
#include <stdio.h>

#include "iDebug.h"
#include "osAbstract.h"
#include "tinyprintf.h"
#include "stm32f4xx_hal.h"


/* 选择打印等级 */
#ifndef LOG_LEVEL
#define LOG_LEVEL				(DBG_LEV_VERBOSE)
#endif
/* 选择打印等级 */

/*打开Logo*/
#if 	LOG_LEVEL >= DBG_LEV_VERBOSE
#define PRINTF_ENABLE			(1)
#endif

static OsAbsMutex txMutex = NULL;
static BOOL initialized = False;

extern UART_HandleTypeDef huart1;

static void iDebugPrintCore(iDebugLevelEnum level, const char *fmt,
		va_list var_arg);
/*****************************************************************************
 **Name:		 	fputc
 **Function:
 **Args:
 **Return:
 ******************************************************************************/
static void stdout_putf(void *unused, char c) {
#if PRINTF_ENABLE
	HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, 0xFFFF);
#endif
}

void iDebugInit(void) {
#if PRINTF_ENABLE
	txMutex = OsAbsMutexCreate();
	if (txMutex == NULL) {
		OsAbsMutexGive(txMutex);
		return;
	}

	//init printf interface
	init_printf(NULL, stdout_putf);
	initialized = True;
#endif
}

void iDebugPrint(iDebugLevelEnum level, const char *fmt, ...) {
#if PRINTF_ENABLE
	if (OS_ABS_ERR_NONE != OsAbsMutexTake(txMutex, OsAbsMaxDelay)) {
		return;
	}

	va_list var_arg;
	va_start(var_arg, fmt);
	iDebugPrintCore(level, fmt, var_arg);
	va_end(var_arg);

	OsAbsMutexGive(txMutex);
#endif
}

static void iDebugPrintCore(iDebugLevelEnum level, const char *fmt,
		va_list var_arg) {
#if PRINTF_ENABLE
	if (True != initialized) {
		return;
	}

	if (level >= LOG_LEVEL && level > DBG_LEV_NO) {
		switch (level) {
		case DBG_LEV_VERBOSE: {
			tfp_printf("V:");
		}
			break;

		case DBG_LEV_DEBUG: {
			tfp_printf("D:");
		}
			break;

		case DBG_LEV_INFO: {
			tfp_printf("I:");
		}
			break;

		case DBG_LEV_WARN: {
			tfp_printf("W:");
		}
			break;

		case DBG_LEV_ERROR: {
			tfp_printf("E:");
		}
			break;

		default:
			break;
		}
		//Print the message
		tfp_format(NULL, stdout_putf, (char*) fmt, var_arg);
	}

#endif
}
