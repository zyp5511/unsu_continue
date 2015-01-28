# -*- coding: utf-8 -*-
"""
Created on Fri Jan 16 15:05:28 2015

@author: Lichao
"""
print(__doc__)
# %% import 
import numpy as np

import matplotlib.pyplot as plt
import pandas as pd
from sklearn.metrics import roc_curve, auc

# %% read and processing data
names = ['airplane','car','face','motorbike']
images = [400,400,218,400]
X = dict()
Y = dict()
for i in range(4):
    X[i]=np.ndarray((0,1))
    Y[i]=np.ndarray((0,1))
    for j in range(4):
        fn = 'C:/Users/Lichao/scratch/single_{0}/single_{0}_{1}_64.txt'.format(names[i],names[j])
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
  
# %% p
# Compute ROC curve and ROC area for each class
fpr = dict()
tpr = dict()
roc_auc = dict()
for i in range(4):
    fpr[i], tpr[i], _ = roc_curve(Y[i], X[i])
    roc_auc[i] = auc(fpr[i], tpr[i])


# Plot ROC curve
plt.figure()

for i in range(4):
    plt.plot(fpr[i], tpr[i], label='ROC curve of class {0} (area = {1:0.2f})'
                                   ''.format(names[i], roc_auc[i]))

plt.plot([0, 1], [0, 1], 'k--')
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic to multi-class')
plt.legend(loc="lower right")
plt.show()