//
//  NegativeCollector.h
//  HumanPoseDetector
//
//  Created by Lichao Chen on 12/3/12.
//  Copyright (c) 2012 Lichao Chen. All rights reserved.
//

#ifndef __HumanPoseDetector__NegativeCollector__
#define __HumanPoseDetector__NegativeCollector__

#include <iostream>
#include "ImageCropper.h"
#include "Feature.h"

using namespace std;
using namespace cv;

class NegativeCollector:public ImageCropper {
protected:
    int patchesPerImage=10;
    
public:
    Mat feas;
    vector<int> category;
    NegativeCollector(){};
    ~NegativeCollector(){};
    void collectSrcDir(string fname);
    void kmean();
    void kmean(int k);
    void exportPatches(string fname);
    
    virtual void setUp(Mat img);
    virtual vector<Rect>::const_iterator getRects(){
        return all_candidates.begin();
    }
    virtual vector<Mat>::const_iterator getMats(){
        return all_candidate_mats.begin();
    }
    virtual vector<Rect>::const_iterator getRectsEnd(){
        return all_candidates.end();
    }
};


#endif /* defined(__HumanPoseDetector__NegativeCollector__) */
