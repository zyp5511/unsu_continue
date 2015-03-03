# -*- coding: utf-8 -*-
"""
Created on Tue Feb 24 16:28:52 2015

@author: Lichao
"""

# %% import 
import numpy as np
import re
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.metrics import roc_curve, auc
home = 'Y:/vault/pami/stat/airplane'
# %% read and processing data
names = ['car','airplane','face','motorbike']
colors = ['c','b','y','g']
t = dict()
n_cat = 4
for i in range(4):
    fn = home+'/single_airplane_{0}.txt'.format(names[i])
    t[names[i]] = pd.read_csv(fn,sep='\t',header=None, names=['indicator','filename','gid','pid','nid'])

# %% read and processing data
p = dict()
n_cat = 4
plt.figure(figsize=(18,8.5))
plt.title('Part activation')
width = 0.2 
for i in range(4):    
    tt = t[names[i]].groupby('pid').size()/t[names[i]].shape[0]
    p[names[i]]=plt.bar(tt.index+width*i,tt,color=colors[i],width=width, alpha=0.3)
plt.legend(names)
plt.show()
# %% generate avg patches per part
def avgNodePerPart(t,names,n_cat):
    plt.figure(figsize=(18,8.5))
    plt.title('Part activation')
    width = 0.2 
    for i in range(n_cat):
        a =  t[names[i]].groupby(['filename','gid','pid']).size()
        table1 = pd.DataFrame([(e[0][2],e[1]) for e in a.iteritems()],columns=['gid','pcount'])
        tt=table1.groupby('gid').mean()
        plt.bar(tt.index+width*i,tt.pcount,color=colors[i],width=width, alpha=0.3)
    plt.legend(names)
    plt.show()

avgNodePerPart(t,names,n_cat)

# %% filtering hard images in airplane category
groupfn = r'Y:\vault\pami\groups\airplane_old\single_airplane_400_airplane.txt'
t1 = pd.read_csv(groupfn,sep='\t',header=None, names=['filename','gid','quality'])
t1c = t1.groupby('filename').max()['quality']
files = t1c[t1c<10].index
t_trimed = t.copy()
t_trimed['airplane']=t_trimed['airplane'][t_trimed['airplane'].filename.isin(files)]

avgNodePerPart(t_trimed,names,n_cat)