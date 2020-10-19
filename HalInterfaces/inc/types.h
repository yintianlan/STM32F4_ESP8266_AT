#ifndef __types_H
#define __types_H

typedef unsigned int    uint32;
typedef unsigned short	uint16;
typedef unsigned char 	uint8;

//计算数组长度
#define ARRY_LEN(value, type) (sizeof(value)/sizeof(type))

typedef enum
{
	False = 0,
	True = !False,
}BOOL;

#endif
