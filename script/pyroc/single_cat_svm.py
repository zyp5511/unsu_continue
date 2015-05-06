# -*- coding: utf-8 -*-
"""
Created on Mon Apr 13 00:08:04 2015

@author: Lichao
"""

# %% import 
import numpy as np

import matplotlib.pyplot as plt
import pandas as pd
from sklearn.metrics import roc_curve, auc
from sklearn import svm,preprocessing,metrics,cluster,feature_extraction
import lda
import re

# %% environment 
scratch = 'C:/Users/Lichao/scratch/';
names = ['airplane','car','face','motorbike']
# %% read and processing data
X = dict()
Y = dict()
normer = feature_extraction.text.TfidfTransformer(norm='l1',use_idf=False)
membership = np.ndarray((0,1))
for i in range(4):
    Y[i]=np.ndarray((0,1))
    X[i]=np.ndarray((0,1))
    for j in range(4):
#        if j==(i + 3) % 4 or j==(i+2) % 4:
#            continue
        fn = scratch+'svmtrain/{0}_train_{1}_res_64.txt'.format(names[i],re.sub(r'_.*',"",names[j]))
        t1 = pd.read_table(fn,header=None)
        if len(X[i])==0:
            X[i]=t1;
        else:
            X[i] = np.r_[X[i], t1];
        Y[i] = np.append(Y[i], (i==j) * np.ones((t1.shape[0],1)))
        if i==0:
            membership = np.append(membership, j * np.ones((t1.shape[0],1)))
    #X[i] = normer.transform(X[i]);
    #preprocessing.binarize(X[i],copy=False)
            
# %% read and processing test data
X_test = dict()
Y_test = dict()
Y_pred = dict()
for i in range(4):
    Y_test[i]=np.ndarray((0,1))
    for j in range(4):
        fn = scratch+'svmdata/{0}_test_{1}_res_64.txt'.format(names[i],re.sub(r'_.*',"",names[j]))
        t1 = pd.read_table(fn,header=None)
        if j==0:
            X_test[i]=t1;
        else:
            X_test[i] = np.r_[X_test[i], t1];
        Y_test[i] = np.append(Y_test[i], (i==j) * np.ones((t1.shape[0],1)))
    #X_test[i] = normer.transform(X_test[i]);
    #preprocessing.binarize(X_test[i],copy=False)

# %% fit
lin_clfs = dict();
for i in range(4):
    lin_clfs[i]= svm.LinearSVC();
    lin_clfs[i].fit(X[i],Y[i]);
    print(lin_clfs[i].score(X_test[i],Y_test[i]))
# %% Clustering Evaluation
def unsu(membership,labels):
    #print('Estimated number of clusters: %d' % n_clusters_)
    print("Homogeneity: %0.3f" % metrics.homogeneity_score(membership, labels))
    print("Completeness: %0.3f" % metrics.completeness_score(membership, labels))
    print("V-measure: %0.3f" % metrics.v_measure_score(membership, labels))
    print("Adjusted Rand Index: %0.3f" % metrics.adjusted_rand_score(membership, labels))
    print("Adjusted Mutual Information: %0.3f" % metrics.adjusted_mutual_info_score(membership, labels))
    print("Silhouette Coefficient: %0.3f" % metrics.silhouette_score(X[idx], labels, metric='sqeuclidean'))# %% LDA
# %% Compute LDA Clustering
idx = 3
model = lda.LDA(n_topics=5, n_iter=1500, random_state=1)
model.fit(X[idx])  # model.fit_transform(X) is also available
topic_word = model.topic_word_  # model.components_ also works
labels =  np.array(list(map(lambda x:x.argmax(),model.doc_topic_)))
unsu(membership,labels)

# %% Compute KMeans Clustering    
idx = 1
af = cluster.KMeans(n_clusters=4).fit(X[idx])
#cluster_centers_indices = af.cluster_centers_indices_
labels = af.labels_

#n_clusters_ = len(cluster_centers_indices)




