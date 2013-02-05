//
//  PatchDetector.h
//  HumanPoseDetector
//
//  Created by Lichao Chen on 2/1/13.
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#ifndef __HumanPoseDetector__PatchDetector__
#define __HumanPoseDetector__PatchDetector__

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;

class PatchDetector
{
public:
	virtual ~PatchDetector(void){};
	virtual void detect(const vector<float>& vec, int&c, float&score)=0;
};
#endif /* defined(__HumanPoseDetector__PatchDetector__) */
