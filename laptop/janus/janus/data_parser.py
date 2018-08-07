import os
import subprocess
import csv
import re
import numpy as np
class DataParser:
    def __init__(self, filename, dir_changed_to, gene_regex, gene_map, plink):
        self.file = filename
        self.dir_changed_to = dir_changed_to
        self.rs_regex = re.compile("^rs\d{1,}")
        self.gene_regex = gene_regex
        self.gene_map = gene_map
        self.plink = plink
        return

    def __call__(self):
        if self.plink:
            return self.plink_style_data()
        else:
            return self.hapmap_style_data()

    def plink_style_data(self):
        os.chdir(self.dir_changed_to)
        output = subprocess.check_output(["wc", self.file])
        new_genes = np.ndarray(
            shape=(int(output.split()[0]) + 1,), 
            dtype=[('gene_name', np.unicode, 16), ('alleles', np.unicode, 3), ('row', np.int32)])
        new_snps = np.ndarray(shape=(int(output.split()[0]),int(int(output.split()[1])/int(output.split()[0])) - 11))
        csv_file = csv.reader(open(self.file, "r"), delimiter=" ")
        row_counter = 0
        for row in csv_file:
            if (self.rs_regex.match(row[0])):
                column_counter = 0
                for item in row:
                    if self.rs_regex.match(item):
                        # give it the rs#, the possible alleles, and column for later
                        new_genes[row_counter] = (item, row[1], row_counter)
                    elif self.gene_regex.match(item):
                        try:
                            new_snps[row_counter][column_counter] = self.gene_map[item]
                        except ValueError:
                            new_snps[row_counter][column_counter] = -1
                        finally:
                            column_counter += 1
                row_counter += 1
        return ((new_genes, new_snps))
    
    def hapmap_style_data(self):
        os.chdir(self.dir_changed_to)
        output = subprocess.check_output(["wc", self.file])
        new_genes = np.ndarray(
            shape=(int(output.split()[0]) + 1,), 
            dtype=[('gene_name', np.unicode, 16), ('alleles', np.unicode, 3), ('row', np.int32)])
        new_snps = np.ndarray(shape=(int(output.split()[0]),int(int(output.split()[1])/int(output.split()[0])) - 11))
        csv_file = csv.reader(open(self.file, "r"), delimiter=" ")
        row_counter = 0
        for row in csv_file:
            if (self.rs_regex.match(row[0])):
                column_counter = 0
                for item in row:
                    if self.rs_regex.match(item):
                        # give it the rs#, the possible alleles, and column for later
                        new_genes[row_counter] = (item, row[1], row_counter)
                    elif self.gene_regex.match(item):
                        try:
                            new_snps[row_counter][column_counter] = self.gene_map[item]
                        except ValueError:
                            new_snps[row_counter][column_counter] = -1
                        finally:
                            column_counter += 1
                row_counter += 1
        return ((new_genes, new_snps))