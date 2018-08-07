/*
 * cluster.h
 *
 *  Created on: Mar 31, 2018
 *      Author: matt
 */
#include "Edge.h"
#include "Vertex.h"
#include "base_logger.h"
#include "gml_writer.h"
#include <map>
#ifndef BFS_CLUSTER_H_
#define BFS_CLUSTER_H_
namespace AI {

/*
 *
 */
class Cluster {
private:
	/*
	 * Private variables
	 */
	GmlWriter * gmlWriter;
	BaseLogger * logger;
	std::vector<AI::Vertex *> * verticies;
	std::vector<AI::Edge *> * edges;
	std::map<int, int> * vertToArrayLoc;
	std::map<int, int> * vert;
	float * density;
	float * vertMatrix;
	float baseDensity;
	int mapNum;
	int id;
	/*
	 * Private functions
	 */
	void findBaseDensity();
	void findDensities();
	void buildArray();
	AI::Vertex * findNextVertex(std::vector<AI::Vertex *> * currentState,
			std::vector<AI::Vertex *> * currentList);
	Vertex * findVertex(int index);
	void orderVertexs();
	void removeEdge(Edge * edge);
	void removeVertex(Vertex * vert);
	void removeVertex(std::vector<Vertex * > * v, Vertex * vert);
	void redoVertToArrayLoc ();
	void checkEdges();
	bool doesVertexExist(Vertex * vert);
	bool doesVertexExist(std::vector<AI::Vertex *> * v, Vertex * vert);
	void cleanEdges();
	bool doesEdgeExist(Edge * edge);
	bool doesEdgeExist(std::vector<Edge *> * v, Edge * edge);
	void removeDuplicateEdges();
	void orderArray();
	float * makeVertexTensor(Vertex * v);
public:
	/*
	 * Getters
	 * All values are const so that they do not allow changes
	 * All changes must happen through setters/adders
	 */
	std::vector<AI::Edge *> * getEdges(){return this->edges;};
	std::vector<AI::Vertex *> * getVerticies(){return this->verticies;};
	std::map<int, int> * getVertMap(){return this->vertToArrayLoc;};
	const int getId() {return this->id;};
	/*
	 * Setters/adders
	 */
	void addVertex(AI::Vertex * vert);
	void addEdge(AI::Edge * edge);

	Cluster(BaseLogger * logger);
	bool shouldClusterBeSplit();
	void splitCluster(AI::Cluster * newCluster);
	virtual ~Cluster();
	void logClusterState();
	void setVertexClusterId();
};

} /* namespace AI */

#endif /* BFS_CLUSTER_H_ */
