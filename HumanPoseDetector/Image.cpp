//
//  Image.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 2/1/13.
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#include "Image.h"


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
}
void ImageWrapper::collectResult(){
    
}
void ImageWrapper::calcClusHist(){
    
}