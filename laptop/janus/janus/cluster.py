import numpy as np
import logging
import tensorflow as tf
import copy

global cluster_id
cluster_id = 0


class Cluster:
    def __init__(self, vertices = None, edges = None):
        global cluster_id
        self.id = cluster_id
        cluster_id += 1
        self.vertices = vertices
        self.edges = edges
        self.densities = []
        self.vertex_matrix = None
        self.base_density = 0
        self.vertex_to_array_loc_map = {}
        self.array_loc_to_vertex_map = {}

    def does_vertex_exist(self, vertex):
        try:
            self.vertices.index(vertex)
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

    def find_best_vertex(self, vertex, vertex_list):
        best_so_far = vertex_list[0]
        for vert in vertex_list:
            return

    def order_vertices(self):
        verts = copy.deepcopy(self.vertices)
        new_vert_order = []
        for vertex in self.vertices:
            return
        return 

    def build_array(self):
        self.make_vert_map()
        self.vertex_matrix = np.zeros(shape=(self.vertices.__len__(), self.vertices.__len__()))
        for edge in self.edges:
            source = self.vertex_to_array_loc_map[edge.target.id]
            target = self.vertex_to_array_loc_map[edge.source.id]
            self.vertex_matrix[source][target] = edge.weight
            self.vertex_matrix[target][source] = edge.weight

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
        self.compute_base_density()
        for i in range(self.vertices.__len__()):
            for j in range(i):
                denominator = np.sum(self.vertex_matrix[j:][j:i]) + np.sum(self.vertex_matrix[j:i][j:]) + ((i - j) ** 2)
                if (np.sum(self.vertex_matrix[j:i][j:i])/ denominator) > self.base_density:
                   return True
        return False

    def split_cluster(self, new_cluster):
        self.compute_base_density()
        densities = []
        indexes = []
        # Find max
        for i in range(self.vertices.__len__()):
            for j in range(i):
                densities.append(np.sum(self.vertex_matrix[j:i][j:i]) / (np.sum(self.vertex_matrix[j:][j:i]) + np.sum(self.vertex_matrix[j:i][j:]) + ((i - j) ** 2)))
                indexes.append((i, j))
        max_index = densities.index(max(densities))
        # Move the vertexs and edges over
        for x in range(indexes[max_index][0], indexes[max_index][1]):
            vertex_to_move = self.vertices[self.array_loc_to_vertex_map[x]]
            new_cluster.vertices.append(vertex_to_move)
            self.vertices.remove(vertex_to_move)
            for edge in vertex_to_move.edges:
                new_cluster.edges.append(edge)
                self.edges.remove(edge)
        return

