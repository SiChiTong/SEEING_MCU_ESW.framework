/*
 * Xilinx.c
 *
 *  Created on: 2015-11-17
 *      Author: Administrator
 */


#include "Xilinx.h"

#define SE_XILINX_FPGA_ROM_HEAD_LENGTH			8

/*
 * the default function is that of verify FPGA ROM and get ROM size
 */
static SeInt8 SeXiFpgaCheckRom(SeUInt8* pRomHead, SeUInt32* nRomSize)
{
	SeUInt16 iMagicCode = 0;

	if(pRomHead == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(nRomSize == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	iMagicCode = ((*pRomHead) << 0)			\
				| ((*(pRomHead + 1)) << 8) 	\
				| ((*(pRomHead + 2)) << 16)	\
				| ((*(pRomHead + 3)) << 24);

	if(iMagicCode != 0x1970)
	{

		return SE_RETURN_ERROR;
	}

	*nRomSize = ((*(pRomHead + 4)) << 0)		\
				| ((*(pRomHead + 5)) << 8) 	\
				| ((*(pRomHead + 6)) << 16)	\
				| ((*(pRomHead + 7)) << 24);

	return SE_RETURN_OK;
}

SeInt8 SeConfigXilinxFpga(SeXilinxFpgaConfigOperation tOper)
{
	SeUInt16 nTimes;
	SeUInt32 nReaded;
	SeBool blPassFlag = SeFalse;
	SeUInt32 iPosition = 0;
	SeUInt8 pBuffer[SE_BUFFER_SIZE_256] = {0};
	SeUInt32 nRealRomSize = 0;
	SeUInt32 nLoadedRomSize = 0;

	if(tOper.fpSeXiFpgaCfgInit() == SE_RETURN_ERROR)
	{
		SeErrorPrint("Xilinx FPGA configure bus is not correct!");
		return SE_RETURN_ERROR;
	}

	/*
	 * notify FPGA that ROM will download
	 */
	tOper.fpSeXiFpgaClrProgb();

	tOper.fpSeXiFpgaSetProgb();

	/*
	 * read first block of ROM, include head information
	 */
	iPosition = 0;
	if(tOper.fpSeXiFpgaGetRomData(iPosition, pBuffer, SE_XILINX_FPGA_ROM_HEAD_LENGTH, &nReaded) != SE_RETURN_OK)
	{
		SeErrorPrint("Get Xilinx FPGA ROM data fail!");
		return SE_RETURN_ERROR;
	}
	iPosition += nReaded;

	//SeDataPrint("FPGA head data", pBuffer, SE_XILINX_FPGA_ROM_HEAD_LENGTH);

	/*
	 * check ROM head and get ROM size
	 */
	if(SeXiFpgaCheckRom(pBuffer, &nRealRomSize) != SE_RETURN_OK)
	{
		SeErrorPrint("Verify Xilinx FPGA ROM fail!");
		return SE_RETURN_ERROR;
	}

	if(nRealRomSize <= SE_XILINX_FPGA_ROM_HEAD_LENGTH)
	{
		SeErrorPrint("Invalid Xilinx FPGA ROM!");
		return SE_RETURN_ERROR;
	}

	/*
	 * wait for FPGA
	 */
	for(nTimes = 0; nTimes < CONFIG_FPGA_DOWNLOAD_TIMEOUT; nTimes++)
	{
		if(tOper.fpSeXiFpgaGetInitb() == SE_BIT_HIGH)
		{
			blPassFlag = SeTrue;
			break;
		}
	}
	if(blPassFlag == SeFalse)
	{
		SeErrorPrint("Wait for Xilinx FPGA ready signal timeout!");
		return SE_RETURN_ERROR;
	}

	/*
	 * start to download
	 */
	SeNormalPrint("Starting FPGA ROM download,total size:%d.", nRealRomSize);

	SeProgressStart("Loading FPGA...");
	while(nLoadedRomSize < nRealRomSize)
	{
		SeUInt32 iByteIndex;
		SeFloat fPercentage = 0.00f;

		if(tOper.fpSeXiFpgaGetRomData(iPosition, pBuffer, SE_BUFFER_SIZE_256, &nReaded) != SE_RETURN_OK)
		{
			SeErrorPrint("Get Xilinx FPGA ROM data fail!");
			return SE_RETURN_ERROR;
		}

		for(iByteIndex = 0; iByteIndex < nReaded; iByteIndex++)
		{
			SeUInt8 iBitIndex;
			SeUInt8 bData = pBuffer[iByteIndex];

			for(iBitIndex = 0; iBitIndex < 8; iBitIndex++)
			{
				tOper.fpSeXiFpgaClrClk();
				if((bData&0x80) == 0x80)
				{
					tOper.fpSeXiFpgaSetDout();
				}else{
					tOper.fpSeXiFpgaClrDout();
				}
				tOper.fpSeXiFpgaSetClk();
				bData <<= 1;
			}
		}

		iPosition += nReaded;
		nLoadedRomSize += nReaded;

		fPercentage = ((SeFloat)nLoadedRomSize/ (SeFloat)nRealRomSize);
		fPercentage = fPercentage*100;

		SeProgressPrint((SeUInt8)fPercentage, 3);
	}
	SeProgressFinish();

	/*
	 * wait FPGA done
	 */
	for(nTimes = 0; nTimes < CONFIG_FPGA_DOWNLOAD_TIMEOUT; nTimes++)
	{
		if(tOper.fpSeXiFpgaGetDone() == SE_BIT_HIGH)
		{
			blPassFlag = SeTrue;
			break;
		}
	}
	if(blPassFlag == SeFalse)
	{
		SeErrorPrint("Wait for Xilinx FPGA done signal timeout!");
		return SE_RETURN_ERROR;
	}

	/*
	 * reset FPGA
	 */
	tOper.fpSeXiFpgaClrRst();
	tOper.fpSeXiFpgaSetRst();

	SeNormalPrint("FPGA ready!");

	return SE_RETURN_OK;
}

