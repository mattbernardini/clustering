import networkx
import os
os.chdir('janus')
gml_file = networkx.readwrite.gml.read_gml(path='2749nodes', label=None)
print("here")
