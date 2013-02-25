#pragma once
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class FeatureWriter
{
	public:
		FeatureWriter(void);
		~FeatureWriter(void);
		void saveYAML(string fsfn, const Mat& feavec);
		void saveTab(string fname, const Mat& feavec);
};

