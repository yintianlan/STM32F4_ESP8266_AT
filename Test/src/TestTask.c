/*
 * TestTask.c
 *
 *  Created on: 2020年8月19日
 *      Author: LinQian
 */
#include "osAbstract.h"
#include "tos_k.h"
#include "cmsis_os.h"
#include "main.h"

#include "TestTask.h"

#if 0

//这个size需要注意，如果太小，会导致执行频繁，而其它任务被饿死
#define TEST_TASK_STK_SIZE				512
static void test_task(void const *pdata);
osThreadDef(test_task, osPriorityNormal, 1, TEST_TASK_STK_SIZE);

void test_task(void const *arg) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	while(1){
		OsAbsSleep(200);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
		OsAbsSleep(300);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	}
}


void test_task_create(void)
{
	osThreadCreate(osThread(test_task), NULL);
}

#endif
