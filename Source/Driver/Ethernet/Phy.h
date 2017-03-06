/*
 * Phy.h
 *
 *  Created on: 2016年2月24日
 *      Author: seeing
 */

#ifndef _DRIVER_ETHERNET_PHY_H_
#define _DRIVER_ETHERNET_PHY_H_

#include "../../Global/Include.h"

SeInt8 SeEthernetPhyInit(SeEthernetPhyDescription* pEthernetPhyDescription);

void SeEthernetPhyReset(void);
SeEthernetLinkSpeedType SeEthernetGetLinkSpeed(void);
SeBool SeEthernetIsLinked(void);

#endif /* _DRIVER_ETHERNET_PHY_H_ */
