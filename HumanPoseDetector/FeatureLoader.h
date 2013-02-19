#pragma once
#include<opencv2/opencv.hpp>
#include<fstream>
using namespace std;
using namespace cv;
class FeatureLoader
{
public:
	FeatureLoader(void);
	~FeatureLoader(void);

	Mat loadYAML(string fsfn);//load feature vecs from YAML, small
    Mat loadBigYAML(string fsfn);//load feature vecs from YAML, big


};

