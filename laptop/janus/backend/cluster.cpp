/*
 * cluster.cpp
 *
 *  Created on: Mar 31, 2018
 *      Author: matt
 */

#include "cluster.h"

namespace AI {
Cluster::Cluster(BaseLogger * logger)
{
  	this->logger = logger;
  	this->verticies = new std::vector<AI::Vertex *>();
  	this->edges = new std::vector<AI::Edge *>();
  	this->vertToArrayLoc = new std::map<int, int>();
  	this->vert = new std::map<int, int>();
  	this->density = NULL;
  	this->vertMatrix = NULL;
  	this->baseDensity = 0;
  	this->mapNum = 0;
  	this->id = clusterId++;
  	this->gmlWriter = NULL;
  }
};
