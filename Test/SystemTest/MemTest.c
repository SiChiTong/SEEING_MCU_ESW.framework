/*
 * 使用说明：
 * 1，在SEEING_MCU_ESW.framework目录下的CMakeLists文件中的
 * SET(PROJECT_SOURCES
	Source/Global/Startup.c
	Source/Global/Initial.c
	Source/User/Application.c
	Source/User/Driver.c
	TEST/MemTest.c
)中添加“TEST/MemTest.c”
 */

// includes----------------------------------------------------------------------------------
#include "../SystemTest/MemTest.h"

#include "../../Source/Global/Include.h"
#include "../../Source/Config/Config.h"
//#include "stdint.h"
// source define-----------------------------------------------------------------------------
#define UsedMalloc(nsize)	SeMemoryAlloc(nsize)
#define UsedFree(padd)	SeMemoryFree(padd)
#define NULL 0

//堆空间大小
//#define HEAP_TOTAL_SIZE configTOTAL_HEAP_SIZE
#define HEAP_TOTAL_SIZE	(15*1024)
//最多可以分配的地址数
#define MLLC_ADD_NUM  (HEAP_TOTAL_SIZE/4)

//用户一次可以分配的最大空间
#define HEAP_USER_SIZE	1024

//分配的地址存储区
SeUInt32 * pMllcAdd[MLLC_ADD_NUM];//此处有待改进
//记录分配空间大小的存储区
SeUInt16 MllcSize[MLLC_ADD_NUM];//此处有待改进
//记录存储位置的变量
int I_Mllc = 0;
//记录已经分配的内存大小
SeUInt16 RecordMemSizeUse = 0;

//function define-----------------------------------------------------------------------------
int SeMemTest(SeUInt32 ss)
{
	int tmp;
	// 产生随机数
	srand(ss);
	tmp = rand();
	//为奇数，则分配内存
	if(tmp%2)
	{
		//分配随机大小的内存，1024 Byte以内
		MllcSize[I_Mllc] = (tmp%(HEAP_USER_SIZE/4) + 1) *4;//每次分配的字节数为4的整数倍，且不大于1024
		SeNormalPrint("malloc size :%d",MllcSize[I_Mllc] );
		pMllcAdd[I_Mllc] = UsedMalloc(MllcSize[I_Mllc]);
		//分配失败则释放全部分配的内存
		if(pMllcAdd[I_Mllc] == NULL)
		{
			while(I_Mllc > 0)
			{
				I_Mllc--;
				UsedFree(pMllcAdd[I_Mllc]);
				RecordMemSizeUse -= MllcSize[I_Mllc];
			}
			return -1;
		}
		else
		{
			RecordMemSizeUse += MllcSize[I_Mllc];
			I_Mllc++;
			return 1;
		}
		
	}
	//为偶数，则释放内存
	else
	{
		if(I_Mllc > 0)
		{
			SeNormalPrint("free size :%d",MllcSize[tmp%I_Mllc]);
			UsedFree(pMllcAdd[tmp%I_Mllc]);//随机释放某一分配的内存
			RecordMemSizeUse -= MllcSize[tmp%I_Mllc];
			pMllcAdd[tmp%I_Mllc] = pMllcAdd[I_Mllc - 1];
			MllcSize[tmp%I_Mllc] = MllcSize[I_Mllc - 1];
			I_Mllc --;
			return 1;
		}
		else
		{
			return 0;
		}		
	}
}
// endfile------------------------------------------
