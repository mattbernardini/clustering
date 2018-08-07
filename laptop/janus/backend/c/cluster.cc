/*
 * cluster.c
 *
 *  Created on: Apr 22, 2018
 *      Author: matt
 */
#include "cluster.h"
#include <math.h>
#ifdef ENV_OMP
#include <omp.h>
#endif // OMP

static density_loc find_split_point(Cluster self)
{
	density_loc rt_value = { 0 };
	uint64_t num_verticies = self->_verticies._length_of_list;
	for (size_t row = 0; row < num_verticies; row++)
		for (size_t col = row; col < num_verticies; col++)
			if (self->_densities[(num_verticies * row) + col].den > rt_value.den)
				rt_value = self->_densities[(num_verticies * row) + col];
	return rt_value;
}

uint64_t summation(uint64_t n)
{
	if (n == 1)
		return 1;
	else
		return n + summation(n -1);
}


list push_front(list old_head, void * data)
{
#ifdef ENV_MPI
	list new_head = (list)shmalloc(sizeof(struct _list));
#else
	list new_head = (list)calloc(1, sizeof(struct _list));
#endif
	new_head->data = data;
	new_head->next = old_head ? old_head : NULL;
	return new_head;
}
Cluster init_cluster (
	list edges,
	list verticies)
{
#ifdef ENV_MPI
	Cluster new_cluster = (Cluster)shmalloc(sizeof(struct _cluster));
#else
	Cluster new_cluster = (Cluster)calloc(1, sizeof(struct _cluster));
#endif
	new_cluster->_base_density = 0;
	new_cluster->mapNum = 0;
	new_cluster->id = 0;
	if (edges)
#ifdef ENV_OMP
		#pragma omp parallel for
#endif // OMP
		for (list travler = edges; travler != NULL; travler = travler->next)
			new_cluster->_edges._head = push_front(new_cluster->_edges._head, travler);
	if (verticies)
#ifdef ENV_OMP // TODO: Check that this can be parallelized via OMP
		#pragma omp parallel for
#endif // OMP
		for (list travler = verticies; travler != NULL; travler = travler->next)
			new_cluster->_verticies._head = push_front(new_cluster->_verticies._head, travler),
			new_cluster->_verticies._length_of_list++;
	return new_cluster;
}

void find_and_set_base_density(Cluster self)
{
	float numerator = 0,
				denominator = pow(self->_verticies._length_of_list, 2);
#ifdef DIRECTED
	for (size_t i = 0; i < self->_verticies._length_of_list; i++)
		for (size_t j = 0; j < self->_verticies._length_of_list; j++)
			numerator += self->_vert_map[(self->_verticies._length_of_list * i) + j];
#else
	for (size_t i = 0; i < self->_verticies._length_of_list; i++)
		for (size_t j = i; j < self->_verticies._length_of_list; j++)
			numerator += self->_vert_map[(self->_verticies._length_of_list * i) + j];	
#endif // DEBUG
	self->_base_density = numerator / denominator;
}

void find_and_build_densities(Cluster self)
{
	uint64_t num_verticies = self->_verticies._length_of_list;
#ifdef ENV_MPI
	if (self->_densities)
		shfree(self->_densities);
	self->_densities = (density_loc*)shmalloc(sizeof(density_loc) * summation(num_verticies));
#else // ENV_MPI
	if (self->_densities)
		free(self->_densities);
	self->_densities = (density_loc*)calloc(summation(num_verticies), sizeof(density_loc));
#endif // ENV_MPI
	for (size_t outer_limit = 0; outer_limit < num_verticies; outer_limit++)
	{
		for (size_t inner_limit = 0; inner_limit < outer_limit; inner_limit++)
		{
			float numerator = 0, denominator = pow(outer_limit - inner_limit, 2);
			for (size_t row_position = inner_limit; row_position < outer_limit; row_position++)
#ifdef DIRECTED
				for(size_t col_position = inner_limit; col_position < row_position; col_position++)
#else  // DIRECTED
				for(size_t col_position = row_position; col_position < outer_limit; col_position++)
#endif // DIRECTED
					numerator += self->_vert_mat[(num_verticies * row_position) + col_position];
			self->_densities[(num_verticies * outer_limit) + inner_limit] = { 
					.den = numerator / denominator,
					.start_loc = inner_limit,
					.end_loc = outer_limit
				};
		}
	}
}

bool should_cluster_be_split(Cluster self)
{
	uint64_t num_verticies = self->_verticies._length_of_list;
	for (size_t row = 0; row < num_verticies; row++)
		for (size_t col = row; col < num_verticies; col++)
			if (self->_densities[(num_verticies * row) + col].den > self->_base_density)
				return true;
	return false;
}

Cluster split_cluster(Cluster self)
{
	Cluster new_cluster = init_cluster(NULL, NULL);
	density_loc split_point = find_split_point(self);
	// Move travler into position

}