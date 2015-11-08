//
//  FeaturePartitioner.h
//  HumanPoseDetector
//
//  Created by Lichao Chen on 2/19/13.
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#ifndef __HumanPoseDetector__FeaturePartitioner__
#define __HumanPoseDetector__FeaturePartitioner__

#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
class FeaturePartitioner {

public:
  void exportPatches(vector<int> category, string srcdir, string desdir);
  void kmean(Mat &feavec, vector<int> &category, int k);
  void kmean(Mat &feavec, vector<int> &category, int k, const TermCriteria &tc);
};
#endif /* defined(__HumanPoseDetector__FeaturePartitioner__) */
