#pragma once
#include<opencv2/opencv.hpp>
using namespace std;
const int vlen=4480;
class HumanDetector
{
public:
	HumanDetector(void);
	HumanDetector(string fname);
	~HumanDetector(void);
	virtual void detect(const vector<float>& vec, int&c, float&score);
private:
	bool load(string fname);
	float b;//constant term in svm
	float svm[vlen];//svm coeff;
	
};

