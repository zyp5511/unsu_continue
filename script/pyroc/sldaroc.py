# -*- coding: utf-8 -*-
"""
Created on Tue May 12 10:38:02 2015

@author: Lichao
"""

# %% import 
import numpy as np
import re

import matplotlib.pyplot as plt
import pandas as pd
from sklearn.metrics import roc_curve, auc
home = 'C:/users/lichao/documents/'
fn = 'pred.txt'
t1 = pd.read_csv(home+fn,index_col=[0])

# %% read and processing data
names = ['car','face','motorbike','airplane']
images = [400,217,400,400]
Y = dict()
n_cat = 4
for i in range(n_cat):
    Y[i]=np.ndarray((0,1))
    for j in range(4):
        Y[i] = np.append(Y[i], ((i%4)==j) * np.ones((images[j],1)))
    Y[i] = Y[i].astype(bool)
  
# %% p
# Compute ROC curve and ROC area for each class
fpr = dict()
tpr = dict()
roc_auc = dict()
for i in range(n_cat):
    fpr[i], tpr[i], _ = roc_curve(Y[i], t1[names[i]])
    roc_auc[i] = auc(fpr[i], tpr[i])


# Plot ROC curve
plt.figure(figsize=(11,8.5))

for i in range(n_cat):
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