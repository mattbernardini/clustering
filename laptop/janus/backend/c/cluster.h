/*
 * cluster.h
 *
 *  Created on: Apr 22, 2018
 *      Author: matt
 */

#include "edge.h"
#include "vertex.h"
#include "vmap.h"
#include <stdint.h>
#ifndef CLUSTER_H_
#define CLUSTER_H_



struct _list {
	struct _list * next;
	void * data;
};
typedef struct _list * list;
struct _head {
	list _head;
	uint64_t _length_of_list;
};
typedef struct _density {
	uint64_t start_loc;
	uint64_t end_loc;
	float den;
}density_loc;
typedef struct _head head;
struct _cluster
{	
	head _edges;
	head _verticies;
	struct{
		float * _vert_map;
		uint64_t _num_vert_map;
	};
	struct{
		density_loc * _densities;
		uint64_t _num_densities;
	};
	struct{
		float * _vert_mat;
		uint64_t _num_vert_mat;
	};
	float _base_density;
	int mapNum;
	int id;
};

typedef struct _cluster * Cluster;

Cluster init_cluster (
	list edges,
	list verticies);
void find_and_set_base_density(Cluster self); // HAS SIDE EFFECTS
void find_and_build_densities(Cluster self); // HAS SIDE EFFECTS
bool should_cluster_be_split(Cluster self);
Cluster split_cluster(Cluster self);

#endif /* SRC_CLUSTER_H_ */
