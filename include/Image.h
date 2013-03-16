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

#ifndef SEQ_IMG 
#include "tbb/tbb.h"
#endif

using namespace tbb;

class ImageWrapper {
	shared_ptr<PatchDetector> pd;
	shared_ptr<ImageCropper> ic;
	concurrent_vector<Result> results;
	vector<vector<Rect>> rtb; //reversal lookup table
	Mat img;

	public:
	vector<int> histogram;
	ImageWrapper(shared_ptr<PatchDetector>  detector, shared_ptr<ImageCropper> cropper);
	~ImageWrapper(){};
	void setImage(Mat image);
	void setImage(string imgFilename);
	void collectPatches();
	void collectResult();
	void collectResult(const PCA& pca);
	void calcClusHist();
	void setBins(int n);
	void scan(const vector<bool>& gamecard, const PCA& pca);
	bool match(vector<bool>);
	Rect matchArea(vector<bool>);
	vector<vector<Rect>> matchAreaDebug(vector<bool>);
};
#endif /* defined(__HumanPoseDetector__Image__) */
