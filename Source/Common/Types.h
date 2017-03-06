


#ifndef _TYPES_H_
#define _TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * define basic variant types
 */
typedef signed char SeInt8;
typedef unsigned char SeUInt8;
typedef signed short SeInt16;
typedef unsigned short SeUInt16;
typedef signed long SeInt32;
typedef unsigned long SeUInt32;
typedef signed long long SeInt64;
typedef unsigned long long SeUInt64;
typedef float SeFloat;
typedef double SeDouble;
typedef char* SeString;
typedef char SeChar;
typedef const SeString SeConstString;

#define SeNull ((void*)0)

/*
 * define ip address
 */
typedef struct
{
	SeUInt8 aIpAddress[4];
}SeIpAddress;

typedef SeUInt16 SeIpPort;

/*
 * define MAC address
 */
typedef struct
{
	SeUInt8 aMacAddress[6];
}SeMac;

/*
 * define boolean values
 */
typedef enum
{
	SeFalse = 0,
	SeTrue = 1
}SeBool;

typedef struct
{
	SeUInt64 iUtcSeconds;
	SeUInt64 iMicroSeconds;
}SeTimeValue;

/*
 * define bit logic
 */
#define SE_BIT_HIGH			1
#define SE_BIT_LOW			0

/*
 * define function return value
 */
#define SE_RETURN_OK				1
#define SE_RETURN_ERROR				0
#define SE_RETURN_TIMEOUT			-1

/*
 * define data buffer common sizes
 */
#define SE_BUFFER_SIZE_8	8
#define SE_BUFFER_SIZE_16	16
#define SE_BUFFER_SIZE_32	32
#define SE_BUFFER_SIZE_64	64
#define SE_BUFFER_SIZE_128	128
#define SE_BUFFER_SIZE_256	256
#define SE_BUFFER_SIZE_512	512
#define SE_BUFFER_SIZE_1024	1024

/*
 * define system standard I/O redir function
 */
typedef struct
{
	SeInt8 (*fpSeStdioInit)();
	SeInt8 (*fpSeFputc)(SeUInt8 bChar);
	SeInt8 (*fpSeFgetc)(SeUInt8* bChar);
}SeStdioOperation;

/*
 * define tcp/udp connections type
 */
typedef enum
{
	SeNetworkTcpConnection = 1,
	SeNetworkUdpConnection,
}SeNetworkConnectionType;

typedef struct
{
	SeMac tDeviceMac;
	SeIpAddress tDefaultIp;
	SeIpAddress tDefaultMask;
	SeIpAddress tDefaultGate;
}SeNetworkDescription;

typedef struct
{
	SeInt8 (*fpSeNetIfInit)(SeNetworkDescription* pNetworkDescription);
}SeNetworkOperation;

/*
 * network connection description
 */
typedef struct
{
	SeIpAddress tLocalIp;
	SeIpPort tLocalPort;
	SeIpAddress tRemoteIp;
	SeIpPort tRemotePort;
	SeNetworkConnectionType tConnectionType;
}SeNetworkConnectionDescription;

/*
 * define SPI peripheral operation base structure
 */
typedef struct
{
	SeUInt8 iType;
	SeBool blInited;
	void* pOperation;
}SeSpiBusDescription;

/*
 * define SPI bus types
 */
#define SE_SPI_BUS_TYPE_INTERNAL		1
#define SE_SPI_BUS_TYPE_SIMULATE		2
#define SE_SPI_BUS_TYPE_QUAD			3

/*
 * define internal SPI bus operation base structure
 */
typedef struct
{
	SeInt8 (*fpSeInternalSpiPreInit)(void);
	SeInt8 (*fpSeInternalSpiRead)(SeUInt8* bData);
	SeInt8 (*fpSeInternalSpiWrite)(SeUInt8 bData);
	void (*fpSeInternalSpiSetCs)(void);
	void (*fpSeInternalSpiClrCs)(void);
}SeInternalSpiOperation;

/*
 * define simulate SPI bus operation base structure
 */
typedef struct
{
	SeInt8 (*fpSeSimulateSpiPreInit)(void);
	void (*fpSeSimulateSpiSetCs)(void);
	void (*fpSeSimulateSpiClrCs)(void);
	void (*fpSeSimulateSpiSetScl)(void);
	void (*fpSeSimulateSpiClrScl)(void);
	void (*fpSeSimulateSpiSetSdo)(void);
	void (*fpSeSimulateSpiClrSdo)(void);
	SeUInt8 (*fpSeSimulateSpiGetSdi)(void);
}SeSimulateSpiOperation;

/*
 * define quad SPI bus operation base structure
 */
typedef union
{
	struct
	{
		SeUInt8 D0:1;
		SeUInt8 D1:1;
		SeUInt8 D2:1;
		SeUInt8 D3:1;
		SeUInt8 :4;
	}Bit;
	SeUInt8 Byte;
}SeQuadSpiData;

typedef struct
{
	SeInt8 (*fpSeQuadSpiPreInit)(void);
	void (*fpSeQuadSpiSetCs)(void);
	void (*fpSeQuadSpiClrCs)(void);
	void (*fpSeQuadSpiSetScl)(void);
	void (*fpSeQuadSpiClrScl)(void);
	void (*fpSeQuadSpiSetSdo)(SeQuadSpiData tData);
	SeQuadSpiData (*fpSeQuadSpiGetSdi)(void);
}SeQuadSpiOperation;


/*
 * define SPI Slave description
 */
typedef struct
{
	SeBool blAllocated;
	SeUInt8* pTransmitBuffer;
	SeInt16 nTransmitBufferLength;
	SeUInt8* pReceiveBuffer;
	SeInt16 nReceiveBufferLength;
}SeSpiSlaveBufferDescription;

typedef struct
{
	void (*fpSeSpiSlaveReceiveRequest)(SeUInt8* pRequestData, SeInt16 nRequestLength);
	void (*fpSeSpiSlaveMakeResponse)(SeUInt8* pResponseData, SeInt16* nResponseLength);
	void (*fpSeSpiSlaveLaterProcess)(void);
}SeSpiSlaveCallback;

typedef struct
{
	SeInt8 (*fpSeSpiSlavePreInit)(SeSpiSlaveBufferDescription tBufferDesc, SeSpiSlaveCallback tCallbacks);
}SeSpiSlaveOperation;

typedef struct
{
	SeBool blInited;
	SeSpiSlaveBufferDescription tBufferDescription;
	SeSpiSlaveOperation tOperation;
	SeSpiSlaveCallback tCallbacks;
}SeSpiSlaveDescription;

/*
 * define all type FPGA register types,Seeing stipulate 8bit,16bit,24bit,32bit throw spi bus
 */
typedef SeUInt8 SeFpgaSpiRegisterType8Bit;

typedef union
{
	SeUInt8 aData[2];
	SeUInt16 hData;
}SeFpgaSpiRegisterType16Bit;

typedef union
{
	SeUInt8 aData[3];
	SeUInt32 hData;
}SeFpgaSpiRegisterType24Bit;

typedef union
{
	SeUInt8 aData[4];
	SeUInt32 hData;
}SeFpgaSpiRegisterType32Bit;

typedef union
{
	SeUInt8 aData[6];
	SeUInt64 hData;
}SeFpgaSpiRegisterType48Bit;

typedef union
{
	SeUInt8 aData[8];
	SeUInt64 hData;
}SeFpgaSpiRegisterType64Bit;

/*
 * define Xilinx FPGA program I/O operation base structure
 */
typedef struct
{
	SeInt8 (*fpSeXiFpgaCfgInit)(void);
	SeInt8 (*fpSeXiFpgaGetRomData)(SeUInt32 iSeekSourcePosition, SeUInt8* pDestination, SeUInt32 nBytesToRead, SeUInt32* nBytesReaded);
	void (*fpSeXiFpgaSetProgb)(void);
	void (*fpSeXiFpgaClrProgb)(void);
	SeUInt8 (*fpSeXiFpgaGetInitb)(void);
	void (*fpSeXiFpgaSetCs)(void);
	void (*fpSeXiFpgaClrCs)(void);
	void (*fpSeXiFpgaSetRst)(void);
	void (*fpSeXiFpgaClrRst)(void);
	void (*fpSeXiFpgaSetClk)(void);
	void (*fpSeXiFpgaClrClk)(void);
	SeUInt8 (*fpSeXiFpgaGetDone)(void);
	SeUInt8 (*fpSeXiFpgaGetDin)(void);
	void (*fpSeXiFpgaSetDout)(void);
	void (*fpSeXiFpgaClrDout)(void);
}SeXilinxFpgaConfigOperation;


/*
 * define IIC peripheral operation base structure
 */
typedef struct
{
	SeUInt8 iType;
	SeBool blInited;
	void* pOperation;
}SeI2cBusDescription;

/*
 * define IIC bus types
 */
#define SE_I2C_BUS_TYPE_INTERNAL		1
#define SE_I2C_BUS_TYPE_SIMULATE		2

/*
 * define internal I2C bus operation base structure
 */
typedef struct
{
	SeInt8 (*fpSeInternalI2cPreInit)(void);
	SeInt8 (*fpSeInternalI2cRead)(SeUInt8* bData, SeBool blAck);
	SeInt8 (*fpSeInternalI2cWrite)(SeUInt8 bData, SeBool* blAck);
	SeInt8 (*fpSeInternalI2cStart)(void);
	SeInt8 (*fpSeInternalI2cStop)(void);
	SeInt8 (*fpSeInternalI2cAck)(void);
	SeInt8 (*fpSeInternalI2cNack)(void);
}SeInternalI2cOperation;

/*
 * define simulate I2C operation base structure
 */
typedef struct
{
	SeInt8 (*fpSeSimulateI2cPreInit)(void);
	SeUInt8 (*fpSeSimulateI2cGetSda)(void);
	void (*fpSeSimulateI2cClrScl)(void);
	void (*fpSeSimulateI2cSetScl)(void);
	void (*fpSeSimulateI2cClrSda)(void);
	void (*fpSeSimulateI2cSetSda)(void);
}SeSimulateI2cOperation;

/*
 * define GPIO operation base structure
 */
typedef struct
{
	SeBool blInited;
	void* pOperation;
}SeGpioDescription;

typedef struct
{
	SeInt8 (*fpSeGpioInit)(void);
	void (*fpSeGpioSetPin)(void);
	void (*fpSeGpioClrPin)(void);
	SeUInt8 (*fpSeGpioGetPin)(void);
}SeGpioOperation;

/*
 * define UART operations and descriptions
 */
typedef struct
{
	SeUInt32 iBaudRate;
	SeBool blInited;
	void* pOperation;
}SeUartDescription;

typedef struct
{
	SeInt8 (*fpSeUartPreInit)(SeUInt32 iBaudRate);
	SeInt8 (*fpSeUartWriteByte)(SeUInt8 bData);
	SeInt8 (*fpSeUartReadByte)(SeUInt8* bData);
}SeUartOperation;

/*
 * define link speed enum
 */
typedef enum
{
	SeEthernetLinkSpeedTypeUnknown = 0,
	SeEthernetLinkSpeedType10MHalfDuplex,
	SeEthernetLinkSpeedType10MFullDuplex,
	SeEthernetLinkSpeedType100MHalfDuplex,
	SeEthernetLinkSpeedType100MFullDuplex,
	SeEthernetLinkSpeedType1000MFullDuplex,
}SeEthernetLinkSpeedType;

/*
 * define phy description
 */
typedef struct
{
	SeInt8 (*fpSeEthernetPhyPreInit)(void);
	void (*fpSeEthernetPhyReset)(void);
	SeEthernetLinkSpeedType (*fpSeEthernetGetLinkSpeed)(void);
	SeBool (*fpSeEthernetIsLinked)(void);
}SeEthernetPhyDescription;

/*
 * define ethernet hardware level description
 */
typedef struct
{
	SeEthernetPhyDescription* pEthernetPhyDescription;
	SeInt8 (*fpSeEthernetInit)(void);
}SeEthernetDescription;

/*
 * define Arithmetic of large number structure
 */
typedef struct
{
	SeUInt8 nPolarity;						//polarity:  0=positive;1=negative
	SeUInt8 nPoint;							//decimal point: 0=none,1=have
	SeUInt8 *pIntegerPart;				//integer part,low->high
	SeUInt8 nIntegerPartLength;		//integer part length
	SeUInt8 *pDecimalPart;				//8bit decimal part: small denomiator -> big
	SeUInt8 nDecimalPartLength;	//decimal part length
}SeLargeNumber;

/*
 * define W25x spi flash  description structure
 */
typedef struct
{
	SeUInt8 iW25xSpiIndex;
	SeUInt8 iW25xWriteProtectPinIndex;
	SeUInt8 iW25xCsPinIndex;
}SeW25xDescription;

/*
 * define At24cxx e2prom chips types
 */
typedef enum
{
	At24c01 = 0,
	At24c02,
	At24c04,
	At24c08,
	At24c16,
	At24c32,
	At24c64,
	At24c128,
	At24c256,
	At24c512
}SeAt24cxxE2promType;

/*
 * define At24cxx e2prom description structure
 */
typedef struct
{
	SeUInt8 iAt24cxxI2cIndex;
	SeUInt8 hAt24cxxDeviceAddress;
	SeAt24cxxE2promType tAt24cxxType;
	SeUInt16 iAt24cxxChipSize;
}SeAt24cxxDescription;

/*
 * define Cat9555 description structure
 */
typedef struct
{
	SeUInt8 iCat9555I2cIndex;
	SeUInt8 hCat9555DeviceAddress;
}SeCat9555Description;

/*
 * define platform delay method
 */
typedef struct
{
	void (*fpSeDelayMs)(void);
}SeDelayOperation;

/*
 * define system update type
 */
typedef enum
{
	SeUpdateApplicationTypeNone = 0,
	SeUpdateApplicationTypeMcu,
	SeUpdateApplicationTypeFpga,
	SeUpdateApplicationTypeExecutable,
	SeUpdateApplicationTypeConfiguration
}SeUpdateApplicationType;

typedef struct
{
	SeInt8 (*fpSeTftpClientOpen)(SeString sDescription, SeUpdateApplicationType tUpdateApplicationType);
	SeInt8 (*fpSeTftpClientWrite)(SeUInt32 iPosition, SeUInt8* pData, SeUInt16 nLength);
	SeInt8 (*fpSeTftpClientClose)(void);
}SeTftpClientGetFileOperation;

typedef struct
{
	SeInt8 (*fpSeTftpClientOpen)(SeString sDescription, SeUpdateApplicationType tUpdateApplicationType);
	SeInt8 (*fpSeTftpClientRead)(SeUInt32 iPosition, SeUInt8* pData, SeUInt16 nLength);
	SeInt8 (*fpSeTftpClientClose)(void);
}SeTftpClientPutFileOperation;

/*
 * a non-operation function
 */
inline void SeNopRoutine(void){}

/*
 * define user initial method
 */
extern SeInt8 SeUserDriverInit(void);
extern SeInt8 SeUserApplicationInit(void);

#ifdef __cplusplus
}
#endif

#endif
