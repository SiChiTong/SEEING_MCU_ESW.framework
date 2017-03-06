/*
 * Xmodem.c
 *
 *  Created on: 2016年2月18日
 *      Author: seeing
 */

#include "Xmodem.h"

/*
 * XMODEM protocol keywords
 */
#define XMODEM_SOH					0x01
#define XMODEM_STX					0x02
#define XMODEM_EOT					0x04
#define XMODEM_ACK					0x06
#define XMODEM_NAK					0x15
#define XMODEM_CAN					0x18
#define XMODEM_CTRLZ				0x1A

#define XMODEM_START				'C'

#define XMODEM_RETRY_TIMES	32

static SeXmodemDescription tXmodemDesc;
static SeUInt8 aXmodemReceiveBuffer[1030] = {0};
static SeBool blGetByteError = SeTrue;

/*
static SeUInt8 SeXmodemGetByte(void)
{
	SeUInt8 pChar;
	SeUInt32 timeout = 0;

	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{
		timeout++;
		if(timeout == 0x600000) //1s
		{
			blGetByteError = SeTrue;
			return 0;
		}
	}

	blGetByteError = SeFalse;
	pChar = USART_ReceiveData(USART1);
	return pChar;
}

static void SeXmodemPutByte(SeUInt8 bData)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{}
	USART_SendData(USART1, (SeUInt8) bData);
}
*/

static SeUInt8 SeXmodemGetByte(void)
{
	SeUInt8 pChar;
	SeUInt32 timeout = 0;

	while (SeTrue)
	{
		SeInt16 nGot;
		SeUartRead(tXmodemDesc.iXmodemUartIndex, &pChar, 1, &nGot);
		if(nGot == 0)
		{
			timeout++;
			if(timeout == 0x600000) //1s
			{
				blGetByteError = SeTrue;
				return 0;
			}
		}
	}

	blGetByteError = SeFalse;
	return pChar;
}

static void SeXmodemPutByte(SeUInt8 bData)
{
	SeInt16 nSet;
	SeUartWrite(tXmodemDesc.iXmodemUartIndex, &bData, 1, &nSet);
}

static SeBool SeXmodemCheckData(SeBool blUseCrc16, SeUInt8* pData, SeUInt16 nLength)
{
	if(blUseCrc16)
	{
		SeUInt16 hCalCrc = SeCrc16Ccitt((SeInt8*)pData, nLength);
		SeUInt16 hTarCrc = ((SeUInt16)pData[nLength] << 8) + pData[nLength + 1];
		if(hCalCrc != hTarCrc)
		{
			return SeFalse;
		}
	}
	else
	{
		SeUInt16 iCount;
		SeUInt8 hChecksum = 0;

		for(iCount = 0; iCount < nLength; ++iCount)
		{
			hChecksum += *(pData + iCount);
		}

		if(hChecksum != pData[nLength] )
		{
			return SeFalse;
		}
	}

	return SeTrue;
}

SeInt8 SeXmodemConfig(SeXmodemDescription tXmodemDescription)
{
	tXmodemDesc = tXmodemDescription;
	SeNormalPrint("Xmodem configure success! %d",tXmodemDesc.iXmodemUartIndex);

	return SE_RETURN_OK;
}


SeInt8 SeXmodemReceive(SeUInt32 nBytesToReceive, SeUInt32* nBytesReceived,
		SeUInt32 nAddress, SeXmodemWriteData fpSeXmodemWriteData)
{
	SeBool blUseStartChar = SeTrue;
	SeBool blIsReceiving = SeFalse;
	SeBool blUseCrc16;
	SeUInt16 iRetry;
	SeUInt8 bRcvByte;
	SeUInt8 iPackIndex = 1;
	SeUInt32 nRcvBytes = 0;
	SeUInt16 iDataIndex = 0;
	SeUInt16 nPackSize;
	SeUInt8 nTempComplement;

	SeUInt8* pBufPtr = SeNull;

	SeUInt8 bStartByte = XMODEM_START;

	SeUInt8 iCount;
	if(fpSeXmodemWriteData == SeNull)
	{
		SeErrorPrint("Xmodem description is not correct!Maybe write function doesn't setup!");
		return SE_RETURN_ERROR;
	}

	memset(aXmodemReceiveBuffer, 0, sizeof(aXmodemReceiveBuffer));

	while(SeTrue)
	{

		for(iRetry = 0; iRetry < XMODEM_RETRY_TIMES; iRetry++)
		{
			if(blIsReceiving == SeFalse)
			{
				SeXmodemPutByte(bStartByte);
			}

			bRcvByte = SeXmodemGetByte();
			if(blGetByteError)
			{
				continue;
			}

			switch(bRcvByte)
			{
				case XMODEM_SOH:
					nPackSize = 128;
					goto XmodemReceiveLable;
				case XMODEM_STX:
					nPackSize = 1024;
					goto XmodemReceiveLable;
				case XMODEM_EOT:
					SeXmodemPutByte(XMODEM_ACK);
					*nBytesReceived = nRcvBytes;
					return SE_RETURN_OK;
				case XMODEM_CAN:
					bRcvByte = SeXmodemGetByte();
					if(bRcvByte == XMODEM_CAN)
					{
						SeXmodemPutByte(XMODEM_ACK);
						return SE_RETURN_ERROR;
					}
					break;
				default:
					break;
			}
		}

		SeXmodemPutByte(XMODEM_CAN);
		SeXmodemPutByte(XMODEM_CAN);
		SeXmodemPutByte(XMODEM_CAN);
		return SE_RETURN_ERROR;

XmodemReceiveLable:
		blIsReceiving = SeTrue;

		if(blUseStartChar)
			blUseCrc16 = SeTrue;
		else
			blUseCrc16 = SeFalse;

		pBufPtr = aXmodemReceiveBuffer;
		*pBufPtr++ = bRcvByte;
		for(iDataIndex = 0; iDataIndex < (nPackSize + (blUseCrc16?1:0) + 3); ++iDataIndex)
		{
			bRcvByte = SeXmodemGetByte();
			if(blGetByteError)
			{
				goto XmodemRejectLable;
			}
			*pBufPtr++ = bRcvByte;
		}

		nTempComplement  = ~aXmodemReceiveBuffer[2];
		if((aXmodemReceiveBuffer[1] == nTempComplement) &&
				((aXmodemReceiveBuffer[1] == iPackIndex) || (aXmodemReceiveBuffer[1] == (iPackIndex - 1))) &&
				(SeXmodemCheckData(blUseCrc16, &aXmodemReceiveBuffer[3], nPackSize)))
		{
			if(aXmodemReceiveBuffer[1] == iPackIndex)
			{
				SeUInt32 nPayloadLength;
				nPayloadLength = nBytesToReceive - nRcvBytes;

				if(nPayloadLength > nPackSize)
				{
					nPayloadLength = nPackSize;
				}
				if(nPayloadLength > 0)
				{

					if(fpSeXmodemWriteData(nAddress, &aXmodemReceiveBuffer[3], nPayloadLength) == SE_RETURN_OK)
					{
						nAddress += nPayloadLength;
						nRcvBytes += nPayloadLength;
					}
					else
					{
						return SE_RETURN_ERROR;
					}
				}
				++iPackIndex;
			}

			SeXmodemPutByte(XMODEM_ACK);
			continue;
		}

XmodemRejectLable:
		SeXmodemPutByte(XMODEM_NAK);
	}

	return SE_RETURN_OK;
}
