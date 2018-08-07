/*
 * Vertex.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: matt
 */

#include "vertex.h"

#include <vector>
namespace AI {

Vertex::Vertex(unsigned int index)
{
	this->index = index;
	this->weight = 0;
	this->edges = new std::vector<AI::Edge *>();
	this->label = NULL;
}

Vertex::~Vertex() {
	delete this->edges;
}

void Vertex::setLabel(char * label)
{
    int len = strlen(label);
    char * newLabel = (char *)malloc(sizeof(char) * len);
    if (this->label != NULL)
    {
        free(this->label);
        this->label = NULL;
    }
    this->label = newLabel;
}
float Vertex::getEdgeWeightTotal()
{
	float rtValue = 0;
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); it++)
	{
		rtValue += (*it)->getWeight();
	}
	return rtValue;
}

} /* namespace AI */
