#include "KNNDetector.h"


KNNDetector::KNNDetector(void)
{
    
}


KNNDetector::KNNDetector(string vecfname,string indfname,bool isIndexed):vlen(4480)
{
	ifstream fin(indfname);
//    ifstream fin(indfname.c_str());
	string line;
	auto clusind = vector<int>();
	int ncounter = 0;
	while (getline(fin,line))
	{
		ncounter ++;
		int ind = stoi(line);
//        int ind = atoi(line.c_str());
		clusind.push_back(ind);
	}
    
	fin.close();
	cout<<"there are "<<ncounter<<" numbers"<<endl;
    Mat featureVec(ncounter,vlen,CV_32F);//Memory should be released by Index
    
    fin.open(vecfname);
//    fin.open(vecfname.c_str());
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
//            float val = atof(item.c_str());
            featureVec.at<float>(count-1,i)=val;
            sum+=val;
        }
        //cout<<sum<<endl;
    }
    cout<<"there are "<<count<<" vectors"<<endl;
    init(featureVec, clusind, isIndexed);
}

KNNDetector::KNNDetector(const Mat& featureVec,vector<int>& indvec, bool isIndexed ){
    init(featureVec, indvec, isIndexed);
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

void KNNDetector::init(const Mat& featureVec, const vector<int>& indvec, bool isIndexed){
    clus = indvec;
    feavec = featureVec;
    
	if(!isIndexed){
		KDTreeIndexParams indexParams;
		feaind=make_shared<Index>(feavec,indexParams);//Has to use pointer. otherwise ->index will be released by the local copy.
        FileStorage fs("index.yml", FileStorage::WRITE);
        if (!fs.isOpened()){
            fs.open("index.yml", FileStorage::WRITE);
            fs << "feature" << featureVec;
            fs << "index" << indvec;
            fs.release();
        }
		cout<<"index build"<<endl;
        feaind->save("kdtree1000clusterspurified.txt");
		cout<<"index saved"<<endl;
	} else{
		feaind = make_shared<Index>();
        feaind->load(feavec,"/Users/lichao/data/temp/kdtree100clusterspurified.txt");
		cout<<"index loaded"<<endl;
	}
}
