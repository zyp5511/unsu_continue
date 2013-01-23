#include "KNNDetector.h"


KNNDetector::KNNDetector(void)
{

}


KNNDetector::KNNDetector(string vecfn,string clusfn)
{
	loadText(vecfn, clusfn);
}

KNNDetector::KNNDetector(const Mat& featureVec,vector<int>& clusVec){
	feavec = featureVec;
	clus = clusVec;
	KDTreeIndexParams indexParams;
	feaind=make_shared<Index>(feavec,indexParams);
}


KNNDetector::~KNNDetector(void)
{
}


void KNNDetector::detect(const vector<float>& vec, int&c, float&score){
	int n = 5;
	vector<int> ind(n);
	vector<float> dis(n);
	feaind->knnSearch(vec,ind,dis,n);
	if (dis[0]<70){
		c = clus[ind[0]];
		score = dis[0];
		cout<<"==============="<<endl;

		for(auto i=0;i<ind.size();i++){
			cout<<ind[i]<<"\t:\t"<<clus[ind[i]]<<"\t:\t"<<dis[i]<<endl;
		}
		cout<<"==============="<<endl;
	}
	else{
		c = -1;
	}
}


void KNNDetector::save(string fsfn,string indfn){
	//save to FileStorage
	FileStorage fs(fsfn, FileStorage::WRITE);

	fs << "feature" << feavec;
	fs << "index" << Mat(clus);
	fs.release();

	//save flann index
	cout<<"index build"<<endl;
	feaind->save(indfn);
	cout<<"index saved"<<endl;
}

void KNNDetector::load(string fsfn,string indfn){//incompelete: FSload

	FileStorage fs;
	cout<<"opening "<<fsfn<<endl;
	fs.open(fsfn, FileStorage::READ);
	Mat clusMat;

	cout<<"loading clus list"<<endl;
	fs["index"] >> clusMat;
	cout<<"transpose clus list"<<endl;
	clus = vector<int>(clusMat.begin<int>(),clusMat.end<int>());

	cout<<"loading feature matrix"<<endl;

	fs["feature"] >> feavec;
	fs.release();


	feaind = make_shared<Index>();
	cout<<"loading flann index"<<endl;
	feaind->load(feavec,indfn);
	cout<<"index loaded"<<endl;

}
void KNNDetector::loadText(string vecfn,string clusfn){
	ifstream fin(clusfn);
	string line;
	clus = vector<int>();
	int ncounter = 0;
	while (getline(fin,line))
	{
		ncounter ++;
		int ind = stoi(line);
		clus.push_back(ind);
	}

	fin.close();
	cout<<"there are "<<ncounter<<" numbers"<<endl;


	feavec= Mat(ncounter,vlen,CV_32F);//Memory should be released by Index

	fin.open(vecfn);
	int count =0;
	while(getline(fin,line)) {
		count++;
		stringstream ss(line);
		string item;
		getline(ss,item,' ');
		float sum=0;
		for(int i=0;i<vlen;i++){
			getline(ss, item, ' ');
			float val = stof(item);
			feavec.at<float>(count-1,i)=val;
			sum+=val;
		}
	}
	cout<<"there are "<<count<<" vectors"<<endl;
	fin.close();

	//init Index;
	KDTreeIndexParams indexParams;
	feaind=make_shared<Index>(feavec,indexParams);
}
void KNNDetector::loadYAML(string fsfn,string indfn){
	ifstream fin(fsfn);
	//load feature
	//1st&2nd lines
	string line;
	getline(fin,line); cout<<line<<endl;getline(fin,line);cout<<line<<endl;
	int mr,mc;
	string k;
	fin>>k;
	fin>>mr;
	cout<<"there are "<<mr<<" rows"<<endl;
	fin>>k;
	fin>>mc;
	cout<<"there are "<<mc<<" clos"<<endl;
	fin>>k; fin>>k; fin>>k; fin>>k;
	int count =5;
	feavec = Mat(mr,mc,CV_32F);//Memory should be released by Index
	for(int i=0;i<mr;i++){
		for(int j=0;j<mc;j++){
			string val;
			fin>>val;
			if (count>0||i==mr-1&&j>mc-5)
			{
				cout<<val<<"\t"<<stof(val)<<endl;
				count--;
			}
			feavec.at<float>(i,j)=stof(val);
		}
	}
	cout<<"feature loaded"<<endl;
	//load index
	getline(fin,line); getline(fin,line);
	int clur,cluc;
	fin>>k;
	fin>>clur;
	cout<<"there are "<<clur<<" rows"<<endl;
	fin>>k;
	fin>>cluc;
	cout<<"there are "<<cluc<<" clos"<<endl;
	fin>>k; fin>>k; fin>>k; fin>>k;
	clus=vector<int>(clur);

	count=5;
	for(int j=0;j<clur;j++){
			string val;
			fin>>val;
			if (count>0||j>clur-5)
			{
				cout<<val<<"\t"<<stoi(val)<<endl;
				count--;
			}
			clus[j]=stoi(val);
	}

	//close file
	fin.close();

	feaind = make_shared<Index>();
	cout<<"loading flann index"<<endl;
	feaind->load(feavec,indfn);
	cout<<"index loaded"<<endl;
}
