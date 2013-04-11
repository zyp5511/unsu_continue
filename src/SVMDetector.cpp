#include <opencv2/opencv.hpp>
#include <fstream>
#include <sstream>
#include "SVMDetector.h"

using namespace cv;
using namespace std;


SVMDetector::SVMDetector(string fname){
	ifstream fin(fname);
	string line;
	getline(fin,line);

	b = stof(line);

	getline(fin,line);
	stringstream ss(line);
	string item;
	for(int i=0;i<vlen;i++){
		getline(ss, item, ',');
		svm[i]=stof(item);
	}
	fin.close();
}

SVMDetector::SVMDetector(const Mat& feavec, const vector<int>& label){
	CvSVMParams params;
	params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	
	Mat labelsMat(label);
	// Train the SVM
	SVM.train(feavec, labelsMat, Mat(), Mat(), params);

}
SVMDetector::~SVMDetector(void)
{
}

void SVMDetector::detect(const vector<float>& vec, int& cat, float&score){
	score = SVM.predict(Mat(vec).t(),true);
	cat = score>0?1:-1;
}
