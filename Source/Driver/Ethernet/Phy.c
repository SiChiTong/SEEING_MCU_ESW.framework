/*
 * Link.c
 *
 *  Created on: 2016年2月24日
 *      Author: seeing
 */

#include "Phy.h"

SeEthernetPhyDescription* pEthernetPhyDesc = SeNull;

SeInt8 SeEthernetPhyInit(SeEthernetPhyDescription* pEthernetPhyDescription)
{
	if(pEthernetPhyDescription == SeNull)
	{
		SeErrorPrint("Ethernet PHY description is not correct!");
		return SE_RETURN_ERROR;
	}

	if(pEthernetPhyDescription->fpSeEthernetPhyPreInit == SeNull)
	{
		SeErrorPrint("Ethernet PHY pre-initial method is not correct!");
		return SE_RETURN_ERROR;
	}

	pEthernetPhyDesc = pEthernetPhyDescription;

	SeNormalPrint("Ethernet PHY initial success!");
	return SE_RETURN_OK;
}

void SeEthernetPhyReset(void)
{
	if(pEthernetPhyDesc != SeNull)
	{
		if(pEthernetPhyDesc->fpSeEthernetPhyReset != SeNull)
		{
			pEthernetPhyDesc->fpSeEthernetPhyReset();
		}
	}
}

SeEthernetLinkSpeedType SeEthernetGetLinkSpeed(void)
{
	if(pEthernetPhyDesc != SeNull)
	{
		if(pEthernetPhyDesc->fpSeEthernetGetLinkSpeed != SeNull)
		{
			return pEthernetPhyDesc->fpSeEthernetGetLinkSpeed();
		}
	}
	return SeEthernetLinkSpeedTypeUnknown;
}

SeBool SeEthernetIsLinked(void)
{
	if(pEthernetPhyDesc != SeNull)
	{
		if(pEthernetPhyDesc->fpSeEthernetIsLinked != SeNull)
		{
			return pEthernetPhyDesc->fpSeEthernetIsLinked();
		}
	}
	return SeFalse;
}
