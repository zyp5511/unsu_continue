//
//  ImageCropper.h
//  HumanPoseDetector
//
//  Created by Lichao Chen on 2/1/13.
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#ifndef __HumanPoseDetector__ImageCropper__
#define __HumanPoseDetector__ImageCropper__

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
class ImageCropper {
public:
  virtual ~ImageCropper(
      void){}; // cannot be default, otherwise wouldn't compile under gcc 4.7
  virtual void setUp(Mat img) = 0;
  virtual void setStride(int aStride) { stride = aStride; }
  virtual void exportPatches(string fname){}
  virtual void setSize(int r, int c) {
    patch_r = r;
    patch_c = c;
  };

  virtual vector<Rect>::const_iterator getRects() { return all_rects.begin(); }
  virtual vector<Mat>::const_iterator getMats() { return all_mats.begin(); }
  virtual vector<Rect>::const_iterator getRectsEnd() { return all_rects.end(); }
  virtual vector<Mat>::const_iterator getMatsEnd() { return all_mats.end(); }
  size_t size() { return all_rects.size(); }
  vector<Rect> all_rects;
  vector<Mat> all_mats;

protected:
  int patch_r;
  int patch_c;
  int stride;
  vector<Mat> pyr;
};
#endif /* defined(__HumanPoseDetector__ImageCropper__) */
