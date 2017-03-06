/*
 * Fpga.h
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#ifndef _FPGA_APPLICATION_H_
#define _FPGA_APPLICATION_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 *
 * FPGA system registers
 *
 *
 */
#define SE_FPGA_SYSTEM_PAGE_ADDRESS			0x00

typedef enum
{
	SeFpgaSystemTestRegisterAddress = 0x00,
	SeFpgaSystemFpgaVersionRegisterAddress = 0x10,
	SeFpgaSystemBomVersionRegisterAddress,
	SeFpgaSystemPhyIdRegisterAddress,
	SeFpgaSystemPllLockRegisterAddress,
	SeFpgaSystemClockControlRegisterAddress,
	SeFpgaSystemFpgaTypeRegisterAddress,
	SeFpgaSystemPage1TypeRegisterAddress,
	SeFpgaSystemPage2TypeRegisterAddress,
	SeFpgaSystemPage3TypeRegisterAddress,
	SeFpgaSystemPage4TypeRegisterAddress,
	SeFpgaSystemPage5TypeRegisterAddress,
	SeFpgaSystemPage6TypeRegisterAddress,
	SeFpgaSystemPage7TypeRegisterAddress,
	SeFpgaSystemPage8TypeRegisterAddress,
	SeFpgaSystemPage9TypeRegisterAddress,
	SeFpgaSystemPage10TypeRegisterAddress,
	SeFpgaSystemPage11TypeRegisterAddress,
	SeFpgaSystemPage12TypeRegisterAddress,
	SeFpgaSystemPage13TypeRegisterAddress,
	SeFpgaSystemPage14TypeRegisterAddress,
	SeFpgaSystemPage15TypeRegisterAddress,
	SeFpgaSystemBaseTimeRegisterAddress,
	SeFpgaSystemBaseTimeUpdateRegisterAddress = 0x46,
	SeFpgaSystemRealTimeUpdateRegisterAddress,
	SeFpgaSystemRealTimeRegisterAddress
}SeFpgaSystemRegisters;

typedef struct
{
	SeUInt8 aTestRegisterArray[16];
}SeFpgaSystemTestRegister;

typedef SeUInt8 SeFpgaSystemFpgaVersionRegister;

typedef union
{
	struct
	{
		SeUInt8 Version :3;
		SeUInt8 :5;
	}Bit;
	SeUInt8 Byte;
}SeFpgaSystemBomVersionRegister;

typedef union
{
	struct
	{
		SeUInt8 PhyId :3;
		SeUInt8 :5;
	}Bit;
	SeUInt8 Byte;
}SeFpgaSystemPhyIdRegister;

typedef union
{
	struct
	{
		SeUInt8 Lock :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaSystemPllLockRegister;

typedef union
{
	struct
	{
		SeUInt8 ClockEnable :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaSystemClockControlRegister;

typedef SeUInt8 SeFpgaSystemFpgaTypeRegister;

typedef SeUInt8 SeFpgaSystemFpgaPageTypeRegister;

typedef struct
{
	SeUInt16 iMilliSecond;
	SeUInt32 iUtcTime;
}SeFpgaSystemTimeRegister;

typedef union
{
	struct
	{
		SeUInt8 BaseTimeUpdate :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaSystemBaseTimeUpdateRegister;

typedef union
{
	struct
	{
		SeUInt8 RealTimeUpdate :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaSystemRealTimeUpdateRegister;

typedef enum
{
	SeFpgaSystemPageFunctionNull,
	SeFpgaSystemPageFunctionMeasure,
	SeFpgaSystemPageFunction192KHzDatalogger,
	SeFpgaSystemPageFunctionUart,
	SeFpgaSystemPageFunctionThdn,
	SeFpgaSystemPageFunctionPdm,
	SeFpgaSystemPageFunction125MHzDatalogger = 0x0A,
	SeFpgaSystemPageFunctionAd9265Driver,
}SeFpgaSystemPageFunctionTypes;

typedef enum
{
	SeFpgaChipTypeNone,
	SeFpgaChipType_XC6SLX45_CSG324,
}SeFpgaSystemChipTypes;

/*
 *
 *
 * FPGA measure registers
 *
 *
 */
typedef enum
{
	SeFpgaMeasureTestRegisterAddress = 0x00,

	SeFpgaMeasureChannelEnableRegisterAddress = 0x10,
	SeFpgaMeasurePeriodRegisterAddress,
	SeFpgaMeasureStartRegisterAddress = 0x13,
	SeFpgaMeasureStateRegisterAddress,


}SeFpgaMeasureRegisters;

typedef SeUInt8 SeFpgaMeasureTestRegister;

typedef union
{
	struct
	{
		SeUInt8 Enable :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaMeasureChannelEnableRegister;

typedef SeUInt16 SeFpgaMeasurePeriodRegister;

typedef union
{
	struct
	{
		SeUInt8 Start :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaMeasureStartRegister;

typedef union
{
	struct
	{
		SeUInt8 State :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaMeasureStateRegister;

/*
 *
 *
 *
 * define Data Logger registers
 *
 *
 *
 */
typedef enum
{
	SeFpgaDataLoggerTestRegisterAddress = 0x00,
	SeFpgaDataLoggerAcquisitionStartRegisterAddress = 0x1E,
	SeFpgaDataLoggerByteCountRegisterAddress = 0x3A,
	SeFpgaDataLoggerAcquisitionStatusRegisterAddress = 0x3E,
	SeFpgaDataLoggerAcquisitionFailStateRegisterAddress,
	SeFpgaDataLoggerAcquisitionBufferRegisterAddress = 0xFE,
}SeFpgaDataLoggerRegisters;

typedef SeUInt8 SeFpgaDataLoggerTestRegister;

typedef union
{
	struct
	{
		SeUInt8 Start :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaDataLoggerAcquisitionStartRegister;

typedef SeUInt32 SeFpgaDataLoggerByteCountRegister;

typedef union
{
	struct
	{
		SeUInt8 ReadOver:1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaDataLoggerAcquisitionStatusRegister;

typedef SeUInt8 SeFpgaDataLoggerAcquisitionFailStateRegister;

typedef union
{
	SeUInt8 aBuffer[2];
	SeUInt16 hData;
}SeFpgaDataLoggerAcquisitionBufferRegister;

/*
 *
 *
 *
 * define AD9265 driver register
 *
 *
 *
 */
typedef enum
{
	SeFpgaAd9265TestRegisterAddress = 0x00,
	SeFpgaAd9265SelfCheckEnableRegisterAddress,
	SeFpgaAd9265MeasureRangeAndImpedanceSelectRegisterAddress,
	SeFpgaAd9265DefaultMeasureImpedanceSelectRegisterAddress,
	SeFpgaAd9265AcquisitionTimeRegisterAddress,
	SeFpgaAd9265AcquisitionControlRegisterAddress,
	SeFpgaAd9265AcquisitionStatusRegisterAddress,
	SeFpgaAd9265ControlRegisterAddress = 0x29,
	SeFpgaAd9265SpiTransRegisterAddress = 0x2A,
	SeFpgaAd9265SpiTransLaunchRegisterAddress = 0x2D,
	SeFpgaAd9265SpiTransStateRegisterAddress = 0x2E,
	SeFpgaAd9265SpiReturnDataRegisterAddress,
}SeFpgaAd9265Registers;

typedef SeUInt8 SeFpgaAd9265TestRegister;

typedef SeUInt8 SeFpgaAd9265SelfCheckEnableRegister;

typedef union
{
	struct
	{
		SeUInt8 Range :1;
		SeUInt8 Impedance :1;
		SeUInt8 :6;
	}Bit;
	SeUInt8 Byte;
}SeFpgaAd9265MeasureRangeAndImpedanceSelectRegister;

typedef union
{
	struct
	{
		SeUInt8 Impedance :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaAd9265DefaultMeasureImpedanceSelectRegister;

typedef SeUInt8 SeFpgaAd9265AcquisitionTimeRegister;

typedef union
{
	struct
	{
		SeUInt8 Start :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaAd9265AcquisitionControlRegister;

typedef union
{
	struct
	{
		SeUInt8 Done :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaAd9265AcquisitionStatusRegister;

typedef union
{
	struct
	{
		SeUInt8 Disable :1;
		SeUInt8 Sync :1;
		SeUInt8 :6;
	}Bit;
	SeUInt8 Byte;
}SeFpgaAd9265ControlRegister;

typedef union
{
	struct
	{
		SeUInt8 Launch :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaAd9265SpiTransLaunchRegister;

typedef union
{
	struct
	{
		SeUInt8 Done :1;
		SeUInt8 :7;
	}Bit;
	SeUInt8 Byte;
}SeFpgaAd9265SpiTransStateRegister;

typedef SeUInt8 SeFpgaAd9265SpiReturnDataRegister;





















/*
 * define fpga measure frequency method
 */
typedef enum
{
	FpgaMeasureFreqLsm = 0,
	FpgaMeasureFreqMt = 1
}SeFpgaMeasureFreqMethod;

SeInt8 SeFpgaRegisterInit(void);

SeInt8 SeFpgaRegisterRead(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nLength);

SeInt8 SeFpgaRegisterWrite(SeUInt16 hAddress, SeUInt8* pData, SeInt16 nLength);

// 基准时钟频率寄存器
#define FPGA_CLOCK_FREQUENCY_REG 0x01
#define FPGA_CLOCK_FREQUENCY_REG_SIZE 3

// 峰峰值寄存器
#define FPGA_VPP_DATA_REG 0x04
#define FPGA_VPP_DATA_REG_SIZE 2

// 通道使能寄存器
#define FPGA_ENABLE_CHANNEL_REG 0x10

// 测量时间寄存器
#define FPGA_MEASURE_TIME_REG 0x11
#define FPGA_MEASURE_TIME_REG_SIZE 2

// 测量开始寄存器
#define FPGA_MEASURE_START_REG 0x13

// 测量状态寄存器
#define FPGA_MEASURE_STATE_REG 0x14

// 分频系数寄存器
#define FPGA_DIVISION_REG 0x15
#define FPGA_DIVISION_REG_SIZE 2

// 转方波阈值寄存器
#define FPGA_THRESHOLD_REG 0x12
#define FPGA_THRESHOLD_REG_SIZE 8

// 转方波翻转类型寄存器
#define FPGA_THRESHOLD_TYPE_REG 0x22
#define FPGA_THRESHOLD_TYPE_REG_SIZE 8

// 测量量程寄存器
#define FPGA_AMPLITUDE_CHECK_VPP_REG 0x18
#define FPGA_AMPLITUDE_CHECK_VPP_SIZE 2

// 峰峰值累加值寄存器
#define FPGA_AMPLITUDE_VPP_SUM_REG 0x80
#define FPGA_AMPLITUDE_VPP_SUM_SIZE 8

// 峰峰值测量周期数寄存器
#define FPGA_AMPLITUDE_VPP_CYCLE_COUNT_REG 0x60
#define FPGA_AMPLITUDE_VPP_CYCLE_COUNT_SIZE 8

// LSM测量数据Ex寄存器
#define FPGA_FREQ_LSM_X_REG              0x20
#define FPGA_FREQ_LSM_X_SIZE             8

// LSM测量数据Ey寄存器
#define FPGA_FREQ_LSM_Y_REG              0x28
#define FPGA_FREQ_LSM_Y_SIZE             8

// LSM测量数据Exy寄存器
#define FPGA_FREQ_LSM_XY_REG             0x30
#define FPGA_FREQ_LSM_XY_SIZE            8

// LSM测量数据Exx寄存器
#define FPGA_FREQ_LSM_XX_N_REG           0x38
#define FPGA_FREQ_LSM_XX_N_SIZE          8

// LSM测量周期数
#define FPGA_FREQ_LSM_CYCLE_CNT_REG      0x40
#define FPGA_FREQ_LSM_CYCLE_CNT_SIZE     2

// 低脉宽周期数寄存器
#define FPGA_PULSE_WIDTH_LOW_CYCLE_CNT_REG 0x90
#define FPGA_PULSE_WIDTH_LOW_CYCLE_CNT_REG_SIZE 3

// 低脉宽最大值寄存器
#define FPGA_PULSE_WIDTH_LOW_MAX_REG 0x98
#define FPGA_PULSE_WIDTH_LOW_MAX_REG_SIZE 3

// 低脉宽最小值寄存器
#define FPGA_PULSE_WIDTH_LOW_MIN_REG 0xA0
#define FPGA_PULSE_WIDTH_LOW_MIN_REG_SIZE 2

// 低脉宽累加值寄存器
#define FPGA_PULSE_WIDTH_LOW_SUM_REG 0xA8
#define FPGA_PULSE_WIDTH_LOW_SUM_REG_SIZE 4

// 低脉宽平方和寄存器
#define FPGA_PULSE_WIDTH_LOW_SQUARE_SUM_REG 0xB0
#define FPGA_PULSE_WIDTH_LOW_SQUARE_SUM_REG_SIZE 8

// 高脉宽周期数寄存器
#define FPGA_PULSE_WIDTH_HIGH_CYCLE_CNT_REG 0xD8
#define FPGA_PULSE_WIDTH_HIGH_CYCLE_CNT_REG_SIZE 3

// 高脉宽最大值寄存器
#define FPGA_PULSE_WIDTH_HIGH_MAX_REG 0xB8
#define FPGA_PULSE_WIDTH_HIGH_MAX_REG_SIZE 2

// 高脉宽最小值寄存器
#define FPGA_PULSE_WIDTH_HIGH_MIN_REG 0xC0
#define FPGA_PULSE_WIDTH_HIGH_MIN_REG_SIZE 2

// 高脉宽累加值寄存器
#define FPGA_PULSE_WIDTH_HIGH_SUM_REG 0xC8
#define FPGA_PULSE_WIDTH_HIGH_SUM_REG_SIZE 4

// 高脉宽平方和寄存器
#define FPGA_PULSE_WIDTH_HIGH_SQUARE_SUM_REG 0xD0
#define FPGA_PULSE_WIDTH_HIGH_SQUARE_SUM_REG_SIZE 8

// PDM输出控制寄存器
#define FPGA_PDM_SIGNAL_CTRL_REG 0x11

// PDM数据源选择寄存器
#define FPGA_PDM_SOURCE_SELECT_REG 0x12

// PDM幅值控制寄存器
#define FPGA_PDM_VOLT_CTRL_REG 0x30
#define FPGA_PDM_VOLT_CTRL_REG_SIZE 2

// PDM频率控制寄存器
#define FPGA_PDM_FREQ_CTRL_REG 0x38
#define FPGA_PDM_FREQ_CTRL_REG_SIZE 4

// 波特率设置寄存器
#define FPGA_UART_BAUD_RATE_REG 0x21
#define FPGA_UART_BAUD_RATE_REG_SIZE 4

// 串口参数寄存器
#define FPGA_UART_PARAMETER_REG 0x20

// 串口功能寄存器
#define FPGA_UART_FUNCTION_REG 0x25

// 参数载入寄存器
#define FPGA_UART_CONFIG_LOAD_REG 0x26

SeInt8 SeFpgaEnableChannel(SeUInt8 nChannel);
SeInt8 SeFpgaDisableChannel(SeUInt8 nChannel);
SeInt8 SeFpgaConfigTime(SeUInt8 nChannel, SeUInt16 uTime);
SeInt8 SeFpgaConfigDivision(SeUInt8 nChannel, SeUInt16 uDivision);
SeInt8 SeFpgaConfigThreshold(SeUInt8 nChannel, SeUInt16 uThreshold);
SeInt8 SeFpgaStartAD(SeUInt8 nChannel);
SeInt8 SeFpgaStartMeasure(SeUInt8 nChannel);
SeInt8 SeFpgaGetState(SeUInt8 nChannel);
SeBool SeFpgaMeasureIsTimeout(SeUInt8 nChannel, SeUInt32 nTimeout);

typedef struct
{
	SeUInt16 nCycleCount;
	SeUInt32 nClock;
	SeUInt16 nDivision;
	SeUInt64 nX;
	SeUInt64 nY;
	SeUInt64 nXY;
	SeUInt64 nXX;
	SeDouble dFrequency;
}SeFpgaFrequency;

SeInt8 SeFpgaGetFrequencyMeasureResult(SeUInt8 nChannel, SeFpgaFrequency *pFrequency);
SeInt8 SeFpgaGetAmplitudeMeasureResult(SeUInt8 nChannel, SeDouble *pAmplitude);

typedef struct
{
	SeUInt32 nLowCycleCount;		// 低电平脉宽周期数
	SeUInt32 nLowMax;					// 低电平脉宽最大值
	SeUInt32 nLowMin;					// 低电平脉宽最小值
	SeDouble dLowAverage;			// 低电平脉宽均值
	SeDouble dLowVariance;		// 低电平脉宽方差

	SeUInt32 nHighCycleCount;	// 高电平脉宽周期数
	SeUInt32 nHighMax;				// 高电平脉宽最大值
	SeUInt32 nHighMin;					// 高电平脉宽最小值
	SeDouble dHighAverage;		// 高电平脉宽均值
	SeDouble dHighVariance;		// 高电平脉宽方差

	SeDouble dDutyRatio;				// 占空比
}SeFpgaPulseWidth;

SeInt8 SeFpgaGetPulseWidthMeasureResult(SeUInt8 nChannel, SeFpgaPulseWidth *pPulseWidth);

SeInt8 SeFpgaPdmConfigFrequency(SeUInt8 nChannel, SeUInt32 nFrequency);
SeInt8 SeFpgaPdmConfigVolt(SeUInt8 nChannel, SeUInt16 nVolt);
SeInt8 SeFpgaPdmSelectSignalSource(SeUInt8 nChannel, SeUInt8 nSource);
SeInt8 SeFpgaPdmStart(SeUInt8 nChannel);
SeInt8 SeFpgaPdmStop(SeUInt8 nChannel);

SeInt8 SeFpgaUartConfig(SeUInt8 nChannel, SeUInt32 nBaudrate,  SeUInt8 nBits,
		SeUInt8 nStop, SeUInt8 nParity, SeBool blAddTimestamp, SeBool blEcho);

#define FPGA_PARAMETER_OPTION_DEBUG 0x01

#ifdef __cplusplus
}
#endif

#endif /* _FPGA_APPLICATION_H_ */
