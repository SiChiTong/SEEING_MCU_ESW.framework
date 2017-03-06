/*
 * Ad56x7.h
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */
#ifndef _AD56X7_H_
#define _AD56X7_H_

#include "../../../../Global/Include.h"

/* Set ad56x7 reference voltage 2.5v */
#define AD56X7_VREF		(2.5)
/* Convert  voltage(v)  to register data  */ 
#define AD56X7_VoltToData(v)		((SeUInt16)(((v * 65536) / AD56X7_VREF)+0.3))
/* Convert  register data to voltage(v) */
#define AD56X7_DataToVolt(data)		((data*AD56X7_VREF)/65536)	

/* Set ad56x7 channel */
typedef enum{
	Ad56x7ChannelA = 1,    /* channel A */
	Ad56x7ChannelB = 2,    /* channel B */
    Ad56x7ChannelAB = 0,  /* all channel */
}SeAd56x7ChannelNumber;

/*
 * Ad56x7 hardware description
 */
typedef struct
{
	SeUInt8 iAd56x7I2cIndex;
	SeUInt8 bAd56x7DeviceAddress;	
}SeAd56x7Description;

/*
 * Ad56x7 interface function
 */
SeInt8 SeAd56x7ChipInit(SeAd56x7Description tAd56x7Description);
SeInt8 SeAd56x7SetRegData(SeUInt8 hAddress, SeAd56x7ChannelNumber tChannel, SeUInt16 iData);
SeInt8 SeAd56x7SetOutputVolt(SeUInt8 hAddress, SeAd56x7ChannelNumber tChannel, SeDouble fVolt);

#endif

