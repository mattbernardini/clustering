/*
 * edge.c
 *
 *  Created on: Apr 22, 2018
 *      Author: matt
 */
#include <stdlib.h>
#include "edge.h"
#include "vertex.h"
static int id = 0;
struct _edge * initEdge(
		struct _edge * self,
		struct _vertex * source,
		struct _vertex * target,
		float weight)
{
	self = (Edge)malloc(sizeof(struct _edge));
	self->id = id++;
	self->source = source;
	self->target = target;
	self->weight = weight;
}
struct _vertex * getOther(struct _edge * self, struct _vertex * vertex)
{
	return vertex->id == self->source->id ? self->target : self->source;
}
