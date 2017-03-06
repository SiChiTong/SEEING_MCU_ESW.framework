/*
 * String.h
 *
 *  Created on: 2015-11-19
 *      Author: Administrator
 */

#ifndef _STRING_H_
#define _STRING_H_

#include "../../Global/Include.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
void SePrintf(SeConstString csFormat, ...);
void SeScanf(SeConstString csFormat, ...);
*/

#ifdef CONFIG_USING_FFLUSH
#define USING_FFLUSH SeTrue
#else
#define USING_FFLUSH SeFalse
#endif

#define SePrintf(...)	\
			do{	\
				printf(__VA_ARGS__);	\
				if(USING_FFLUSH){	fflush(stdout);	}	\
			}while(0)

#define SeScanf scanf

/*
 * character operation
 */
SeChar SePutCh(SeChar cOut);
SeChar SeGetCh(void);

/*
 * config STD I/O
 */
SeInt8 SeStdConfig(SeStdioOperation* pStdio);

/*
 * normal print
 */
void SeNormalPrint(SeConstString csFormat, ...);

/*
 * error print
 */
void SeErrorPrint(SeConstString csFormat, ...);

/*
 * debug print
 */
void SeDebugPrint(SeConstString csFormat, ...);

/*
 * printf bytes
 */
void SeDataPrint(SeConstString csTitle, SeUInt8* pData, SeUInt16 nLength);

/*
 * progress print
 */
void SeProgressStart(SeConstString csTitle);
void SeProgressPrint(SeUInt8 iPercentage, SeUInt8 nStep);
void SeProgressFinish(void);

/*
 * running print
 */
void SeHoldLinePrint(SeConstString csFormat, ...);

/*
 * welcome log print
 */
void SeWelcomePrint(void);

#ifdef __cplusplus
}
#endif

#endif /* _STRING_H_ */
