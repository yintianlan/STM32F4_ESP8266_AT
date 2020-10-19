#include "osAbstract.h"
#include "tos_k.h"
#include "cmsis_os.h"
#include "main.h"
#include "iDebug.h"

#include "fly_task_board.h"


#define		LED_ON		GPIO_PIN_RESET
#define		LED_OFF		GPIO_PIN_SET

//这个size需要注意，如果太小，会导致执行频繁，而其它任务被饿死
#define TEST_TASK_STK_SIZE				512
static void test_task(void const *pdata);
osThreadDef(test_task, osPriorityNormal, 1, TEST_TASK_STK_SIZE);

void parseServerCmd(uint8_t *data_ptr, uint32_t length){
	if (!data_ptr)
		return;

	uint8_t cmd = data_ptr[0];

	if (cmd == '0') {
		HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin, LED_OFF);//关灯
	} else
	if (cmd == '1'){
		HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin, LED_ON);//开灯
	}
}

static void test_task(void const *arg) {

	while(1){
		OsAbsSleep(500);
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
	}
}


void fly_task_board_create(void)
{
	iDebugPrint(DBG_LEV_INFO, "board task create\r\n");
	osThreadCreate(osThread(test_task), NULL);
}



