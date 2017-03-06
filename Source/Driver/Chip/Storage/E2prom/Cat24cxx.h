/*
 * Cat24cxx.h
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#ifndef _CAT24CXX_H_
#define _CAT24CXX_H_

#include "../../../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Cat24cxx chip type
 */
typedef enum
{
	Cat24c01 = 0,
	Cat24c02,
	Cat24c04,
	Cat24c08,
	Cat24c16,
}SeCat24cxxE2promType;

/*
 * Cat24cxx hardware description
 */
typedef struct
{
	SeUInt8 iCat24cxxI2cIndex;
	SeUInt8 hCat24cxxDeviceAddress;
	SeCat24cxxE2promType tCat24cxxType;
	SeUInt16 iCat24cxxChipSize;
}SeCat24cxxDescription;

/*
 * Cat24cxx chip page size /page number /chip size
 */
#define CAT24C01_PAGE_SIZE          16
#define CAT24C01_PAGE_NUM           8
#define CAT24C01_SIZE               (CAT24C01_PAGE_SIZE * CAT24C01_PAGE_NUM)

#define CAT24C02_PAGE_SIZE          16
#define CAT24C02_PAGE_NUM           16
#define CAT24C02_SIZE               (CAT24C02_PAGE_SIZE * CAT24C02_PAGE_NUM)

#define CAT24C04_PAGE_SIZE          16
#define CAT24C04_PAGE_NUM           32
#define CAT24C04_SIZE               (CAT24C04_PAGE_SIZE * CAT24C04_PAGE_NUM)

#define CAT24C08_PAGE_SIZE          16
#define CAT24C08_PAGE_NUM           64
#define CAT24C08_SIZE               (CAT24C08_PAGE_SIZE * CAT24C08_PAGE_NUM)

#define CAT24C16_PAGE_SIZE          16
#define CAT24C16_PAGE_NUM           128
#define CAT24C16_SIZE               (CAT24C16_PAGE_SIZE * CAT24C16_PAGE_NUM)

/*
 * Cat24cxx interface function
 */
SeInt8 SeCat24cxxE2promInit(SeCat24cxxDescription* pCat24cxxDescription);
SeInt8 SeCat24cxxE2promCheckIsOK(SeCat24cxxDescription* pCat24cxxDescription);
SeInt8 SeCat24cxxE2promReadData(SeCat24cxxDescription* pCat24cxxDescription, SeUInt16 hAddress, SeUInt8 *pBuffer, SeUInt16 nBytes);
SeInt8 SeCat24cxxE2promWriteData(SeCat24cxxDescription* pCat24cxxDescription, SeUInt16 hAddress, SeUInt8 *pBuffer, SeUInt16 nBytes);

#ifdef __cplusplus
}
#endif

#endif

