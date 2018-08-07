/*
 * Edge.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: matt
 */

#include "Edge.h"
#include "Vertex.h"
namespace AI {
static int edgeIdCounter = 0;
Edge::Edge() {
	this->source = nullptr;
	this->target = nullptr;
	this->weight = 0;
	this->id = edgeIdCounter++;

}
Edge::Edge(AI::Vertex * source, AI::Vertex * target, double weight)
{
	this->source = source;
	this->target = target;
	this->weight = weight;
	source->addEdge(this);
	target->addEdge(this);
	this->id = edgeIdCounter++;
}
AI::Vertex * Edge::getOther(AI::Vertex * v)
{
	return v->getIndex() == this->source->getIndex() ?
			this->target : this->source;
};
Edge::~Edge() {
	// TODO Auto-generated destructor stub
}

} /* namespace AI */
