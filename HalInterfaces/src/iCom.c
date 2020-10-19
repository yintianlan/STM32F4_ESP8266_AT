#include <stdlib.h>

#include "usart.h"
#include "iCom.h"
#include "osAbstract.h"
#include "iCharArrayBuffer.h"
#include "iDebug.h"

#include "cmsis_os.h"
#include "tos_k.h"
#include "tos_at.h"

#define MAIN_TASK_STK_SIZE              1024
void iComDistributeTask(void *pdata);
osThreadDef(iComDistributeTask, osPriorityNormal, 1, MAIN_TASK_STK_SIZE);


#define USART_PRIORITY_BASE			(5)
#define ItemLen		(sizeof(iComInnerObjects)/sizeof(iComInnerObjectDef))

typedef UART_HandleTypeDef IComUartHandleTypedef;

/**
 * 外部变量
 */
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

/**
 * DMA发送变量
 */
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_tx;
DMA_HandleTypeDef hdma_usart6_tx;
/*************************Local variables********************************/

typedef struct {
	iComBitrateDef bitrateName;
	uint32 bitrateValue;
} bitrateMapDef;

/*波特率对应表*/
const bitrateMapDef biterateMap[] = {
	{ .bitrateName = COM_BITRATE_9600,	.bitrateValue = 9600, },
	{ .bitrateName = COM_BITRATE_19200, .bitrateValue = 19200, },
	{ .bitrateName = COM_BITRATE_38400, .bitrateValue = 38400, },
	{ .bitrateName = COM_BITRATE_57600, .bitrateValue = 57600, },
	{ .bitrateName = COM_BITRATE_115200, .bitrateValue = 115200, },
	{ .bitrateName = COM_BITRATE_230400, .bitrateValue = 230400, },
	{ .bitrateName = COM_BITRATE_460800, .bitrateValue = 460800, },
	{ .bitrateName = COM_BITRATE_921600, .bitrateValue = 921600, },
};

typedef struct {
	BOOL Inited;
	iComIdDef comId;
	uint8 rxByte;
	uint8 txByte;
//	uint8 xferUseInterrupt; //1:用中断方式发送数据 0:不适用中断发送
	uint8 xferUseMode; // 0:普通发送 ,1:用中断方式发送数据,2:DMA方式发送数据
	uint32 errorCode;
	uint32 priority;		//串口优先级;(基础优先级+偏移优先级);数值越小优先级越高

	OsAbsMutex mutexTxLock;
	k_sem_t semTxDone;

	charArrayBufferDef buffer;
	const IRQn_Type irqHandleId;
	USART_TypeDef *uartInstance;
	IComUartHandleTypedef *comHandle;

	DMA_Stream_TypeDef *dma_stream;
	uint32_t dma_channel;
	DMA_HandleTypeDef *dma_usart_tx;
	const IRQn_Type dma_irqHandleId;

	//Callback
	void (*GotOneFrameFromIrqCallback)(void);
} iComInnerObjectDef;

/*串口ID-UART对应表*/
iComInnerObjectDef iComInnerObjects[] = {
	//COM0
	{
		.comId = COM_0,
		.irqHandleId = USART1_IRQn,
		.uartInstance = USART1,
		.comHandle = &huart1,
		.dma_stream = DMA2_Stream7,
		.dma_channel = DMA_CHANNEL_4,
		.dma_usart_tx = &hdma_usart1_tx,
		.dma_irqHandleId = DMA2_Stream7_IRQn,
		.mutexTxLock = NULL
	},

	//COM1
	{
		.comId = COM_1,
		.irqHandleId = USART2_IRQn,
		.uartInstance = USART2,
		.comHandle = &huart2,
		.dma_stream = DMA1_Stream6,
		.dma_channel = DMA_CHANNEL_4,
		.dma_usart_tx = &hdma_usart2_tx,
		.dma_irqHandleId = DMA1_Stream6_IRQn,
		.mutexTxLock = NULL
	},

	//COM2
	{
		.comId = COM_2,
		.irqHandleId = USART3_IRQn,
		.uartInstance = USART3,
		.comHandle = &huart3,
		.dma_stream = DMA1_Stream3,
		.dma_channel = DMA_CHANNEL_4,
		.dma_usart_tx = &hdma_usart3_tx,
		.dma_irqHandleId = DMA1_Stream3_IRQn,
		.mutexTxLock = NULL
	},

	//COM3
	{
		.comId = COM_3,
		.irqHandleId = USART6_IRQn,
		.uartInstance = USART6,
		.comHandle = &huart6,
		.dma_stream = DMA2_Stream6,
		.dma_channel = DMA_CHANNEL_5,
		.dma_usart_tx = &hdma_usart6_tx,
		.dma_irqHandleId = DMA2_Stream6_IRQn,
		.mutexTxLock = NULL
	},
};

static uint8 totalObject_Item = 0;

static void UartStartReceive(const iComInnerObjectDef *com);
static void clearFlags(UART_HandleTypeDef *huart);
/*************************local methods********************************/
/*串口 发送完成回调*/
void IComDriverXferDoneCallbackIrq(UART_HandleTypeDef *huart) {
	uint32 id;

	for (id = 0; id < totalObject_Item; id++) {
		if (iComInnerObjects[id].comHandle == huart) {
			tos_sem_post(&iComInnerObjects[id].semTxDone);
			break;
		}
	}
}
void IComUartErrorCallbackIrq(UART_HandleTypeDef *huart) {
	//Find instance first
	uint16 id;

	for (id = 0; id < totalObject_Item; id++) {
		if (iComInnerObjects[id].comHandle == huart) {
			//Enabel receive
			if (huart->ErrorCode) {
				iComInnerObjects[id].errorCode = huart->ErrorCode;
			}

			if (__HAL_UART_GET_FLAG(huart, USART_SR_PE)) {
				//Parity Error
				__HAL_UART_CLEAR_PEFLAG(huart);
			}

			if (__HAL_UART_GET_FLAG(huart, USART_SR_FE)) {
				//Framing Error
				__HAL_UART_CLEAR_FEFLAG(huart);
			}

			if (__HAL_UART_GET_FLAG(huart, USART_SR_NE)) {
				//Noise Error Flag
				__HAL_UART_CLEAR_NEFLAG(huart);
			}

			if (__HAL_UART_GET_FLAG(huart, USART_SR_ORE)) {
				//溢出中断
				__HAL_UART_CLEAR_OREFLAG(huart);
			}

			UartStartReceive(&iComInnerObjects[id]);
			break;
		}
	}
}

static uint32_t totalCount1;
static uint32_t totalCount2;
static uint32_t totalCount3;
void __inline__ IComUartRxCallbackIrq(UART_HandleTypeDef *huart) {
	if(huart == &huart3){
		totalCount1++;
		ICharArrayPutOneFromIsr(&iComInnerObjects[2].buffer, *(uint8 *)(--huart->pRxBuffPtr));
	}
	else if(huart == &huart2){
		totalCount2++;
		ICharArrayPutOneFromIsr(&iComInnerObjects[1].buffer, *(uint8 *)(--huart->pRxBuffPtr));
	}
	else if(huart == &huart6){
		totalCount3++;
		ICharArrayPutOneFromIsr(&iComInnerObjects[3].buffer, *(uint8 *)(--huart->pRxBuffPtr));
	}

    huart->RxXferSize = 1;
    huart->RxXferCount = 1;
    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->RxState = HAL_UART_STATE_BUSY_RX;

    /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
    SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

    /* Enable the UART Parity Error and Data Register not empty Interrupts */
    SET_BIT(huart->Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);
}

static iComInnerObjectDef* FindIComObject(const iComObjectDef *iCom) {
	uint16 id;

	for (id = 0; id < totalObject_Item; id++) {
		if (iComInnerObjects[id].comId == iCom->com) {
			return &iComInnerObjects[id];
		}
	}

	return NULL;
}

static void UartStartReceive(const iComInnerObjectDef *com) {
	if (com) {
		HAL_UART_Receive_IT((UART_HandleTypeDef*) com->comHandle,
				(uint8*) &com->rxByte, 1);
		//选择中断源，抢占优先级，响应优先级
//		HAL_NVIC_SetPriority(com->irqHandleId, com->priority, 0);
		//使能中断
		NVIC_EnableIRQ(com->irqHandleId);
	}
}

/*************************Pulic methods********************************/

iComStatus IComConstruct(const iComObjectDef *iCom) {
	iComStatus res = ICOM_ERROR;
	uint16 id = 0;
	uint32 bitrate = 0xffffffff;

	totalObject_Item = ARRY_LEN(iComInnerObjects, iComInnerObjectDef);

	if (iCom) {
		for (id = 0; id < ARRY_LEN(biterateMap, bitrateMapDef); id++) {
			if (biterateMap[id].bitrateName == iCom->bitrate) {
				//Found prescaler
				bitrate = biterateMap[id].bitrateValue;
				break;
			}
		}

		if (id >= ARRY_LEN(biterateMap, bitrateMapDef)) {
			iDebugPrint(DBG_LEV_ERROR, "iCom bitrate not found: %d\r\n",
					iCom->bitrate);
		}
	} else {
		iDebugPrint(DBG_LEV_ERROR, "iCom construct null error\r\n");
	}

	if (bitrate != 0xffffffff) {
		for (id = 0; id < totalObject_Item; id++) {
			if (iCom->com == iComInnerObjects[id].comId
					&& iComInnerObjects[id].Inited != True) {
				iComInnerObjectDef *pObject = &iComInnerObjects[id];

				pObject->GotOneFrameFromIrqCallback = iCom->GotFrameFromIrq;

//				pObject->xferUseInterrupt =
//						iCom->xferMode == COM_XFER_INTERRUPT ? 1 : 0;
				pObject->xferUseMode = iCom->xferMode ;

				if (K_ERR_NONE != tos_sem_create(&pObject->semTxDone, 0)) {
					iDebugPrint(DBG_LEV_ERROR, "TxDone sem err\r\n");
					res = ICOM_ERROR;
					goto ICOM_ERR_MUTEX;
				}

				pObject->mutexTxLock = OsAbsMutexCreate();
				if (pObject->mutexTxLock == NULL) {
					OsAbsMutexDestory(pObject->mutexTxLock);
					res = ICOM_MUTEX_CREATE_ERROR;
					goto ICOM_ERR_MUTEX;
				}

				//Init buffer
				pObject->buffer.length = iCom->bufferLength;
				pObject->buffer.data = OsAbsMalloc(pObject->buffer.length);
				pObject->buffer.head = 0;
				pObject->buffer.tail = 0;
				if (pObject->buffer.data == NULL) {
					iDebugPrint(DBG_LEV_WARN,
							"iCom rx fifo null with com: %d\r\n", id);
				}

				pObject->priority = iCom->priority + USART_PRIORITY_BASE;
				pObject->priority &= 0xffff;
				if(pObject->priority <= USART_PRIORITY_BASE){
					//优先级过高,使用默认优先级
					pObject->priority = USART_PRIORITY_BASE;
				}

				pObject->comHandle->Instance = pObject->uartInstance;
				pObject->comHandle->Init.BaudRate = bitrate;
				pObject->comHandle->Init.WordLength = UART_WORDLENGTH_8B;
				pObject->comHandle->Init.StopBits = UART_STOPBITS_1;
				pObject->comHandle->Init.Parity = UART_PARITY_NONE;
				pObject->comHandle->Init.Mode = UART_MODE_TX_RX;
				pObject->comHandle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
				pObject->comHandle->Init.OverSampling = UART_OVERSAMPLING_16;

				pObject->comHandle->pRxBuffPtr = &pObject->rxByte;
				pObject->comHandle->RxXferSize =
						pObject->comHandle->RxXferCount = 1;
				pObject->comHandle->pTxBuffPtr = &pObject->txByte;
				pObject->comHandle->TxXferSize =
						pObject->comHandle->TxXferCount = 1;

				if (HAL_UART_Init(pObject->comHandle) == HAL_OK) {
					res = ICOM_SUCCESS;

					if(pObject->xferUseMode == COM_XFER_INTERRUPT){
						//选择中断源，抢占优先级，响应优先级
						HAL_NVIC_SetPriority(pObject->irqHandleId, pObject->priority, 0);
						//使能中断
						NVIC_EnableIRQ(pObject->irqHandleId);
					} else
					if(pObject->xferUseMode == COM_XFER_DMA){
						/* DMA controller clock enable */
						//一定要先开启时钟再初始化
						__HAL_RCC_DMA2_CLK_ENABLE();
						__HAL_RCC_DMA1_CLK_ENABLE();
						/* DMA_Stream_IRQn interrupt configuration */
						HAL_NVIC_SetPriority(pObject->dma_irqHandleId, pObject->priority, 0);
						HAL_NVIC_EnableIRQ(pObject->dma_irqHandleId);

						/* USART_TX Init */
						pObject->dma_usart_tx->Instance = pObject->dma_stream;
						pObject->dma_usart_tx->Init.Channel = pObject->dma_channel;
						pObject->dma_usart_tx->Init.Direction = DMA_MEMORY_TO_PERIPH;
						pObject->dma_usart_tx->Init.PeriphInc = DMA_PINC_DISABLE;
						pObject->dma_usart_tx->Init.MemInc = DMA_MINC_ENABLE;
						pObject->dma_usart_tx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
						pObject->dma_usart_tx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
						pObject->dma_usart_tx->Init.Mode = DMA_NORMAL;
						pObject->dma_usart_tx->Init.Priority = pObject->priority == USART_PRIORITY_BASE ?
								DMA_PRIORITY_VERY_HIGH : DMA_PRIORITY_MEDIUM;
						pObject->dma_usart_tx->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
						if (HAL_DMA_Init(pObject->dma_usart_tx) != HAL_OK) {
							iDebugPrint(DBG_LEV_WARN,
									"iCom tx dma init err, com: %d\r\n", id);
						}

//						__HAL_LINKDMA(pObject->comHandle, hdmatx, hdma_usart6_tx);
						//这个循环等同于上面那句的功能
					    do{
							 pObject->comHandle->hdmatx = pObject->dma_usart_tx;
							 pObject->dma_usart_tx->Parent = pObject->comHandle;
						 } while(0U);
					}

				} else {
					iDebugPrint(DBG_LEV_ERROR,
							"iCom HAL_init failed with com: %d\r\n", id);
				}

				break;
			}
		}

		if (id >= totalObject_Item) {
			iDebugPrint(DBG_LEV_ERROR, "iCom port not found: %d\r\n",
					iCom->com);
		}
	}
	//互斥量创建错误
	ICOM_ERR_MUTEX:

	return res;
}

iComStatus IComDestory(const iComObjectDef *iCom) {
	if (iCom == NULL)
		return ICOM_ERROR;

	iComInnerObjectDef *com = FindIComObject(iCom);

	if (com == NULL) {
		return ICOM_ERROR;
	}

	NVIC_DisableIRQ(com->irqHandleId);
	OsAbsFree(com->buffer.data);
	HAL_UART_DeInit(com->comHandle);
	com->GotOneFrameFromIrqCallback = NULL;
	com->Inited = False;

	return ICOM_SUCCESS;
}

void IComStart(const iComObjectDef *iCom) {
	if (iCom == NULL)
		return;

	iComInnerObjectDef *com = FindIComObject(iCom);

	clearFlags(com->comHandle);

	if (com) {
		HAL_UART_Receive_IT((UART_HandleTypeDef*) com->comHandle,
				(uint8*) &com->rxByte, 1);
		//选择中断源，抢占优先级，响应优先级
		HAL_NVIC_SetPriority(com->irqHandleId, com->priority, 0);
		//使能中断
		NVIC_EnableIRQ(com->irqHandleId);
	} else {
		iDebugPrint(DBG_LEV_ERROR,
				"iCom start null error in IComStart with com: %d\r\n",
				iCom->com);
	}
}

void IComStop(const iComObjectDef *iCom) {
	if (iCom == NULL)
		return;

	iComInnerObjectDef *com = FindIComObject(iCom);

	if (com) {
		__HAL_UART_DISABLE_IT(com->comHandle, UART_IT_RXNE);
		NVIC_DisableIRQ(com->irqHandleId);
	} else {
		iDebugPrint(DBG_LEV_ERROR,
				"iCom stop null error in IComStop with com: %d\r\n", iCom->com);
	}
}

uint32 IComGetErrorCode(const iComObjectDef *iCom) {
	if (iCom != NULL) {
		iComInnerObjectDef *com = FindIComObject(iCom);

		if (com != NULL) {
			return com->errorCode;
		}
	}

	return 0;
}

void IComClearErrorCode(const iComObjectDef *iCom) {
	if (iCom != NULL) {
		iComInnerObjectDef *com = FindIComObject(iCom);

		if (com != NULL) {
			com->errorCode = 0;
		}
	}
}

iComStatus IComTransmit(const iComObjectDef *iCom, const uint8 *data,
		uint16 size) {
	iComStatus res = ICOM_ERROR;
	int tryCount = 3;
	uint32_t timeout = 10;
	k_err_t err = K_ERR_NONE;
	timeout = 10 * size + 1000;

	if (iCom != NULL) {
		iComInnerObjectDef *com = FindIComObject(iCom);

		if (com) {
			if (OS_ABS_ERR_NONE == OsAbsMutexTake(com->mutexTxLock, OsAbsMaxDelay)) {
				if (com->xferUseMode == COM_XFER_DMA) {
					//DMA发送模式
					if (HAL_OK != HAL_UART_Transmit_DMA(com->comHandle, (uint8*) data, size)) {
						OsAbsSleep(timeout);
					} else {
						res = ICOM_SUCCESS;
					}

					//Wait tx complete
					err = tos_sem_pend(&com->semTxDone, tos_millisec2tick(timeout));
					if (K_ERR_NONE == err) {
						res = ICOM_SUCCESS;
					}
					else {
						res = ICOM_ERROR;
					}
				} else
				if (com->xferUseMode == COM_XFER_INTERRUPT) {
					while (HAL_OK != HAL_UART_Transmit_IT(com->comHandle, (uint8*) data, size)
							&& tryCount-- > 0) {
						//如果之前使用了互斥量，这里不推荐使用TencentOS的等待函数，因为tos_task_delay等待函数会阻塞线程
						//如果使用了，等待超时时间尽可能的设小值
						//OsAbsDelay(timeout);
						OsAbsSleep(timeout);
					}

					//Wait tx complete
					err = tos_sem_pend(&com->semTxDone, tos_millisec2tick(timeout));
					if (K_ERR_NONE == err) {
						res = ICOM_SUCCESS;
					}
					else {
						res = ICOM_ERROR;
					}
				} else {
					//普通发送模式，超时等待时间可能需要长一点
					if (HAL_OK!= HAL_UART_Transmit(com->comHandle, (uint8*) data, size, timeout)) {
						res =ICOM_ERROR;
					} else {
						res = ICOM_SUCCESS;
					}
				}

				OsAbsMutexGive(com->mutexTxLock);
			}
		}//end if(com)
	}

	return res;
}

void IComPushOneDataToArray(const iComObjectDef *iCom, uint8 data) {
	if (iCom != NULL) {
		iComInnerObjectDef *com = FindIComObject(iCom);

		if (com) {
			ICharArrayPutOne(&com->buffer, data);
		}
	}
}

uint32 IComGetData(const iComObjectDef *iCom, const uint8 *dest, uint32 size) {
	uint32 dataRead = 0;

	if (size == 0)
		return 0;

	uint8 *pDest = (uint8*) dest;

	if (iCom != NULL) {
		iComInnerObjectDef *com = FindIComObject(iCom);

		dataRead = ICharArrayGetMany(&com->buffer, pDest, size);
	}

	return dataRead;
}

BOOL IComIsEmpty(const iComObjectDef *iCom) {
	BOOL res = False;
	if(!iCom)return res;

	iComInnerObjectDef *com = FindIComObject(iCom);
	if(!com)return res;
	return !ICharArrayHaveNext(&com->buffer);
}

static void clearFlags(UART_HandleTypeDef *huart){
	if (__HAL_UART_GET_FLAG(huart, USART_SR_PE)) {
		//Parity Error
		__HAL_UART_CLEAR_PEFLAG(huart);
	}

	if (__HAL_UART_GET_FLAG(huart, USART_SR_FE)) {
		//Framing Error
		__HAL_UART_CLEAR_FEFLAG(huart);
	}

	if (__HAL_UART_GET_FLAG(huart, USART_SR_NE)) {
		//Noise Error Flag
		__HAL_UART_CLEAR_NEFLAG(huart);
	}

	if (__HAL_UART_GET_FLAG(huart, USART_SR_ORE)) {
		//溢出中断
		__HAL_UART_CLEAR_OREFLAG(huart);
	}
}

void __inline__ IComIrqPostProcess(UART_HandleTypeDef *huart){
	uint16 id = 0;

	for (id = 0; id < totalObject_Item; id++) {
		if (iComInnerObjects[id].comHandle == huart) {
			clearFlags(huart);
		}
	}
}

void iComDistributeTask(void *arg) {
	while(1){
		OsAbsSleep(100);

		if (1) {
			uint16 id;

			for (id = 0; id < totalObject_Item; id++) {
				//Callback
				if (iComInnerObjects[id].GotOneFrameFromIrqCallback != NULL) {
					iComInnerObjects[id].GotOneFrameFromIrqCallback();
				}
			}
		}
	}
}


void IComInit(){
	 osThreadCreate(osThread(iComDistributeTask), NULL);
}
