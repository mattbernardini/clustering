import numpy as np
from point import Point
from vertex import Vertex
from cluster import Cluster
from edge import Edge
from corrolation import Corroloation
import tensorflow as tf
import scipy
import queue
"""

"""
class Network:
    def __init__(self, task_queue = None, result_queue = None, num_points=1000, num_dims=3, corrolation_function = np.corrcoef):
        self.edges = []         
        self.vertices = []
        self.points = []
        self.clusters = []
        self.num_points = num_pointss
        self.num_dims = num_dims
        self.corrolation_function = corrolation_function
        self.task_queue = task_queue
        self.result_queue = result_queue
        self.vert_matrix = None
        self.generate_points()

    def generate_points(self):
        for i in range(self.num_points):
            self.points.append(Point(self.num_dims, np.random.random_integers(100, size=(self.num_dims,))))
            self.vertices.append(Vertex())

    def compute_corrolations(self):
        self.vert_matrix = np.ndarray(shape=(len(self.points), len(self.points)))
        tmp_holder = []
        for point in self.points:
            self.task_queue.put(
                Corroloation(
                    self.corrolation_function,
                    point,
                    self.points
                )
            )
        for x in range(len(self.points)):
            tmp_holder.append(self.result_queue.get())
        counter = 0
        for x in self.find_total_ordering(tmp_holder):
            self.vert_matrix[counter] = x[:][-1]

    def find_total_ordering(self, unsorted_array: []) -> []:
        final_ordering = []
        best_start_tensor = unsorted_array[0]
        for tensor in unsorted_array:
            if (np.sum(tensor[:][-1]) > np.sum(best_start_tensor[:][-1])):
                best_start_tensor = tensor
        # append initial start tensor
        final_ordering.append(best_start_tensor)
        past_tensor = best_start_tensor
        # loop through and append the next best until empty
        # this greedy style should take under n^2 time
        for x in range(len(unsorted_array)):
            # take the best of the rest
            next_tensor = past_tensor.index(np.max(set.difference(set(unsorted_array), set(final_ordering))))
            final_ordering.append(next_tensor)
            past_tensor = next_tensor
        return final_ordering

    def find_clusters(self):
        q = queue.Queue()
        first_cluster = Cluster()
        first_cluster.edges = self.edges
        first_cluster.vertices = self.vertices
        first_cluster.build_array()
        self.clusters.append(first_cluster)
        q.put(first_cluster)
        while not q.empty():
            cluster = q.get()
            if cluster.should_cluster_be_split():
                new_cluster = Cluster()
                cluster.split_cluster(new_cluster)
                self.clusters.append(new_cluster)
                if not cluster.verticies.__len__() > 0:
                    q.put(cluster)
                if new_cluster.verticies.__len__() > 0:
                    q.put(new_cluster)
