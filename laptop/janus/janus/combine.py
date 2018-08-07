import os, csv, re, numpy as np
from consumer import Consumer
from data_parser import DataParser
import multiprocessing
from scipy.ndimage import correlate, laplace

"""
Pushes from 1d(scalar, tensor rank 1) to 3d(vecotr, tensor rank 1) allowing a better 
representation of how the data looks in my head

1, 0, 0 and 0, 1, 0 represent homozoygote
    1, 0 represents either a homozoygote A or T
    0, 1 represents either a homozoygote C or G

1, 1, 0 represents a split, and is equidistance from either homozyogte representation 
of the data

.5, .5, 1 / sqrt(2) is missing, and is the same distance from every point, 1.  The third
dimension was included to push this point to be distance 1 from every other point, making 
all points have the same ecludian distance from eachother.
"""
def convert(x):
    if x % 4 == x / 4:
        if x % 4 == 0 or x % 4 == 3:
            return [1, 0, 0]
        else:
            return [0, 1, 0]
    elif x == 17:
        return [np.sqrt(2)/2, 3/4, np.sqrt((15/16) - np.power(1 - (np.sqrt(2) / 2), 2) )]
    return [1, 1, 0]
# AT bind
# CG bind
# Setup variables we need
g = np.concatenate((np.random.beta(1, 2, 9).reshape(3, 3), np.random.beta(1, 2, 9).reshape(3, 3)))
total = np.zeros(shape=(3,))
for x in np.random.beta(1, 2, 15).reshape(5, 3):
    total = np.add(x, total)
print(len(g))
total = np.divide(total, len(g[0]))
gene_map = {
    'AA': 0,
    'AC': 1,
    'AG': 2,
    'AT': 3,
    'CA': 4,
    'CC': 5,
    'CG': 6,
    'CT': 7,                                  
    'GA': 8,
    'GC': 9,
    'GG': 10,
    'GT': 11,
    'TA': 12,
    'TC': 13,
    'TG': 14,
    'TT': 15,
    'NN': 17
}
gene_regex = re.compile("(AA|AC|AG|AT|CA|CC|CG|CT|GA|GC|GG|GT|TA|TC|TG|TT|NN)") 
# Setup multiprocessing environment
generationTasks = multiprocessing.JoinableQueue()
generationResults = multiprocessing.Queue()
generationConsumers = 2
generationConsumers = [ Consumer(generationTasks, generationResults)
            for i in range(generationConsumers) ]
for w in generationConsumers:
    w.start()
dir_changed_to = "janus"
# Parse data files on multiple cores
snp_data = []
gene_data = []
counter = 0
os.chdir(dir_changed_to)
# Pump queue with tasks
for file in os.listdir("."):
    if file.endswith(".txt"):
        counter += 1
        generationTasks.put(
            DataParser(
                file, 
                dir_changed_to, 
                gene_regex, 
                gene_map))
# Get resutls
for i in range(counter):
    result = generationResults.get()
    gene_data.append(result[0])
    snp_data.append(result[1])
# Create sets and get intersection
same_snps = set.intersection(   set({ x for x in gene_data[0][:]['gene_name'] }), 
                                set({ x for x in gene_data[1][:]['gene_name'] }))
total_array = np.ndarray(shape=(same_snps.__len__(), len(snp_data[0][0]) + len(snp_data[1][0]), 3))
row_counter = 0
for snp in same_snps:
    if snp != '':
        total_array[row_counter] = np.concatenate(
            (   
                [convert(x) for x in snp_data[0][gene_data[0][np.where(gene_data[0][:]['gene_name'] == snp)]['row']][0]], 
                [convert(x) for x in snp_data[1][gene_data[1][np.where(gene_data[1][:]['gene_name'] == snp)]['row']][0]]
            ), 
                axis=0)
        row_counter += 1
# compute pearson
for i in range(len(total_array)):
    for j in range(i, len(total_array)):
        print("here")

