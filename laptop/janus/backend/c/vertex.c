/*
 * vertex.c
 *
 *  Created on: Apr 22, 2018
 *      Author: matt
 */
#include <stdlib.h>
#include "vertex.h"
static int id = 0;
struct _vertex * initVertex(
		struct _vertex * self,
		char * label,
		float weight,
		int clusterId)
{
	self = (Vertex)malloc(sizeof(struct _vertex));
	self->id = id++;
	self->clusterId = clusterId;
	self->weight = weight;
	self->getEdgeWeightTotal = getEdgeWeightTotal;
	if (label != NULL)
	{
		int stringSize = strlen(label);
		self->label = (char*)calloc(stringSize + 1, sizeof(char));
		strncpy(self->label, label, stringSize);
	}
	return self;
}

float getEdgeWeightTotal (struct _vertex * self)
{
	Enode travler = self->edges;
	float total = 0;
	while (travler != NULL)
	{
		total += travler->edge->weight;
		travler = travler->next;
	}
	return total;
}
