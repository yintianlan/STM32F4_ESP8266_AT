#include <stdlib.h>
#include <string.h>

#include "iCharArrayBuffer.h"
#include "osAbstract.h"


void ICharArrayInitEmpty(charArrayBufferDef* const ibuffer)
{
	OsAbsEnterCriticalSection();

	if(ibuffer)
	{
		ibuffer->head = ibuffer->tail = 0;
	}

	OsAbsExitCriticalSection();
}

void ICharArrayPutOne(charArrayBufferDef* const ibuffer, uint8 data)
{
	if(ibuffer != NULL)
	{
		OsAbsEnterCriticalSection();

		//Copy data to buffer
		//这里void*类型data变量进行了位移，有些编译器不知道你一个位移是多大，需要强制转换一下
		*((uint8 *)((uint32)ibuffer->data + ibuffer->tail++)) = data;

		if(ibuffer->tail >= ibuffer->length)
		{
			ibuffer->tail = 0;
		}

		if(ibuffer->head == ibuffer->tail)
		{
			ibuffer->head++;
			if(ibuffer->head >= ibuffer->length)
			{
				ibuffer->head = 0;
			}
		}


		OsAbsExitCriticalSection();
	}
}


void ICharArrayPutOneFromIsr(charArrayBufferDef* const ibuffer, uint8 data)
{
	if(ibuffer != NULL)
	{
		OsAbsEnterCriticalSectionFromIsr();
		//Copy data to buffer
		*((uint8 *)((uint32)ibuffer->data + ibuffer->tail++)) = data;

		if(ibuffer->tail >= ibuffer->length)
		{
			ibuffer->tail = 0;
		}

		if(ibuffer->head == ibuffer->tail)
		{
			ibuffer->head++;
			if(ibuffer->head >= ibuffer->length)
			{
				ibuffer->head = 0;
			}
		}

		OsAbsExitCriticalSectionFromIsr();
	}
}

BOOL ICharArrayHaveNext(charArrayBufferDef* const ibuffer)
{
	BOOL res = False;

	if(ibuffer != NULL)
	{
		if(ibuffer->head != ibuffer->tail)
		{
			res = True;
		}
	}

	return res;
}

BOOL ICharArrayGetNextOne(charArrayBufferDef* const ibuffer, uint8 * pData)
{
	BOOL res = False;
	if(!pData)return res;

	if(ibuffer != NULL)
	{
		OsAbsEnterCriticalSection();
		if(ibuffer->head != ibuffer->tail)
		{
			//Copy data to destination
			*pData = *(uint8 *)((uint32)ibuffer->data + ibuffer->head++);
			res = True;

			//Reset head pointer if reached to end
			if(ibuffer->head >= ibuffer->length)
			{
				ibuffer->head = 0;
			}
		}

		OsAbsExitCriticalSection();
	}

	return res;
}

uint32 ICharArrayGetMany(charArrayBufferDef* const ibuffer, const uint8 *dest, uint32 size)
{
	uint32 dataRead = 0;

	if (!size || !ibuffer)
		return 0;

	uint8 *pDest = (uint8*) dest;
	while (ICharArrayGetNextOne(ibuffer, pDest)) {
		dataRead += 1;
		pDest += 1;

		if (dataRead >= size) {
			break;
		}
	}

	return dataRead;
}



