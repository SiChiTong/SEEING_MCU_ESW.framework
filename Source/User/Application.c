/*
 * Application.c
 *
 *  Created on: 2016年1月26日
 *      Author: seeing
 */


#include "Application.h"
#include "Descriptions.h"
#include "../Platform/Stm32f107/Stm32f10x.h"
#include "Applications/Handles.h"
#include "Applications/Registers.h"
#include "Applications/Motion.h"

static SeSpiSlaveOperation tSpiSlaveOper;
static SeSpiSlaveDescription tSpiSlaveDesc;

SeInt8 SeUserApplicationInit(void)
{
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

	SeRegisterCommand("help", SeHelpHandle);
	SeRegisterCommand("motor_spd", SeUserSetMotorSpeedHandle);

	tSpiSlaveOper.fpSeSpiSlavePreInit = SeStm32f107Spi2SlaveInit;
	tSpiSlaveDesc.tOperation = tSpiSlaveOper;
	if(SeSpiSlaveServiceInit(SE_SPI_SLAVE_INDEX, tSpiSlaveDesc, BASE_REG_END) != SE_RETURN_OK)
	{
		return SE_RETURN_ERROR;
	}

	SeNormalPrint("Spi slave init ok!");

	/*
	 * registers
	 */
	SeSpiSlaveServiceRegisterHandle(BASE_REG_TEST, SeUserRegisterTestHandle);
	SeSpiSlaveServiceRegisterHandle(BASE_REG_PID_KP, SeUserRegisterPidKpHandle);
	SeSpiSlaveServiceRegisterHandle(BASE_REG_PID_KI, SeUserRegisterPidKiHandle);
	SeSpiSlaveServiceRegisterHandle(BASE_REG_PID_KD, SeUserRegisterPidKdHandle);
	SeSpiSlaveServiceRegisterHandle(BASE_REG_DIST_PER_PULSE, SeUserRegisterTicksPerMeterHandle);
	SeSpiSlaveServiceRegisterHandle(BASE_REG_WHEEL_TRACK, SeUserRegisterWheelTrackHandle);
	SeSpiSlaveServiceRegisterHandle(BASE_REG_TARGET_X, SeUserRegisterTargetXHandle);
	SeSpiSlaveServiceRegisterHandle(BASE_REG_TARGET_Y, SeUserRegisterTargetYHandle);
	SeSpiSlaveServiceRegisterHandle(BASE_REG_TARGET_THETA, SeUserRegisterTargetThetaHandle);

	/*
	 * Motion
	 */
	SeUserMotionInit();

	return SE_RETURN_OK;
}
