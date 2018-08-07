/*
 * Edge.h
 *
 *  Created on: Mar 27, 2018
 *      Author: matt
 */

#include <vector>
#include "base_logger.h"

#ifndef BFS_EDGE_H_
#define BFS_EDGE_H_

namespace AI {
/*
 * Forward declare class vertex in order to avoid
 * conflicts in includes
 */
class Vertex;

/*
 *
 */
class Edge {
private:
	AI::Vertex * source;
	AI::Vertex * target;
	double weight;
	int id;
public:
	Edge();
	Edge(AI::Vertex * source, AI::Vertex * target, double weight);
	/*
	 * Setters
	 */
	void setWeight(double weight){this->weight = weight;};
	void setTarget(AI::Vertex * target){this->target = target;};
	void setSource(AI::Vertex * source){this->source = source;};
	virtual ~Edge();
	/*
	 * Getters
	 */
	AI::Vertex * getTarget(){return this->target;};
	AI::Vertex * getSource(){return this->source;};
	AI::Vertex * getOther(AI::Vertex * v);
	double getWeight(){return this->weight;};
	int getId(){return this->id;};
};

} /* namespace AI */

#endif /* BFS_EDGE_H_ */
