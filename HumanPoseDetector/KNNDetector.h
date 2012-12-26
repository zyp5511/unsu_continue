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
	KNNDetector(string vecfname,string indfname);
    KNNDetector(const Mat& featureVec,vector<int>& indvec);
    
	~KNNDetector(void);
	virtual void detect(const vector<float>& vec, int&c, float&score);
    void save(string fsfn,string indfn);//save feature and indice list and kNN index to FileStorage;
    void load(string fsfn,string indfn);//load feature and indice list and kNN index from FileStorage
    void loadText(string vecfn,string clusfn);//load feature nad indice list from text file, and init index

private:
    
	vector<int> clus;//cluster tag
    shared_ptr<Index> feaind;//index for knn search
	Mat feavec;//feature index, hold memory
	int vlen=4480;
	
    
};

