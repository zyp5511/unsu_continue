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
root = os.path.join("/media","cixi","cos-net");


## Read from file
g = ig.Graph.Read_Edgelist(os.path.join(root,"net095_noweight.txt"),directed=False)
##ig.plot(g,vertex_label=g.vs.indices)


## FastGreedy Community Finding
vd = g.simplify().community_fastgreedy();
ml_mem = g.simplify().community_multilevel();
## plot dandrogen
#ig.plot(vd)
## dandrogen to clustering
mem = vd.as_clustering()

def topcluster( amem,n ):
    clusize = amem.sizes()
    sortedind=np.argsort(clusize)[::-1]
    print([i for i in sortedind[0:n]])
    print([clusize[i] for i in sortedind[0:n]])
    print("modularity is",amem.modularity);




topcluster(mem,20)
topcluster(ml_mem,20)
##ig.plot(mem);

## export membership

def exportmem( amem, afname ):
    with open(os.path.join(root, afname),"w") as f:
        f.write('\n'.join(str(i) for i in amem.membership));
        
exportmem(mem,"fastgreedy_095.txt")
exportmem(ml_mem,"ml_095.txt")




