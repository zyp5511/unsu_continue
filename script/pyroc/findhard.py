# -*- coding: utf-8 -*-
"""
Created on Fri Jan 27 23:39:28 2015

@author: Lichao
"""
print(__doc__)
# %% import 
import numpy as np
import pandas as pd
import os;
import shutil;
# %% read and processing data
home = 'Y:/vault/pami/groups'
test_template = 'Y:/vault/pami/data/{0}_test/'
hard_template = 'Y:/vault/pami/single_{0}/hard/{1}/'
names = ['airplane','car','face','motorbike']
images = [400,400,218,400]
t = dict()
for i in range(3):
    t[names[i]]=dict();
    for j in range(4):
        hard_dir = hard_template.format(names[i],names[j]);
        test_dir = test_template.format(names[j])
        if not os.path.exists(hard_dir):
            os.makedirs(hard_dir)
        fn = home+'/single_{0}/single_{0}_{1}_64.txt'.format(names[i],names[j])
        t1 = pd.read_csv(fn,sep='\t',index_col=[0,1],header=None, names=['filename','gid','quality'])
        if t1.shape[0]>0:
            t[names[i]][names[j]] = t1.groupby(level='filename').max()['quality']
            hardidx = t[names[i]][names[j]]<5
            for fn in t[names[i]][names[j]][hardidx].index:
                shutil.copy(test_dir+fn, hard_dir+fn)
            rest = images[j]-t[names[i]][names[j]].shape[0]
        else:
            rest = images[j]       

  