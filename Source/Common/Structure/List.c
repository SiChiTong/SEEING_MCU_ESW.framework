/*
 * List.c
 *
 *  Created on: 2015-12-7
 *      Author: Administrator
 */


#include "List.h"

SeLinkList* SeLinkListCreate(void)
{
	SeLinkList* pLinkList = SeNull;

	pLinkList = SeMemoryAlloc(sizeof(SeLinkList));

	pLinkList->pItem = SeNull;
	pLinkList->pNext = SeNull;

	return pLinkList;
}

void SeLinkListFree(SeLinkList* pList)
{
	SeLinkList* pListNode = SeNull;
	if(pList == SeNull)
		return;
	pListNode = pList;
	while(pListNode != SeNull)
	{
		SeLinkList* pNextNode = pListNode->pNext;

		SeMemoryFree(pListNode);

		pListNode = pNextNode;
	}

	return;
}

void SeLinkListAddNode(SeLinkList* pList, void* pItem)
{
	SeLinkList* pNode = SeNull;

	if(pList == SeNull || pItem == SeNull)
		return;

	pNode = SeMemoryAlloc(sizeof(SeLinkList));
	if(pNode == SeNull)
		return;
	pNode->pItem = pItem;
	pNode->pNext = SeNull;

	while(SeTrue)
	{
		if(pList->pNext == SeNull)
		{
			pList->pNext = pNode;
			break;
		}
		pList = pList->pNext;
	}

	return;
}

void SeLinkListRemoveNode(SeLinkList* pList, void* pItem)
{
	SeLinkList* pPrivNode = SeNull;
	SeLinkList* pCurNode = SeNull;
	if(pList == SeNull || pItem == SeNull)
		return;

	pPrivNode = pCurNode = pList;
	while(pCurNode != SeNull)
	{
		if(pPrivNode->pItem == pItem)
		{
			pPrivNode->pNext = pCurNode->pNext;
			SeMemoryFree(pCurNode);
			break;
		}
		pPrivNode = pCurNode;
		pCurNode = pCurNode->pNext;
	}

	return;
}

SeInt16 SeLinkListGetNodeCount(SeLinkList* pList)
{
	SeLinkList* pNode = pList->pNext;
	SeUInt16 nCount = 0;

	while(pNode != SeNull)
	{
		nCount++;
		pNode = pNode->pNext;
	}

	return nCount;
}

void* SeLinkListGetItemByIndex(SeLinkList* pList, SeInt16 iIndex)
{
	SeLinkList* pNode = pList->pNext;
	SeUInt16 iNodeIndex = 0;

	while(pNode != SeNull)
	{
		if(iNodeIndex == iIndex)
		{
			break;
		}
		iNodeIndex++;
		pNode = pNode->pNext;
	}

	return pNode->pItem;
}

void* SeLinkListGetCurrentItem(SeLinkList* pList)
{
	if(pList == SeNull)
		return SeNull;
	return pList->pItem;
}

SeLinkList* SeLinkListGetNextNode(SeLinkList* pList)
{
	if(pList == SeNull)
		return SeNull;
	return pList->pNext;
}

SeBool SeLinkListNoNext(SeLinkList* pList)
{
	if(pList == SeNull)
		return SeTrue;
	if(pList->pNext == SeNull)
		return SeTrue;
	return SeFalse;
}
