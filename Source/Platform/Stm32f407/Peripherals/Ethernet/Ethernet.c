/*
 * Ethernet.c
 *
 *  Created on: 2016年2月25日
 *      Author: seeing
 */

#include "Ethernet.h"

#include "../../../../Middleware/Lwip.h"

#include "stm32f4x7_eth.h"
#include <string.h>

static SeNetworkDescription* pNetworkDesc = SeNull;

static err_t SeStm32f407NetIfInitRoutine(struct netif *pNetif);
static void SeStm32f407EthernetIfInput(void* pvParameters);
static void SeStm32f407EthernetArpTimer(void *pArg);

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 'e'


static struct netif* pGlobalNetIf;
xSemaphoreHandle tInputSem = SeNull;

/*
 * define variants for stm32f407 ethernet lib
 */
/* Ethernet Rx & Tx DMA Descriptors */
extern ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];
/* Ethernet Receive buffers  */
extern uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE];
/* Ethernet Transmit buffers */
extern uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE];
/* Global pointers to track current transmit and receive descriptors */
extern ETH_DMADESCTypeDef  *DMATxDescToSet;
extern ETH_DMADESCTypeDef  *DMARxDescToGet;
/* Global pointer for last received frame infos */
extern ETH_DMA_Rx_Frame_infos *DMA_RX_FRAME_infos;

void ETH_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Frame received */
	if ( ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R) == SET)
	{
		/* Give the semaphore to wakeup LwIP task */
		xSemaphoreGiveFromISR( tInputSem, &xHigherPriorityTaskWoken );
	}

	/* Clear the interrupt flags. */
	/* Clear the Eth DMA Rx IT pending bits */
	ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);

	/* Switch tasks if necessary. */
	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	}
}

SeInt8 SeStm32f407NetIfInit(SeNetworkDescription* pNetworkDescription)
{
	struct ip_addr tIp, tMask, tGate;
	if(pNetworkDescription == SeNull)
	{
		SeErrorPrint("Network description is not correct!");
		return SE_RETURN_ERROR;
	}

	pGlobalNetIf = SeMemoryAlloc(sizeof(struct netif));
	if(pGlobalNetIf == SeNull)
	{
		SeErrorPrint("Network description allocate memory fail!");
		return SE_RETURN_ERROR;
	}

	SeNormalPrint(">>>Stm32f407 network interface created!");

	tcpip_init(SeNull, SeNull);

	SeNormalPrint(">>>Stm32f407 tcp/ip inited!");

	IP4_ADDR(&tIp, pNetworkDescription->tDefaultIp.aIpAddress[0], pNetworkDescription->tDefaultIp.aIpAddress[1], pNetworkDescription->tDefaultIp.aIpAddress[2], pNetworkDescription->tDefaultIp.aIpAddress[3]);
	IP4_ADDR(&tMask, pNetworkDescription->tDefaultMask.aIpAddress[0], pNetworkDescription->tDefaultMask.aIpAddress[1] , pNetworkDescription->tDefaultMask.aIpAddress[2], pNetworkDescription->tDefaultMask.aIpAddress[3]);
	IP4_ADDR(&tGate, pNetworkDescription->tDefaultGate.aIpAddress[0], pNetworkDescription->tDefaultGate.aIpAddress[1], pNetworkDescription->tDefaultGate.aIpAddress[2], pNetworkDescription->tDefaultGate.aIpAddress[3]);

	pNetworkDesc = pNetworkDescription;

	netif_add(pGlobalNetIf, &tIp, &tMask, &tGate, SeNull, &SeStm32f407NetIfInitRoutine, &tcpip_input);

	SeNormalPrint(">>>Stm32f407 network interface added!");

	netif_set_default(pGlobalNetIf);

	SeNormalPrint(">>>Stm32f407 network interface using default configuration!");

	netif_set_up(pGlobalNetIf);

	SeNormalPrint(">>>Stm32f407 network interface setup success!");

	return SE_RETURN_OK;
}



/**
* In this function, the hardware should be initialized.
* Called from ethernetif_init().
*
* @param netif the already initialized lwip network interface structure
*        for this ethernetif
*/
static void SeStm32f407EthernetLowLevelInit(struct netif *pNetif)
{
	SeUInt32 iCount;

	/* set netif MAC hardware address length */
	pNetif->hwaddr_len = ETHARP_HWADDR_LEN;

	/* set netif MAC hardware address */
	pNetif->hwaddr[0] =  pNetworkDesc->tDeviceMac.aMacAddress[0];
	pNetif->hwaddr[1] =  pNetworkDesc->tDeviceMac.aMacAddress[1];
	pNetif->hwaddr[2] =  pNetworkDesc->tDeviceMac.aMacAddress[2];
	pNetif->hwaddr[3] =  pNetworkDesc->tDeviceMac.aMacAddress[3];
	pNetif->hwaddr[4] =  pNetworkDesc->tDeviceMac.aMacAddress[4];
	pNetif->hwaddr[5] =  pNetworkDesc->tDeviceMac.aMacAddress[5];

	/* set netif maximum transfer unit */
	pNetif->mtu = 1500;

	/* Accept broadcast address and ARP traffic */
	pNetif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

	pGlobalNetIf = pNetif;

	/* create binary semaphore used for informing ethernetif of frame reception */
	if (tInputSem == SeNull)
	{
		vSemaphoreCreateBinary(tInputSem);
		xSemaphoreTake(tInputSem, 0);
	}

	/* initialize MAC address in ethernet MAC */
	ETH_MACAddressConfig(ETH_MAC_Address0, pNetif->hwaddr);

	/* Initialize Tx Descriptors list: Chain Mode */
	ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
	/* Initialize Rx Descriptors list: Chain Mode  */
	ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

	/* Enable Ethernet Rx interrrupt */
	{
		for(iCount = 0; iCount < ETH_RXBUFNB; iCount++)
		{
			ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[iCount], ENABLE);
		}
	}

	/* Enable the checksum insertion for the Tx frames */
	{
		for(iCount = 0; iCount < ETH_TXBUFNB; iCount++)
		{
			ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[iCount], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
		}
	}

	/* create the task that handles the ETH_MAC */
	//xTaskCreate(SeStm32f407EthernetIfInput, (signed char*) "Eth_if", SE_BUFFER_SIZE_512, SeNull, configMAX_PRIORITIES - 1, SeNull);

	SeInt8 iStm32f407EthRcvTaskIndex;
	SeTaskAdd(&iStm32f407EthRcvTaskIndex, SeStm32f407EthernetIfInput, SeNull, SeTaskPriorityHighest, 512);

	/* Enable MAC and DMA transmission and reception */
	ETH_Start();
}


/**
* This function should do the actual transmission of the packet. The packet is
* contained in the pbuf that is passed to the function. This pbuf
* might be chained.
*
* @param netif the lwip network interface structure for this ethernetif
* @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
* @return ERR_OK if the packet could be sent
*         an err_t value if the packet couldn't be sent
*
* @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
*       strange results. You might consider waiting for space in the DMA queue
*       to become availale since the stack doesn't retry to send a packet
*       dropped because of memory failure (except for the TCP timers).
*/

static err_t SeStm32f407EthernetLowLevelOutput(struct netif *pNetif, struct pbuf *pBuf)
{
	static xSemaphoreHandle tTxSem = SeNull;
	struct pbuf *pTempBuf;
	SeUInt8* pBuffer ;
	__IO ETH_DMADESCTypeDef *pDmaTxDesc;
	SeUInt16 nFrameLength = 0;
	SeUInt32 nBufferOffset = 0;
	SeUInt32 nBytesLeftToCopy = 0;
	SeUInt32 nPayloadOffset = 0;

	if (tTxSem == SeNull)
	{
		vSemaphoreCreateBinary (tTxSem);
	}

	if (xSemaphoreTake(tTxSem, 250))
	{
		pDmaTxDesc = DMATxDescToSet;
		pBuffer = (SeUInt8*)(pDmaTxDesc->Buffer1Addr);
		nBufferOffset = 0;

		for(pTempBuf = pBuf; pTempBuf != SeNull; pTempBuf = pTempBuf->next)
		{
			if((pDmaTxDesc->Status & ETH_DMATxDesc_OWN) != (SeUInt32)RESET)
			{
				goto SeStm32f407EthernetLowLevelOutputError;
			}

			/* Get bytes in current lwIP buffer  */
			nBytesLeftToCopy = pTempBuf->len;
			nPayloadOffset = 0;

			/* Check if the length of data to copy is bigger than Tx buffer size*/
			while( (nBytesLeftToCopy + nBufferOffset) > ETH_TX_BUF_SIZE )
			{
				/* Copy data to Tx buffer*/
				memcpy( (SeUInt8*)((SeUInt8*)pBuffer + nBufferOffset), (SeUInt8*)((SeUInt8*)pTempBuf->payload + nPayloadOffset), (ETH_TX_BUF_SIZE - nBufferOffset) );

				/* Point to next descriptor */
				pDmaTxDesc = (ETH_DMADESCTypeDef *)(pDmaTxDesc->Buffer2NextDescAddr);

				/* Check if the buffer is available */
				if((pDmaTxDesc->Status & ETH_DMATxDesc_OWN) != (SeUInt32)RESET)
				{
					goto SeStm32f407EthernetLowLevelOutputError;
				}

				pBuffer = (SeUInt8 *)(pDmaTxDesc->Buffer1Addr);

				nBytesLeftToCopy = nBytesLeftToCopy - (ETH_TX_BUF_SIZE - nBufferOffset);
				nPayloadOffset = nPayloadOffset + (ETH_TX_BUF_SIZE - nBufferOffset);
				nFrameLength = nFrameLength + (ETH_TX_BUF_SIZE - nBufferOffset);
				nBufferOffset = 0;
			}

			/* Copy the remaining bytes */
			memcpy( (SeUInt8*)((SeUInt8*)pBuffer + nBufferOffset), (SeUInt8*)((SeUInt8*)pTempBuf->payload + nPayloadOffset), nBytesLeftToCopy);
			nBufferOffset = nBufferOffset + nBytesLeftToCopy;
			nFrameLength = nFrameLength + nBytesLeftToCopy;
		}

		/* Prepare transmit descriptors to give to DMA*/
		ETH_Prepare_Transmit_Descriptors(nFrameLength);

		/* Give semaphore and exit */
SeStm32f407EthernetLowLevelOutputError:
		xSemaphoreGive(tTxSem);
	}

	return ERR_OK;
}

/**
* Should allocate a pbuf and transfer the bytes of the incoming
* packet from the interface into the pbuf.
*
* @param netif the lwip network interface structure for this ethernetif
* @return a pbuf filled with the received packet (including MAC header)
*         NULL on memory error
*/
static struct pbuf * SeStm32f407EthernetLowLevelInput(struct netif *pNetif)
{
	struct pbuf *pBuf= SeNull, *pTempBuf;
	SeUInt32 nLength;
	FrameTypeDef tFrame;
	SeUInt8 *pBuffer;
	__IO ETH_DMADESCTypeDef *pDMARxDesc;
	SeUInt32 nBufferOffset = 0;
	SeUInt32 nPayloadOffset = 0;
	SeUInt32 nBytesLeftToCopy = 0;
	SeUInt32 iCount = 0;

	/* get received frame */
	tFrame = ETH_Get_Received_Frame_interrupt();

	/* Obtain the size of the packet and put it into the "len" variable. */
	nLength = tFrame.length;
	pBuffer = (SeUInt8 *)tFrame.buffer;

	if (nLength > 0)
	{
		/* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
		pBuf = pbuf_alloc(PBUF_RAW, nLength, PBUF_POOL);
	}

	if (pBuf != SeNull)
	{
		pDMARxDesc = tFrame.descriptor;
		nBufferOffset = 0;
		for(pTempBuf = pBuf; pTempBuf != SeNull; pTempBuf = pTempBuf->next)
		{
			nBytesLeftToCopy = pTempBuf->len;
			nPayloadOffset = 0;

			/* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size*/
			while( (nBytesLeftToCopy + nBufferOffset) > ETH_RX_BUF_SIZE )
			{
				/* Copy data to pbuf*/
				memcpy( (SeUInt8*)((SeUInt8*)pTempBuf->payload + nPayloadOffset), (SeUInt8*)((SeUInt8*)pBuffer + nBufferOffset), (ETH_RX_BUF_SIZE - nBufferOffset));

				/* Point to next descriptor */
				pDMARxDesc = (ETH_DMADESCTypeDef *)(pDMARxDesc->Buffer2NextDescAddr);
				pBuffer = (SeUInt8 *)(pDMARxDesc->Buffer1Addr);

				nBytesLeftToCopy = nBytesLeftToCopy - (ETH_RX_BUF_SIZE - nBufferOffset);
				nPayloadOffset = nPayloadOffset + (ETH_RX_BUF_SIZE - nBufferOffset);
				nBufferOffset = 0;
			}

			/* Copy remaining data in pbuf */
			memcpy((SeUInt8*)((SeUInt8*)pTempBuf->payload + nPayloadOffset), (SeUInt8*)((SeUInt8*)pBuffer + nBufferOffset), nBytesLeftToCopy);
			nBufferOffset = nBufferOffset + nBytesLeftToCopy;
		}
	}

	/* Release descriptors to DMA */
	pDMARxDesc = tFrame.descriptor;

	/* Set Own bit in Rx descriptors: gives the buffers back to DMA */
	for (iCount = 0; iCount < DMA_RX_FRAME_infos->Seg_Count; iCount++)
	{
		pDMARxDesc->Status = ETH_DMARxDesc_OWN;
		pDMARxDesc = (ETH_DMADESCTypeDef *)(pDMARxDesc->Buffer2NextDescAddr);
	}

	/* Clear Segment_Count */
	DMA_RX_FRAME_infos->Seg_Count = 0;

	/* When Rx Buffer unavailable flag is set: clear it and resume reception */
	if ((ETH->DMASR & ETH_DMASR_RBUS) != (SeUInt32)RESET)
	{
		/* Clear RBUS ETHERNET DMA flag */
		ETH->DMASR = ETH_DMASR_RBUS;
		/* Resume DMA reception */
		ETH->DMARPDR = 0;
	}
	return pBuf;
}


/**
* This function is the ethernetif_input task, it is processed when a packet
* is ready to be read from the interface. It uses the function low_level_input()
* that should handle the actual reception of bytes from the network
* interface. Then the type of the received packet is determined and
* the appropriate input function is called.
*
* @param netif the lwip network interface structure for this ethernetif
*/
void SeStm32f407EthernetIfInput(void* pvParameters)
{
	struct pbuf *pBuf;

	for(;;)
	{
		if(xSemaphoreTake(tInputSem, 100) == pdTRUE)
		{
SeStm32f407EthernetIfInputTryGetNextFrame:
			pBuf = SeStm32f407EthernetLowLevelInput(pGlobalNetIf);
			if(pBuf != SeNull)
			{
				if (ERR_OK != pGlobalNetIf->input(pBuf, pGlobalNetIf))
				{
					pbuf_free(pBuf);
					pBuf = SeNull;
				}
				else
				{
					xSemaphoreTake( tInputSem, 0);
					goto SeStm32f407EthernetIfInputTryGetNextFrame;
				}
			}
		}
	}
}


void SeStm32f407EthernetArpTimer(void *pArg)
{
  etharp_tmr();
  sys_timeout(ARP_TMR_INTERVAL, SeStm32f407EthernetArpTimer, SeNull);
}




err_t SeStm32f407NetIfInitRoutine(struct netif *pNetif)
{
	LWIP_ASSERT("netif != NULL", (pNetif != SeNull));

	#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	pNetif->hostname = "lwip";
	#endif /* LWIP_NETIF_HOSTNAME */

	pNetif->name[0] = IFNAME0;
	pNetif->name[1] = IFNAME1;

	pNetif->output = etharp_output;
	pNetif->linkoutput = SeStm32f407EthernetLowLevelOutput;

	/* initialize the hardware */
	SeStm32f407EthernetLowLevelInit(pNetif);

	etharp_init();
	sys_timeout(ARP_TMR_INTERVAL, SeStm32f407EthernetArpTimer, SeNull);

	return ERR_OK;
}
