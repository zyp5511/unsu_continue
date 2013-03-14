//
//  Image.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 2/1/13.
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#include "Image.h"


using namespace std;
ImageWrapper::ImageWrapper(shared_ptr<PatchDetector>  detector, shared_ptr<ImageCropper> cropper){
	pd = detector;
	ic = cropper;
}
void ImageWrapper::setImage(Mat image){
	img = image;

}
void ImageWrapper::setImage(string imgFilename){
	img = imread(imgFilename);

}
void ImageWrapper::collectPatches(){
	ic->setUp(img);
	cout<<"We collected "<<ic->size()<<" patches."<<endl;
}
void ImageWrapper::collectResult(const PCA& pca){
	auto it_mats = ic->getMats();
	auto end_mats = ic->getMatsEnd();
	int c =0;
#ifndef SEQ_IMG 
	auto mat_count = ic->all_mats.size();
	results = concurrent_vector<Result>(mat_count);
	parallel_for(
			size_t(0),
			mat_count,
			[&](size_t i)
			{
			Mat temp = ic->all_mats[i].clone();
			Feature fea(temp,pca);
			fea.detect(*pd);
			results[i]=(fea.getResult()); }
			);
#else
	for(;it_mats!=end_mats;it_mats++){
		Mat temp = it_mats -> clone();
		Feature fea(temp,pca);
		fea.detect(*pd);
		results.push_back(fea.getResult());
		c++;
	}

#endif
}
void ImageWrapper::collectResult(){
	auto it_mats = ic->getMats();
	auto end_mats = ic->getMatsEnd();
	int c =0;
	for(;it_mats!=end_mats;it_mats++){
		Mat temp = it_mats -> clone();
		Feature fea(temp);
		fea.detect(*pd);
		results.push_back(fea.getResult());
		c++;
	}
}
void ImageWrapper::calcClusHist(){
	auto it_rects = ic->getRects();
	auto end_rects = ic->getRectsEnd();
	int counter = 0;
	for ( ; it_rects!=end_rects; it_rects++,counter++) {
		if (results[counter].category>-1) {
			histogram[results[counter].category]++;
			rtb[results[counter].category].push_back(*it_rects);
		}
	}
}

void ImageWrapper::setBins(int n){
	histogram = vector<int>(n,0);
	results = concurrent_vector<Result>();
	rtb = vector<vector<Rect>>(n,vector<Rect>());
}

bool ImageWrapper::match(vector<bool> gamecard){
	bool matched =false;
	size_t len = histogram.size();
	for (int i=0; i<len; i++) {
		if ((gamecard[i]&&histogram[i]>0)) {
			matched = true;
		}
	}
	return matched;
}
Rect ImageWrapper::matchArea(vector<bool> gamecard){
	size_t len = histogram.size();
	vector<Point> points;
	for (int i=0; i<len; i++) {
		if(gamecard[i]){
			for_each(rtb[i].begin(), rtb[i].end(), [&points](Rect r){
					points.push_back(r.tl());
					points.push_back(r.br());
					});
		}
	}
	return boundingRect(points);
}

vector<vector<Rect>> ImageWrapper::matchAreaDebug(vector<bool> gamecard){
	auto res = vector<vector<Rect>>();
	size_t len = histogram.size();
	for (int i=0; i<len; i++) {
		if(gamecard[i]){
			res.push_back(rtb[i]);
		}
	}
	return res;
}
