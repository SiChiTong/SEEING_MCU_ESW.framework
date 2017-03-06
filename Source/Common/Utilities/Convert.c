/*
 * Convert.c
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#include "Convert.h"

#define SECOND	1
#define MINUTE	SECOND * 60
#define HOUR	MINUTE * 60
#define DAY	HOUR * 24
#define FOURYEARS	(366 + 365 +365 +365);
SeUInt8 MON1[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
SeUInt8 MON2[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
SeChar sDateAndTime[SE_BUFFER_SIZE_32] = {0};

static SeUInt64 SeMktime(SeUInt32 iYear0, SeUInt32 iMon0, SeUInt32 iDay, SeUInt32 iHour, SeUInt32 iMin, SeUInt32 iSec)
{
	SeUInt32 iMon = iMon0, iYear = iYear0;

	/* 1..12 -> 11,12,1..10 */
	if (0 >= (SeInt32) (iMon -= 2)) {
		iMon += 12;	/* Puts Feb last since it has leap day */
		iYear -= 1;
	}

	return ((((SeUInt64)
		  (iYear/4 - iYear/100 + iYear/400 + 367*iMon/12 + iDay) +
		  iYear*365 - 719499
	    )*24 + iHour /* now have hours */
	  )*60 + iMin /* now have minutes */
	)*60 + iSec; /* finally seconds */
}



SeTimeValue SeStringToUtcTime(SeString sDateAndTime)
{
	SeTimeValue tUtcTime;
	SeSystemTimeDescription tLocalTime;

	/* 2016/01/15 09:30:08 */
	tLocalTime.iYear = atoi(sDateAndTime);
	tLocalTime.iMonth = atoi(sDateAndTime+5);
	tLocalTime.iDay = atoi(sDateAndTime+8);
	tLocalTime.iHour = atoi(sDateAndTime+11);
	tLocalTime.iMinute = atoi(sDateAndTime+14);
	tLocalTime.iSecond = atoi(sDateAndTime+17);

	tUtcTime.iUtcSeconds = SeMktime(tLocalTime.iYear, tLocalTime.iMonth, tLocalTime.iDay, tLocalTime.iHour, tLocalTime.iMinute, tLocalTime.iSecond);
	tUtcTime.iMicroSeconds = 0;
	return tUtcTime;
}

static void SeGetMonthAndDay(SeUInt32 nDays, SeUInt32 *pMonth, SeUInt32 *pDay, SeUInt8 IsLeapYear)
{
	SeUInt32 *pMonths = IsLeapYear?MON2:MON1;
	SeUInt8 i;
    for (  i=0; i<12; ++i )
    {
        int nTemp = nDays - pMonths[i];
        if ( nTemp<=0 )
        {
            *pMonth = i+1;
            if ( nTemp == 0 )
               *pDay = pMonths[i];
            else
                *pDay = nDays;
            break;
        }
        nDays = nTemp;
    }
}

SeString SeUtcTimeToString(SeTimeValue tUtcTime)
{
	SeUInt64 iTime;

	iTime = tUtcTime.iUtcSeconds;
	SeUInt32 nDays = (iTime / (24 * 3600) )+ 1;
	SeUInt32 nYear4 = nDays/FOURYEARS;
	SeUInt32 nRemain = nDays%FOURYEARS;
	SeUInt32 nDesYear = 1970 + nYear4*4;
	SeUInt32 nDesMonth = 0, nDesDay = 0, nDesHour = 0, nDesMinute = 0, nDesSecond = 0;
	SeUInt8 bLeapYear = 0;
	    if ( nRemain<365 )
	    {

	    }
	    else if ( nRemain<(365+365) )
	    {
	        nDesYear += 1;
	        nRemain -= 365;
	    }
	    else if ( nRemain<(365+365+365) )
	    {
	        nDesYear += 2;
	        nRemain -= (365+365);
	    }
	    else
	    {
	        nDesYear += 3;
	        nRemain -= (365+365+365);
	        bLeapYear = 1;
	    }
	   SeGetMonthAndDay(nRemain, &nDesMonth, &nDesDay, bLeapYear);
	   nDesSecond = iTime % MINUTE;
	   nDesMinute = (iTime % HOUR) / MINUTE;
	   nDesHour = (iTime % DAY) / HOUR;
	   sprintf(sDateAndTime," %04u/%02u/%02u %02u/%02u/%02u",nDesYear,nDesMonth,nDesDay,nDesHour,nDesMinute,nDesSecond);
	   return sDateAndTime;
}

