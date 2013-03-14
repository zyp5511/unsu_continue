#pragma once
#include <opencv2/opencv.hpp>
#include "PatchDetector.h"

using namespace std;
using namespace cv;

struct Result {
	Result(int c, float s){category=c;score=s;};
	Result(){category = -1; score = 0;};
	int category;
	float score;
};

class Feature
{
	public:
		Feature(void);
		~Feature(void);
		Feature(const vector<float>& aVec){vec=aVec;};
		Feature(Mat& patch);
		Feature(Mat& patch, const PCA& pca);
		void detect(PatchDetector& hd);
		int getCategory(){return res.category;};
		float getScore(){return res.score;};
		Result getResult(){return res;}
		//Result getResult(Mat& mat);
		float l2(Feature b);
		void toHeadless();
		vector<float> vec;//should be private

	private:

		Result res;
		//    int category=-1;
		//    float score=0;;
		vector<float> process(uchar *im, int& len, const int *dims, const int sbin);
};

