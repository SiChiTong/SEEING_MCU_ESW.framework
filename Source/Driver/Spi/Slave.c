/*
 * Slave.c
 *
 *  Created on: 2017年2月22日
 *      Author: seeing
 */

#include "Slave.h"

static SeSpiSlaveDescription* pSpiSlaveChannels[CONFIG_MAX_SPI_SLAVE_CHANNELS] = {0};

SeInt8 SeSpiSlaveInit(SeInt8 iIndex, SeSpiSlaveDescription* pSpiSlave)
{
	if(iIndex < 0 || iIndex > (CONFIG_MAX_SPI_SLAVE_CHANNELS - 1))
	{
		return SE_RETURN_ERROR;
	}

	if(pSpiSlave == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pSpiSlave->tOperation.fpSeSpiSlavePreInit(pSpiSlave->tBufferDescription, pSpiSlave->tCallbacks) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	pSpiSlaveChannels[iIndex] = pSpiSlave;

	pSpiSlaveChannels[iIndex]->blInited = SeTrue;

	return SE_RETURN_OK;
}
