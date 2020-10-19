#include <stdlib.h>

#include "tos_k.h"
#include "osAbstract.h"
//#include "chip.h"


void OsAbsDelay(uint32 delay) {
	HAL_Delay(delay);
}

uint32 OsAbsTickGet(void) {
	return HAL_GetTick();
}

uint32 OsAbsGetElaspedTick(uint32 tickPre) {
	uint32 tickNow, timeElapsed;

	tickNow = OsAbsTickGet();
	timeElapsed =
			tickNow >= tickPre ?
					tickNow - tickPre : (0xffffffff - tickPre) + tickNow + 1;

	return timeElapsed;
}

void* OsAbsMalloc(uint32 size) {
	return tos_mmheap_alloc(size);
}

void OsAbsFree(void *pv) {
	tos_mmheap_free(pv);
}

void OsAbsTaskPauseScheduler(void) {
	tos_knl_sched_lock();
}

void OsAbsTaskResumeScheduler(void) {
	tos_knl_sched_unlock();
}

#ifdef __GNUC__
void OsAbsEnterCriticalSection(void) {
	__disable_irq(); //关总中断
}

void OsAbsExitCriticalSection(void) {
	__enable_irq(); //开总中断
}

void OsAbsEnterCriticalSectionFromIsr(void){
	__disable_irq();//关总中断
}

void OsAbsExitCriticalSectionFromIsr(void){
	__enable_irq(); //开总中断
}
#else
extern void CPU_Disable_Irq(void);
extern void CPU_Enable_Irq(void);
void OsAbsEnterCriticalSection(void) {
//	__disable_irq(); //关总中断
	CPU_Disable_Irq();
}

void OsAbsExitCriticalSection(void) {
//	__enable_irq(); //开总中断
	CPU_Enable_Irq();
}

void OsAbsEnterCriticalSectionFromIsr(void){
//	__disable_irq();//关总中断
	CPU_Disable_Irq();
}

void OsAbsExitCriticalSectionFromIsr(void){
//	__enable_irq(); //开总中断
	CPU_Enable_Irq();
}

#endif


/*
创建一个二进制信号量
*/
OsAbsSemaphore OsAbsSemaphoreCreate(void)
{
	k_sem_t *semaphore = OsAbsMalloc(sizeof(k_sem_t));
	if (semaphore == NULL)
		return NULL;
	if (K_ERR_NONE == tos_sem_create(semaphore, 0)) {
		return semaphore;
	}

	OsAbsFree(semaphore);
	return NULL;
}

/*
等待信号量
*/
uint32 OsAbsSemaphoreTake(OsAbsSemaphore semaphore, uint32 timeout)
{
	k_err_t error;

	error = tos_sem_pend(semaphore, tos_millisec2tick(timeout));

	if (K_ERR_NONE == error) {
		return OS_ABS_ERR_NONE;
	}
	return error;
}

/*
*归还一个信号量
*/
void OsAbsSemaphoreGive(OsAbsSemaphore semaphore)
{
	if (K_ERR_NONE == tos_sem_post(semaphore)) {
		return;
	}
	return;
}

/*
*删除信号量
*/
void OsAbsSemaphoreDelete(OsAbsSemaphore semaphore)
{
	if (semaphore == NULL)
		return;

	tos_sem_destroy(semaphore);
}

OsAbsMutex OsAbsMutexCreate(void) {
	k_mutex_t *mutex = OsAbsMalloc(sizeof(k_mutex_t));
	if (mutex == NULL)
		return NULL;
	if (K_ERR_NONE == tos_mutex_create(mutex)) {
		return mutex;
	}

	OsAbsFree(mutex);
	return NULL;
}

void OsAbsMutexDestory(OsAbsMutex mutex) {
	if (mutex == NULL)
		return;

	tos_mutex_destroy(mutex);
}

uint32 OsAbsMutexTake(OsAbsMutex mutex, uint32 timeout) {
	k_err_t error;
	if (timeout != OsAbsMaxDelay && timeout != 0) {
		error = tos_mutex_pend_timed(mutex, tos_millisec2tick(timeout));
	} else {
		error = tos_mutex_pend(mutex);
	}

	if (K_ERR_NONE == error) {
		return OS_ABS_ERR_NONE;
	}
	return error;
}

void OsAbsMutexGive(OsAbsMutex mutex) {
	k_err_t error;
	error = tos_mutex_post(mutex);
	if (K_ERR_NONE == error) {
		return;
	}
	return;
}

void OsAbsSleep(uint32 time) {
	tos_task_delay(tos_millisec2tick(time));
}

void OsAbsYield(void) {
	tos_task_yield();
}

//static unsigned int taskIdRegistryTable = 0;
//static unsigned int taskCount = 0;
//uint32 OsAbsTaskFeedDogRegist(void) {
//	OsAbsEnterCriticalSection();

//	uint32 tskId;

//	tskId = 0x01 << taskCount;
//	taskCount++;

//	/*被注册进入taskIdRegistryTable中的任务必须按时喂狗*/
//	taskIdRegistryTable |= tskId;

//	OsAbsExitCriticalSection();

//	return tskId;
//}

//void OsAbsTaskFeedDog(uint32 taskId) {
//	static unsigned int tsk = 0;

//	tsk |= taskId;

//	/*所有任务动成功报道后再喂狗*/
//	if (tsk == taskIdRegistryTable) {
//		ChipFeedDog();
//		tsk = 0;
//	}
//}

