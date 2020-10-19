#include "applicationEntry.h"
#include "iDebug.h"
#include "osAbstract.h"
#include "board.h"
#include "iCom.h"
#include "tPool.h"
#include "tasks.h"

#include "tos_k.h"
#include "cmsis_os.h"

#ifdef DEBUG
#include "userTestTasks.h"
#endif

#define REC_TASK_STK_SIZE				512
void initTask(void const * pdata);
osThreadDef(initTask, osPriorityNormal, 1, REC_TASK_STK_SIZE);

static void printHello(void) {
	//Print hello string
	iDebugPrint(DBG_LEV_INFO, "==============================\r\n");
	iDebugPrint(DBG_LEV_INFO, "         HELLO WORLD\r\n");
	iDebugPrint(DBG_LEV_INFO, "==============================\r\n");
}

static void poolInterfaceInit(void) {
	//Init tPool
	tPoolConfigDef tpoolCfg;
	tpoolCfg.Malloc = OsAbsMalloc;
	tpoolCfg.Free = OsAbsFree;
	tpoolCfg.getSysTick = OsAbsTickGet;
	if (tPoolInit(&tpoolCfg) != TP_OK) {
		//Error
		iDebugPrint(DBG_LEV_ERROR, "Pool initialize error\r\n");
	}
}

void initTask(void const * pdata) {
	iDebugInit(); //初始化调试log打印接口
	printHello(); //打印hello world
	poolInterfaceInit(); //初始化池接口
	boardInit(); //板子初始化
	IComInit();

	/*
	 * 创建工作任务
	 */
	fly_task_board_create();
	fly_task_net_create();
	
#ifdef DEBUG
//	com_test_task_create();
#endif

	/*destroy current running thread */
	tos_task_destroy(osThreadGetId());
}

/**
 * 用户程序入口
 */
void applicationEntry(void) {
  	//先创建任务，才可以使用信号量等
	osThreadCreate(osThread(initTask), NULL);
}
