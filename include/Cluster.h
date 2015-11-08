/*
* Cluster.h
*
*  Created on: Feb 25, 2013
*      Author: lichao
*/

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Cluster {
public:
  static vector<Cluster> makeClusters(const Mat &feavecs,
                                      const vector<int> &ind, int k);
  Cluster();
  Cluster(const Mat &feavec);
  virtual ~Cluster();
  float getAvgDistance();
  float getAvgRadius();
  float getCenterNorm();
  float getMaxDistance();
  float getMinDistance();
  float distance(const Cluster &b) { return norm(feamean, b.feamean); }

private:
  void init();
  float avgDistance;
  float avgRadius;
  float centerNorm;
  float maxDistance;
  float minDistance;
  Mat feamean;
  Mat feavec;
};

#endif /* CLUSTER_H_ */
