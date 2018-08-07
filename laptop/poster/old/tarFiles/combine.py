import os, csv, re, numpy as np
from .consumer import Consumer
from .data_parser import DataParser
import multiprocessing
# Setup the multiprocessing environment
generationTasks = multiprocessing.JoinableQueue()
generationResults = multiprocessing.Queue()
generationConsumers = multiprocessing.cpu_count()
generationConsumers = [ Consumer(generationTasks, generationResults)
            for i in range(generationConsumers) ]
for w in generationConsumers:
    w.start()

# Parse data files on multiple cores
snp_data = []
gene_data = []
counter = 0
for file in os.listdir("."):
    if file.endswith(".txt"):
        counter += 1
        generationTasks.put(DataParser(file))
for i in range(counter):
    result = generationResults.get()
    gene_data.append(result[0])
    snp_data.append(result[1])


