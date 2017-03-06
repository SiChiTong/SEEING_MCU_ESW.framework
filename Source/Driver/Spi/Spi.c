

#include "Spi.h"
#include "Internal.h"
#include "Simulate.h"
#include "Quad.h"

static SeSpiBusDescription* pSpiBuses[CONFIG_MAX_SPI_PERIPHERALS] = {0};

SeInt8 SeSpiInit(SeInt8 iIndex, SeSpiBusDescription* pSpi)
{
	SeInt8 iResult;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_SPI_PERIPHERALS - 1))
	{
		return SE_RETURN_ERROR;
	}

	if(pSpi == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(pSpi->pOperation == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	switch(pSpi->iType)
	{
		case SE_SPI_BUS_TYPE_INTERNAL:
			iResult = SeInternalSpiInit(pSpi->pOperation);
			break;
		case SE_SPI_BUS_TYPE_SIMULATE:
			iResult = SeSimulateSpiInit(pSpi->pOperation);
			break;
		case SE_SPI_BUS_TYPE_QUAD:
			iResult = SeQuadSpiInit(pSpi->pOperation);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	pSpiBuses[iIndex] = pSpi;

	pSpiBuses[iIndex]->blInited = SeTrue;

	return SE_RETURN_OK;
}

SeInt8 SeSpiRead(SeInt8 iIndex, SeUInt8* bData)
{
	SeInt8 iResult;
	SeSpiBusDescription* pSpi;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_SPI_PERIPHERALS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pSpiBuses[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	if(bData == SeNull)
	{

		return SE_RETURN_ERROR;
	}

	pSpi = pSpiBuses[iIndex];

	if(pSpi->blInited == SeFalse)
	{
		return SE_RETURN_ERROR;
	}

	switch(pSpi->iType)
	{
		case SE_SPI_BUS_TYPE_INTERNAL:
			iResult = SeInternalSpiRead(pSpi->pOperation, bData);
			break;
		case SE_SPI_BUS_TYPE_SIMULATE:
			iResult = SeSimulateSpiRead(pSpi->pOperation, bData);
			break;
		case SE_SPI_BUS_TYPE_QUAD:
			iResult = SeQuadSpiRead(pSpi->pOperation, bData);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}

SeInt8 SeSpiWrite(SeInt8 iIndex, SeUInt8 bData)
{
	SeInt8 iResult;
	SeSpiBusDescription* pSpi;
	if(iIndex < 0 || iIndex > (CONFIG_MAX_SPI_PERIPHERALS - 1))
	{

		return SE_RETURN_ERROR;
	}

	if(pSpiBuses[iIndex] == SeNull)
	{
		return SE_RETURN_ERROR;
	}

	pSpi = pSpiBuses[iIndex];

	if(pSpi->blInited == SeFalse)
	{
		return SE_RETURN_ERROR;
	}

	switch(pSpi->iType)
	{
		case SE_SPI_BUS_TYPE_INTERNAL:
			iResult = SeInternalSpiWrite(pSpi->pOperation, bData);
			break;
		case SE_SPI_BUS_TYPE_SIMULATE:
			iResult = SeSimulateSpiWrite(pSpi->pOperation, bData);
			break;
		case SE_SPI_BUS_TYPE_QUAD:
			iResult = SeQuadSpiWrite(pSpi->pOperation, bData);
			break;
		default:
			iResult = SE_RETURN_ERROR;
			break;
	}

	if(iResult == SE_RETURN_ERROR)
	{
		return SE_RETURN_ERROR;
	}

	return SE_RETURN_OK;
}
