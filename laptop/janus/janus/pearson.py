import numpy as np
from scipy.spatial.distance import euclidean

class Pearson:
    def __init__(self, tensor1, tensor2):
        self.tensor1 = tensor1
        self.tensor2 = tensor2
    
    def __call__(self):
        tensor1_total = np.zeros(shape=(3,))
        tensor2_total = np.zeros(shape=(3,))
        for x in self.tensor1:
            tensor1_total = np.add(tensor1_total, x)
        for x in self.tensor2:
            tensor2_total = np.add(tensor2_total, x)
        tensor1_total = np.divide(tensor1_total, len(self.tensor1))
        tensor2_total = np.divide(tensor2_total, len(self.tensor2))
        return euclidean(tensor1_total, tensor2_total)
