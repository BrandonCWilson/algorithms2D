#include "priority_queue.h"
#include "simple_logger.h"

PriorityQueue *pq_new(int numElements)
{
	PriorityQueue *pq;
	if (numElements == 0)
	{
		slog("Cannot create a priority queue with 0 elements");
		return NULL;
	}
	pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
	memset(pq, NULL, sizeof(PriorityQueue));

	pq->nodeList = (PriorityNode *)malloc(sizeof(PriorityNode)*numElements);
	memset(pq->nodeList, NULL, sizeof(PriorityNode)*numElements);

	pq->numElements = numElements;
	if (pq->numElements == 10)
	{
		slog("created a pq with 10 slots;");
	}
	return pq;
}

void pq_delete(PriorityNode *pnode)
{
	memset(pnode,NULL,sizeof(PriorityNode));
}

void *pq_delete_max(PriorityQueue *pq)
{
	int i;
	PriorityNode *max = NULL;
	void *data;

	for (i = 0; i < pq->numElements; i++)
	{
		if (max == NULL)
		{
			if (pq->nodeList[i].data != NULL)
			{
				max = &pq->nodeList[i];
			}
			continue;
		}
		if (pq->nodeList[i].priority >= max->priority)
		{
			max = &(pq->nodeList[i]);
		}
	}
	data = max->data;
	slog("Width returned: %i", max->priority);
	pq_delete(max);
	return data;
}


void pq_insert(PriorityQueue *pq, void *data, int priority) 
{
	int i;

	for (i = 0; i < pq->numElements; i++)
	{
		if (pq->nodeList[i].data == NULL)
		{
			pq->nodeList[i].data = data;
			pq->nodeList[i].priority = priority;
			return;
		}
	}
}