import numpy as np
import scipy as sp
import matplotlib as mpl
import igraph as ig

# generate network
g = ig.Graph.Full(3)
for i in g.es:
	print(i.tuple)

	
