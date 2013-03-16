#pragma once
#include "PatchDetector.h"
#include "FeatureLoader.h"
#include <fstream>
#include <memory>

using namespace cv;

class KNNDetector :
	public PatchDetector
{
	public:
		KNNDetector(void)=default;
		KNNDetector(string vecfname,string clusfname);
		KNNDetector(const Mat& featurevec,vector<int>& clusvec):feavec(featurevec),clus(clusvec){};
		~KNNDetector(void)=default;
		virtual void detect(const vector<float>& vec, int&c, float&score) override;
		void load(string fsfn,string clusfn);//load feature and indice list
	protected:
		vector<int> clus;//cluster tag
		Mat feavec;//feature index, hold memory
};

