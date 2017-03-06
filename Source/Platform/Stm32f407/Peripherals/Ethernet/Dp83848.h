/*
 * Dp83848_Phy.h
 *
 *  Created on: 2015-12-10
 *      Author: Administrator
 */

#ifndef _DP83848_PHY_H
#define _DP83848_PHY_H

#include "../../../../Global/Include.h"

/* Dp83848 phy address */
#define DP83848_PHY_ADDR        0x01

void SeDp83848PhyInit(void);
void SeDp83848PhyReset(void);
SeEthernetLinkSpeedType SeDp83848PhyGetLinkSpeed(void);
SeBool SeDp83848PhyGetIsLinked(void);

#endif /* _DP83848_PHY_H_ */
