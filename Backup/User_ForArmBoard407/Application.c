/*
 * Application.c
 *
 *  Created on: 2016年1月26日
 *      Author: seeing
 */


#include "../User_ForArmBoard407/Application.h"

#include "../Platform/Stm32f407/Stm32f407.h"
#include "../User_ForArmBoard407/Descriptions.h"

static SeNetworkDescription tNetDesc;
static SeNetworkOperation tNetOper;

static SeTerminalMessageDescription tTermMsgDesc;
static SeNetworkMessageDescription tNetMsgDesc;

SeInt8 SeUserApplicationInit(void)
{
	if(SeFpgaRegisterInit() != SE_RETURN_OK)
	{
		SeErrorPrint("Initial FPGA registers fail!");
		return SE_RETURN_ERROR;
	}

	if(SeCommandsInit() != SE_RETURN_OK)
	{
		SeErrorPrint("Initial command service fail!");
		return SE_RETURN_ERROR;
	}

	if(SeTerminalInit() != SE_RETURN_OK)
	{
		SeErrorPrint("Initial terminal fail!");
		return SE_RETURN_ERROR;
	}

/*
	tTermMsgDesc.iUartIndex = SE_UART_P1_INDEX;
	if(SeTerminalMessageInit(&tTermMsgDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("Initial terminal fail!");
		return SE_RETURN_ERROR;
	}
*/
	tNetDesc.tDeviceMac.aMacAddress[0] = 0xAA;
	tNetDesc.tDeviceMac.aMacAddress[0] = 0xBB;
	tNetDesc.tDeviceMac.aMacAddress[0] = 0xCC;
	tNetDesc.tDeviceMac.aMacAddress[0] = 0xDD;
	tNetDesc.tDeviceMac.aMacAddress[0] = 0xEE;
	tNetDesc.tDeviceMac.aMacAddress[0] = 0xFF;
	tNetDesc.tDefaultIp.aIpAddress[0] = 192;
	tNetDesc.tDefaultIp.aIpAddress[1] = 168;
	tNetDesc.tDefaultIp.aIpAddress[2] = 100;
	tNetDesc.tDefaultIp.aIpAddress[3] = 200;
	tNetDesc.tDefaultMask.aIpAddress[0] = 255;
	tNetDesc.tDefaultMask.aIpAddress[1] = 255;
	tNetDesc.tDefaultMask.aIpAddress[2] = 255;
	tNetDesc.tDefaultMask.aIpAddress[3] = 0;
	tNetDesc.tDefaultGate.aIpAddress[0] = 192;
	tNetDesc.tDefaultGate.aIpAddress[1] = 168;
	tNetDesc.tDefaultGate.aIpAddress[2] = 100;
	tNetDesc.tDefaultGate.aIpAddress[3] = 1;
	tNetOper.fpSeNetIfInit = SeStm32f407NetIfInit;
	if(SeNetworkInit(&tNetDesc, &tNetOper) != SE_RETURN_OK)
	{
		SeErrorPrint("Initial network fail!");
		return SE_RETURN_ERROR;
	}

	tNetMsgDesc.tPort = 7600;
	tNetMsgDesc.nTimeout = 1000;
	if(SeNetworkMessageInit(&tNetMsgDesc) != SE_RETURN_OK)
	{
		SeErrorPrint("Initial network message service fail!");
		return SE_RETURN_ERROR;
	}

	/*
	 * register command
	 */
	SeRegisterCommand("help", SeHelpHandle);
	SeRegisterCommand("update", SeUpdateHandle);
	SeRegisterCommand("tftp_update", SeTftpUpdateHandle);
	SeRegisterCommand("fpga_read", SeFpgaReadHandle);
	SeRegisterCommand("fpga_write", SeFpgaWriteHandle);
	SeRegisterCommand("datalogger", SeAd9265DataAcquisitionHandle);


	return SE_RETURN_OK;
}
