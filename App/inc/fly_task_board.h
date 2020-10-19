#ifndef __fly_task_board_H
#define __fly_task_board_H

#include "tos_k.h"



/*
 * 创建任务
 */
extern void fly_task_board_create(void);

extern void parseServerCmd(uint8_t *data_ptr, uint32_t length);


#endif

