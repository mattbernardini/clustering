/*
 * linked_list.c
 *
 *  Created on: Apr 22, 2018
 *      Author: matt
 */

#include "linked_list.h"
#include <stdlib.h>
#include <mpp/shmem.h>

Node appendNode(struct list * self, void * data)
{	/* If we are the first node */
#ifdef ENV_MPI
	Node newNode = (Node)shmalloc(sizeof(struct list));
#else
	Node newNode = (Node)malloc(sizeof(struct list));
#endif
	newNode->next = NULL;
	newNode->data = data;
	if (self == NULL)
		self = newNode;
	else
	{
		Node travler = self;
		while (travler->next != NULL)
			travler = travler->next;
		travler->next = newNode;
	}
	return self;
}
Node deleteNode(struct list * self, void * data, bool (*compare_function)(void *, void*))
{	Node searchNode = self, lagNode = NULL;
	while (searchNode != NULL && !compare_function(searchNode->data, data))
	{	lagNode = searchNode;
		searchNode = searchNode->next;
	}
	if (compare_function(searchNode->data, data))
	{	/* If we are after the first node make head the next node and free */
		if (lagNode == NULL)
		{	if (searchNode->next == NULL)
				self = NULL;
			else
				self = searchNode->next;
			free(searchNode);
		}
		else
		{	lagNode->next = searchNode->next;
			free(searchNode);
		}
	}
	return self;
}
int getLengthOfList(struct list * self)
{	if (self->next == NULL)
		return 1;
	else
		return getLengthOfList(self->next) + 1;
}
Node getNodeAt(struct list * self, int index)
{	int count = 0;
	Node travler = self;
	while (count != index && travler != NULL)
		travler = travler->next;
	return travler;
}


