/*
 * Vertex.h
 *
 *  Created on: Mar 27, 2018
 *      Author: matt
 */

#include <vector>
#include <string.h>
#include <stdlib.h>
#include "base_logger.h"

#include "edge.h"

#ifndef BFS_VERTEX_H_
#define BFS_VERTEX_H_

namespace AI {
/*
 *
 */
class Vertex {
private:
	unsigned int index;		// make large in order to use for other objects
	double weight;
	std::vector<AI::Edge *> * edges;	// edges
	char * label;
	int clusterId;
public:
	Vertex(unsigned int index);
	virtual ~Vertex();

	/*
	 * Getters
	 */
	double getWeight(){return this->weight;};
	char * getLabel(){return this->label;};
	unsigned int getIndex(){return this->index;};
	int getClusterId(){return this->clusterId;};
	std::vector<AI::Edge *> * getEdges() {return this->edges;};
	float getEdgeWeightTotal();
	/*
	 * Setters
	 */
	void setIndex(unsigned long long index) {this->index = index;};
	void setWeight(double weight){this->weight = weight;};
	void setLabel(char * label);
	void addEdge (AI::Edge * edgeToAdd){this->edges->push_back(edgeToAdd);};
	void setClusterId(int clusterId){this->clusterId = clusterId;};

};

} /* namespace AI */

#endif /* BFS_VERTEX_H_ */
