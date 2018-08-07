/*
 * Network.h
 *
 *  Created on: Mar 27, 2018
 *      Author: matt
 */
#include "edge.h"
#include "vertex.h"
#include "cluster.h"
#include "base_logger.h"
#include "gml_writer.h"
#include <vector>
#include <cstdio>
#ifndef BFS_NETWORK_H_
#define BFS_NETWORK_H_
namespace AI {

/*
 *
 */
class Network {
private:
	bool directed;
	GmlWriter * gmlWriter;
	std::vector<AI::Vertex *> * verticies;
	std::vector<AI::Edge *> * edges;
	std::vector<AI::Cluster *> * clusters;
	BaseLogger * logger;
    char * fileName;
	FILE * inputFile;
	double * vertMatrix;
	int * rowSize;
	double * density;
	void removeCluster(Cluster * cluster);
	void clearOutClusters();
	Vertex * findVertexPtr (std::vector<Vertex *> * v, int index);
public:
	Network(char * fileName, BaseLogger * logger);
	virtual ~Network();
	void findClusters();
	void readGmlFile();
	void bfs();
	void bfs(AI::Cluster * clusters);
	void printFinalClusters();
};

} /* namespace AI */

#endif /* BFS_NETWORK_H_ */
