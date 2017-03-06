/*
 * Thd.h
 *
 *  Created on: 2015-12-24
 *      Author: Administrator
 */

#ifndef _THD_APPLICATION_H_
#define _THD_APPLICATION_H_

#include "../../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FPGA_PARAMETER_OPTION_THDN 0x02
#define FPGA_PARAMETER_OPTION_THD 0x04
#define FPGA_PARAMETER_OPTION_SINAD 0x08
#define FPGA_PARAMETER_OPTION_FREQ 0x10

#define THD_MEASURE_CHANNEL_LEFT 0x01
#define THD_MEASURE_CHANNEL_RIGHT 0x02
#define THD_MEASURE_CHANNEL_DUAL 0x03

// 输入电压范围寄存器
#define FPGA_THDN_VPP_RANGE_REG 0x04
#define FPGA_THDN_VPP_RANGE_REG_SIZE 2

// FFT长度寄存器
#define FPGA_THDN_FFT_LENGTH_REG 0x06
#define FPGA_THDN_FFT_LENGTH_REG_SIZE 2

// 有效带宽寄存器
#define FPGA_THDN_BANDWIDTH_REG 0x11
#define FPGA_THDN_BANDWIDTH_REG_SIZE 2

// 抽取倍数寄存器
#define FPGA_THDN_DIV_DATA_REG 0x15

// 基波位置A寄存器
#define FPGA_THDN_BASE_INDEX_A_REG 0x20
#define FPGA_THDN_BASE_INDEX_A_REG_SIZE 2

// 基波数据A寄存器
#define FPGA_THDN_BASE_DATA_A_REG 0x22
#define FPGA_THDN_BASE_DATA_A_REG_SIZE 6

// 基波位置B寄存器
#define FPGA_THDN_BASE_INDEX_B_REG 0x28
#define FPGA_THDN_BASE_INDEX_B_REG_SIZE 2

// 基波数据A寄存器
#define FPGA_THDN_BASE_DATA_B_REG 0x2A
#define FPGA_THDN_BASE_DATA_B_REG_SIZE 6

// 最大失真位置A
#define FPGA_THDN_DIST_INDEX_A_REG 0x30
#define FPGA_THDN_DIST_INDEX_A_REG_SIZE 2

// 最大失真数据A
#define FPGA_THDN_DIST_DATA_A_REG 0x32
#define FPGA_THDN_DIST_DATA_A_REG_SIZE 6

// 最大失真位置B
#define FPGA_THDN_DIST_INDEX_B_REG 0x38
#define FPGA_THDN_DIST_INDEX_B_REG_SIZE 2

// 最大失真数据B
#define FPGA_THDN_DIST_DATA_B_REG 0x3A
#define FPGA_THDN_DIST_DATA_B_REG_SIZE 6

// 基波能量寄存器
#define FPGA_THDN_BASE_POWER_REG 0x40
#define FPGA_THDN_BASE_POWER_REG_SIZE 8

// 带内总能量寄存器
#define FPGA_THDN_BAND_POWER_REG 0x50
#define FPGA_THDN_BAND_POWER_REG_SIZE 8

// FFT位置寄存器
#define FPGA_THDN_FFT_INDEX_REG 0x60
#define FPGA_THDN_FFT_INDEX_REG_SIZE 2

// FFT数据寄存器
#define FPGA_THDN_FFT_DATA_REG 0x62
#define FPGA_THDN_FFT_DATA_REG_SIZE 6

SeInt8 SeFpgaThdConfigBandwidth(SeUInt8 nChannel, SeUInt16 nBand);
SeUInt8 SeFpgaThdnGetSoundChannel(SeChar *pChannel);
SeInt8 SeFpgaThdGetThdnResult(SeUInt8 nChannel, SeDouble *pThdn);
SeInt8 SeFpgaThdGetThdResult(SeUInt8 nChannel, SeUInt32 nHarmCount, SeDouble *pThd);
SeInt8 SeFpgaThdGetSinadResult(SeUInt8 nChannel, SeDouble *pSinad);
SeInt8 SeFpgaThdGetFreqResult(SeUInt8 nChannel, SeDouble *pFreq, SeDouble *pMvolt);

#ifdef __cplusplus
}
#endif

#endif /* THD_H_ */
