/*
 * cluster.cpp
 *
 *  Created on: Mar 31, 2018
 *      Author: matt
 */

#include "cluster.h"

#include <math.h>
#include <queue>
#include <stdlib.h>
namespace AI {
static int clusterId = 0;
static const char className[] = "Cluster";
Cluster::Cluster(BaseLogger * logger)
{
	this->logger = logger;
	this->verticies = new std::forward_list<AI::Vertex *>();
	this->edges = new std::forward_list<AI::Edge *>();
	this->vertexMap = new std::forward_list<Vmap *>();
	this->density = NULL;
	this->vertMatrix = NULL;
	this->baseDensity = 0;
	this->mapNum = 0;
	this->id = clusterId++;
}

Cluster::~Cluster() {

}
void Cluster::logClusterState()
{
	this->cleanEdges();
	char buffer[4096], tmp[16];
	sprintf(tmp, "%i", this->id);
	strcat(tmp, ".gml");
	this->gmlWriter = new GmlWriter(tmp);
	fprintf(stderr, "Opened gml file %s\n", tmp);
	memset(buffer, '\0', sizeof(char) * 512);
	memset(tmp, '\0', sizeof(char) * 16);
	strcat(buffer, "Vertex: ");
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); ++it)
	{
		this->gmlWriter->writeNode(static_cast<int>((*it)->getIndex()));
		//sprintf(tmp, "%4i ", static_cast<int>((*it)->getIndex()));
		//strcat(buffer, tmp);
		//memset(tmp, '\0', sizeof(char) * 16);
	}
	strcat(buffer, "\n\t\t\t\t\t\tEdges: ");
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); ++it)
	{
		this->gmlWriter->writeEdge(static_cast<int>((*it)->getSource()->getIndex()),
				static_cast<int>((*it)->getTarget()->getIndex()), (*it)->getWeight());
		//sprintf(tmp, "%u-%u\t", (*it)->getSource()->getIndex(), (*it)->getTarget()->getIndex());
		//strcat(buffer, tmp);
	}
	this->gmlWriter->writeEnd();
    this->gmlWriter->~GmlWriter();
	//this->logger->writeToLogFile(INFO, "%s %i: %s",
	//		className, this->id, buffer);
}
bool Cluster::doesVertexExist(Vertex * vert)
{
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); ++it)
	{
		if (vert->getIndex() == (*it)->getIndex())
			return true;
	}
	return false;
}
bool Cluster::doesVertexExist(std::vector<AI::Vertex *> * v, Vertex * vert)
{
	for (std::vector<Vertex *>::iterator it = v->begin();
			it != v->end(); ++it)
	{
		if (vert->getIndex() == (*it)->getIndex())
			return true;
	}
	return false;
}
bool Cluster::doesEdgeExist(Edge * edge)
{
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); ++it)
	{
		if (edge->getId() == (*it)->getId())
			return true;
	}
	return false;
}
bool Cluster::doesEdgeExist(std::vector<Edge *> * v, Edge * edge)
{
	for (std::vector<Edge *>::iterator it = v->begin();
			it != v->end(); ++it)
	{
		if (edge->getId() == (*it)->getId())
			return true;
	}
	return false;
}
void Cluster::removeDuplicateEdges()
{
	std::queue<Edge *> p;
	std::vector<Edge *> seen;
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); it++)
	{
		if (!this->doesEdgeExist(&seen, (*it)))
		{
			seen.push_back((*it));
		}
		else
		{
			p.push((*it));
		}
	}
	while (!p.empty())
	{
		Edge * tmp = p.front();
		p.pop();
		this->removeEdge(tmp);
	}
}
float * Cluster::makeVertexTensor(Vertex * v)
{
	float * rtTensor = (float*)calloc(this->verticies->size(), sizeof(float));
	for (std::vector<Edge *>::iterator it = v->getEdges()->begin();
			it != v->getEdges()->end(); it++)
	{
		rtTensor[(*this->vertToArrayLoc)[(*it)->getOther(v)->getIndex()]] = (*it)->getWeight();
	}
	return rtTensor;
}
AI::Vertex * Cluster::findNextVertex(std::vector<AI::Vertex *> * available,
		std::vector<AI::Vertex *> * currentList)
{
	Vertex 	* last = currentList->back();
	float 	* compare = this->makeVertexTensor(last),
			* other = NULL,
			bestSoFar = 100,
			test = 0;
	Vertex 	* rtVertex = available->front();
	for (int i = 0; i < available->size(); i++)
	{
		test = 0;
		other = this->makeVertexTensor(available->at(i));
		for (int j = 0; j < this->verticies->size(); j++)
			test += abs(other[j] - compare[j]);
		if (bestSoFar > test)
		{
			rtVertex = available->at(i);
			bestSoFar = test;
		}
	}
	return rtVertex;
}
void Cluster::orderVertexs()
{
	Vertex * start = this->verticies->front();
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); ++it)
	{
		if (start->getEdgeWeightTotal() < (*it)->getEdgeWeightTotal())
			start = (*it);
	}
	std::vector<AI::Vertex *> * currentList = new std::vector<AI::Vertex *>(*this->verticies);
	this->removeVertex(currentList, start);
	std::vector<AI::Vertex *> * finalOrdering = new std::vector<AI::Vertex *>();
	finalOrdering->push_back(start);
	while (finalOrdering->size() != this->verticies->size())
	{
		AI::Vertex * tmp = this->findNextVertex(currentList, finalOrdering);
		this->removeVertex(currentList, tmp);
		finalOrdering->push_back(tmp);
	}
}
void Cluster::checkEdges()
{
	std::queue<Edge *> p;
	std::vector<Edge *> t;
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); it++)
	{
		if (!this->doesVertexExist((*it)->getTarget()) ||
				!this->doesVertexExist((*it)->getSource()))
		{
			p.push((*it));
			t.push_back((*it));
		}
	}
	while (!p.empty())
	{
		Edge * tmp = p.front();
		p.pop();
		this->removeEdge(tmp);
	}
	this->removeDuplicateEdges();
}
void Cluster::redoVertToArrayLoc ()
{
	this->vertToArrayLoc->clear();
	this->vert->clear();
	this->mapNum = 0;
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); ++it)
	{
		this->vertToArrayLoc->insert(std::pair<int, int>((*it)->getIndex(), this->mapNum));
		this->vert->insert(std::pair<int, int>(this->mapNum++, (*it)->getIndex()));
	}
}
void Cluster::addVertex(AI::Vertex * vert)
{
	this->verticies->push_back(vert);
	this->vertToArrayLoc->insert(std::pair<int, int>(vert->getIndex(), this->mapNum));
	this->vert->insert(std::pair<int, int>(this->mapNum++, vert->getIndex()));
	this->logger->writeToLogFile(INFO, "%s %i: Added new vertex %i",
			className, this->id, vert->getIndex());
}
void Cluster::addEdge(AI::Edge * edge)
{
	this->edges->push_back(edge);
}
void Cluster::buildArray()
{
	this->checkEdges();
	this->orderVertexs();
	fprintf(stdout, "Cluster %i, Row Size: %i\n", this->id,(int) this->verticies->size());
	int s = pow(this->verticies->size(), 2);
	if (this->vertMatrix != NULL)
		free(this->vertMatrix);
	this->vertMatrix = (float*)malloc(sizeof(float) * s);
	memset(this->vertMatrix, 0, sizeof(float) * s);
	for (int i = 0; i < this->edges->size(); i++)
	{
		int sLoc = (*this->vertToArrayLoc)[this->edges->at(i)->getSource()->getIndex()];
		int tLoc = (*this->vertToArrayLoc)[this->edges->at(i)->getTarget()->getIndex()];
		this->vertMatrix[(sLoc * this->verticies->size()) + tLoc] = this->edges->at(i)->getWeight();
		this->vertMatrix[(tLoc * this->verticies->size()) + sLoc] = this->edges->at(i)->getWeight();
	}
	/*
	for (int i = 0; i < s; i++)
	{
		char buf[128];
		snprintf (buf, sizeof buf, "%f ", this->vertMatrix[i]);
		fprintf(stdout, "%s", buf);
		if (i != 0 && i % this->verticies->size() == this->verticies->size() - 1)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n\n");
	*/
}
void Cluster::findBaseDensity()
{
	this->buildArray();
	int s = this->verticies->size();
	float numerator = 0,
			denominator = (s) * (s);
	// Loop controls finding numerator
	for (int j = 0; j < s; j++)
	{
		for (int k = 0; k < s; k++)
		{
			numerator += this->vertMatrix[(s * j) + k];
		}
	}
	this->baseDensity = numerator / denominator;
}
void Cluster::setVertexClusterId()
{
	for (int i = 0; i < this->verticies->size(); i++)
		this->verticies->at(i)->setClusterId(this->id);
}
bool Cluster::shouldClusterBeSplit()
{
	/*
	 * Make sure we are ready to calculate this
	 */
	this->findDensities();
	bool rtvalue = false;
	for (unsigned int i = 0; i < this->verticies->size() * 2; i++)
	{
		if (this->density[i] > this->baseDensity)
		{
			rtvalue = true;
			fprintf(stdout, "Base: %f, New: %f\n", this->baseDensity,
					this->density[i]);
		}

	}
	return rtvalue;
}
Vertex * Cluster::findVertex(int index)
{
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); ++it)
	{
		if ((*it)->getIndex() == static_cast<unsigned int>(index))
		{
			return (*it);
		}
	}
	return NULL;
}
void Cluster::removeEdge(Edge * edge)
{
	int counter = 0;
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); it++, counter++)
	{
		if (edge->getId() == (*it)->getId())
		{
			this->edges->erase(this->edges->begin() + counter);
			return;
		}
	}
}
void Cluster::removeVertex(Vertex * vert)
{
	int counter = 0;
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); it++, counter++)
	{
		if (vert->getIndex() == (*it)->getIndex())
		{
			this->verticies->erase(this->verticies->begin() + counter);
			return;
		}
	}
}
void Cluster::removeVertex(std::vector<Vertex * > * v, Vertex * vert)
{
	int counter = 0;
	for (std::vector<Vertex *>::iterator it = v->begin();
			it != v->end(); it++, counter++)
	{
		if (vert->getIndex() == (*it)->getIndex())
		{
			v->erase(v->begin() + counter);
			return;
		}
	}
}
void Cluster::cleanEdges()
{
	std::queue<Edge *> q;
	for (std::vector<Edge *>::iterator it = this->edges->begin();
			it != this->edges->end(); it++)
	{
		q.push((*it));
	}
	while (!q.empty())
	{
		Edge * tmp = q.front();
		q.pop();
		if ((this->findVertex(static_cast<int>(tmp->getSource()->getIndex())) == NULL) ||
				this->findVertex(static_cast<int>(tmp->getSource()->getIndex())) == NULL)
		{
			this->removeEdge(tmp);
		}
	}
}
void Cluster::splitCluster(AI::Cluster * newCluster)
{
	this->findDensities();
	unsigned int maxDensity = 0;
	for (unsigned int i = 0; i < this->verticies->size() * 2; i++)
	{
		if (this->density[i] > this->density[maxDensity])
			maxDensity = i;
	}
	if (maxDensity >= this->verticies->size())
	{
		for (unsigned int i = this->verticies->size() - 1; i >= maxDensity; i--)
		{
			/*
			 * Move the vertex to the new cluster and remove from ours
			 */
			Vertex * tmp = this->findVertex(this->vert->at(i));
			newCluster->addVertex(tmp);
			this->removeVertex(tmp);
			/*
			 * Move all of the edges
			 */
			for (std::vector<Edge *>::iterator it = tmp->getEdges()->begin();
					it != tmp->getEdges()->end(); it++)
			{
				newCluster->addEdge((*it));
				this->removeEdge((*it));
			}

		}
	}
	else
	{
		for (unsigned int i = 0; i <= maxDensity; i++)
		{
			/*
			 * Move the vertex to the new cluster and remove from ours
			 */
			Vertex * tmp = this->findVertex((*this->vert)[i]);
			newCluster->addVertex(tmp);
			this->removeVertex(tmp);
			/*
			 * Move all of the edges
			 */
			for (std::vector<Edge *>::iterator it = tmp->getEdges()->begin();
					it != tmp->getEdges()->end(); it++)
			{
				newCluster->addEdge((*it));
				this->removeEdge((*it));
			}

		}
	}

	//this->cleanEdges();
	//newCluster->cleanEdges();
	this->redoVertToArrayLoc();
}
void Cluster::findDensities()
{
	/*
	 * Make sure we are setup properly
	 */
	this->findBaseDensity();
	// Local variables
	int s = this->verticies->size();
	/*
	 * Allocate memory for densities
	 */
    if (this->density != NULL)
        free(this->density);
	this->density = (float*)malloc(sizeof(float) * s * 2);
	memset(this->density, 0, sizeof(float) * s * 2);
	/*
	 * Loop through all of the rows and columns building up
	 * our density measurments
	 */
	for (int i = 0; i < s; i++)
	{
		/*
		 * Local variables to each iteration
		 */
		float numerator = 0, numerator2 = 0,
				denominator = (i +1) * (i + 1), denominator2 = (i +1) * (i + 1);
		// Loop controls finding numerator
		for (int j = 0; j <= i; j++)
		{
			for (int k = 0; k <= i; k++)
			{
				numerator += this->vertMatrix[(s * j) + k];
				if ((s * (s - j) + (s - k)) < s &&
                        (s * (s-j)) + (s - k) >= 0)
				    numerator2 += this->vertMatrix[(s * (s - j)) + (s - k)];
			}
		}
		// Loop through and build denominator
		for (int j = 0; j <= i; j++)
		{
			for (int k = i + 1; k < s; k++)
			{

				if (this->vertMatrix[(s * j) + k] != 0)
					denominator += this->vertMatrix[(s * j) + k];
				if ((s * (s - j) + (s - k)) < s &&
                        (s * (s-j)) + (s - k) >= 0 &&
                        this->vertMatrix[(s * (s - j)) + (s - k)] != 0 )
					denominator2 += this->vertMatrix[(s * (s - j)) + (s - k)];
			}
		}
		density[i] = numerator / denominator;
		density[s + i] = numerator2 / denominator2;
	}
}
} /* namespace AI */
