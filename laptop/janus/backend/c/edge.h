/*
 * edge.h
 *
 *  Created on: Apr 22, 2018
 *      Author: matt
 */

#ifndef SRC_EDGE_H_
#define SRC_EDGE_H_
struct _vertex;
struct _edge
{
	int id;
	struct _vertex * source;
	struct _vertex * target;
	float weight;
	struct _vertex * (*getOther)(struct _edge * self, struct _vertex * vertex);
};
struct _edge * initEdge(
		struct _vertex * source,
		struct _vertex * target,
		float weight);
typedef struct _edge * Edge;
#endif /* SRC_EDGE_H_ */
