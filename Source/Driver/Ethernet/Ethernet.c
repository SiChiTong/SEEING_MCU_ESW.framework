/*
 * Ethernet.c
 *
 *  Created on: 2016年2月25日
 *      Author: seeing
 */

#include "Ethernet.h"

SeInt8 SeEthernetInit(SeEthernetDescription* pEthernetDescription)
{
	if(pEthernetDescription == SeNull)
	{
		SeErrorPrint("Ethernet description is not correct!");
		return SE_RETURN_ERROR;
	}

	if(pEthernetDescription->fpSeEthernetInit == SeNull)
	{
		SeErrorPrint("Ethernet initial method is not correct!");
		return SE_RETURN_ERROR;
	}

	if(SeEthernetPhyInit(pEthernetDescription->pEthernetPhyDescription) != SE_RETURN_OK)
	{
		SeErrorPrint("Ethernet PHY initial fail!");
		return SE_RETURN_ERROR;
	}

	if(pEthernetDescription->fpSeEthernetInit() != SE_RETURN_OK)
	{
		SeErrorPrint("Ethernet initial fail!");
		return SE_RETURN_ERROR;
	}

	/*
	 * display ethernet info
	 */

	SeNormalPrint("Ethernet initial success!");
	return SE_RETURN_OK;
}
