# -*- coding: utf-8 -*-
"""
Created on Fri Jan 27 23:39:28 2015

@author: Lichao
"""
print(__doc__)
# %% import 
import numpy as np
import pandas as pd

# %% read and processing data
home = '/home/lichao/scratch/pami/groups/'
names = ['airplane','car','face','motorbike']
images = [400,400,218,400]
for i in range(4):
    X[i]=np.ndarray((0,1))
    Y[i]=np.ndarray((0,1))
    for j in range(4):
        fn = home+'/single_{0}/single_{0}_{1}_64.txt'.format(names[i],names[j])
        t1 = pd.read_csv(fn,sep='\t',index_col=[0,1],header=None, names=['filename','gid','quality'])
        if t1.shape[0]>0:
            t1c = t1.groupby(level='filename').max()['quality'].as_matrix()
            X[i] = np.append(X[i], t1c);
            Y[i] = np.append(Y[i], (i==j) * np.ones((t1c.shape[0],1)))
            rest = images[j]-t1c.shape[0]
        else:
            rest = images[j]
        if rest > 0:            
            X[i] = np.append(X[i], np.zeros(rest));
            Y[i] = np.append(Y[i], (i==j) * np.ones(rest));
    Y[i] = Y[i].astype(bool)
    X[i] = X[i].astype(int)
  
