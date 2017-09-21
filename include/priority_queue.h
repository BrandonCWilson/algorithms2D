#include <SDL.h>
#include "linkedlist.h"

#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H

typedef struct
{
	void *data;
	Uint32 *priority;
}PriorityNode;

typedef struct
{
	PriorityNode *nodeList;
	int numElements;
}PriorityQueue;

PriorityQueue *pq_new(int numElements);

void pq_delete(PriorityNode *pnode);

void *pq_delete_max(PriorityQueue *pq);

void pq_insert(PriorityQueue *pq, void *data, int priority);

#endif
#pragma once
