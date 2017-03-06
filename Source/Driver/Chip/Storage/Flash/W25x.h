/*
 * W25x.h
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#ifndef _W25X_H_
#define _W25X_H_

#include "../../../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Page size 256 byte,Sector size 4k byte,Block size 64k byte
 */
#define W25X_PAGE_SIZE		        256
#define W25X_SECTOR_SIZE		(W25X_PAGE_SIZE*16)
#define W25X_BLOCK_SIZE		    (W25X_SECTOR_SIZE*16)
#define W25X16_CHIP_SIZE		    (W25X_BLOCK_SIZE*32)
#define W25X32_CHIP_SIZE		    (W25X_BLOCK_SIZE*64)
#define W25X64_CHIP_SIZE		    (W25X_BLOCK_SIZE*128)
#define W25X_CHIP_SIZE		        W25X16_CHIP_SIZE

/*
 * W25X interface function
 */
SeInt8 SeW25xSpiFlashInit(SeW25xDescription* tW25xDescription);
SeInt8 SeW25xSpiFlashReadData(SeW25xDescription* pW25xDescription, SeUInt32 hAddress, SeUInt8 *pBuffer, SeUInt32 nBytes);
SeInt8 SeW25xSpiFlashFastReadData(SeW25xDescription* pW25xDescription, SeUInt32 hAddress, SeUInt8 *pBuffer, SeUInt32 nBytes);
SeInt8 SeW25xSpiFlashWriteData(SeW25xDescription* pW25xDescription, SeUInt32 hAddress, SeUInt8 *pBuffer, SeUInt32 nBytes);
SeInt8 SeW25xSpiFlashReadStatusRegister(SeW25xDescription* pW25xDescription, SeUInt8 *pState);

SeInt8 SeW25xSpiFlashSectorErase(SeW25xDescription* pW25xDescription, SeUInt32 hAddress);
SeInt8 SeW25xSpiFlashBlockErase(SeW25xDescription* pW25xDescription, SeUInt32 hAddress);
SeInt8 SeW25xSpiFlashChipErase(SeW25xDescription* pW25xDescription, SeUInt8 iEraseMode);

SeInt8 SeW25xSpiFlashPowerDown(SeW25xDescription* pW25xDescription);
SeInt8 SeW25xSpiFlashWakeUp(SeW25xDescription* pW25xDescription);

SeInt8 SeW25xFlashReadDeviceID(SeW25xDescription* pW25xDescription, SeUInt8 *pId);
SeInt8 SeW25xSpiFlashReadManufactDeviceID(SeW25xDescription* pW25xDescription, SeUInt16 *pId);
SeInt8 SeW25xSpiFlashReadJedecDeviceID(SeW25xDescription* pW25xDescription, SeUInt32 *pId);

#ifdef __cplusplus
}
#endif

#endif 

