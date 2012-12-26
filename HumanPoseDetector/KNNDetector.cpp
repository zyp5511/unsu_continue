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
    
    FileStorage fs(fsfn, FileStorage::READ);
    Mat clusMat;

    fs["feature"] >> feavec;
    fs["index"] >> clusMat;
    clus = vector<int>(clusMat.begin<int>(),clusMat.end<int>());
    
    
    feaind = make_shared<Index>();
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
    
    //init Index;
    KDTreeIndexParams indexParams;
    feaind=make_shared<Index>(feavec,indexParams);
}
