#pragma once

#include "PatchClassDetector.h"
#include "FeatureLoader.h"
#include <fstream>
#include <memory>

using namespace cv;

class KNNDetector: public PatchClassDetector {
public:
	KNNDetector(void) {
	}
	KNNDetector(string vecfname, string clusfname);
	KNNDetector(const Mat& featurevec, vector<int>& clusvec) :
			feavec(featurevec), clus(clusvec) {
	}
	~KNNDetector(void) {
	}
	virtual void classify(const vector<float>& vec, int&c, float&score)
			override;
	void load(string fsfn, string clusfn) override; //load feature and indice list
protected:
	vector<int> clus; //cluster tag
	Mat feavec; //feature index, hold memory
};

