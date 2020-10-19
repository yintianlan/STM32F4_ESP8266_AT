#ifndef __icom_H
#define __icom_H


#include "types.h"
#include "stm32f4xx_hal.h"


typedef enum
{
	ICOM_SUCCESS = 0,
	ICOM_ERROR,
	ICOM_MUTEX_CREATE_ERROR,
}iComStatus;

typedef enum
{
	COM_0 = 0,
	COM_1,
	COM_2,
	COM_3,
	COM_4G = COM_2,
}iComIdDef;

typedef enum
{
	COM_BITRATE_9600 = 0,
	COM_BITRATE_19200,
	COM_BITRATE_38400,
	COM_BITRATE_57600,
	COM_BITRATE_115200,
	COM_BITRATE_230400,
	COM_BITRATE_460800,
	COM_BITRATE_921600,
}iComBitrateDef;

typedef enum{
	COM_XFER_STANDARD = 0, //中断发送
	COM_XFER_INTERRUPT,		//普通发送
	COM_XFER_DMA,	//DMA发送
}iComXferMode;

typedef struct
{
	//串口号
	iComIdDef com;

  //发送模式
	iComXferMode xferMode;

	//波特率
	iComBitrateDef bitrate;

	//相对优先级(相对于串口列表)
	uint32 priority;

	//buffer长度(字节)
	uint32 bufferLength;

	/*收到一帧数据，中断上下文执行*/
	void (*GotFrameFromIrq)(void);
}iComObjectDef;

/*
*构建一个ICom接口对象,这个方法应该首先被调用
*/
extern iComStatus IComConstruct(const iComObjectDef* iCom);

/**
 * 销毁一个ICom对象并释放相关的资源
 */
extern iComStatus IComDestory(const iComObjectDef* iCom);
/*
*开始COM接收
*/
extern void IComStart(const iComObjectDef* iCom);

/*
*停止COM的动作
*/
extern void IComStop(const iComObjectDef* iCom);

/*
*获取串口错误码
*/
extern uint32 IComGetErrorCode(const iComObjectDef* iCom);

/*
*清除错误码
*/
extern void IComClearErrorCode(const iComObjectDef* iCom);

/*
*发送一帧数据至串口
*@par *iCom：COM对象引用
*@par *data：数据地址
*@par *size：数据大小
*@ret 成功返回ICOM_SUCCESS；
*/
extern iComStatus IComTransmit(const iComObjectDef* iCom, const uint8 * data, uint16 size);

/*
*获取缓存中数据
*@par *iCom：COM对象引用
*@par dest:数据待存放的位置（容器）
*@par size:容器的大小
*ret:读取到的字节数(小于等于容器大小)
*/
extern uint32 IComGetData(const iComObjectDef* iCom, const uint8 * dest, uint32 size);

/**
 * 往缓冲区中放一个数据
 */
extern void IComPushOneDataToArray(const iComObjectDef *iCom, uint8 data);

/*
*接收到一个串口字节后的回调,由STM32库调用
*/
extern void IComUartRxCallbackIrq(UART_HandleTypeDef *huart);

/*
*串口错误中断回调，由STM32库调用
*/
extern void IComUartErrorCallbackIrq(UART_HandleTypeDef *huart);

/**
 * 串口发送完成回调
 */
extern void IComDriverXferDoneCallbackIrq(UART_HandleTypeDef *huart);

/**
 * Lcom的后置处理
 */
extern void IComIrqPostProcess(UART_HandleTypeDef *huart);

/**
 * 做初始化
 */
extern void IComInit();

extern BOOL IComIsEmpty(const iComObjectDef *iCom);

#endif
