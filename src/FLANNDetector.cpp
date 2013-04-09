#include "FLANNDetector.h"

void FLANNDetector::init(){
	vlen = 4480;
}

FLANNDetector::FLANNDetector(void)
{
	init();

}


FLANNDetector::FLANNDetector(string vecfn,string clusfn)
{
	init();
	loadText(vecfn, clusfn);
}

FLANNDetector::FLANNDetector(const Mat& featureVec,vector<int>& clusVec){
	init();
	feavec = featureVec;
	clus = clusVec;
	KDTreeIndexParams indexParams;
	feaind=make_shared<Index>(feavec,indexParams);
}



void FLANNDetector::detect(const vector<float>& vec, int&c, float&score){
	int n = 9;
	vector<int> ind(n);
	vector<float> dis(n);
	feaind->knnSearch(vec,ind,dis,n);
	map<int,int> count = map<int,int>();
	if (dis[0]<20){
		cout<<"==============="<<endl;
		for(auto i=0;i<ind.size();i++){
			int nind = ind[i];
			int nclu = clus[nind];
			count[nclu]++;
			cout<<nind<<"\t:\t"<<nclu<<"\t:\t"<<dis[i]<<endl;
		}
		int max =0;
		int maxi = -1;
		for(const auto& p:count){
			if (p.second>max){
				max = p.second;
				maxi = p.first; 
			} 
		}
		cout<<"==============="<<endl;
		if(max>2){
			c = maxi;
			score = dis[0];
		} else {
			c=-1;
		}
	}
	else{
		c = -1;
	}
}


void FLANNDetector::save(string fsfn,string indfn){
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

void FLANNDetector::load(string fsfn,string indfn){//incompelete: FSload

	FileStorage fs;
	cout<<"opening "<<fsfn<<endl;
	fs.open(fsfn, FileStorage::READ);

	cout<<"loading feature matrix"<<endl;
	fs["feature"] >> feavec;
	int nr =feavec.rows;

	cout<<"loading clus list"<<endl;
	clus = vector<int>(nr);

	fs["index"] >> clus;
	cout<<"nr = "<<nr<<endl;
	cout<<"clus size = "<<clus.size()<<endl;

	fs.release();


	feaind = make_shared<Index>();
	cout<<"loading flann index"<<endl;
	feaind->load(feavec,indfn);
	cout<<"index loaded"<<endl;

}
void FLANNDetector::loadText(string vecfn,string clusfn){
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
void FLANNDetector::loadYAML(string fsfn,string indfn){
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
	vlen = mc;
	fin>>k; fin>>k; fin>>k; fin>>k;
	int count =5;
	feavec = Mat(mr,mc,CV_32F);//Memory should be released by Index
	for(int i=0;i<mr;i++){
		if(!(i % 1000)) {
			cout<<"have read "<<i<<" lines"<<endl;
		}
		for(int j=0;j<mc;j++){
			string val;
			fin>>val;

			if (count>0||(i==mr-1&&j>mc-5))
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

	ifstream my_file(indfn);
	if (my_file.good())
	{
		my_file.close();

		feaind->load(feavec,indfn);
	}else {
		cout<<"file doesn't exist, begin creating..."<<endl;
		KDTreeIndexParams indexParams;
		feaind=make_shared<Index>(feavec,indexParams);

		cout<<"index build"<<endl;
		feaind->save(indfn);
		cout<<"index saved"<<endl;
	}
	cout<<"index loaded"<<endl;
}
