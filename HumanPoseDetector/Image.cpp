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
void ImageWrapper::setImage(Mat& image){
    img = image;
    
}
void ImageWrapper::setImage(string imgFilename){
    img = imread(imgFilename);
    
}
void ImageWrapper::collectPatches(){
    ic->setUp(img);
}
void ImageWrapper::collectResult(){
    vector<Mat>::const_iterator it_mats = ic->getMats();
    vector<Mat>::const_iterator end_mats = ic->getMatsEnd();
    for(;it_mats!=end_mats;it_mats++){
        Feature fea = Feature(*it_mats);
        fea.detect(*pd);
        results.push_back(fea.getResult());
    }
}
void ImageWrapper::calcClusHist(){
    vector<Rect>::const_iterator it_rects = ic->getRects();
    vector<Rect>::const_iterator end_rects = ic->getRectsEnd();
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
    rtb = vector<vector<Rect>>(n,vector<Rect>());
}

bool ImageWrapper::match(vector<int> gamecard){
    bool matched = true;
    size_t len = histogram.size();
    for (int i=0; i<len&&matched; i++) {
        if (matched&&((gamecard[i]>0&&histogram[i]>0)||(gamecard[i]==0&&histogram[i]==0))) {
            matched = true;
        } else {
            matched = false;
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
