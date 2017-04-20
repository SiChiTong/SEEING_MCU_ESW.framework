/*
 * Handles.c
 *
 *  Created on: 2017年2月25日
 *      Author: seeing
 */

#include "Handles.h"
#include "../Drivers/Motor.h"
#include "Motion.h"

SeInt8 SeUserSetMotorSpeedHandle(SeLinkList* pParameterList, SeString sResult)
{
	SeChar sUsage[] = "usage: motor_spd({left_dir[f/b]},{left_pwm},{right_dir[f/b]},{right_pwm})";

	SeInt16 iParamIndex = 0;
	SeChar sTemp[SE_BUFFER_SIZE_32] = {0};
	SeUInt8 nLeftPwmVal = 0;
	SeUserRollStates tLeftRollState;
	SeUInt8 nRightPwmVal = 0;
	SeUserRollStates tRightRollState;

	for(iParamIndex = 0; iParamIndex < SeLinkListGetNodeCount(pParameterList); iParamIndex++)
	{
		SeCommandParameterInf* pItem = (SeCommandParameterInf*)SeLinkListGetItemByIndex(pParameterList, iParamIndex);
		memset(sTemp, 0, sizeof(sTemp));
		strncpy(sTemp, pItem->sParameter, pItem->nParameterLength);
		if(iParamIndex == 0)
		{
			if(strncmp(sTemp, "?", 1) == 0)
			{
				SeNormalPrint("%s", sUsage);
			}else{
				if(strncmp(sTemp, "f", 1) == 0)
					tLeftRollState = SE_USER_ROLL_FORWARD;
				else tLeftRollState = SE_USER_ROLL_BACKWARD;
			}
		}else if(iParamIndex == 1){
			nLeftPwmVal = strtoul(sTemp, SeNull, 10);
		}else if(iParamIndex == 2){
			if(strncmp(sTemp, "f", 1) == 0)
				tRightRollState = SE_USER_ROLL_FORWARD;
			else tRightRollState = SE_USER_ROLL_BACKWARD;
		}else{
			nRightPwmVal = strtoul(sTemp, SeNull, 10);
		}
	}

	SeUserMotionSetMotor(tLeftRollState, nLeftPwmVal, tRightRollState, nRightPwmVal);
	return SE_RETURN_OK;
}
