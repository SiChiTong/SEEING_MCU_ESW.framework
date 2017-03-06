/*
 * Update.c
 *
 *  Created on: 2016年2月29日
 *      Author: seeing
 */


#include "Update.h"

static SeUpdateDescription tUpdateDesc;

void SeUpdateConfig(SeUpdateDescription tUpdateDescription)
{
	tUpdateDesc = tUpdateDescription;
}

SeInt8 SeUpdateMcuXmodem()
{
	if(tUpdateDesc.fpSeUpdateMcu == SeNull)
		return SE_RETURN_ERROR;

	return tUpdateDesc.fpSeUpdateMcu();
}

SeInt8 SeUpdateFpgaXmodem()
{
	if(tUpdateDesc.fpSeUpdateFpga == SeNull)
			return SE_RETURN_ERROR;

	return tUpdateDesc.fpSeUpdateFpga();
}

static SeTftpClientGetFileOperation tTftpUpdateOperation;
void SeTftpUpdateOperationSet(SeTftpClientGetFileOperation tTftpClientGetFileOperation)
{
	tTftpUpdateOperation = tTftpClientGetFileOperation;
}

SeTftpClientGetFileOperation SeTftpUpdateOperationGet(void)
{
	return tTftpUpdateOperation;
}
