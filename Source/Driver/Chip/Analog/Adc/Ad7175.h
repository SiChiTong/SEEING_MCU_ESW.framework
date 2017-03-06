/*
 * Ad7175.h
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */

#ifndef _AD7175_H_
#define _AD7175_H_

#include "../../../../Global/Include.h"

/*
 * Ad7175 mode type
 */
typedef enum
{
	CnMode = 0x00,        /* continuous conversion */
	SgMode = 0x10,        /* single conversion */
//	SdMode = 0x20,        
	PdMode = 0x30,        /* power down */
	IoCalMode = 0x40,     /* inside calibration */
	SoCalMode = 0x60,     /* system calibration */
	SgCalMode = 0x70      /* system gain */
}SeAd7175ModeType;

/*
 * Ad7175 channel number
 */
typedef enum
{
	Ad7175Chan0 = 0,     /* current channel */ 
	Ad7175Chan1,         /* voltage channel */
	Ad7175Chan2,
	Ad7175Chan3
}SeAd7175ChannelNumber;

/*
 * Ad7175 hardware description
 */
typedef struct
{
	SeUInt8 iAd7175SpiIndex;
    SeUInt8 iAd7175DoutPinIndex;
    SeUInt8 iAd7175CsPinIndex;	
}SeAd7175Description;

/*
 * Ad7175 interface function
 */
SeInt8 SeAd7175Init(SeAd7175Description tAd7175Description);
SeInt8 SeAd7175Start(SeAd7175ChannelNumber tChannel, SeAd7175ModeType tMode);
SeInt8 SeAd7175IsMeasureOk(void);
SeInt8 SeAd7175ReadRegistor(SeUInt8 hAddress, SeUInt8 nSize, SeUInt32 *pData);
SeInt8 SeAd7175WriteRegistor(SeUInt8 hAddress, SeUInt8 nSize, SeUInt32 iValue);
SeInt8 SeAd7175ReadConvertData(SeAd7175ChannelNumber tChannel, SeAd7175ModeType tMode, SeUInt32 *pData);
SeInt8 SeAd7175GetSampleData(SeAd7175ChannelNumber tChannel, SeAd7175ModeType tMode, SeUInt32 *pData);
SeInt8 SeAd7175ModeSet(SeAd7175ModeType tMode);


#endif
