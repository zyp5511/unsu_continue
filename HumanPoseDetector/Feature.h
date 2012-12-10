#pragma once
#include <opencv2/opencv.hpp>
#include "HumanDetector.h"

using namespace std;
using namespace cv;

class Feature
{
public:
	Feature(void);
	~Feature(void);
	Feature(const vector<float>& aVec){vec=aVec;category=-1;};
	Feature(Mat patch);
	void detect(HumanDetector& hd);
	int getCategory(){return category;};
	float getScore(){return score;};
	void toHeadless();
    vector<float> vec;//should be private
	
    
private:
    
    Mat patch;
    int category=-1;
    float score=0;;
    vector<float> process(uchar *im, int& len, const int *dims, const int sbin);
};

