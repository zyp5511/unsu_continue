/*
 * Cluster.cpp
 *
 *  Created on: Feb 25, 2013
 *      Author: lichao
 *  For cluster analysis
 */

#include "Cluster.h"

Cluster::Cluster() {
  avgDistance = -1;
  avgRadius = -1;
  centerNorm = -1;
  maxDistance = -1;
  minDistance = -1;
}

Cluster::Cluster(const Mat &feavec) {
  avgDistance = -1;
  avgRadius = -1;
  centerNorm = -1;
  maxDistance = -1;
  minDistance = -1;
  this->feavec = feavec;
}

Cluster::~Cluster() {}

float Cluster::getCenterNorm() {
  if (centerNorm < 0) {
    init();
  }
  return centerNorm;
}
float Cluster::getAvgRadius() {
  if (avgRadius < 0) {
    init();
  }
  return avgRadius;
}

float Cluster::getAvgDistance() {
  if (avgDistance < 0) {
    init();
  }
  return avgDistance;
}

float Cluster::getMaxDistance() {
  if (maxDistance < 0) {
    init();
  }
  return maxDistance;
}

float Cluster::getMinDistance() {
  if (minDistance < 0) {
    init();
  }
  return minDistance;
}

vector<Cluster> Cluster::makeClusters(const Mat &feavecs,
                                      const vector<int> &ind, int k) {
  vector<Cluster> clus(k);
  vector<Mat> feas(k);
  vector<bool> inited(k, false);
  size_t nr = feavecs.rows;
  for (size_t i = 0; i < nr; i++) {
    int cid = ind[i];
    if (inited[cid]) {
      feas[cid].push_back(feavecs.row(i));
    } else {
      inited[cid] = true;
      feas[cid] = feavecs.row(i).clone();
    }
  }
  for (size_t i = 0; i < k; i++) {
    clus[i] = Cluster(feas[i]);
  }
  return clus;
}

void Cluster::init() {
  size_t numvec = feavec.rows;
  double sum = 0;
  feamean = feavec.row(0).clone();
  for (size_t i = 1; i < numvec; i++) {
    feamean += feavec.row(i);
  }
  feamean /= numvec;
  centerNorm = norm(feamean);
  minDistance = norm(feavec.row(0), feavec.row(1));
  double dis;
  double censum = 0;
  for (size_t i = 0; i < numvec; i++) {
    censum += norm(feavec.row(i), feamean);
  }
  censum /= numvec;
  avgRadius = censum;

  for (size_t i = 0; i < numvec - 1; i++)
    for (size_t j = i + 1; j < numvec; j++) {
      dis = norm(feavec.row(i), feavec.row(j));
      if (dis > maxDistance) {
        maxDistance = dis;
      }
      if (dis < minDistance) {
        minDistance = dis;
      }
      sum += dis;
    }
  avgDistance = sum * 2 / (numvec - 1) / numvec;
}
