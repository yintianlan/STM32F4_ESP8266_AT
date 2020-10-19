/*
 * comTest.c
 *
 *  Created on: 2020年8月18日
 *      Author: LinQian
 */
#if 0
#include "iDebug.h"
#include "iCom.h"
#include "osAbstract.h"
#include "tos_k.h"
#include "cmsis_os.h"


#include "comTest.h"



//这个size需要注意，如果太小，会导致执行频繁，而其它任务被饿死
#define REC_TASK_STK_SIZE				1024
static void com1_test(void const *pdata);
osThreadDef(com1_test, osPriorityNormal, 1, REC_TASK_STK_SIZE);

static void com2_test(void const *pdata);
osThreadDef(com2_test, osPriorityNormal, 1, REC_TASK_STK_SIZE);


//串口
static iComObjectDef com1;
static iComObjectDef com2;

//接收信号量
static OsAbsSemaphore sem_com1;
static OsAbsSemaphore sem_com2;

/**
 * 串口有新数据
 */
static void com1_callback(void) {
	//释放信号量
	OsAbsSemaphoreGive(sem_com1);
}

static void com2_callback(void) {
	//释放信号量
	OsAbsSemaphoreGive(sem_com2);
}


static uint8_t rec_pool1[1024];
void com1_test(void const *arg) {
	sem_com1 = OsAbsSemaphoreCreate();
	if(sem_com1 == NULL){
		return;
	}

	static uint32_t totalRec1 = 0;

	com1.com = COM_1;
	com1.priority = 3;
	com1.bitrate = COM_BITRATE_115200;
	com1.bufferLength = 1024;
//	com1.xferMode = COM_XFER_STANDARD;
//	com1.xferMode = COM_XFER_INTERRUPT;
	com1.xferMode = COM_XFER_DMA;
	com1.GotFrameFromIrq = com1_callback;

	iDebugPrint(DBG_LEV_INFO, "com1 init...\r\n");
	if (ICOM_SUCCESS != IComConstruct(&com1)) {
		iDebugPrint(DBG_LEV_ERROR, "com1 init error\r\n");
		return;
	}

	//usart start
	IComStart(&com1);

	uint32_t rec_size;
	OsAbsErr err;

	while (1) {
		err = OsAbsSemaphoreTake(sem_com1, 3*1000);
		if (err != OS_ABS_ERR_NONE) {
			iDebugPrint(DBG_LEV_WARN, "com1 may be error :%x\r\n", err);
			IComStart(&com1);
			continue;
		}

		while(!IComIsEmpty(&com1))
		{
			rec_size = IComGetData(&com1, rec_pool1, sizeof(rec_pool1));
			if (rec_size > 0) {
				IComTransmit(&com1, rec_pool1, rec_size);
			}
			totalRec1 += rec_size;
		}
	}

}

static uint8_t rec_pool2[1024];
void com2_test(void const *arg) {
	sem_com2 = OsAbsSemaphoreCreate();
	if(sem_com2 == NULL){
		return;
	}
	static uint32_t totalRec2 = 0;

	com2.com = COM_2;
	com2.priority = 2;
	com2.bitrate = COM_BITRATE_115200;
	com2.bufferLength = 1024;
//	com2.xferMode = COM_XFER_STANDARD;
//	com2.xferMode = COM_XFER_INTERRUPT;
	com2.xferMode = COM_XFER_DMA;
	com2.GotFrameFromIrq = com2_callback;

	iDebugPrint(DBG_LEV_INFO, "com2 init...\r\n");
	if (ICOM_SUCCESS != IComConstruct(&com2)) {
		iDebugPrint(DBG_LEV_ERROR, "com2 init error\r\n");
		return;
	}

	//usart start
	IComStart(&com2);

	uint32_t rec_size;
	osAbsState err;

	while (1) {
		err = OsAbsSemaphoreTake(sem_com2, 3*1000);
		if (err != OS_ABS_ERR_NONE) {
			iDebugPrint(DBG_LEV_WARN, "com2 may be error :%x\r\n", err);
			IComStart(&com2);
			continue;
		}

		while(!IComIsEmpty(&com2))
		{
			rec_size = IComGetData(&com2, rec_pool2, sizeof(rec_pool2));
			if (rec_size > 0) {
				IComTransmit(&com2, rec_pool2, rec_size);
			}
			totalRec2 += rec_size;
		}
	}
}


void com_test_task_create(void) {
	//com1
	osThreadCreate(osThread(com1_test), NULL);

	//com2
	osThreadCreate(osThread(com2_test), NULL);

}

#endif
