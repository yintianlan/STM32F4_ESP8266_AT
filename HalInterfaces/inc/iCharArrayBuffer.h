#ifndef __iCharArrayBuffer_H
#define __iCharArrayBuffer_H

#include "types.h"

//Buffer定义
typedef struct
{
	uint32 head;		//buffer头
	uint32 tail;		//buffer尾
	uint32 length;		//长度

	void * data;		//数据池首地址
}charArrayBufferDef;

/*
*对队列进行初始化操作(将队列的head和tail指针置为0)
*@par ibuffer：buffer对象
*/
extern void ICharArrayInitEmpty(charArrayBufferDef* const ibuffer);

/*
*将数据放入队列
*@par ibuffer：buffer对象
*@par src：待存放的数据地址
*/
extern void ICharArrayPutOne(charArrayBufferDef* const ibuffer, uint8 data);

/*
*测试是否存在数据在buffer中
*@par ibuffer：buffer对象
*@par True if test ok.
*/
extern BOOL ICharArrayHaveNext(charArrayBufferDef* const ibuffer);

/*
*从队列中取出一个元素
*@par ibuffer：buffer对象
*@par True if ok.
*/
extern BOOL ICharArrayGetNextOne(charArrayBufferDef* const ibuffer, uint8 * pData);

/**
 *批量获取数据
 */
extern uint32 ICharArrayGetMany(charArrayBufferDef* const ibuffer, const uint8 *dest, uint32 size);

/*
*中断处理时将数据放入队列
*@par ibuffer：buffer对象
*@par src：待存放的数据地址
*/
extern void ICharArrayPutOneFromIsr(charArrayBufferDef* const ibuffer, uint8 data);

#endif
