/*
 * Command.h
 *
 *  Created on: 2015-11-23
 *      Author: Administrator
 */

#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "../Global/Include.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef SeInt8 (*SeCommandProcess)(SeLinkList* pParamList, SeString sResult);

/*
 * command parameters link list
 */
typedef struct{
	SeUInt8 nParameterLength;	/*	not including '\0'	*/
	SeChar sParameter[CONFIG_TERMINAL_PARAMETER_LENGTH];
}SeCommandParameterInf;

/*
 * command operation type
 */
typedef struct{
	SeUInt8 nCommandLength;		/*	not including '\0'	*/
	SeChar sCommand[CONFIG_TERMINAL_COMMAND_LENGTH];
	SeCommandProcess cbCommandProcess;
}SeCommandInf;


/*
 * register a command
 */
SeInt8 SeRegisterCommand(SeConstString csCommand, SeCommandProcess cbProcessFunction);

/*
 * execute a command, with parameters, directly
 */
SeInt8 SeExecuteFullCommand(SeString sFullCommand, SeString sResult);

/*
 * execute a command, with command process routine
 */
SeInt8 SeExecuteCommandAndWaitResult(SeString sFullCommand, SeString sResult, SeUInt16 nTimeout);


/*
 * initial commands
 */
SeInt8 SeCommandsInit(void);

/*
 * finish commands
 */
SeInt8 SeCommandsFinish(void);

/*
 * check the input string
 */
SeBool SeCommondCheckBuffer(SeUInt8* pInputBuffer, SeInt16 nInputLength);

#ifdef __cplusplus
}
#endif

#endif /* _COMMAND_H_ */
