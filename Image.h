//
//  Image.h
//  HumanPoseDetector
//
//  Created by Lichao Chen on 2/1/13.
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#ifndef __HumanPoseDetector__Image__
#define __HumanPoseDetector__Image__

#include <iostream>
#include <memory>
#include "PatchDetector.h"
#include "ImageCropper.h"
#include "Feature.h"
#include "Transform.h"

#ifndef SEQ_IMG
#include "tbb/tbb.h"
#endif

using namespace tbb;

class ImageWrapper {
  shared_ptr<PatchDetector> pd;
  shared_ptr<ImageCropper> ic;
  concurrent_vector<Result> results;
  vector<vector<Result>> rtb; // reversal lookup table
  vector<LCTransform> transforms;
  CascadeClassifier face_cascade;
  Mat img;

public:
  vector<int> histogram;

  ImageWrapper(){};
  ImageWrapper(shared_ptr<PatchDetector> detector,
               shared_ptr<ImageCropper> cropper);
  ~ImageWrapper(){};

  /*
  * pre-processing
  */

  void setImage(Mat image);
  void setImage(string imgFilename);
  void collectPatches();
  void setBins(int n);
  void export_Patches(string fname);
  /*
  * batch KNN matching, observation vector generating, pattern matching
  */
  void collectResult();
  void collectResult(const PCA &pca, bool with_fea_vec);
  void calcClusHist();
  bool match(const vector<bool>
                 &); // if certain pattern are matched by image's histogram
  Rect matchArea(const vector<bool> &); // minRect cover all rects from certain
                                        // clusters of kNN result
  vector<vector<Result>> getMatchedResults(
      const vector<bool>
          &gamecard); // all rects from certain clusters of kNN result
  vector<LCTransform> getLCTransforms(const vector<bool> &gc,
                                      const vector<bool> &core_gc);
  vector<Result> getGoodResults();

  /*
  * OpenCV haar detector
  */
  void loadCVModel(string modelfn);
  vector<Rect> getocvresult(void);

  /*
  * scanning latent variables
  */
  void scan(const PCA &pca); // best match for a given set of clusters
  vector<Rect> scanDebug(int i);
  vector<Result> getBestResults(
      size_t len); // rects of top i responce from certain clusters of scan result
};
#endif /* defined(__HumanPoseDetector__Image__) */
