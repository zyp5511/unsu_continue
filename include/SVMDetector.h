#pragma once
#include "PatchDetector.h"
using namespace std;
const int vlen=4480;
class SVMDetector: public
PatchDetector
{
public:
	SVMDetector(string fname);
	SVMDetector(const Mat& feavec, const vector<int>& label);
    ~SVMDetector(void);
	virtual void detect(const vector<float>& vec, int&c, float&score) override;
private:
	CvSVM SVM;
	bool load(string fname);
	float b;//constant term in svm
	float svm[vlen];//svm coeff;
	
};

