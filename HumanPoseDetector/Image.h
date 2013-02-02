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
#include "PatchDetector.h"
#include "ImageCropper.h"
class ImageWrapper {
    shared_ptr<PatchDetector> pd;
    shared_ptr<ImageCropper> ic;
    vector<int> results;
    vector<int> histogram;
    Mat img;
    
public:
    ImageWrapper(shared_ptr<PatchDetector>  detector, shared_ptr<ImageCropper> cropper);
    ~ImageWrapper();
    void setImage(Mat& image);
    void setImage(string imgFilename);
    void collectPatches();
    void collectResult();
    void calcClusHist();
};
#endif /* defined(__HumanPoseDetector__Image__) */
