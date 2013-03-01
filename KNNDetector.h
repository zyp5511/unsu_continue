#pragma once
#include "PatchDetector.h"
#include <fstream>
#include <memory>

using namespace cv;
using namespace flann;

class KNNDetector :
public PatchDetector
{
public:
	KNNDetector(void);
	KNNDetector(string vecfname,string indfname);
    KNNDetector(const Mat& featureVec,vector<int>& indvec);
    
    ~KNNDetector(void);
	virtual void detect(const vector<float>& vec, int&c, float&score) override;
    
    void save(string fsfn,string indfn);//save feature and indice list and kNN index to FileStorage;
    void load(string fsfn,string indfn);//load feature and indice list and kNN index from FileStorage
    void loadText(string vecfn,string clusfn);//load feature nad indice list from text file, and init index
    void loadYAML(string fsfn,string indfn);//load feature nad indice list from text file, and init index

private:
    
	vector<int> clus;//cluster tag
    shared_ptr<Index> feaind;//index for knn search
	Mat feavec;//feature index, hold memory
	int vlen;//hard coded vector length, not good..
	void init();
	
    
};
