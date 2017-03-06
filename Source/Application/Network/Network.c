/*
 * Network.c
 *
 *  Created on: 2016年1月8日
 *      Author: seeing
 */

#include "Network.h"
#include "Adapter/Adapter.h"

static SeNetworkDescription* pNetworkDesc = SeNull;
static SeNetworkOperation* pNetworkOper = SeNull;

SeInt8 SeNetworkInit(SeNetworkDescription* pNetworkDescription, SeNetworkOperation* pNetworkOperation)
{
	if(pNetworkDescription == SeNull)
	{
		SeErrorPrint("Network description is not correct!");
		return SE_RETURN_ERROR;
	}

	if(pNetworkOperation == SeNull)
	{
		SeErrorPrint("Network operation is not correct!");
		return SE_RETURN_ERROR;
	}

	if(pNetworkOperation->fpSeNetIfInit(pNetworkDescription) != SE_RETURN_OK)
	{
		SeErrorPrint("Network interface initial fail!");
		return SE_RETURN_ERROR;
	}

	pNetworkDesc = pNetworkDescription;
	pNetworkOper = pNetworkOperation;

	return SE_RETURN_OK;
}
