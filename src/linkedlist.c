#include "linkedlist.h"
#include "simple_logger.h"

Node *linklist_new_node()
{
	Node *node;
	node = (Node *)malloc(sizeof(Node));
	if (!node)
	{
		slog("unable to allocate new linklist node");
		return NULL;
	}
	memset(node, 0, sizeof(Node));
	return node;
}

int linklist_free_node(Node *node)
{
	if (!node)
	{
		slog("attempting to free a node that points to data");
		return -2;
	}
	if (node->data != NULL)
	{
		slog("attempting to free a node that points to data");
		return -1;
	}
	free(node);
	return 0;
}

/*eol@eof*/