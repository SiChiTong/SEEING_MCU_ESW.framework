/*
 * Ad760x.h
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */

#ifndef _AD760X_H_
#define _AD760X_H_

#include "../../../../Global/Include.h"

/*
 * Ad760x channel number
 */
typedef enum {
    Ad760xChannel1 = 0,
    Ad760xChannel2 = 1,
    Ad760xChannel3 = 2,
    Ad760xChannel4 = 3,
    Ad760xChannel5 = 4,
    Ad760xChannel6 = 5,
    Ad760xChannel7 = 6,
    Ad760xChannel8 = 7,
}SeAd760xChannelNumber;

/*
 * Ad760x range type
 */
typedef enum {
    Ad760xRange0 = 0,
    Ad760xRange1
}SeAd760xRange;

/*
 * Ad760x sample mode
 */
typedef enum {
    Ad760xOSampleMode0 = 0,
    Ad760xOSampleMode1,
    Ad760xOSampleMode2,
    Ad760xOSampleMode3,
    Ad760xOSampleMode4,
    Ad760xOSampleMode5,
    Ad760xOSampleMode6
}SeAd760xSampleMode;

/*
 * Ad760x hardware description
 */
typedef struct
{
	SeUInt8 iAd760xResetPinIndex;
	SeUInt8 iAd760xSclkPinIndex;
	SeUInt8 iAd760xCsPinIndex;
	SeUInt8 iAd760xDoutPinIndex;
	SeUInt8 iAd760xOs0PinIndex;
	SeUInt8 iAd760xOs1PinIndex;
	SeUInt8 iAd760xOs2PinIndex;
	SeUInt8 iAd760xRangePinIndex;
	SeUInt8 iAd760xConvertPinIndex;
	SeUInt8 iAd760xBusyPinIndex;
}SeAd760xDescription;

/*
 * Ad760x mode logger
 */
typedef struct{
    SeUInt8 iRange;  /* 5,10 */
    SeUInt8 iOs; /* 0=NO,1=2,2=4,3=8,4=16,5=32,6=64  */
}SeAd760xModeLogger;

/*
 * Ad760x interface function
 */
SeInt8 SeAd760xInit(SeAd760xDescription tAd760xDescription);
SeInt8 SeAd760xIsCheckOK(void);
SeInt8 SeAd760xStartConvert(void);
SeInt8 SeAd760xGetSampleData(SeAd760xChannelNumber tChannel, SeUInt32 *pData);
SeInt8 SeAd760xGetSampleVolt(SeAd760xChannelNumber tChannel, SeDouble *pfMillivolt);
SeAd760xModeLogger SeAd760xReadMode(void);
SeInt8 SeAd760xRangeSet(SeAd760xRange tRange);
SeInt8 SeAd760xModeSet(SeAd760xSampleMode tOs);
SeInt8 SeAd760xReset(void);

#endif

