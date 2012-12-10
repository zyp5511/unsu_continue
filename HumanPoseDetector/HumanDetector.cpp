#include <opencv2/opencv.hpp>
#include <fstream>
#include <sstream>
#include "HumanDetector.h"

using namespace cv;
using namespace std;


HumanDetector::HumanDetector(string fname){
//    ifstream fin(fname);
	ifstream fin(fname.c_str());
	string line;
	getline(fin,line);
    
//	b = stof(line);
    b = atof(line.c_str());
    
	getline(fin,line);
	stringstream ss(line);
	string item;
	for(int i=0;i<vlen;i++){
		getline(ss, item, ',');
//		svm[i]=stof(item);
        svm[i]=atof(item.c_str());
	}
	fin.close();
}

HumanDetector::HumanDetector(void):HumanDetector("/Users/lichao/data/112012/ori_svm.txt")
{
}

HumanDetector::~HumanDetector(void)
{
}

void HumanDetector::detect(vector<float>& vec, int& cat, float&score){
	float sum=-b;
	for(int i = 0;i< vlen;i++){
		sum+=svm[i]*vec[i];
	}
	cout<<sum<<endl;
	if (sum>0){
		cat = 1;
	}
	else{
		cat= -1;
	}
	score = sum;
}
