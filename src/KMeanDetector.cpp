#include "KMeanDetector.h"

KMeanDetector::KMeanDetector(string vecfn,string clusfn) {
	load(vecfn, clusfn);
}

void KMeanDetector::load(string vecfn,string clusfn){
	//Load vector matrix
	auto fl = FeatureLoader();
	feavec = fl.loadTab(vecfn);
	int in_count = feavec.rows;

	//Load cluster index
	ifstream fin(clusfn);
	int cmax = 0;
	clus = vector<int>(in_count);
	for(int i=0;i<in_count;i++){
		fin>>clus[i];
		if (clus[i]>cmax)
			cmax=clus[i];
	}
	fin.close();

	centers=Mat(cmax+1, feavec.cols, CV_32F, Scalar(0));
	vector<int> vec_count(cmax+1, 0);

	for(int i=0;i<in_count;i++){
		centers.row(clus[i])+=feavec.row(i);
		vec_count[clus[i]]++;
	}
	for(int i=0;i<cmax+1;i++){
		centers.row(i)/=vec_count[i];
	}

}



void KMeanDetector::detect(const vector<float>& vec, int&c, float&score){
	int n = 5;
	vector<int> ind(n);
	vector<float> dis(n,FLT_MAX);
	//fill(dis.begin(),dis.end(),FLT_MAX);
	auto len = centers.rows;
	for(int i=0;i<len;i++){
		auto temp = norm(centers.row(i),vec);
		int j = n;
		while(j>0&&temp<dis[j-1]){
			if(j<n){
				dis[j]=dis[j-1];
			}
			j--;
		}
		if (j<n){
			dis[j]=temp;
			ind[j]=i;
		}
	}
	if (dis[0]<12){
		c = ind[0];
		score = dis[0];//not good.. not real distance.
	}
	else{
		c = -1;
	}
}

