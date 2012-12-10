#pragma once
#include "HumanDetector.h"
#include <fstream>
#include <memory>
using namespace cv;
using namespace flann;

class KNNDetector :
	public HumanDetector
{
public:
	KNNDetector(void);
	KNNDetector(string vecfname,string indfname, bool isIndexed = false);
    KNNDetector(Mat& feavec,vector<int> indvec, bool isIndexed = false);
	~KNNDetector(void);
	virtual void detect(vector<float>& vec, int&c, float&score);
    shared_ptr<Index> feaind;
private:
	vector<int> clus;
	Mat feavec;
	int vlen;
	

};

