import numpy as np
from .point import Point
from .vertex import Vertex
from .cluster import Cluster
from .edge import Edge
import tensorflow as tf
import scipy
import queue

class Network:
    def __init__(self, num_points=1000, num_dims=3):
        self.edges = []
        self.vertices = []
        self.points = []
        self.clusters = []
        self.num_points = num_points
        self.num_dims = num_dims
        self.generate_points()

    def generate_points(self):
        for i in range(self.num_points):
            self.points.append(Point(self.num_dims, np.random.random_integers(100, size=(self.num_dims,))))
            self.vertices.append(Vertex())

    def compute_corrolations(self):
        for i in range(self.points.__len__()):
            for j in range(i + 1,self.points.__len__()):
                t = self.points[i].location
                cor = np.corrcoef(self.points[i].location, self.points[j].location)
                new_edge = Edge(self.vertices[i], self.vertices[j], cor[0][1])
                self.vertices[i].edges.append(new_edge)
                self.vertices[j].edges.append(new_edge)
                self.edges.append(new_edge)

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
