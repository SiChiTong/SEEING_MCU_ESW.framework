/*
 * Flash.h
 *
 *  Created on: 2016年2月22日
 *      Author: seeing
 */

#ifndef _STM32F407_FLASH_H_
#define _STM32F407_FLASH_H_

#include "../../../../Global/Include.h"
#include "../../Manufacturer/stm32f4xx_conf.h"

//FLASH起始地址
#define STM32_FLASH_BASE        0x08000000 	//STM32 FLASH的起始地址

//FLASH 扇区起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes

#define USER_FLASH_END_ADDRESS        0x080FFFFF


#define SeStm32f407FlashLock() FLASH_Lock()
#define SeStm32f407FlashUnlock() FLASH_Unlock()
#define SeStm32f407FlashClearFlag() FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|\
        FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR)

#define SeStm32f407FlashEraseSector(nSectorNo) FLASH_EraseSector(nSectorNo,VoltageRange_3)
#define SeStm32f407FlashReadWord(nAddr) (*(vu32*)nAddr)

SeInt8 SeStm32f407FlashWritedata(SeUInt32 nAddress, SeUInt8 * pData, SeUInt32 nLength);


#endif /* _STM32F407_FLASH_H_ */
