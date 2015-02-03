# -*- coding: utf-8 -*-
"""
Created on Mon Feb  2 14:10:38 2015

@author: Lichao
"""

# %% import 
import numpy as np
import re
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.metrics import roc_curve, auc
home = 'Y:/vault/pami/groups/'
# %% read and processing data
names = ['single','multi']
X = dict()
Y = dict()
for i in range(2):
    fn = home+'/{0}_car/{0}_car_car_64.txt'.format(names[i])
    t1 = pd.read_csv(fn,sep='\t',index_col=[0,1],header=None, names=['filename','gid','quality'])
    if t1.shape[0]>0:
        t1c = t1.groupby(level='filename').max()['quality'].as_matrix()
        plt.hist(t1c,bins=np.arange(0.5,30,1) )
      
