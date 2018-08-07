from edge import Edge

global vertex_id
vertex_id = 0


class Vertex:
    def __init__(self):
        global vertex_id
        self.id = vertex_id
        vertex_id += 1
        self.edges = []
        self.label = None
        self.cluster_id = 0

    def add_edge(self, vertex_creating_edge_to, weight):
        for edge in self.edges:
            if edge.get_other(self).id == vertex_creating_edge_to:
                return
        new_edge = Edge(self, vertex_creating_edge_to, weight)
        self.edges.append(new_edge)
        vertex_creating_edge_to.edges.append(new_edge)
