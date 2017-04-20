/*
 * L3G4200D.h
 *
 *  Created on: 2017年4月18日
 *      Author: seeing
 */

#ifndef _CHIP_SENSOR_L3G4200D_H_
#define _CHIP_SENSOR_L3G4200D_H_

#include "../../../Global/Include.h"

typedef struct{
	SeUInt8 iL3G4200DIndex;
	SeUInt8 hL3G4200DMajorAddress;
	SeUInt8 hL3G4200DMinorAddress;
}SeL3G4200DDescription;

#define L3G4200D_ADDRESS				0x69
#define L3G4200D_DEFAULT_ADDRESS		0x69

#define L3G4200D_REG_WHO_AM_I			0x0F
#define L3G4200D_REG_CTRL_REG1			0x20
#define L3G4200D_REG_CTRL_REG2			0x21
#define L3G4200D_REG_CTRL_REG3			0x22
#define L3G4200D_REG_CTRL_REG4			0x23
#define L3G4200D_REG_CTRL_REG5			0x24
#define L3G4200D_REG_REFERENCE			0x25
#define L3G4200D_REG_OUT_TEMP			0x26
#define L3G4200D_REG_STATUS				0x27
#define L3G4200D_REG_OUT_X_L				0x28
#define L3G4200D_REG_OUT_X_H				0x29
#define L3G4200D_REG_OUT_Y_L				0x2A
#define L3G4200D_REG_OUT_Y_H				0x2B
#define L3G4200D_REG_OUT_Z_L				0x2C
#define L3G4200D_REG_OUT_Z_H				0x2D
#define L3G4200D_REG_FIFO_CTRL			0x2E
#define L3G4200D_REG_FIFO_SRC			0x2F
#define L3G4200D_REG_INT1_CFG			0x30
#define L3G4200D_REG_INT1_SRC			0x31
#define L3G4200D_REG_INT1_THS_XH			0x32
#define L3G4200D_REG_INT1_THS_XL			0x33
#define L3G4200D_REG_INT1_THS_YH			0x34
#define L3G4200D_REG_INT1_THS_YL			0x35
#define L3G4200D_REG_INT1_THS_ZH			0x36
#define L3G4200D_REG_INT1_THS_ZL			0x37
#define L3G4200D_REG_INT1_DURATION		0x38

#define L3G4200D_DEVICE_ID 					0xD3

typedef union
{
	struct
	{
		SeUInt8 XEN:1;
		SeUInt8 YEN:1;
		SeUInt8 ZEN:1;
		SeUInt8 PD:1;
		SeUInt8 BW:2;
		SeUInt8 DR:2;
	}Bit;
	SeUInt8 Byte;
}SeL3G4200DCntlReg1;

typedef union
{
	struct
	{
		SeUInt8 HPCF:4;
		SeUInt8 HPM:2;
		SeUInt8 :2;
	}Bit;
	SeUInt8 Byte;
}SeL3G4200DCntlReg2;

typedef union
{
	struct
	{
		SeUInt8 L2EMPTY:1;
		SeUInt8 L2RUN:1;
		SeUInt8 L2WTM:1;
		SeUInt8 L2DRDY:1;
		SeUInt8 PPOD:1;
		SeUInt8 HLACT:1;
		SeUInt8 L1BOOT:1;
		SeUInt8 L1INT:1;
	}Bit;
	SeUInt8 Byte;
}SeL3G4200DCntlReg3;

typedef union
{
	struct
	{
		SeUInt8 SIM:1;
		SeUInt8 ST:2;
		SeUInt8 :1;
		SeUInt8 FS:2;
		SeUInt8 BLE:1;
		SeUInt8 BDU:1;
	}Bit;
	SeUInt8 Byte;
}SeL3G4200DCntlReg4;

typedef union
{
	struct
	{
		SeUInt8 OUTSEL:2;
		SeUInt8 INT1SEL:2;
		SeUInt8 HPEN:1;
		SeUInt8 :1;
		SeUInt8 FIFOEN:1;
		SeUInt8 BOOT:1;
	}Bit;
	SeUInt8 Byte;
}SeL3G4200DCntlReg5;

typedef union
{
	struct
	{
		SeUInt8 XDA:1;
		SeUInt8 YDA:1;
		SeUInt8 ZDA:1;
		SeUInt8 ZYXDA:1;
		SeUInt8 XOR:1;
		SeUInt8 YOR:1;
		SeUInt8 ZOR:1;
		SeUInt8 ZYXOR:1;
	}Bit;
	SeUInt8 Byte;
}SeL3G4200DStatusReg;

typedef union
{
	struct
	{
		SeUInt8 WTM:5;
		SeUInt8 FM:3;
	}Bit;
	SeUInt8 Byte;
}SeL3G4200DRefReg;

typedef union
{
	struct
	{
		SeUInt8 FSS:5;
		SeUInt8 :1;
		SeUInt8 OVRN:1;
		SeUInt8 WTM:1;
	}Bit;
	SeUInt8 Byte;
}SeL3G4200DFifoSrcReg;

SeInt8 SeL3G4200DChipInit(SeL3G4200DDescription tL3G4200DDescription);

SeInt8 SeL3G4200DSelfCheck(void);

SeInt8 SeL3G4200DRegRead(SeUInt8 hAddress, SeUInt8* pValue);

SeInt8 SeL3G4200DRegWrite(SeUInt8 hAddress, SeUInt8 hValue);

#endif /* SOURCE_DRIVER_CHIP_SENSOR_L3G4200D_H_ */
