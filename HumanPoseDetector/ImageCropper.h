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
class ImageCropper
{
public:
	virtual ~ImageCropper(void);
	virtual void setUp(Mat img);
	virtual void setSize(int r, int c);
    
    virtual vector<Rect>::const_iterator getRects(){
        return all_rects.begin();
    }
    virtual vector<Mat>::const_iterator getMats(){
        return all_mats.begin();
    }
    virtual vector<Rect>::const_iterator getRectsEnd(){
        return all_rects.end();
    }
    
protected:
	int patch_r;
	int patch_c;
	int stride;
    vector<Mat> pyr;
    vector<Rect> all_rects;
	vector<Mat> all_mats;
    
};
#endif /* defined(__HumanPoseDetector__ImageCropper__) */
