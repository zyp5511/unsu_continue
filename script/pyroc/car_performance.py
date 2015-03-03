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

# %% read and processing data

def pltPartStat(names,fnformat,title='Quality Score Histogram'):
    plt.figure(figsize=(11,8.5))
    plt.title('Part activation')
    for i in range(len(names)):
        fn = fnformat.format(names[i])
        t1 = pd.read_csv(fn,sep='\t',index_col=[0,1],header=None, names=['filename','gid','quality'])
        if t1.shape[0]>0:
            t1c = t1.groupby(level='filename').max()['quality'].as_matrix()
            plt.hist(t1c,bins=np.arange(0.5,t1c.max()+1,1),alpha=0.3)
    plt.legend(names)
            
# %% car single vs multi
home = 'Y:/vault/pami/groups/'
names = ['single','multi']
pltPartStat(names,home+'/{0}_car/{0}_car_car_64.txt')
            
# %% airplane with 2node per part filtering(400) vs not(old)
names = ['400','old']
pltPartStat(names,'Y:/vault/pami/groups/airplane_{0}/single_airplane_400_airplane.txt')            
# %% car single vs multi
names = ['airplane','car','face','motorbike']
pltPartStat(names,'Y:/vault/pami/groups/airplane_old/single_airplane_400_{0}.txt')            