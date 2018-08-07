global edge_id
edge_id = 0


class Edge:
    def __init__(self, source, target, weight=1):
        global edge_id
        self.id = edge_id
        edge_id += 1
        self.source = source
        self.target = target
        self.weight = weight

    def get_other(self, vertex):
        return self.source if self.target.id == vertex.id else self.target
