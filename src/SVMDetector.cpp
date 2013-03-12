#include <opencv2/opencv.hpp>
#include <fstream>
#include <sstream>
#include "SVMDetector.h"

using namespace cv;
using namespace std;


SVMDetector::SVMDetector(string fname){
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


SVMDetector::~SVMDetector(void)
{
}

void SVMDetector::detect(const vector<float>& vec, int& cat, float&score){
	cout<<"using HumanDetector"<<endl;
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
