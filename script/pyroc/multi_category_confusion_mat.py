# -*- coding: utf-8 -*-
"""
Created on Wed Feb  4 21:34:56 2015

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
names = ['airplane','car','face','motorbike']
images = [400,400,218,400]
X = dict()
Y = dict()
Y_pred = dict()
for i in range(4):
    if i == 0:
        Y_true = np.ndarray((0,1));
    for j in range(4):
        fn = scratch+'/multi_{0}/multi_{0}_{1}_64.txt'.format(names[i],re.sub(r'_.*',"",names[j]))
        t1 = pd.read_csv(fn,sep='\t',index_col=[0,1],header=None, names=['filename','gid','quality'])
        if t1.shape[0]>0:
            t1c = t1.groupby(level='filename').max()['quality']
            for f,q in t1c.iteritems():
                fullimgfn = names[j]+"_"+f;
                if not fullimgfn in X:
                    X[fullimgfn]=dict()
                X[fullimgfn][names[i]]=q
                X[fullimgfn]["class"]=j

# %% pandas transform
XX = pd.DataFrame(X).transpose().fillna(0);
# %% fit
XX_train = XX[names].as_matrix()
Y = XX['class'].as_matrix()
lin_clf= svm.LinearSVC(multi_class='ovr');
lin_clf.fit(XX_train,Y);
Y_pred = lin_clf.predict(XX_train)

# %% confusion matrix

table  = pd.DataFrame(metrics.confusion_matrix(Y,Y_pred),columns=names)
table = table.div(table.sum(axis=0))
print(table.to_latex())
