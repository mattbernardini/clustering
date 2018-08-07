/*
 * vmap.h
 *
 *  Created on: Apr 22, 2018
 *      Author: matt
 */
#include <stdlib.h>
#ifndef SRC_C_VMAP_H_
#define SRC_C_VMAP_H_
static int id = 0;
struct _vmap
{
	int id;
	int arryLoc;
	int vertexId;
};
typedef struct _vmap * Vmap;
Vmap initVmap(int arrayLoc, int vertexId)
{
	Vmap rtValue = (Vmap)malloc(sizeof(struct _vmap));
	rtValue->arryLoc = arrayLoc;
	rtValue->vertexId = vertexId;
	rtValue->id = id++;
	return rtValue;
}

#endif /* SRC_C_VMAP_H_ */
