//
//  NegativeCollector.h
//  HumanPoseDetector
//
//  Created by Lichao Chen on 12/3/12.
//  Copyright (c) 2012 Lichao Chen. All rights reserved.
//

#ifndef __HumanPoseDetector__NegativeCollector__
#define __HumanPoseDetector__NegativeCollector__

#include <iostream>
#include "ImageCropper.h"
#include "Feature.h"

using namespace std;
using namespace cv;

class RandomCropper:public ImageCropper {
	protected:
		int patchesPerImage;
		Mat feavec;
		vector<int> seperators;

	public:
		vector<int> category;
		RandomCropper(){patchesPerImage = 50;};
		~RandomCropper(){};
		void collectSrcDir(string fname);

		void kmean();
		void kmean(int k);
		void pca();
		void exportPatches(string fname);
		void exportFeatures(string fname);
		void exportSeperators(string fname);

		virtual void setUp(Mat img) override;
};


#endif /* defined(__HumanPoseDetector__NegativeCollector__) */
