#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <SDL.h>

typedef struct Node_S Node;
typedef struct LinkedList_S LinkedList;

struct Node_S
{
	void *data;
	struct Node_S *next;
};

struct LinkedList_S
{
	struct Node_S *head;
};

#endif
