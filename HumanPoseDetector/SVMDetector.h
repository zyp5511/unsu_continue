#pragma once
#include "PatchDetector.h"
using namespace std;
const int vlen=4480;
class SVMDetector: public
PatchDetector
{
public:
	SVMDetector(void);
	SVMDetector(string fname);
    ~SVMDetector(void);
	virtual void detect(const vector<float>& vec, int&c, float&score) override;
private:
	bool load(string fname);
	float b;//constant term in svm
	float svm[vlen];//svm coeff;
	
};

