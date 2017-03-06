/*
 * Tftp.h
 *
 *  Created on: 2016年3月14日
 *      Author: seeing
 */

#ifndef _APPLICATION_NETWORK_TRANSFER_TFTP_H_
#define _APPLICATION_NETWORK_TRANSFER_TFTP_H_

#include "../../../Global/Include.h"

typedef enum
{
	SeTftpFileTypeNetAscII = 0,
	SeTftpFileTypeOctet
}SeTftpFileType;

typedef struct
{
	SeIpAddress tServerIp;
	SeIpPort tServerPort;
}SeTftpClientDescription;

SeInt8 SeTftpClientGetFile(SeTftpClientDescription tTftpClientDescription, SeString sFileName, SeTftpFileType tTftpFileType, SeTftpClientGetFileOperation tTftpClientGetFileOperation, SeString sDescription, SeUpdateApplicationType tUpdateApplicationType);
SeInt8 SeTftpClientPutFile(SeTftpClientDescription tTftpClientDescription, SeString sFileName, SeTftpFileType tTftpFileType, SeTftpClientPutFileOperation tTftpClientPutFileOperation, SeString sDescription, SeUpdateApplicationType tUpdateApplicationType);

#endif /* _APPLICATION_NETWORK_TRANSFER_TFTP_H_ */
