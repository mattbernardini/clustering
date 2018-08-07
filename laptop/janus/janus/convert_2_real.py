#!/usr/bin/python3
import os
import subprocess
import csv
import re
import numpy as np
class DataParser:
    def __init__(self):
        self.rs_regex = re.compile("^rs\d{1,}")
        self.gene_regex = re.compile("(AA|AC|AG|AT|CA|CC|CG|CT|GA|GC|GG|GT|TA|TC|TG|TT|NN)") 
        self.gene_map = {
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
        return
    
    def __call__(self):
        
        