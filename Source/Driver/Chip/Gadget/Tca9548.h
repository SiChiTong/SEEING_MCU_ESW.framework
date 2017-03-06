/*
 * Tca9548.c
 *
 *  Created on: 2015-12-9
 *      Author: Administrator
 */

#ifndef _TCA9548_H_
#define _TCA9548_H_

#include "../../../Global/Include.h"

/*
 * Tca9548 hardware description
 */
typedef struct
{
	SeUInt8 iTca9548I2cIndex;
	SeUInt8 bTca9548DeviceAddress;	
}SeTca9548Description;

/*
 * Tca9548 interface function
 */
SeInt8 SeTca9548Init(SeTca9548Description tTca9548Description);
SeInt8 SeTca9548Config(SeUInt8 hAddess, SeUInt8 iChannelx);
SeInt8 SeTca9548ConfigRead(SeUInt8 hAddess, SeUInt8 *pChannelx);
SeInt8 SeTca9548SelectOneChannel(SeUInt8 iChanel);
SeInt8 SeTca9548CloseOneChan(SeUInt8 iChanel);
SeInt8 SeTca9548OpenOneChan(SeUInt8 iChanel);
SeInt8 SeTca9548ConfigAllChan(SeUInt8 bChanelConfig);

#endif
