/*
 * L3G4200D.c
 *
 *  Created on: 2017年4月18日
 *      Author: seeing
 */

#include "L3G4200D.h"

static SeL3G4200DDescription tGlobalL3G4200DDesc;

SeInt8 SeL3G4200DChipInit(SeL3G4200DDescription tL3G4200DDescription)
{
	if(tL3G4200DDescription.iL3G4200DIndex < 0 || tL3G4200DDescription.iL3G4200DIndex > CONFIG_MAX_I2C_PERIPHERALS)
	{
		return SE_RETURN_ERROR;
	}

	tGlobalL3G4200DDesc = tL3G4200DDescription;

	return SE_RETURN_OK;
}

SeInt8 SeL3G4200DSelfCheck(void)
{
	SeUInt8 bVal;

	if(SeL3G4200DRegRead(L3G4200D_REG_WHO_AM_I, &bVal) != SE_RETURN_ERROR)
		return SE_RETURN_ERROR;

	if(bVal != L3G4200D_DEVICE_ID)
		return SE_RETURN_ERROR;

	return SE_RETURN_OK;
}

SeInt8 SeL3G4200DRegRead(SeUInt8 hAddress, SeUInt8* pValue)
{
	SeBool blAck = SeFalse;

	if(pValue == SeNull)
		return SE_RETURN_ERROR;

	if(SeI2cStart(tGlobalL3G4200DDesc.iL3G4200DIndex) != SE_RETURN_OK)
		return SE_RETURN_ERROR;

	if(SeI2cWrite(tGlobalL3G4200DDesc.iL3G4200DIndex, tGlobalL3G4200DDesc.hL3G4200DMajorAddress, &blAck) != SE_RETURN_OK
			&& !blAck)
		return SE_RETURN_ERROR;

	if(SeI2cWrite(tGlobalL3G4200DDesc.iL3G4200DIndex, hAddress, &blAck) != SE_RETURN_OK
			&& !blAck)
		return SE_RETURN_ERROR;

	if(SeI2cStart(tGlobalL3G4200DDesc.iL3G4200DIndex) != SE_RETURN_OK)
			return SE_RETURN_ERROR;

	if(SeI2cWrite(tGlobalL3G4200DDesc.iL3G4200DIndex, tGlobalL3G4200DDesc.hL3G4200DMajorAddress | 0x01, &blAck) != SE_RETURN_OK
			&& !blAck)
		return SE_RETURN_ERROR;

	if(SeI2cRead(tGlobalL3G4200DDesc.iL3G4200DIndex, pValue, SeFalse) != SE_RETURN_OK)
		return SE_RETURN_ERROR;

	if(SeI2cStop(tGlobalL3G4200DDesc.iL3G4200DIndex) != SE_RETURN_OK)
		return SE_RETURN_ERROR;

	return SE_RETURN_OK;
}

SeInt8 SeL3G4200DRegWrite(SeUInt8 hAddress, SeUInt8 hValue)
{
	SeBool blAck = SeFalse;

	if(SeI2cStart(tGlobalL3G4200DDesc.iL3G4200DIndex) != SE_RETURN_OK)
		return SE_RETURN_ERROR;

	if(SeI2cWrite(tGlobalL3G4200DDesc.iL3G4200DIndex, tGlobalL3G4200DDesc.hL3G4200DMajorAddress, &blAck) != SE_RETURN_OK
			&& !blAck)
		return SE_RETURN_ERROR;

	if(SeI2cWrite(tGlobalL3G4200DDesc.iL3G4200DIndex, hAddress, &blAck) != SE_RETURN_OK
			&& !blAck)
		return SE_RETURN_ERROR;

	if(SeI2cWrite(tGlobalL3G4200DDesc.iL3G4200DIndex, hValue, &blAck) != SE_RETURN_OK
			&& !blAck)
		return SE_RETURN_ERROR;

	if(SeI2cStop(tGlobalL3G4200DDesc.iL3G4200DIndex) != SE_RETURN_OK)
		return SE_RETURN_ERROR;

	return SE_RETURN_OK;
}
