#pragma once
#include "PatchDetector.h"
#include "FeatureLoader.h"
#include <fstream>
#include <memory>

using namespace cv;

class KMeanDetector :
	public PatchDetector 
	/*classify patches by comaparing distances from centers of clusters, instead 
												 of all training samples*/
{
	public:
		KMeanDetector(void)=default;
		KMeanDetector(string vecfname,string clusfname);
		KMeanDetector(const Mat& featurevec,vector<int>& clusvec):feavec(featurevec),clus(clusvec){};
		~KMeanDetector(void){};
		virtual void detect(const vector<float>& vec, int&c, float&score) override;
		void load(string fsfn,string clusfn) override;//load feature and indice list
	protected:
		Mat centers;
		vector<int> clus;//cluster tag
		Mat feavec;//feature index, hold memory
};

