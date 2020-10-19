#ifndef __idebug_H
#define __idebug_H

#include "types.h"


typedef enum
{
	DBG_LEV_NO = 0,
	DBG_LEV_VERBOSE,
	DBG_LEV_DEBUG,
	DBG_LEV_INFO,
	DBG_LEV_WARN,
	DBG_LEV_ERROR,
}iDebugLevelEnum;

/*LOG打印初始化*/
extern void iDebugInit(void);

/*LOG打印，不能在中断上下文调用*/
extern void iDebugPrint(iDebugLevelEnum level, const char *fmt, ...);

/**
 * 创建打印任务
 */
extern void fly_task_idebug_create(void);

#endif
