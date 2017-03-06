#include "gtest/gtest.h"
#include "Include.h"

TEST(TestApplicationFpga, FrequencyLsmAlgorithm)
{
	//ASSERT_EQ(SE_RETURN_OK, SeFpgaGetPulseWidthMeasureResult(0,0));
}

TEST(TestApplicationFpga, GetAmplitudeMeasureResult)
{
	SeUInt8 iIndex;
	SeUInt8 nChannel = 1;
	SeUInt8 aVppSum[8], aVppCycleCount[8];
	SeFpgaSpiRegisterType16Bit nCheckVpp;
	SeUInt16 nPage = nChannel << 8;
	SeUInt16 nAddr = nPage | FPGA_AMPLITUDE_CHECK_VPP_REG;


	nCheckVpp.hData = 1234;
	SeFpgaWrite16BitRegister(nAddr, nCheckVpp);
	nCheckVpp.hData = 0;
	SeFpgaRead16BitRegister(nAddr, &nCheckVpp);
	ASSERT_EQ(nCheckVpp.hData, 1234);

	for(iIndex = 0; iIndex < 8; iIndex++)
	{
		aVppSum[iIndex] = 1 + iIndex;
		aVppCycleCount[iIndex] = 2 + iIndex;
 	}

	nAddr = nPage | FPGA_AMPLITUDE_VPP_SUM_REG;
	SeFpgaWriteDatas(nAddr, aVppSum, 8);
	SeFpgaReadDatas(nAddr, aVppCycleCount, 8);

	for(iIndex = 0; iIndex < 8; iIndex++)
	{
		ASSERT_EQ(aVppCycleCount[iIndex], 1 + iIndex);
	}
}

