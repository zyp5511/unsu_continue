import numpy as np
import scipy as sp
import matplotlib as mpl
import igraph as ig
import os

# generate network
##g = ig.Graph.Full(3)
##for i in g.es:
##    print(i.tuple)


## define data dir
root = os.path.join("~","allnet");


## Read from file
g = ig.Graph.Read_Edgelist(os.path.join(root,"network2m_noweight.txt"),directed=False)
##ig.plot(g,vertex_label=g.vs.indices)


## FastGreedy Community Finding
##vd = g.simplify().community_fastgreedy();
#### plot dandrogen
####ig.plot(vd)
#### dandrogen to clustering
##mem = vd.as_clustering()

## Multilevel Community Finding
mem = g.simplify().community_multilevel();

print("modularity is",mem.modularity);
##ig.plot(mem);

## export membership
with open(os.path.join(root,"multilevel.txt"),"w") as f:
    f.write('\n'.join(str(i) for i in mem.membership));

