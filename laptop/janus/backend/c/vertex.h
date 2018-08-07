/*
 * vertex.h
 *
 *  Created on: Apr 22, 2018
 *      Author: matt
 */
//#include "edge.h"
//#include "linked_list.h"
#include <forward_list>
#include "edge.h"
#ifndef SRC_VERTEX_H_
#define SRC_VERTEX_H_

struct _vertex
{
	int id;
	int clusterId;
	char * label;
	double weight;
	std::forward_list<Edge> edges;
	float (*getEdgeWeightTotal)(struct _vertex * self);
};
struct _vertex * initVertex(
		char * label,
		float weight,
		int clusterId);

typedef struct _vertex * Vertex;

#endif /* SRC_VERTEX_H_ */
