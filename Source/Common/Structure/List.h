/*
 * List.h
 *
 *  Created on: 2015-12-7
 *      Author: Administrator
 */

#ifndef _LIST_H_
#define _LIST_H_

//#include "../../Global/Include.h"
#include "../Types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SeLinkListType
{
	void* pItem;
	struct SeLinkListType* pNext;
}SeLinkList;

SeLinkList* SeLinkListCreate(void);
void SeLinkListFree(SeLinkList* pList);
void SeLinkListAddNode(SeLinkList* pList, void* pItem);
void SeLinkListRemoveNode(SeLinkList* pList, void* pItem);
SeInt16 SeLinkListGetNodeCount(SeLinkList* pList);
void* SeLinkListGetItemByIndex(SeLinkList* pList, SeInt16 iIndex);

void* SeLinkListGetCurrentItem(SeLinkList* pList);
SeLinkList* SeLinkListGetNextNode(SeLinkList* pList);
SeBool SeLinkListNoNext(SeLinkList* pList);

#ifdef __cplusplus
}
#endif

#endif /* _LIST_H_ */
