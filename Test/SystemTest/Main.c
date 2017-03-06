#include "../../Source/Global/Include.h"
#include "../SystemTest/MemTest.h"
SeInt8 main(void)
{
	SeBool blSuccess = SeTrue;
	SeUInt32 i = 0;
	if(SeSystemInit() == SE_RETURN_ERROR)
	{
		blSuccess = SeFalse;
	}
	SeNormalPrint("Memory test\r\n");
	for(;;i++)
	{
		SeNormalPrint("heap usage:%d",RecordMemSizeUse);
		if( SeMemTest( i ) < 0 )
		{
			SeNormalPrint("FAIL\r\n");
			break;
		}
		else
		{
			SeNormalPrint("OK\r\n");
		}
		SeDelayMs(500);
	}
	while(SeTrue)
	{

	}

	return 0;
}
