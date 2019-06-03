//
//  PatchDetector.h
//  HumanPoseDetector
//
//  Created by Lichao Chen on 2/1/13.
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Feature.h"

using namespace std;

class PatchDetector {
public:
  PatchDetector(void){};
  virtual ~PatchDetector(void){};
  virtual void detect(Feature &feature);
  virtual void detect(const vector<float> &vec, int &c, float &score,
                      bool &accepted,Mat &neighborResponses,Mat &dists);
  virtual void detect(const vector<float> &vec, int &c, float &score,
                        bool &accepted);
  virtual void load(string fsfn,
                    string clusfn){}; // virtual fun for data loading
};
