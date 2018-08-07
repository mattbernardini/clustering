/*
 * linked_list.h
 *
 *  Created on: Apr 22, 2018
 *      Author: matt
 */

#ifndef SRC_LINKED_LIST_H_
#define SRC_LINKED_LIST_H_

struct list {
	struct list * next;
	void * data;
};
typedef struct list * Node;
int getLengthOfList(struct list * self);
Node deleteNode(struct list * self, void * data, bool (*compare_function)(void *, void*));
Node getNodeAt(struct list * self, int index);
Node appendNode(struct list * self, void * data);

#endif /* SRC_LINKED_LIST_H_ */
