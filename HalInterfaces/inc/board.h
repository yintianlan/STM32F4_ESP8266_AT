#ifndef _iBoard_H_
#define _iBoard_H_

#include "types.h"

/**
 * 非法的电压值
 */
#define ADC_VOLTAGE_INVALID		(0xFFFF)
/**
 * 内部参考电压标准值
 */
#define VREF_VOLTAGE 		(1200)

/**
 * 可用的ADC端口
 */
typedef enum{
	ADC_1 = 0,
	ADC_2,
	ADC_3,
	ADC_4,
	ADC_CH_MAX = ADC_3,
}iBoardAdcEnumDef;

/*所有可用接口定义*/
typedef enum
{
	GPIO_1 = 0,
	GPIO_2,
	GPIO_3,
	GPIO_4,
	GPIO_5,
	GPIO_6,
	GPIO_7,
	GPIO_8,
	GPIO_9,
	GPIO_10,
	GPIO_11,
	GPIO_12,
	GPIO_13,
	GPIO_14,
	GPIO_15,
	GPIO_16,
	GPIO_17,
	GPIO_18,
	GPIO_19,
	GPIO_20,
	GPIO_21,
	GPIO_22,
	GPIO_23,
	GPIO_24,
	GPIO_25,
	GPIO_26,
	GPIO_27,
	GPIO_28,
	GPIO_29,
	GPIO_30,
	GPIO_31,
	GPIO_32,
	GPIO_33,
	GPIO_34,
	GPIO_35,
	GPIO_36,
	GPIO_37,
	GPIO_38,
	GPIO_39,
	GPIO_MAX = GPIO_39,			//GPIO索引上限，不能大于等于该索引
}iBoardIoEnumDef;

typedef enum
{
	IO_MODE_INPUT = 0,	//输入	
	IO_MODE_OUTPUT,		//输出
}iBoardIoModeDef;

typedef enum
{
	IO_PULL_NOPULL = 0,	//浮空
	IO_PULL_UP,			//上拉
	IO_PULL_DOWN,		//下拉
}iBoardPullModeDef;

typedef enum
{
	IO_MODE_IT_RISING = 0,	//上升
	IO_MODE_IT_FALLING,			//下降
	IO_MODE_IT_RISING_FALLING,		//上升和下降
}iBoardIoModeIrqDef;

/*
*输入模式下的中断回调
*@par io:io索引
*@par state:io输入状态
*/
typedef void(*inputIrqCallback)(iBoardIoEnumDef io, uint8 state);

typedef struct
{
	iBoardIoEnumDef gpio;		//IO索引
	iBoardIoModeDef mode;		//IO模式	
	iBoardPullModeDef pull;		//io上下拉配置
	BOOL irqEnable;				//输入模式下的中断使能
	iBoardIoModeIrqDef irqMode;	//中断触发模式
	
	inputIrqCallback inIrqCallback;	//输入回调
}iBoardIoCfgDef;

/**
 * 板子初始化
 */
extern void boardInit(void);

/**
 * 使能ADC
 */
extern void boardAdcEnable(void);

/**
 * 关闭ADC
 */
extern void boardAdcDisable(void);

/**
 *　获取特定通道的电压值(单位mv)
 *　0xffff为无效值
 */
extern uint16 boardAdcGetVoltage(iBoardAdcEnumDef ch);

/*
*初始化外部IO
*/
extern void boardGpioInit(iBoardIoCfgDef * ioCfg);

/*使能中断*/
extern void boardGpioIrqEnable(iBoardIoEnumDef io);

/*关闭中断*/
extern void boardGpioIrqDisable(iBoardIoEnumDef io);

/*
*控制翻转
*@par io:io
*/
extern void boardGpioTogglePin(iBoardIoEnumDef io);

/*
*控制输出
*@par io:io
*@par state:0:拉低; else:拉高
*/
extern void boardGpioOutSet(iBoardIoEnumDef io, uint8 state);

/*
*获取IO输入状态
*@par io:io
*@ret : 0:输入为低;else:输入为高
*/
extern uint8 boardGpioInputGet(iBoardIoEnumDef io);

#endif
