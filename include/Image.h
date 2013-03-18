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
	/*
	 * pre-processing
	 */
	void setImage(Mat image);
	void setImage(string imgFilename);
	void collectPatches();
	void setBins(int n);
	
	/*
	 * batch KNN matching, observation vector generating, pattern matching
	 */
	void collectResult();
	void collectResult(const PCA& pca);
	void calcClusHist();
	bool match(vector<bool>);//if certain pattern are matched by image's histogram
	Rect matchArea(vector<bool>);//minRect cover all rects from certain clusters of kNN result 
	vector<vector<Rect>> matchAreaDebug(vector<bool>);//all rects from certain clusters of kNN result

	/*
	 * scanning latent variables
	 */
	void scan(const vector<bool>& gamecard, const PCA& pca);//best match for a given set of clusters
	vector<Rect> scanDebug(int i);//rects of top i responce from certain clusters of scan result
};
#endif /* defined(__HumanPoseDetector__Image__) */
