import numpy as np

class Corroloation:
    def __init__(self, corrolation_function, point, points):
        self.corrolation_function = corrolation_function
        self.point = point
        self.points = points

    def __call__(self):
        rt_tensor = np.ndarray(shape=(len(self.points),), dtype=[('point_from', np.int32), ('point_to', np.int32), ('corrolation', np.float_)])
        counter = 0
        for P in self.points:
            rt_tensor[counter] = (self.point.id, P.id, self.corrolation_function(P, self.point))
        return rt_tensor
