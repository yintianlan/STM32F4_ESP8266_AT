#ifndef __osabstract_H
#define __osabstract_H

#include "types.h"

#define OsAbsMaxDelay		(0xffffffff)

typedef void *OsAbsSemaphore;
typedef void *OsAbsTaskHandle;
typedef void *OsAbsMutex;
typedef uint32 osAbsState;

typedef enum {
	OS_ABS_ERR_NONE = 0,		//无错误
	OS_ABS_FAILED = 0xf1,
} OsAbsErr;

#define OSABS_SUCCESS 	((osAbsState) 1)
#define OSABS_FAILED 	((osAbsState) 0)

/*
 *延时函数
 */
extern void OsAbsDelay(uint32 delay);

/*
 *获取当前systick值
 */
extern uint32 OsAbsTickGet(void);

/*
 *计算tickPre到此刻所消逝的时间(ms)
 */
extern uint32 OsAbsGetElaspedTick(uint32 tickPre);

/*
 *时钟更新
 */
extern void OsAbsTickUpdate(void);

/*
 *从heap中申请内存
 *@par size：字节数
 *ret :NULL if failed
 */
extern void* OsAbsMalloc(uint32 size);

/*
 *是否申请的内存
 *@par pv：通过OsAbsMalloc申请的内存的首地址。
 */
extern void OsAbsFree(void *pv);

/*
 *当前任务睡眠time(ms)
 *@par 时间。
 */
extern void OsAbsSleep(uint32 time);

/*
 *操作系统暂停调度，不能在中断上下文调用
 */
extern void OsAbsTaskPauseScheduler(void);

/*
 *操作系统恢复调度，不能在中断上下文调用
 */
extern void OsAbsTaskResumeScheduler(void);

/*
 *当前任务进入临界区，不能在中断中调用
 */
extern void OsAbsEnterCriticalSection(void);

/*
 *当前任务退出临界区，不能在中断中调用
 */
extern void OsAbsExitCriticalSection(void);

/*
 *当前任务进入临界区，在中断中调用
 */
extern void OsAbsEnterCriticalSectionFromIsr(void);

/*
 *当前任务退出临界区，在中断中调用
 */
extern void OsAbsExitCriticalSectionFromIsr(void);

/*
创建一个二进制信号量
*/
extern OsAbsSemaphore OsAbsSemaphoreCreate(void);

/*
等待信号量
*/
extern uint32 OsAbsSemaphoreTake(OsAbsSemaphore semaphore, uint32 timeout);

/*
*归还一个信号量
*/
extern void OsAbsSemaphoreGive(OsAbsSemaphore semaphore);

/*
*删除信号量
*/
extern void OsAbsSemaphoreDelete(OsAbsSemaphore semaphore);

/*
*获取当前任务
*/
extern OsAbsTaskHandle OsAbsGetCurrentTaskHandle(void);

/*
 放弃cpu的占用，不能在中断中使用
 */
extern void OsAbsYield(void);

/*
*给任务一个通知事件
*/
extern void OsAbsTaskNotifyGive(OsAbsTaskHandle task);

/*
*给任务一个通知事件
*如果被通知的任务优先级高于当前被打断任务的优先级，HigherPriorityTaskWoken被职位True。
*HigherPriorityTaskWoken的初始值必须是False
*/
extern void OsAbsTaskNotifyGiveFromIsr(OsAbsTaskHandle task, BOOL *HigherPriorityTaskWoken);

/*
*暂停任务并等待通知
*@par ClearCountOnExit：等到通知后清空内部事件值
*@par timeout：超时(ms)
*ret :被标记的事件
*/
extern uint32 OsAbsTaskNotifyTake(BOOL ClearCountOnExit, uint32 timeOut);

/*
 创建一个互斥量
 */
extern OsAbsMutex OsAbsMutexCreate(void);

/*
 销毁一个互斥量
 */
extern void OsAbsMutexDestory(OsAbsMutex mutex);

/*
 等待互斥量
 */
extern uint32 OsAbsMutexTake(OsAbsMutex mutex, uint32 timeout);

/*
 *释放一个互斥量
 */
extern void OsAbsMutexGive(OsAbsMutex mutex);



/*
 *任务注册喂狗服务，注册该服务后该任务会获得一个任务id。
 *每个注册后的任务须定期喂狗：OsAbsTaskFeedDog，否则系统会在
 *看门够超时后被复位。
 */
extern uint32 OsAbsTaskFeedDogRegist(void);

/*
 *任务喂狗
 */
extern void OsAbsTaskFeedDog(uint32 taskId);

#endif
