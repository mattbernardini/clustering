/*
 * Network.cpp
 *
 *  Created on: Mar 27, 2018
 *      Author: matt
 */

#include "Network.h"
#include <cstdio>
#include <vector>
#include <queue>
#include <math.h>
#include <unistd.h>
namespace AI {
static const char className[] = "Network";
Vertex * Network::findVertexPtr (std::vector<Vertex *> *v, int index)
{
	for (std::vector<Vertex *>::iterator it = v->begin();
			it != v->end(); ++it)
	{
		if ((*it)->getIndex() == static_cast<unsigned int>(index))
		{
			return (*it);
		}
	}
	return NULL;
}

Network::Network(char * fileName, BaseLogger * logger)
{
	// TODO Auto-generated constructor stub
	this->logger = logger;
	const char r[] = {'r'};
    this->fileName = fileName;
	this->verticies = new std::vector<Vertex *>();
	this->edges = new std::vector<Edge *> ();
	this->vertMatrix = NULL;
	this->clusters = new std::vector<AI::Cluster *>();
	this->inputFile = fopen(fileName, r);
	this->directed = false;
	this->rowSize = NULL;
	this->density = NULL;
	this->gmlWriter = NULL;
	this->readGmlFile();
	this->bfs();
	this->findClusters();
	this->printFinalClusters();
}

Network::~Network() {
	// TODO Auto-generated destructor stub
}
void Network::findClusters()
{
	this->logger->writeToLogFile(INFO, "%s: Making clusters", className);
	/*
	 * Queue all clusters that we found
	 */
	int lastClusterCount = 0;
	std::queue<Cluster *> q;
	for (unsigned int i = 0; i < this->clusters->size(); i++)
	{
		q.push(this->clusters->at(i));
	}
	/*
	 * Keep queueing each cluster until they cannot be split
	 */
	while (!q.empty())
	{
		// Pull the first cluster from the queue
		Cluster * tmp = q.front();
		q.pop();

		// See if we can split it
		if (tmp->shouldClusterBeSplit())
		{
			fprintf(stderr, "Making new cluster\n");
			// If we can then make a new cluster and add
			// them both to the queue
			//tmp->logClusterState();
			AI::Cluster * newCluster = new AI::Cluster(this->logger);
			tmp->splitCluster(newCluster);
			this->clusters->push_back(newCluster);
			if (!tmp->getVerticies()->empty())
				q.push(tmp);
			if (!newCluster->getVerticies()->empty())
				q.push(newCluster);
			//newCluster->logClusterState();
		}
	}
	for (int i = 0; i < this->clusters->size(); i++)
		this->clusters->at(i)->setVertexClusterId();
	fprintf(stderr, "Done making clusters\n");
}
void Network::readGmlFile()
{
	char string[50];
	/* Clear out beginning of file */
	while (1)
	{
		/* If we hit the end of the file log and exit */
		if(feof(this->inputFile))
		{
			this->logger->writeToLogFile(FATAL, "%s: No graph declaration", className);
			exit(-1);
		}


		fscanf(this->inputFile, "%s", string);

		/* Stop when we get to graph declaration */
		if(strncmp(string, "graph", 5) == 0)
			break;
	}
	while(1)
	{
		/* If we hit the end of the file no edges */
		if (feof(this->inputFile))
			break;
		/* Scan in next string */
		fscanf(this->inputFile, "%s", string);
		/* Check to see if we have a node id */
		if (strncmp(string, "id", 2) == 0)
		{
			/* If we did create new vertex and add to vector */
			fscanf(this->inputFile, "%s", string);
			int num = atoi(string);
			this->verticies->push_back(new Vertex(static_cast<unsigned int>(num)));
			this->logger->writeToLogFile(INFO, "%s: New vertex with ID: %i", className, num);
		}
		/* Check to see if we made it to edge declarations */
	    if(strncmp(string, "edge", 4) == 0)
	      break; // stop when read in first "edge"
	}
	while (!feof(this->inputFile)) { // read edges until reach end of file
	    fscanf(this->inputFile, "%s", string); // read in string from file

	    if(!strncmp(string, "[", 1) == 0) // check for correct character
	    {
			this->logger->writeToLogFile(FATAL, "Malformed gml file");
	    	break;
	    }
	    fscanf(this->inputFile, "%s", string); // read in string from file
	    if(!strncmp(string, "source", 6) == 0) // check for correct character
	    {
			this->logger->writeToLogFile(FATAL, "Malformed gml file");
	    	exit(-1);
	    }

	    fscanf(this->inputFile, "%s", string); // read in start node from file
	    Vertex * source = this->findVertexPtr(this->verticies, atoi(string));

	    fscanf(this->inputFile, "%s", string); // read in string from file
	    if(!strncmp(string, "target", 6) == 0) // check for correct character
	    {
			this->logger->writeToLogFile(FATAL, "Malformed gml file");
	    	 exit(-1);
	    }


	    fscanf(this->inputFile, "%s", string); // read in target node from file
	    Vertex * target = this->findVertexPtr(this->verticies, atoi(string));

	    fscanf(this->inputFile, "%s", string); // read in string from file

	    float weight = 1.0;

	    if(strncmp(string, "weight", 6) == 0)
	    {// check to see if weight specified
	      fscanf(this->inputFile, "%s", string); // read in string from file
	      weight = atof(string);
	      fscanf(this->inputFile, "%s", string); // read in string from file
	      fscanf(this->inputFile, "%s", string); // read in string from file
	    }
	    else
	    {
	    	fscanf(this->inputFile, "%s", string);

	    }
	    this->logger->writeToLogFile(
			DEBUG,
			"%s: Added new edge connecting vertexs source: %i and target: %i with weight %f",
			className, source->getIndex(), target->getIndex(), weight);
	    this->edges->push_back(new Edge(source, target, weight));
	}
}
void Network::bfs(AI::Cluster * clusters)
{
	std::vector<Vertex *> visitied;
	std::queue<Vertex *> q;
	for (std::vector<Vertex *>::iterator it = clusters->getVerticies()->begin();
			it != clusters->getVerticies()->end(); ++it)
	{
		/*
		 * If we have already visited this point ignore it
		 */
		if (this->findVertexPtr(&visitied, (*it)->getIndex()) == NULL)
		{
			q.push((*it));
			/*
			 * Else we loop through the edges and make the graph
			 */
			AI::Cluster * cluster = new AI::Cluster(this->logger);
			while (!q.empty())
			{
				Vertex * tmp = q.front();
				q.pop();
				if (this->findVertexPtr(&visitied, tmp->getIndex()) == NULL)
				{
					visitied.push_back(tmp);
					cluster->addVertex(tmp);
					for (std::vector<Edge *>::iterator eit = tmp->getEdges()->begin();
							eit != tmp->getEdges()->end(); ++eit)
					{
						cluster->addEdge((*eit));
						Vertex * o = (*eit)->getSource()->getIndex() == tmp->getIndex() ?
								(*eit)->getTarget() :
								(*eit)->getSource();
						q.push(o);
					}
				}

			}
			//cluster->logClusterState();
			this->clusters->push_back(cluster);
		}

	}
	for (unsigned int i = 0; i < this->clusters->size(); i++)
	{
		fprintf(stderr, "New cluster %i\n", i);
		for (std::vector<Vertex *>::iterator it = (this->clusters->at(i))->getVerticies()->begin();
					it !=  (this->clusters->at(i))->getVerticies()->end(); ++it)
		{
			fprintf(stderr, "%u\n", static_cast<unsigned int>((*it)->getIndex()));
		}
	}
}
void Network::bfs()
{
	std::vector<Vertex *> visitied;
	std::queue<Vertex *> q;
	for (std::vector<Vertex *>::iterator it = this->verticies->begin();
			it != this->verticies->end(); ++it)
	{
		/*
		 * If we have already visited this point ignore it
		 */
		if (this->findVertexPtr(&visitied, (*it)->getIndex()) == NULL)
		{
			q.push((*it));
			/*
			 * Else we loop through the edges and make the graph
			 */
			AI::Cluster * cluster = new AI::Cluster(this->logger);
			while (!q.empty())
			{
				Vertex * tmp = q.front();
				q.pop();
				if (this->findVertexPtr(&visitied, tmp->getIndex()) == NULL)
				{
					visitied.push_back(tmp);
					cluster->addVertex(tmp);
					for (std::vector<Edge *>::iterator eit = tmp->getEdges()->begin();
							eit != tmp->getEdges()->end(); ++eit)
					{
						cluster->addEdge((*eit));
						Vertex * o = (*eit)->getSource()->getIndex() == tmp->getIndex() ?
								(*eit)->getTarget() :
								(*eit)->getSource();
						q.push(o);
					}
				}

			}
			//cluster->logClusterState();
			this->clusters->push_back(cluster);
		}

	}
	for (unsigned int i = 0; i < this->clusters->size(); i++)
	{
		fprintf(stderr, "New cluster %i\n", i);
		for (std::vector<Vertex *>::iterator it = (this->clusters->at(i))->getVerticies()->begin();
					it !=  (this->clusters->at(i))->getVerticies()->end(); ++it)
		{
			fprintf(stderr, "%u\n", static_cast<unsigned int>((*it)->getIndex()));
		}
	}
}
void Network::clearOutClusters()
{
	std::queue<Cluster *> q;
	for (unsigned int i = 0; i < this->clusters->size(); i++)
	{
		if (this->clusters->at(i)->getVerticies()->size() > 0 ||
				!this->clusters->at(i)->getVerticies()->empty())
		{

		}
		else
			q.push(this->clusters->at(i));
	}
	while (!q.empty())
	{
		Cluster * tmp = q.front();
		this->removeCluster(tmp);
		free(tmp);
	}
}
void Network::removeCluster(Cluster * cluster)
{
	int counter = 0;
	for (std::vector<Cluster *>::iterator it = this->clusters->begin();
			it != this->clusters->end(); it++, counter++)
	{
		if (cluster->getId() == (*it)->getId())
		{
			this->clusters->erase(this->clusters->begin() + counter);
			return;
		}
	}
}
void Network::printFinalClusters()
{
	//this->clearOutClusters();
	for (unsigned int i = 0; i < this->clusters->size(); i++)
	{
		if (this->clusters->at(i)->getVerticies()->size() > 0 ||
				!this->clusters->at(i)->getVerticies()->empty())
		{
			this->clusters->at(i)->logClusterState();
		}
	}
	fprintf(this->logger->logFile, "\n\n");
	for (int i = 0; i < this->verticies->size(); i++)
		fprintf(this->logger->logFile, "%d", this->verticies->at(i)->getClusterId());
}
} /* namespace AI */
