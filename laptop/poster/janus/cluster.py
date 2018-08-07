import numpy as np
import logging
import tensorflow as tf

global cluster_id
cluster_id = 0


class Cluster:
    def __init__(self):
        global cluster_id
        self.id = cluster_id
        cluster_id += 1
        self.vertices = []
        self.edges = []
        self.vertex_matrix = None
        self.base_density = 0
        self.vertex_to_array_loc_map = {}
        self.array_loc_to_vertex_map = {}

    def does_vertex_exist(self, vertex):
        try:
            self.verticies.index(vertex)
        except ValueError:
            return False
        return True

    def check_edges(self):
        for edge in self.edges:
            try:
                self.vertices.index(edge.source)
                self.vertices.index(edge.target)
            except ValueError:
                self.edges.remove(edge)

    def order_vertices(self):


    def build_array(self):
        self.make_vert_map()
        self.vertex_matrix = np.zeros(shape=(self.vertices.__len__(), self.vertices.__len__()))
        for edge in self.edges:
            source = self.vertex_to_array_loc_map[edge.target.id]
            target = self.vertex_to_array_loc_map[edge.source.id]
            self.vertex_matrix[source][target] = edge.weight
            self.vertex_matrix[target][source] = edge.weight
        print("done")

    def compute_base_density(self):
        self.base_density = np.sum(self.vertex_matrix) / (self.vertices.__len__() ** 2)

    def make_vert_map(self):
        self.vertex_to_array_loc_map.clear()
        counter = 0
        for vertex in self.vertices:
            self.vertex_to_array_loc_map[vertex.id] = counter
            self.array_loc_to_vertex_map[counter] = vertex.id
            counter += 1

    def should_cluster_be_split(self):
        return

    def split_cluster(self, new_cluster):
        return