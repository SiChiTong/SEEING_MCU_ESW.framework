/*
 * System.c
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#include "System.h"

#define MAC_ADDRESS                         "00:12:34:56:78:9a"
#define IP_ADDRESS                              "192.168.1.32"
#define NETMASK_ADDRESS               "255.255.255.0"
#define GATEWAY_ADDRESS                "192.168.1.1"
#define SYSTEM_SERIAL_NUMBER    "0123456789abcdef"
#define SYSTEM_DATE_YYYYMMDD   "2016/01/01"
#define SYSTEM_TIME_HHMMSS       "08:30:00"

static SeString sSoftwareVersion = "1.00.01";
static SeString sHardwareVersion = "Arm board D2.3";
static SeString sAuthorName = "Administrator";

SeNetworkInforDescription tNetworkInfor;
SeSystemTimeDescription tSystemTime;

SeChar sFpgaVersion[SE_BUFFER_SIZE_8] = {0};
SeChar sFpgaBomVersion[SE_BUFFER_SIZE_8] = {0};
SeChar sSystemSerialNumber[SE_BUFFER_SIZE_32] = {0};
SeChar sLocalTime[SE_BUFFER_SIZE_32] = {0};
SeChar sCompileTime[SE_BUFFER_SIZE_32] = {0};
SeUInt8 iMonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

SeString SeSoftwareVersionGet(void)
{
	return sSoftwareVersion;
}

SeString SeHardwareVersionGet(void)
{
	return sHardwareVersion;
}

SeString SeAuthorNameGet(void)
{
	return sAuthorName;
}

SeString SeFpgaBomVersionGet(void)
{
	SeUInt16 hAddress = 0;
	SeUInt8 bData = 0;

	hAddress = 0x11;
	if(SeFpgaRead8BitRegister(hAddress, &bData) == SE_RETURN_OK)
	{
		sprintf(sFpgaBomVersion, "%d.%d", 0, bData&0x07 );
	}
	else
	{
		sprintf(sFpgaBomVersion, "%d.%d", 0, 0 );
	}
	return sFpgaBomVersion;
}

SeString SeFpgaVersionGet(void)
{
	SeUInt16 hAddress = 0;
	SeUInt8 bData = 0;

	hAddress = 0x10;
	if(SeFpgaRead8BitRegister(hAddress, &bData) == SE_RETURN_OK)
	{
		sprintf(sFpgaVersion, "%d.%d", (bData&0xf0)>>4, bData&0x0f );
	}
	else
	{
		sprintf(sFpgaVersion, "%d.%d", 0, 0 );
	}
	return sFpgaVersion;
}

SeString SeSoftwareCompileTimeGet(void)
{
	sprintf(sCompileTime, "%s %s", __DATE__, __TIME__ );
	return sCompileTime;
}

SeInt8 SeSystemNetworkInit(void)
{
	SeChar sMac[] = MAC_ADDRESS;
	SeChar sIp[] = IP_ADDRESS;
	SeChar sNetmask[] = NETMASK_ADDRESS;
	SeChar sGateway[] = GATEWAY_ADDRESS;

	if(SeSystemMacSet(sMac) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeSystemIpSet(sIp) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeSystemNetmaskSet(sNetmask) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeSystemGatewaySet(sGateway) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeSystemNetworkInforGet(SeNetworkInforDescription *pNetworkInfor)
{
	if(pNetworkInfor == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	pNetworkInfor = &tNetworkInfor;
	return SE_RETURN_OK;
}

SeInt8 SeSystemMacSet(SeString sMac)
{
	SeString sResult = SeNull;
	SeInt8 iInedx = 0;
	SeInt8 iCharInedx = 0;
	SeUInt8 bMac[6] = {0};

	if(sMac == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	sResult = strtok(sMac, ":");
	while(sResult)
	{
		if(iInedx >= 6 || strlen(sResult) > 2)
		{
			return SE_RETURN_ERROR;
		}
		for(iCharInedx = 0; iCharInedx < strlen(sResult); iCharInedx++)
		{
			if(!((sResult[iCharInedx] >= '0' && sResult[iCharInedx] <= '9') || (sResult[iCharInedx] >= 'a' && sResult[iCharInedx] <= 'f') || (sResult[iCharInedx] >= 'A' && sResult[iCharInedx] <= 'F')))
			{
				return SE_RETURN_ERROR;
			}
		}
		bMac[iInedx++] = strtoul(sResult, SeNull, 16);
		sResult = strtok(SeNull, ":");
	}
	for(iInedx = 0; iInedx < 6; iInedx++)
	{
		tNetworkInfor.bMac[iInedx] = bMac[iInedx];
	}
	return SE_RETURN_OK;
}

SeInt8 SeSystemIpSet(SeString sIp)
{
	SeString sResult = SeNull;
	SeInt8 iInedx = 0;
	SeInt8 iCharInedx = 0;
	SeUInt8 bData = 0;
	SeUInt8 bIp[4] = {0};

	if(sIp == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	sResult = strtok(sIp, ".");
	while(sResult)
	{
		if(iInedx >= 4 || strlen(sResult) > 3)
		{
			return SE_RETURN_ERROR;
		}
		for(iCharInedx = 0; iCharInedx < strlen(sResult); iCharInedx++)
		{
			if(!(sResult[iCharInedx] >= '0' && sResult[iCharInedx] <= '9'))
			{
				return SE_RETURN_ERROR;
			}
		}
		bData = atoi(sResult);
		if(bData >=0 && bData <=255)
		{
			bIp[iInedx++] = bData;
		}
		else
		{
			return SE_RETURN_ERROR;
		}
		sResult = strtok(SeNull, ".");
	}
	for(iInedx = 0; iInedx < 4; iInedx++)
	{
		tNetworkInfor.bIp[iInedx] = bIp[iInedx];
	}
	return SE_RETURN_OK;
}

SeInt8 SeSystemNetmaskSet(SeString sNetmask)
{
	SeString sResult = SeNull;
	SeInt8 iInedx = 0;
	SeInt8 iCharInedx = 0;
	SeUInt8 bData = 0;
	SeUInt8 bNetmask[4] = {0};

	if(sNetmask == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	sResult = strtok(sNetmask, ".");
	while(sResult)
	{
		if(iInedx >= 4 || strlen(sResult) > 3)
		{
			return SE_RETURN_ERROR;
		}
		for(iCharInedx = 0; iCharInedx < strlen(sResult); iCharInedx++)
		{
			if(!(sResult[iCharInedx] >= '0' && sResult[iCharInedx] <= '9'))
			{
				return SE_RETURN_ERROR;
			}
		}
		bData = atoi(sResult);
		if(bData >=0 && bData <=255)
		{
			bNetmask[iInedx++] = bData;
		}
		else
		{
			return SE_RETURN_ERROR;
		}
		sResult = strtok(SeNull, ".");
	}
	for(iInedx = 0; iInedx < 4; iInedx++)
	{
		tNetworkInfor.bNetmask[iInedx] = bNetmask[iInedx];
	}
	return SE_RETURN_OK;
}

SeInt8 SeSystemGatewaySet(SeString sGateway)
{
	SeString sResult = SeNull;
	SeInt8 iInedx = 0;
	SeInt8 iCharInedx = 0;
	SeUInt8 bData = 0;
	SeUInt8 bGateway[4] = {0};

	if(sGateway == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	sResult = strtok(sGateway, ".");
	while(sResult)
	{
		if(iInedx >= 4 || strlen(sResult) > 3)
		{
			return SE_RETURN_ERROR;
		}
		for(iCharInedx = 0; iCharInedx < strlen(sResult); iCharInedx++)
		{
			if(!(sResult[iCharInedx] >= '0' && sResult[iCharInedx] <= '9'))
			{
				return SE_RETURN_ERROR;
			}
		}
		bData = atoi(sResult);
		if(bData >=0 && bData <=255)
		{
			bGateway[iInedx++] = bData;
		}
		else
		{
			return SE_RETURN_ERROR;
		}
		sResult = strtok(SeNull, ".");
	}
	for(iInedx = 0; iInedx < 4; iInedx++)
	{
		tNetworkInfor.bGateway[iInedx] = bGateway[iInedx];
	}
	return SE_RETURN_OK;
}

SeInt8 SeSystemSerialNumberInit(void)
{
	if(SeSystemSerialNumberSet((SeString)SYSTEM_SERIAL_NUMBER) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeSystemSerialNumberSet(SeString sSerialNmuber)
{
	if(sSerialNmuber == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	memset(sSystemSerialNumber, 0, sizeof(sSystemSerialNumber) );
	memcpy(sSystemSerialNumber, sSerialNmuber, (strlen(sSerialNmuber)>16) ? 16 : strlen(sSerialNmuber));
	return SE_RETURN_OK;
}

SeString SeSystemSerialNumberGet(void)
{
	return sSystemSerialNumber;
}

SeInt8 SeSystemLocalTimeInit(void)
{
	SeChar sDate[] = SYSTEM_DATE_YYYYMMDD;
	SeChar sTime[] = SYSTEM_TIME_HHMMSS;

	if(SeSystemDateSet(sDate) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	if(SeSystemTimeSet(sTime) == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}
	return SE_RETURN_OK;
}

SeInt8 SeSystemTimeRunning(void)
{
	SeUInt16 iMillisecondTemp;
	SeUInt8 iSecondTemp;

	tSystemTime.iMillisecond += 1;
	if(tSystemTime.iMillisecond < 1000)
	{
		return SE_RETURN_OK;
	}
	iMillisecondTemp = (tSystemTime.iMillisecond)%1000;
	iSecondTemp = (tSystemTime.iMillisecond)/1000;
	tSystemTime.iMillisecond = iMillisecondTemp;
	tSystemTime.iSecond += iSecondTemp;
	if(tSystemTime.iSecond < 60)
	{
		return SE_RETURN_OK;
	}
	tSystemTime.iSecond = 0;
	tSystemTime.iMinute++;
	if(tSystemTime.iMinute < 60)
	{
		return SE_RETURN_OK;
	}
	tSystemTime.iMinute = 0;
	tSystemTime.iHour++;
	if(tSystemTime.iHour < 24)
	{
		return SE_RETURN_OK;
	}
	tSystemTime.iHour = 0;
	tSystemTime.iDay++;
	if(tSystemTime.iMonth == 2)
	{
		if((tSystemTime.iYear%4 == 0) && ((tSystemTime.iYear%100) != 0 || tSystemTime.iYear%400 == 0))
		{
			iMonthDays[1] = 29;
		}
		else
		{
			iMonthDays[1] = 28;
		}
	}
	if(tSystemTime.iDay <= iMonthDays[tSystemTime.iMonth-1])
	{
		return SE_RETURN_OK;
	}
	tSystemTime.iDay = 1;
	tSystemTime.iMonth++;
	if(tSystemTime.iMonth < 12)
	{
		return SE_RETURN_OK;
	}
	tSystemTime.iMonth = 1;
	tSystemTime.iYear++;

	return SE_RETURN_OK;
}

SeInt8 SeSystemDateSet(SeString sDate)
{
	SeString sResult = SeNull;
	SeInt8 iInedx = 0;
	SeInt8 iCharInedx = 0;
	SeUInt16 bData = 0;
	SeUInt16 iDate[3] = {0};

	if(sDate == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	sResult = strtok(sDate, "/");
	while(sResult)
	{
		if(iInedx >= 3 || strlen(sResult) > 4)
		{
			return SE_RETURN_ERROR;
		}
		for(iCharInedx = 0; iCharInedx < strlen(sResult); iCharInedx++)
		{
			if(!(sResult[iCharInedx] >= '0' && sResult[iCharInedx] <= '9'))
			{
				return SE_RETURN_ERROR;
			}
		}
		bData = atoi(sResult);
		if(iInedx == 0 && bData >= 0 && bData <= 9999)
		{
			iDate[iInedx] = bData;
		}
		else if(iInedx == 1 && bData >= 1 && bData <= 12)
		{
			iDate[iInedx] = bData;
		}
		else if(iInedx == 2)
		{
			if(iDate[1] == 2)
			{
				if((iDate[0]%4 == 0) && ((iDate[0]%100) != 0 || iDate[0]%400 == 0))
					iMonthDays[1]=29;
				else
					iMonthDays[1]=28;
			}
			if(bData <= iMonthDays[iDate[1]-1])
				iDate[iInedx] = bData;
			else
				return SE_RETURN_ERROR;
		}
		else
		{
			return SE_RETURN_ERROR;
		}
		iInedx++;
		sResult = strtok(SeNull, "/");
	}
	tSystemTime.iYear = iDate[0];
	tSystemTime.iMonth = iDate[1];
	tSystemTime.iDay = iDate[2];

	return SE_RETURN_OK;
}

SeInt8 SeSystemTimeSet(SeString sTime)
{
	SeString sResult = SeNull;
	SeInt8 iInedx = 0;
	SeInt8 iCharInedx = 0;
	SeUInt8 bData = 0;
	SeUInt8 iTime[3] = {0};

	if(sTime == SeNull)
	{
		return SE_RETURN_ERROR;
	}
	sResult = strtok(sTime, ":");
	while(sResult)
	{
		if(iInedx >= 3 || strlen(sResult) > 2)
		{
			return SE_RETURN_ERROR;
		}
		for(iCharInedx = 0; iCharInedx < strlen(sResult); iCharInedx++)
		{
			if(!(sResult[iCharInedx] >= '0' && sResult[iCharInedx] <= '9'))
			{
				return SE_RETURN_ERROR;
			}
		}
		bData = atoi(sResult);
		if(iInedx == 0 && bData >= 0 && bData <= 24)
		{
			iTime[iInedx] = bData;
		}
		else if(iInedx == 1 && bData >= 0 && bData <= 59)
		{
			iTime[iInedx] = bData;
		}
		else if(iInedx == 2 && bData >= 0 && bData <= 59)
		{
			iTime[iInedx] = bData;
		}
		else
		{
			return SE_RETURN_ERROR;
		}
		iInedx++;
		sResult = strtok(SeNull, ":");
	}
	tSystemTime.iHour = iTime[0];
	tSystemTime.iMinute = iTime[1];
	tSystemTime.iSecond = iTime[2];

	return SE_RETURN_OK;
}

SeString SeSystemLocalTimeGet(void)
{
	sprintf(sLocalTime, "%04u/%02u/%02u %02u:%02u:%02u",tSystemTime.iYear, tSystemTime.iMonth, tSystemTime.iDay, tSystemTime.iHour, tSystemTime.iMinute, tSystemTime.iSecond);
	return sLocalTime;
}

void SeSystemReset(void)
{
	// TODO implement
}


