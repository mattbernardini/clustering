/*
 * cluster.h
 *
 *  Created on: Mar 31, 2018
 *      Author: matt
 */
#include "base_logger.h"
#include "gml_writer.h"
#include <forward_list>

#include "edge.h"
#include "vertex.h"
#ifndef BFS_CLUSTER_H_
#define BFS_CLUSTER_H_
namespace AI {

/*
 *
 */
typedef struct {
	int arrayLocation,
		vertexNumber;
} Vmap;
class Cluster {
private:
	/*
	 * Private variables
	 */
	BaseLogger * logger;
	std::forward_list<AI::Vertex *> * verticies;
	std::forward_list<AI::Edge *> * edges;
	std::forward_list<Vmap *> * vertexMap;
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
