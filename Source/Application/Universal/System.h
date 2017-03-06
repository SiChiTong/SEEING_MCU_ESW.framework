/*
 * System.h
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#ifndef _SYSTEM_APPLICATION_H_
#define _SYSTEM_APPLICATION_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * define system network information structure
 */
typedef struct
{
	SeUInt8 bMac[6];
	SeUInt8 bIp[4];
	SeUInt8 bNetmask[4];
	SeUInt8 bGateway[4];
}SeNetworkInforDescription;

/*
 * define system time structure
 */
typedef struct
{
	SeUInt16 iYear;
	SeUInt8 iMonth;
	SeUInt8 iDay;
	SeUInt8 iHour;
	SeUInt8 iMinute;
	SeUInt8 iSecond;
	SeUInt16 iMillisecond;
}SeSystemTimeDescription;

extern SeNetworkInforDescription tNetworkInfor;
extern SeSystemTimeDescription tSystemTime;

SeString SeSoftwareVersionGet(void);
SeString SeHardwareVersionGet(void);
SeString SeAuthorNameGet(void);
SeString SeFpgaBomVersionGet(void);
SeString SeFpgaVersionGet(void);
SeString SeSoftwareCompileTimeGet(void);

SeInt8 SeSystemNetworkInit(void);
SeInt8 SeSystemMacSet(SeString sMac);
SeInt8 SeSystemIpSet(SeString sIp);
SeInt8 SeSystemNetmaskSet(SeString sNetmask);
SeInt8 SeSystemGatewaySet(SeString sGateway);
SeInt8 SeSystemNetworkInforGet(SeNetworkInforDescription *pNetworkInfor);

SeInt8 SeSystemSerialNumberInit(void);
SeInt8 SeSystemSerialNumberSet(SeString sSerialNmuber);
SeString SeSystemSerialNumberGet(void);

SeInt8 SeSystemLocalTimeInit(void);
SeInt8 SeSystemTimeRunning(void);
SeInt8 SeSystemDateSet(SeString sDate);
SeInt8 SeSystemTimeSet(SeString sTime);
SeString SeSystemLocalTimeGet(void);

void SeSystemReset(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_APPLICATION_H_ */
