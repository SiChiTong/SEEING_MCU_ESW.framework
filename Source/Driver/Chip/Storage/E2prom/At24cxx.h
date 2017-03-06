/*
 * At24cxx.h
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#ifndef _AT24CXX_H_
#define _AT24CXX_H_

#include "../../../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * At24cxx chip page size /page number /chip size
 */
#define AT24C01_PAGE_SIZE				8
#define AT24C01_PAGE_NUM				16
#define AT24C01_SIZE							(AT24C01_PAGE_SIZE * AT24C01_PAGE_NUM)

#define AT24C02_PAGE_SIZE				8
#define AT24C02_PAGE_NUM				32
#define AT24C02_SIZE							(AT24C02_PAGE_SIZE * AT24C02_PAGE_NUM)

#define AT24C04_PAGE_SIZE				16
#define AT24C04_PAGE_NUM				32
#define AT24C04_SIZE							(AT24C04_PAGE_SIZE * AT24C04_PAGE_NUM)

#define AT24C08_PAGE_SIZE				16
#define AT24C08_PAGE_NUM				64
#define AT24C08_SIZE							(AT24C08_PAGE_SIZE * AT24C08_PAGE_NUM)

#define AT24C16_PAGE_SIZE				16
#define AT24C16_PAGE_NUM				128
#define AT24C16_SIZE							(AT24C16_PAGE_SIZE * AT24C16_PAGE_NUM)

#define AT24C32_PAGE_SIZE				32
#define AT24C32_PAGE_NUM				128
#define AT24C32_SIZE							(AT24C32_PAGE_SIZE * AT24C32_PAGE_NUM)

#define AT24C64_PAGE_SIZE				32
#define AT24C64_PAGE_NUM				256
#define AT24C64_SIZE							(AT24C64_PAGE_SIZE * AT24C64_PAGE_NUM)

#define AT24C128_PAGE_SIZE				64
#define AT24C128_PAGE_NUM			256
#define AT24C128_SIZE							 (AT24C128_PAGE_SIZE * AT24C128_PAGE_NUM)

#define AT24C256_PAGE_SIZE				64
#define AT24C256_PAGE_NUM			512
#define AT24C256_SIZE							(AT24C256_PAGE_SIZE * AT24C256_PAGE_NUM)

#define AT24C512_PAGE_SIZE				128
#define AT24C512_PAGE_NUM			512
#define AT24C512_SIZE							(AT24C512_PAGE_SIZE * AT24C512_PAGE_NUM)

/*
 * At24cxx interface function
 */
SeInt8 SeAt24cxxE2promInit(SeAt24cxxDescription* pAt24cxxDescription);
SeInt8 SeAt24cxxE2promReadData(SeAt24cxxDescription* pAt24cxxDescription, SeUInt16 hAddress, SeUInt8 *pBuffer, SeUInt16 nBytes);
SeInt8 SeAt24cxxE2promWriteData(SeAt24cxxDescription* pAt24cxxDescription, SeUInt16 hAddress, SeUInt8 *pBuffer, SeUInt16 nBytes);

#ifdef __cplusplus
}
#endif

#endif

