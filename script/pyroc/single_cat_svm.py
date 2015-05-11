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
import types
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
        fn = scratch+'svm/svmtrain/{0}_train_{1}_res_64.txt'.format(names[i],re.sub(r'_.*',"",names[j]))
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
        fn = scratch+'svm/svmdata/{0}_test_{1}_res_64.txt'.format(names[i],re.sub(r'_.*',"",names[j]))
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
    

# %% add probability to lda 
def patch_me(target):
    def method(target, X, max_iter=20, tol=1e-16):
        X = np.atleast_2d(X)
        phi = target.components_
        alpha = target.alpha
        # for debugging, let's not worry about the documents
        n_topics = len(target.components_)
        res = np.empty((len(X), 1))
        WS, DS = lda.utils.matrix_to_lists(X)
        # TODO: this loop is parallelizable
        for d in range(len(X)):
            # initialization step
            ws_doc = WS[DS == d]
            PZS = phi[:, ws_doc].T * alpha
            PZS /= PZS.sum(axis=1)[:, np.newaxis]
            assert PZS.shape == (len(ws_doc), n_topics)
            PZS_new = np.empty_like(PZS)
            for s in range(max_iter):
                PZS_sum = PZS.sum(axis=0)
                for i, w in enumerate(ws_doc):
                    PZS_sum -= PZS[i]
                    PZS_new[i] = phi[:, w] * (PZS_sum + alpha)
                    PZS_sum += PZS[i]
                PZS_new /= PZS_new.sum(axis=1)[:, np.newaxis]
                delta_naive = np.abs(PZS_new - PZS).sum()
                PZS = PZS_new.copy()
                if delta_naive < tol:
                    break
            theta_doc = PZS.sum(axis=0)
            res[d] = sum(theta_doc)
        return res
    target.method = types.MethodType(method,target)
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
res = [[] for i in range(4)]
for i in range(4):
    idx = i
    model = lda.LDA(n_topics=5, n_iter=1500, random_state=1)
    model.fit(X[idx][membership==i])  # model.fit_transform(X) is also available
    patch_me(model);
    res[i] = model.method(X_test[idx])
#topic_word = model.topic_word_  # model.components_ also works
#labels =  np.array(list(map(lambda x:x.argmax(),model.doc_topic_)))
# %% p
# Compute ROC curve and ROC area for each class
n_cat = 4
fpr = dict()
tpr = dict()
roc_auc = dict()
for i in range(n_cat):
    fpr[i], tpr[i], _ = roc_curve(Y_test[i], res[i])
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
# %% Compute KMeans Clustering    
idx = 1
af = cluster.KMeans(n_clusters=4).fit(X[idx])
#cluster_centers_indices = af.cluster_centers_indices_
labels = af.labels_

#n_clusters_ = len(cluster_centers_indices)
# %% Likelyhood function

