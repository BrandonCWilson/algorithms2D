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
	return pq;
}

void pq_delete(PriorityNode *pnode)
{
	if (!pnode)
		return;
	memset(pnode,NULL,sizeof(PriorityNode));
}

void pq_free_node(PriorityNode *pnode)
{
	if (!pnode)
		return;
	if (pnode->data != NULL)
	{
		slog("Not free-ing this node, there's still data here.");
		return;
	}
	free(pnode);
}

void pq_free_queue(PriorityQueue *pq) 
{
	int i;
	if (!pq)
		return;
	for (i = 0; i < pq->numElements; i++)
	{
		pq_free_node(&pq->nodeList[i]);
	}
	free(pq);
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