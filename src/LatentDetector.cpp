#include "LatentDetector.h"


LatentDetector::LatentDetector(string vecfname,string clusfname,const vector<bool>& gamecard){
	//Load vector matrix
	auto fl = FeatureLoader();
	auto temp= fl.loadTab(vecfname);
	int in_count = temp.rows;
	feavec = Mat(0,temp.cols, CV_32FC1);

	//Load cluster index
	ifstream fin(clusfname);
	clus = vector<int>(in_count);
	for(int i=0;i<in_count;i++){
		fin>>clus[i];
		if(gamecard[clus[i]]) {
			feavec.push_back(temp.row(i));
		}
	}
	cout<<"There are "<<feavec.rows<<" vectors out of "<<temp.rows<<endl;
	fin.close();
}


void LatentDetector::detect(const vector<float>& vec,int&c, float&score, bool&accepted){
	float min = FLT_MAX;
	size_t ind = -1;
	auto len = feavec.rows;
	for(int i=0;i<len;i++){
		auto temp = norm(feavec.row(i),vec);
		if (temp<min) {
			min = temp;
			ind = i;
		}
	}
	c = clus[ind];
	score = min;
}

