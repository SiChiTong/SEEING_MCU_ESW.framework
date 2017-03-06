/*
 * Arithmetic.c
 *
 *  Created on: 2016年1月7日
 *      Author: seeing
 */

#include "Arithmetic.h"

static const SeUInt16 aCrc16Table[256] = {
		 0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
		 0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
		 0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
		 0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
		 0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
		 0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
		 0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
		 0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
		 0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
		 0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
		 0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
		 0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
		 0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
		 0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
		 0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
		 0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
		 0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
		 0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
		 0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
		 0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
		 0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
		 0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
		 0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
		 0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
		 0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
		 0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
		 0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
		 0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
		 0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
		 0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
		 0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
		 0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

static SeUInt16 SeLargeNumberGetDataByteLen(SeUInt8 *pBuf, SeUInt8 nLength)
{
	SeInt8 iCount;

	for(iCount = nLength - 1;  iCount >= 0;  iCount--)
	{
		if(pBuf[iCount] != 0)
		{
			break;
		}
	}

	return iCount + 1;
}

//compare
//-1 : x<y
// 0 : x==y
// 1 : x>y
static SeUInt8 SeLargeNumberCompareFloat(SeLargeNumber tLargeNumberX, SeLargeNumber tLargeNumberY)
{
	SeInt8 iCount;
	SeUInt16 nLengthX, nLengthY;

	//polarity compare
	if(tLargeNumberX.nPolarity > tLargeNumberY.nPolarity)
	{
		return 1;
	}
	else if(tLargeNumberX.nPolarity < tLargeNumberY.nPolarity)
	{
		return -1;
	}

	//integer part compare
	nLengthX = SeLargeNumberGetDataByteLen(tLargeNumberX.pIntegerPart, tLargeNumberX.nIntegerPartLength);
	nLengthY = SeLargeNumberGetDataByteLen(tLargeNumberY.pIntegerPart, tLargeNumberY.nIntegerPartLength);

	if(nLengthX > nLengthY)
	{
		return 1;
	}
	else if(nLengthX < nLengthY)
	{
		return -1;
	}

	for(iCount = nLengthX - 1; iCount >= 0; iCount--)
	{
		if(tLargeNumberX.pIntegerPart[iCount] > tLargeNumberY.pIntegerPart[iCount])
		{
			return 1;
		}
		else if(tLargeNumberX.pIntegerPart[iCount] < tLargeNumberY.pIntegerPart[iCount])
		{
			return -1;
		}
	}

	if((tLargeNumberX.nPoint == tLargeNumberY.nPoint) &&
			(0 == tLargeNumberX.nPoint))
	{
		return 0;
	}
	else if(tLargeNumberX.nPoint > tLargeNumberY.nPoint)
	{
		return 1;
	}
	else if(tLargeNumberX.nPoint < tLargeNumberY.nPoint)
	{
		return -1;
	}

	//decimal part compare
	nLengthX = SeLargeNumberGetDataByteLen(tLargeNumberX.pDecimalPart, tLargeNumberX.nDecimalPartLength);
	nLengthY = SeLargeNumberGetDataByteLen(tLargeNumberY.pDecimalPart, tLargeNumberY.nDecimalPartLength);

	for(iCount=0; iCount < nLengthX && iCount < nLengthY; iCount++)
	{
		if(tLargeNumberX.pDecimalPart[iCount] > tLargeNumberY.pDecimalPart[iCount])
		{
			return 1;
		}
		else if(tLargeNumberX.pDecimalPart[iCount] < tLargeNumberY.pDecimalPart[iCount])
		{
			return -1;
		}
	}

	if(nLengthX == nLengthY)
	{
		return 0;
	}
	else if(nLengthX > nLengthY)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}


//compare integer
//-1 : x<y
// 0 : x==y
// 1 : x>y
static SeInt8 SeLargeNumberCompareInteger(SeUInt8 *ptLargeNumberX,SeUInt8 nLengthX,
		SeUInt8 *ptLargeNumberY, SeUInt8 nLengthY)
{
	SeInt8 iCount;

	//integer part compare

	if(nLengthX > nLengthY)
	{
		return 1;
	}
	else if(nLengthX < nLengthY)
	{
		return -1;
	}

	for(iCount = nLengthX - 1; iCount >= 0; iCount--)
	{
		if(ptLargeNumberX[iCount] > ptLargeNumberY[iCount])
		{
			return 1;
		}
		else if(ptLargeNumberX[iCount] < ptLargeNumberY[iCount])
		{
			return -1;
		}
	}

	return 0;
}


//Integer
//x=x+y
//>=0,back, back_x size
//<0,error
SeInt8 SeLargeNumberAdd(SeUInt8 *pX, SeUInt8 nSizeX, SeUInt8 *pY, SeUInt8 nSizeY)
{
	SeUInt8 nLengthX, nLengthY, nLength;
	SeUInt8 nFlag = 0;
	SeUInt16 nTemp;
	SeUInt64 nTempX, nTempY;
	SeInt8 iCount, iRet;

	if(nSizeX < nSizeY)
	{
		return -1;
	}

	//get true byte len
	nLengthX = SeLargeNumberGetDataByteLen(pX, nSizeX);
	nLengthY = SeLargeNumberGetDataByteLen(pY, nSizeY);

	nLength = (nLengthX >= nLengthY) ? nLengthX : nLengthY;

	if(nLength <= 7)
	{
		nTempX =0;
		nTempY = 0;

		for(iCount = nLengthX - 1; iCount >= 0; iCount--)
		{
			nTempX = nTempX << 8;
			nTempX |= pX[iCount];
		}

		for(iCount = nLengthY - 1; iCount >= 0; iCount--)
		{
			nTempY = nTempY<<8;
			nTempY |= pY[iCount];
		}

		nTempX += nTempY;

		for(iCount = 0; iCount < nLength + 1; iCount++)
		{
			pX[iCount] = nTempX & 0xff;
			nTempX = nTempX >> 8;
		}

		iRet = SeLargeNumberGetDataByteLen(pX, nLength + 1);
		return (iRet > 0) ? iRet : 1;
	}

	nFlag = 0;
	nTemp = 0;
	for(iCount = 0; iCount < nLength; iCount++)
	{
		nTemp = pX[iCount] + pY[iCount];
		nTemp += nFlag;
		if(nTemp >= 0x100)
		{
			nFlag = 1;
			pX[iCount] = nTemp - 0x100;
		}
		else
		{
			pX[iCount] = nTemp;
			nFlag = 0;
		}
	}
	pX[iCount++] += nFlag;

	iRet = SeLargeNumberGetDataByteLen(pX,iCount);
	return (iRet > 0) ? iRet : 1;
}

//x = x-y
//>=0,back, ,x>y
//<0,x<y,back_x size
SeInt8 SeLargeNumberSub(SeUInt8 *pX, SeUInt8 nSizeX, SeUInt8 *pY, SeUInt8 nSizeY)
{
	SeUInt8 nLengthX, nLengthY, nLength, nLengthMax;
	SeUInt8 nFlag=0;
	SeUInt16 nTemp;
	SeUInt8 *pTempX, *pTempY;
	SeUInt64 nTempX,nTempY;
	SeInt8 iCount,iRet;

	if(nSizeX < nSizeY)
	{
		return -1;
	}

	//get true byte len
	nLengthX = SeLargeNumberGetDataByteLen(pX, nSizeX);
	nLengthY = SeLargeNumberGetDataByteLen(pY, nSizeY);

	iRet = SeLargeNumberCompareInteger(pX, nLengthX, pY, nLengthY);
	if(0 == iRet)
	{
		return 1;
	}
	else if(iRet > 0)
	{
		nLength = nLengthY;
		nLengthMax = nLengthX;
		pTempX = pX;
		pTempY = pY;
	}
	else
	{
		nLength = nLengthX;
		nLengthMax = nLengthY;
		pTempX = pY;
		pTempY = pX;
	}

	if(nLengthMax <= 8)
	{
		nTempX =0;
		nTempY = 0;
		for(iCount = nLengthX - 1; iCount >= 0; iCount--)
		{
			nTempX = nTempX<<8;
			nTempX |= pX[iCount];
		}
		for(iCount = nLengthY - 1; iCount >= 0; iCount--)
		{
			nTempY = nTempY << 8;
			nTempY |= pY[iCount];
		}

		if(nTempX >= nTempY)
		{
			nFlag = 1;
    		nTempX -= nTempY;
		}
		else
		{
			nFlag = 0;
    		nTempX = nTempY - nTempX;
		}

		for(iCount = 0; iCount < nLengthMax; iCount++)
		{
			pX[iCount] = nTempX & 0xff;
			nTempX = nTempX >> 8;
		}

		iRet =SeLargeNumberGetDataByteLen(pX, nLengthMax);
		if(nFlag > 0)
		{
			return (iRet > 0) ? iRet : 1;
		}
		else
		{
			iRet = (0 - iRet);
			return (iRet > 0) ? iRet : 1;
		}
	}

	nTemp = 0;
	nFlag=0;
	for(iCount = 0; iCount < nLength; iCount++)
	{
		if(pTempX[iCount] >= nFlag)
		{
			nTemp = pTempX[iCount] - nFlag;
			nFlag = 0;
		}
		else
		{
			nTemp = pTempX[iCount] + 0x100 - nFlag;
			nFlag = 1;
		}
		if(nTemp >= pTempY[iCount])
		{
			nTemp = nTemp - pTempY[iCount];
		}
		else
		{
			nTemp = nTemp + 0x100 - pTempY[iCount];
			nFlag = 1;
		}

		pX[iCount] = nTemp;
	}

	for(; iCount < nLengthMax; iCount++)
	{
		if(pTempX[iCount] >= nFlag)
		{
			pX[iCount] = pTempX[iCount] - nFlag;
			nFlag = 0;
		}
		else
		{
			pX[iCount] = pTempX[iCount] + 0x100 - nFlag;
			nFlag = 1;
		}
	}

	iRet = SeLargeNumberGetDataByteLen(pX, nSizeX);
	return (iRet > 0) ? iRet : 1;
}

//x=x*y
//>=0,back, back_x size
//<0,error
#define BIG_MUL_SIZE	32
SeUInt32 aBigBuf[BIG_MUL_SIZE][BIG_MUL_SIZE]={0};

SeInt8 SeLargeNumberMul(SeUInt8 *pX,SeUInt8 nSizeX, SeUInt8 *pY, SeUInt8 nSizeY)
{

	SeUInt8 nLengthX, nLengthY, nLength;
	SeUInt16 nFlag=0;
	SeUInt8 nTemp;
	SeUInt64 nTempX,nTempY;
	SeInt8 iCount, nX, nY, iRet;

	//get true byte len
	nLengthX = SeLargeNumberGetDataByteLen(pX, nSizeX);
	nLengthY = SeLargeNumberGetDataByteLen(pY, nSizeY);

	//count
	nLength = nLengthX + nLengthY;
	if((nLength > BIG_MUL_SIZE)||(nLengthX > (nSizeX - 1)))
	{
		//buf too little
		return -1;
	}

	if(nLength <= 8)
	{
		nTempX =0;
		nTempY = 0;
		for(iCount = nLengthX - 1; iCount >= 0; iCount--)
		{
			nTempX = nTempX << 8;
			nTempX |= pX[iCount];
		}
		for(iCount = nLengthY - 1; iCount >= 0; iCount--)
		{
			nTempY = nTempY<<8;
			nTempY |= pY[iCount];
		}

		nTempX *=nTempY;

		for(iCount = 0; iCount < nLength; iCount++)
		{
			pX[iCount] = nTempX & 0xff;
			nTempX = nTempX >> 8;
		}

		iRet = SeLargeNumberGetDataByteLen(pX, nLength);
		return (iRet > 0) ? iRet : 1;
	}

	memset(aBigBuf, 0, sizeof(aBigBuf));
	//	An...A2A1A0*Bn...B2B1B0
	for(nY = 0; nY < nLengthY; nY++)
	{
		for(nX = 0; nX < nLengthX; nX++)
		{
			aBigBuf[nX][nY]=pX[nX] * pY[nY];
		}
	}

	nFlag = 0;
	for(iCount = 0; iCount < nLength; iCount++)
	{
		for(nY = 0, nX = iCount; nY < iCount && iCount > 0; nY++, nX--)
		{
			aBigBuf[0][iCount] += aBigBuf[nX][nY];
		}

		nTemp = aBigBuf[0][iCount] % 0x100;
		nFlag = aBigBuf[0][iCount] / 0x100;

		aBigBuf[0][iCount + 1] += nFlag;
		pX[iCount] = nTemp;
	}

	pX[iCount] = aBigBuf[0][iCount];

	iRet = SeLargeNumberGetDataByteLen(pX,iCount + 1);
	return (iRet > 0) ? iRet : 1;
}

#define DIV_BUF_LEN		32
SeInt8 SeLargeNumberGetDiv(SeUInt8 *pDiv, SeUInt8 nDivIndex,
		SeUInt8 nCount, SeUInt8 *pBuf, SeUInt8 nBufIndex)
{
	SeInt8 iCount = 0, iTemp;

	//shift
	for(iCount = nDivIndex - 1; iCount >= 0; iCount--)
	{
		pDiv[iCount + nCount] = pDiv[iCount];
	}

	//get value
	for(iCount = nCount - 1, iTemp = 1; iCount >= 0; iCount--, iTemp++)
	{
		pDiv[iCount] = pBuf[nBufIndex - iTemp];
	}

	return SeLargeNumberGetDataByteLen(pDiv, DIV_BUF_LEN);
}


//x=x%y,q=x/y
SeInt8 SeLargeNumberDiv(SeUInt8 *pX, SeUInt8 nSizeX,
		SeUInt8 *pY, SeUInt8 nSizeY,
		SeUInt8 *pQ, SeUInt8 *nSizeQ)
{
	SeUInt8 nLengthX, nLengthY, nLength = 0;
	SeUInt8 aDiv[DIV_BUF_LEN] = {0};
	SeUInt8 aQuotient[DIV_BUF_LEN] = {0};
	SeUInt8 nTempIndex, nDivIndex, nQuotientCount;
	SeUInt64 nTempX,nTempY, nTempQ;
	SeInt8 iCount, iTemp, iRet;


	//get true byte len
	nLengthX = SeLargeNumberGetDataByteLen(pX, nSizeX);
	nLengthY = SeLargeNumberGetDataByteLen(pY, nSizeY);

	if(0 == nLengthY)
	{
		return -1;
	}
	//count
	nLength = nLengthX + 1 - nLengthY;
	if(*nSizeQ < nLength)
	{
		//quotient buf too little
		return -2;
	}

	iRet = SeLargeNumberCompareInteger(pX, nLengthX, pY, nLengthY);
	if(iRet < 0)
	{
		*nSizeQ = 1;
		return (nLengthX > 0) ? nLengthX : 1;
	}
	else if(0 == iRet)
	{
		pQ[0] = 1;
		*nSizeQ = 1;
		return 1;
	}

	if(nLengthX <= 8)
	{
		nTempX =0;
		nTempY = 0;
		for(iCount = nLengthX - 1; iCount >= 0; iCount--)
		{
			nTempX = nTempX << 8;
			nTempX |= pX[iCount];
		}
		for(iCount = nLengthY - 1; iCount >= 0; iCount--)
		{
			nTempY = nTempY << 8;
			nTempY |= pY[iCount];
		}

		nTempQ = (SeUInt64)(nTempX / nTempY);
		for(iCount = 0; iCount < nLength; iCount++)
		{
			// TODO luanma
			pQ[iCount] = nTempQ & 0xff;
			nTempQ = nTempQ >> 8;
		}

		iRet = SeLargeNumberGetDataByteLen(pQ, nLength);
		*nSizeQ = (iRet > 0) ? iRet : 1;

		nTempX = nTempX % nTempY;

		memset(pX, 0, nSizeX);
		for(iCount = 0; iCount < nLengthX; iCount++)
		{
			// TODO luanma
			pX[iCount] = nTempX & 0xff;
			nTempX = nTempX >> 8;
		}

		iRet = SeLargeNumberGetDataByteLen(pX, nLengthX);
		return (iRet > 0) ? iRet : 1;

	}

	nDivIndex = 0;
	nQuotientCount = 0;
	nTempIndex = nLengthX;
	iRet = SeLargeNumberGetDiv(aDiv, nDivIndex, nLengthY, pX, nLengthX);
	nTempIndex -= nLengthY;
	nDivIndex = iRet;

	while(1)
	{
		iRet = SeLargeNumberCompareInteger(aDiv, nDivIndex, pY, nLengthY);
		if(iRet < 0)
		{
			aQuotient[nQuotientCount++] += 0;
			if(nTempIndex > 0)
			{
				if(nDivIndex < DIV_BUF_LEN)
				{
					iRet = SeLargeNumberGetDiv(aDiv, nDivIndex, 1, pX, nTempIndex);
					nDivIndex = iRet;
					nTempIndex--;
				}

				continue;
			}
			else
			{
				break;
			}
		}


		aQuotient[nQuotientCount]  += 1;
		nDivIndex = SeLargeNumberSub(aDiv, nDivIndex, pY, nLengthY);
	}

	for(iCount = nQuotientCount - 1, iTemp = 0; iCount >= 0 && iTemp < nQuotientCount; iCount--, iTemp++)
	{
		pQ[iCount] = aQuotient[iTemp];
	}

	iRet = SeLargeNumberGetDataByteLen(pQ, nQuotientCount);
	*nSizeQ = (iRet > 0) ? iRet : 1;

	memset(pX, 0,nSizeX);
	nDivIndex = SeLargeNumberGetDataByteLen(aDiv, DIV_BUF_LEN);
	memcpy(pX, aDiv, nDivIndex);

	return nDivIndex > 0 ? nDivIndex : 1;
}

SeUInt16 SeCrc16Ccitt(SeInt8* pData, SeUInt16 nLength)
{
	register SeUInt16 iCount;
	register SeUInt16 hCrc = 0;
	for( iCount = 0; iCount < nLength; iCount++)
		hCrc = (hCrc<<8) ^ aCrc16Table[((hCrc>>8) ^ *(SeInt8*)pData++)&0x00FF];
	return hCrc;
}
