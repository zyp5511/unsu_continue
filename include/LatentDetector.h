#pragma once
#include "KNNDetector.h"
#include "FeatureLoader.h"
#include <fstream>
#include <memory>

using namespace cv;

class LatentDetector:
	public KNNDetector
{
	public:
		LatentDetector(string vecfname,string clusfname,const vector<bool>& gamecard);
		~LatentDetector(void)=default;
		virtual void detect(const vector<float>& vec, int&c, float&score) override;
};

