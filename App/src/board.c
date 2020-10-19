#include <board.h>
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "iDebug.h"

#define GPIO_PRIORITY		(7)


/**
 * GPIO内部类
 */
typedef struct {
	const GPIO_TypeDef *Port;
	const uint16_t Pin;
	const IRQn_Type irqHandleId;

	//Callback
	inputIrqCallback callBack;
//void(*callBack)(iBoardIoEnumDef io, uint8 state);
} iBoardGPIOsDef;

/*端口ID-GPIO对应表*/
iBoardGPIOsDef iBoardGPIOsMap[GPIO_MAX] = {
		//I_GPIO_1
		{ .Port = GPIOC, .Pin = GPIO_PIN_13, .irqHandleId = EXTI15_10_IRQn, },
		//I_GPIO_2
		{ .Port = GPIOA, .Pin = GPIO_PIN_4, .irqHandleId = EXTI4_IRQn, },
		//I_GPIO_3
		{ .Port = GPIOA, .Pin = GPIO_PIN_5, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_4
		{ .Port = GPIOA, .Pin = GPIO_PIN_6, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_5
		{ .Port = GPIOA, .Pin = GPIO_PIN_7, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_6
		{ .Port = GPIOC, .Pin = GPIO_PIN_4, .irqHandleId = EXTI0_IRQn, },
		//I_GPIO_7
		{ .Port = GPIOC, .Pin = GPIO_PIN_5, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_8
		{ .Port = GPIOB, .Pin = GPIO_PIN_0, .irqHandleId = EXTI0_IRQn, },
		//I_GPIO_9
		{ .Port = GPIOB, .Pin = GPIO_PIN_1, .irqHandleId = EXTI1_IRQn, },
		//I_GPIO_10
		{ .Port = GPIOB, .Pin = GPIO_PIN_2, .irqHandleId = EXTI2_IRQn, },
		//I_GPIO_11
		{ .Port = GPIOB, .Pin = GPIO_PIN_14, .irqHandleId = EXTI15_10_IRQn, },
		//I_GPIO_12
		{ .Port = GPIOB, .Pin = GPIO_PIN_15, .irqHandleId = EXTI15_10_IRQn, },
		//I_GPIO_13
		{ .Port = GPIOC, .Pin = GPIO_PIN_8, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_14
		{ .Port = GPIOC, .Pin = GPIO_PIN_9, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_15
		{ .Port = GPIOA, .Pin = GPIO_PIN_8, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_16
		{ .Port = GPIOA, .Pin = GPIO_PIN_15, .irqHandleId = EXTI15_10_IRQn, },
		//I_GPIO_17
		{ .Port = GPIOC, .Pin = GPIO_PIN_10, .irqHandleId = EXTI15_10_IRQn, },
		//I_GPIO_18
		{ .Port = GPIOC, .Pin = GPIO_PIN_11, .irqHandleId = EXTI15_10_IRQn, },
		//I_GPIO_19
		{ .Port = GPIOB, .Pin = GPIO_PIN_3, .irqHandleId = EXTI3_IRQn, },
		//I_GPIO_20
		{ .Port = GPIOB, .Pin = GPIO_PIN_4, .irqHandleId = EXTI4_IRQn, },
		//I_GPIO_21
		{ .Port = GPIOB, .Pin = GPIO_PIN_5, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_22
		{ .Port = GPIOB, .Pin = GPIO_PIN_8, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_23
		{ .Port = GPIOB, .Pin = GPIO_PIN_9, .irqHandleId = EXTI9_5_IRQn, },
		//I_GPIO_24
		{ .Port = GPIOC, .Pin = GPIO_PIN_0, .irqHandleId = EXTI0_IRQn, },
		//I_GPIO_25
		{ .Port = GPIOC, .Pin = GPIO_PIN_1, .irqHandleId = EXTI1_IRQn, },
		//I_GPIO_26
		{ .Port = GPIOC, .Pin = GPIO_PIN_2, .irqHandleId = EXTI2_IRQn, },
		//I_GPIO_27
		{ .Port = GPIOC, .Pin = GPIO_PIN_3, .irqHandleId = EXTI3_IRQn, },
};

/******************************local方法**********************************/
static iBoardGPIOsDef* FindBoardGPIOsMapItem(iBoardIoEnumDef io) {
	if (io > GPIO_MAX)
		return NULL;

	return &iBoardGPIOsMap[io];
}

static void enablePortClock(GPIO_TypeDef *GPIOx) {
	if (GPIOx == GPIOA) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
	} else if (GPIOx == GPIOB) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
	} else if (GPIOx == GPIOC) {
		__HAL_RCC_GPIOC_CLK_ENABLE();
	} else if (GPIOx == GPIOD) {
		__HAL_RCC_GPIOD_CLK_ENABLE();
	} else if (GPIOx == GPIOE) {
		__HAL_RCC_GPIOE_CLK_ENABLE();
	} else if (GPIOx == GPIOF) {
		__HAL_RCC_GPIOF_CLK_ENABLE();
	} else if (GPIOx == GPIOG) {
		__HAL_RCC_GPIOG_CLK_ENABLE();
	} else if (GPIOx == GPIOH) {
		__HAL_RCC_GPIOH_CLK_ENABLE();
	} else if (GPIOx == GPIOI) {
		__HAL_RCC_GPIOI_CLK_ENABLE();
	} 
}

static BOOL IsCurrentPortPinIRQ(GPIO_TypeDef *GPIOx, uint16_t iBoardPin) {
	BOOL result = False;
	uint32_t position = 0x00U;
	uint32_t tmp = 0x00U;

	/* Get the current EXTI IO position */
	position = iBoardPin;

	/* Get the SYSCFG external interrupt configuration registers*/
	tmp = SYSCFG->EXTICR[position >> 2U];
	tmp &= ((0x0FU) << (4U * (position & 0x03U)));
	if (tmp == (GPIO_GET_INDEX(GPIOx) << (4U * (position & 0x03U)))) {
		result = True;
	}

	return result;
}

static void DoExtiIrqHandler(uint16_t extiPin) {
	uint8_t id;

	//遍历查找匹配的PIN
	for (id = 0; id < ARRY_LEN(iBoardGPIOsMap, iBoardGPIOsDef); id++) {
		//找到对应的PIN
		if (iBoardGPIOsMap[id].Pin == extiPin) {
			//是否是配置的PORT
			if (IsCurrentPortPinIRQ((GPIO_TypeDef*) iBoardGPIOsMap[id].Port,
					iBoardGPIOsMap[id].Pin) == True) {
				/* EXTI line interrupt detected */
				if (__HAL_GPIO_EXTI_GET_IT(extiPin) != RESET) {
					//清中断
					__HAL_GPIO_EXTI_CLEAR_IT(extiPin);

					//中断回调函数
					if (iBoardGPIOsMap[id].callBack != NULL) {
						uint8_t state;

						state = boardGpioInputGet((iBoardIoEnumDef) id);
						iBoardGPIOsMap[id].callBack((iBoardIoEnumDef) id,
								state);
					}
				}
			}
		}
	}

}

/******************************IRQHandler方法**********************************/
void EXTI0_IRQHandler(void) {
	DoExtiIrqHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void) {
	DoExtiIrqHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void) {
	DoExtiIrqHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void) {
	DoExtiIrqHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void) {
	DoExtiIrqHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void) {
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_5);
	}
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_6);
	}
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_7);
	}
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_8);
	}
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_9);
	}
}

void EXTI15_10_IRQHandler(void) {
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_10);
	}
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_11);
	}
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_12);
	}
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_13);
	}
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_14);
	}
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET) {
		DoExtiIrqHandler(GPIO_PIN_15);
	}
}

/******************************pulic方法**********************************/
void boardGpioInit(iBoardIoCfgDef *ioCfg) {
	uint8_t id;
	BOOL InitFlag = False;

	if (ioCfg) {
		for (id = 0; id < ARRY_LEN(iBoardGPIOsMap, iBoardGPIOsDef); id++) {
			if (id == ioCfg->gpio) {
				InitFlag = True;
				break;
			}
		}
	} else {
		iDebugPrint(DBG_LEV_ERROR, "iBoard IO construct null\r\n");
	}

	if (InitFlag) {
		/* GPIO Ports Clock Enable */
		enablePortClock((GPIO_TypeDef*) iBoardGPIOsMap[id].Port);

		GPIO_InitTypeDef GPIO_InitStruct = { 0 };

		/*Configure GPIO pin : PxPin */
		GPIO_InitStruct.Pin = iBoardGPIOsMap[id].Pin;

		if (ioCfg->irqEnable == False) {
			//gpio mode
			if (ioCfg->mode == IO_MODE_INPUT) {
				GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			} else {
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
			}
		} else {
			//gpio irq mode
			if (ioCfg->irqMode == IO_MODE_IT_RISING) {
				GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
			} else if (ioCfg->irqMode == IO_MODE_IT_FALLING) {
				GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
			} else {
				GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
			}
		}

		//gpio pull
		if (ioCfg->pull == IO_PULL_NOPULL) {
			GPIO_InitStruct.Pull = GPIO_NOPULL;
		} else if (ioCfg->pull == IO_PULL_UP) {
			GPIO_InitStruct.Pull = GPIO_PULLUP;
		} else {
			GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		}

		//gpio speed
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init((GPIO_TypeDef*) iBoardGPIOsMap[id].Port,
				&GPIO_InitStruct);

		if (ioCfg->irqEnable == True) {
			iBoardGPIOsMap[id].callBack = ioCfg->inIrqCallback;
		}

		InitFlag = False;
	}
}

void boardGpioIrqEnable(iBoardIoEnumDef io) {
	if (io > GPIO_MAX)
		return;

	iBoardGPIOsDef *pIO = FindBoardGPIOsMapItem(io);

	uint32_t position = 0x00U;
	uint32_t iocurrent = 0x00U;
	uint32_t temp = 0x00U;

	if (pIO) {
		/* Get the current IO position */
		iocurrent = position = (uint32_t) (pIO->Pin);

		/*--------------------- EXTI Mode Configuration ------------------------*/
		/* Configure the External Interrupt or event for the current IO */
		/* Enable SYSCFG Clock */
		__HAL_RCC_SYSCFG_CLK_ENABLE();

		temp = SYSCFG->EXTICR[position >> 2U];
		temp &= ~(0x0FU << (4U * (position & 0x03)));
		temp |= ((uint32_t) (GPIO_GET_INDEX(pIO->Port))
				<< (4U * (position & 0x03U)));
		SYSCFG->EXTICR[position >> 2U] = temp;

		/* Enable EXTI_x event */
		EXTI->IMR |= iocurrent;

		/* Sets the priority of an interrupt */
		HAL_NVIC_SetPriority(pIO->irqHandleId, GPIO_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(pIO->irqHandleId);

	}
}

void boardGpioIrqDisable(iBoardIoEnumDef io) {
	if (io > GPIO_MAX)
		return;

	iBoardGPIOsDef *pIO = FindBoardGPIOsMapItem(io);

	uint32_t position = 0x00U;
	uint32_t iocurrent = 0x00U;
	uint32_t tmp = 0x00U;

	if (pIO) {
		/* Get the current IO position */
		iocurrent = position = (uint32_t) (pIO->Pin);

		/*------------------------- EXTI Mode Configuration --------------------*/
		/* Clear the External Interrupt or Event for the current IO */
		tmp = SYSCFG->EXTICR[position >> 2U];
		tmp &= ((0x0FU) << (4U * (position & 0x03U)));
		if (tmp == (GPIO_GET_INDEX(pIO->Port) << (4U * (position & 0x03U)))) {
			tmp = (0x0FU) << (4U * (position & 0x03U));
			CLEAR_BIT(SYSCFG->EXTICR[position >> 2U], tmp);

			/* Clear EXTI line configuration */
			CLEAR_BIT(EXTI->IMR, (uint32_t )iocurrent);
		}

		/* Disable the NVIC interrupt */
		HAL_NVIC_DisableIRQ(pIO->irqHandleId);

	}
}

void boardGpioOutSet(iBoardIoEnumDef io, uint8 state) {
	if (io > GPIO_MAX)
		return;

	iBoardGPIOsDef *pIO = FindBoardGPIOsMapItem(io);

	if (pIO) {
		HAL_GPIO_WritePin((GPIO_TypeDef*) pIO->Port, pIO->Pin,
				(GPIO_PinState) state);
	}
}

uint8 boardGpioInputGet(iBoardIoEnumDef io) {
	uint8 res = 0;
	if (io > GPIO_MAX)
		return 0;

	iBoardGPIOsDef *pIO = FindBoardGPIOsMapItem(io);

	if (pIO) {
		res = HAL_GPIO_ReadPin((GPIO_TypeDef*) pIO->Port, pIO->Pin);
	}

	return res;
}

void boardInit(void) {

}
