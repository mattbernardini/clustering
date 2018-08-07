import numpy as np
import copy

global point_id
point_id = 0
class Point:
    def __init__(self, num_dims, location):
        global point_id
        self.id = point_id
        point_id += 1
        self.location = copy.deepcopy(location)
        self.num_dims = num_dims
