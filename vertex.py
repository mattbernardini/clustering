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

