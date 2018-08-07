import os
import subprocess
import csv
import re
import numpy as np
class DataParser:
    def __init__(self, filename):
        self.file = filename
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
        os.chdir("old")
        new_genes = []
        output = subprocess.check_output(["wc", self.file])
        new_snps = np.ndarray(shape=(int(output.split()[0]),int(int(output.split()[1])/int(output.split()[0])) - 11))
        csv_file = csv.reader(open(self.file, "r"), delimiter=" ")
        row_counter = 0
        for row in csv_file:
            if (self.rs_regex.match(row[0])):
                column_counter = 0
                for item in row:
                    if self.rs_regex.match(item):
                        new_genes.append((item, row[1]))
                    elif self.gene_regex.match(item):
                        new_snps[row_counter][column_counter] = self.gene_map[item]
                        column_counter += 1
                row_counter += 1
        new_genes.append(self.file)
        return ((new_genes, new_snps))