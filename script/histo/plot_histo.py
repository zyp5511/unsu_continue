import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import igraph as ig
import os


## define data dir
root = os.path.join("/media","cixi","cos-net");


## Read from file
dat = np.loadtxt(os.path.join(root,'histo.txt'))

print(dat)
print('total edge is',sum(dat))

## plot
x = np.arange(-1,1.01,0.01)
width = 0.008

plt.bar(x,dat,width=width)
plt.show()
