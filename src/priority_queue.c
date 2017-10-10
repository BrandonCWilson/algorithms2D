#include "priority_queue.h"
#include "simple_logger.h"

PriorityNode *pqlist_new_node()
{
	PriorityNode *node;
	node = (PriorityNode *)malloc(sizeof(PriorityNode));
	if (!node)
	{
		slog("unable to allocate new linklist node");
		return NULL;
	}
	memset(node, 0, sizeof(PriorityNode));
	return node;
}

int pqlist_free_node(PriorityNode *node)
{
	if (!node)
	{
		slog("a null node was passed to free_node");
		return -2;
	}
	if (node->data != NULL)
	{
		//slog("attempting to free a node that points to data");
		// here's a gun, free whatever ya want
		//return -1;
	}
	free(node);
	return 0;
}

void *pqlist_delete_max(PriorityQueueList *pq)
{
	PriorityNode *tmp;
	PriorityNode *max = NULL;
	PriorityNode *prev = NULL;
	void *data;
	if (!pq)
	{
		slog("cannot delete max from a null pqlist..");
		return NULL;
	}
	if (pq->head == NULL)
		return NULL;
	if(pq->head->next)
	{
		tmp = pq->head;
		max = pq->head;
		while (tmp->next != NULL)
		{
			if ((int)max->priority < (int)tmp->next->priority)
			{
				prev = tmp;
				max = tmp->next;
			}
			tmp = tmp->next;
		}
		data = max->data;
		if (max == pq->head)
		{
			pq->head = max->next;
		}
		else
			prev->next = max->next;
		pqlist_free_node(max);
	}
	else if (pq->head)
	{
		// only one item in the list
		data = pq->head->data;
		pqlist_free_node(pq->head);
		pq->head = NULL;
	}
	else
	{
		// the list is empty..
		data = NULL;
		slog("the pqlist is empty. returning null..");
	}
	return data;
};

void *pqlist_delete(PriorityQueueList *pq)
{
	void *tmp;
	if (!pq)
	{
		slog("null pqlist passed to pqlist_delete");
		return NULL;
	}
	if (pq->head == NULL)
	{
		return NULL;
	}
	tmp = pq->head;
	pq->head = pq->head->next;
	pqlist_free_node(tmp);
}

void pqlist_free_list(PriorityQueueList *pq)
{
	while (true)
	{
		// pop nodes off the list arbitrarily until it is empty
		if (!pqlist_delete(pq))
			break;
	}
	free(pq);
}

void pqlist_insert(PriorityQueueList *pq, void *data, int priority)
{
	PriorityNode *pn;
	if (!pq)
	{
		slog("cannot insert into a null pqlist");
		return;
	}
	if (!data)
	{
		slog("cannot insert null data into a pqlist");
		return;
	}
	pn = pqlist_new_node();
	pn->data = data;
	pn->priority = priority;
	pn->next = pq->head;
	pq->head = pn;
	return;
}

PriorityQueueList *pqlist_new()
{
	PriorityQueueList *pq;
	pq = (PriorityQueueList *)malloc(sizeof(PriorityQueueList));
	memset(pq, NULL, sizeof(PriorityQueueList));
	return pq;
}

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

	if (!pq)
		return NULL;

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

	if ((!pq) || (!data))
		return;

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