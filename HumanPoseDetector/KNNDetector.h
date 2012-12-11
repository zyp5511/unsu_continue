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
    KNNDetector(const Mat& featureVec,vector<int>& indvec, bool isIndexed = false);
    
	~KNNDetector(void);
	virtual void detect(const vector<float>& vec, int&c, float&score);
    
private:
    void init(const Mat& featureVec,const vector<int>& indvec, bool isIndexed = false);
	vector<int> clus;//cluster tag
    shared_ptr<Index> feaind;//index for knn search
	Mat feavec;//feature index, hold memory
	int vlen;
	
    
};

