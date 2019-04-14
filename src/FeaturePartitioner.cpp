//
//  FeaturePartitioner.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 2/19/13.
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#include "FeaturePartitioner.h"
void FeaturePartitioner::exportPatches(vector<int> category, string srcdir,
                                       string desdir) {

  if (*srcdir.rbegin() != '/') {
    srcdir = srcdir + "/";
  }
  if (*desdir.rbegin() != '/') {
    desdir = desdir + "/";
  }

  for (size_t i = 0; i < category.size(); i++) {
    string fn = to_string(i + 1) + ".jpg";
    Mat im = imread(srcdir + fn);
    imwrite(desdir + to_string(category[i]) + "/" + fn, im);
  }
}
void FeaturePartitioner::kmean(Mat &feavec, vector<int> &category, int k,
                               const TermCriteria &tc) {
  cout << "Start k-means clustering" << endl;
  double compactness = kmeans(feavec, k, category, tc, 3, KMEANS_PP_CENTERS);
  cout << "Done k-means clustering" << endl;
  cout << "Compactness is " << compactness << endl;
}
void FeaturePartitioner::kmean(Mat &feavec, vector<int> &category, int k) {
  kmean(feavec, category, k, TermCriteria(TermCriteria::MAX_ITER, 30, 0));
}
