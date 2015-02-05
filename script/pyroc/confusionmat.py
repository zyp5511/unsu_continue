# -*- coding: utf-8 -*-
"""
Created on Mon Jan 26 14:18:42 2015

@author: Lichao
"""
print(__doc__)
# %% import 
import numpy as np

import matplotlib.pyplot as plt
import pandas as pd
from sklearn.metrics import roc_curve, auc
from sklearn import svm,preprocessing,metrics
import re

# %% read and processing data
scratch = 'Y:/vault/pami/groups/';
names = ['airplane','car_400','face','motorbike']
images = [400,400,218,400]
X = dict()
Y = dict()
Y_pred = dict()
for i in range(4):
    X[i]=np.ndarray((0,1))
    Y[i]=np.ndarray((0,1))
    if i == 0:
        Y_true = np.ndarray((0,1));
    for j in range(4):
        fn = scratch+'/single_{0}/single_{0}_{1}_64.txt'.format(names[i],re.sub(r'_.*',"",names[j]))
        t1 = pd.read_csv(fn,sep='\t',index_col=[0,1],header=None, names=['filename','gid','quality'])
        if t1.shape[0]>0:
            t1c = t1.groupby(level='filename').max()['quality'].as_matrix()
            X[i] = np.append(X[i], t1c);
            Y[i] = np.append(Y[i], (i==j) * np.ones((t1c.shape[0],1)))
            if i==0:
                Y_true = np.append(Y_true, j * np.ones((t1c.shape[0],1)))
            rest = images[j]-t1c.shape[0]
        else:
            rest = images[j]
        if rest > 0:            
            X[i] = np.append(X[i], np.zeros(rest));
            Y[i] = np.append(Y[i], (i==j) * np.ones(rest));
            if i==0:
                Y_true = np.append(Y_true, j * np.ones(rest))
    Y[i] = Y[i].astype(bool)
    X[i] = X[i].astype(int)
    
# %% read and processing data
def fcolOne(XX,YY,th):
    res= [];    
    res.append(metrics.accuracy_score(preprocessing.binarize(XX[:400],th),np.ones(400)))   
    res.append(metrics.accuracy_score(preprocessing.binarize(XX[400:800],th),np.ones(400)))    
    res.append(metrics.accuracy_score(preprocessing.binarize(XX[800:1018],th),np.ones(218)))    
    res.append(metrics.accuracy_score(preprocessing.binarize(XX[-400:],th),np.ones(400))) 
    #res.append(metrics.precision_score(preprocessing.binarize(XX,th),1-YY)) 
    return res
# %% read and processing data    
th=[4,8,11,8]
mat = np.ndarray((4,4))
for i in range(4):    
    mat[:,i]=np.array(fcolOne(X[i],Y[i],th[i]))
a = pd.DataFrame(mat,columns=names)
print(a.to_latex())
