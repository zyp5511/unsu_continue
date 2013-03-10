//
//  main.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 11/26/12.
//  Copyright (c) 2012 Lichao Chen. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif
#include "SVMDetector.h"
#include "KNNDetector.h"
#include "RandomCropper.h"
#include "ExhaustiveCropper.h"
#include "Feature.h"
#include "Image.h"
#include "FeatureLoader.h"
#include "FeatureWriter.h"
#include "FeaturePartitioner.h"
#include "FeaturePCA.h"
#include "Cluster.h"

using namespace std;
using namespace cv;
using namespace Eigen;


#pragma mark main

int main(int argc, const char * argv[]) {

	//default value
	string srcfolder = "/Users/lichao/data/122012/goodimages/training/";
	string desfolder = "/Users/lichao/data/122012/clneg/";
	string fsfn = "/Users/lichao/data/122012/kNNfea.yml";
	string indfn = "/Users/lichao/data/122012/kNNind.txt";
	clock_t overall_start = clock();



	int k = 100;
	if (argc > 3) {

		k = stoi(argv[2]);
		srcfolder = argv[3];
		desfolder = argv[4];
		fsfn = argv[5];
		indfn = argv[6];

		auto oper = string(argv[1]);

		if (oper == "clusteranalysis") {
			auto fl = FeatureLoader();
			auto feavec = fl.loadTab(fsfn);
			ifstream fin(indfn);
			int numvec = feavec.rows;
			vector<int> ind(numvec);
			for(int i=0;i<numvec;i++){
				fin>>ind[i];
			}
			fin.close();

			//Mat feavec;
			//vector<int> ind(k);
			//FileStorage fs(fsfn,FileStorage::READ);
			//fs["feature"]>>feavec;
			//fs["index"]>>ind;
			//fs.release();
			//cout<<"Feature Loaded"<<endl;

			auto clus = Cluster::makeClusters(feavec, ind, k);
			string resfn = argv[7];
			ofstream fout(resfn);
			fout<<"Index\tMin\tMax\tAvg"<<endl;
			for(int i=0;i<k;i++){
				float mindis = clus[i].getMinDistance();
				float maxdis = clus[i].getMaxDistance();
				float avgdis = clus[i].getAvgDistance();
				fout<<mindis<<"\t"<<maxdis<<"\t"<<avgdis<<endl;
			}
			fout.close();

		} else if (oper == "kmean") {

			auto fp = FeaturePartitioner();
			auto fl = FeatureLoader();
			auto fea = fl.loadTab(fsfn);
			vector<int> category(fea.rows);
			fp.kmean(fea, category, k);

			ofstream fout(indfn);
			for(auto i:category){
				fout<<i<<"\n";
			}
			fout.close(); //use indfn as the destination.
		} else if (oper == "pca") {
#ifndef OLD_PCA
			string evfn = argv[7];
			auto fl = FeatureLoader();

			MatrixXf fea;
			fl.loadTab2Eigen(fsfn,fea);
			auto pca = FeaturePCA(fea,0.95);
			cout << "there are " << pca.el.size() << " components in PCA"
				<< endl;
			auto a = pca.getCVPCA();
			MatrixXf shortfea;
#ifdef DEBUG_PCA
			cout<<"fea:"<<endl<<fea<<endl;
#endif
			pca.projectZeroMean(fea,shortfea);
			FileStorage fs(evfn, FileStorage::WRITE);
			fs << "mean" << a.mean;
			fs << "eigenvalues" << a.eigenvalues;
			fs << "eigenvectors" << a.eigenvectors;
			fs.release();
			cout << "eigenvalue written in " << evfn << endl;
			auto fw = FeatureWriter();
			fw.saveEigen2Tab(indfn, shortfea);		//use indfn as the destination.
#else
			string evfn = argv[7];
			auto fl = FeatureLoader();

			auto fea = fl.loadTab(fsfn);

			PCA a(fea, noArray(), CV_PCA_DATA_AS_ROW, 0.95);
			cout << "there are " << a.eigenvalues.size() << " components in PCA"
				<< endl;
			auto shortfea = a.project(fea);
			FileStorage fs(evfn, FileStorage::WRITE);
			fs << "mean" << a.mean;
			fs << "eigenvalues" << a.eigenvalues;
			fs << "eigenvectors" << a.eigenvectors;
			fs.release();
			cout << "eigenvalue written in " << evfn << endl;
			auto fw = FeatureWriter();
			fw.saveTab(indfn, shortfea);    //use indfn as the destination.
#endif
		} else if (oper == "randomcrop") {
			//set up patch cropper
			string seperator_fn = argv[7];
			auto nc = RandomCropper(k);
			nc.setSize(128, 96); nc.collectSrcDir(srcfolder);
			cout << "Patches created!" << endl;
			nc.exportFeatures(fsfn);
			nc.exportPatches(desfolder);
			nc.exportSeperators(seperator_fn);
		} else if (oper == "knnclassify") {
			string pcafn = argv[7];

			//set up patch cropper
			shared_ptr<KNNDetector> kd(new KNNDetector());
			shared_ptr<ExhaustiveCropper> ec(new ExhaustiveCropper());
			ec->setSize(128, 96);

			FileStorage pcafs(pcafn, FileStorage::READ);
			PCA pca;
			pcafs["mean"] >> pca.mean;
			pcafs["eigenvalues"] >> pca.eigenvalues;
			pcafs["eigenvectors"] >> pca.eigenvectors;
			cout << "PCA loaded" << endl;

			cout << "start loading index" << endl;
			kd->load(fsfn, indfn);

			string name;
			cout << "Please input image filename" << endl;

			vector<bool> gc(k, false);
			gc[14] = gc[15] = gc[19] = true;

			while (getline(cin, name)) {
				try {
					auto fname = srcfolder + name + ".jpg";
					cout << "loading file " << fname << endl;

					ImageWrapper iw(kd,ec);
					Mat mat = imread(fname);

					iw.setImage(mat);
					iw.setBins(k);
					iw.collectPatches();

					iw.collectResult(pca);
					iw.calcClusHist();
					vector<int> vec = iw.histogram;
					for(int i=0;i<k;i++) {
						cout<<vec[i]<<",";
					}
					cout<<endl;
					Scalar colors[]= {Scalar(255,0,0),Scalar(0,255,0),Scalar(0,0,255),
						Scalar(0,255,255)};
					int count = 0;

					if (iw.match(gc)) {
						cout<<fname<<" matched!"<<endl;
						auto r= iw.matchArea(gc);
						Mat out = mat.clone();
						rectangle(out,r,Scalar(255,255,255));
						vector<vector<Rect>> debugs = iw.matchAreaDebug(gc);
						for_each(debugs.begin(), debugs.end(), [&out,&count,&colors](vector<Rect>& rs) {
								for_each(rs.begin(), rs.end(), [&out,&count,&colors](Rect r) {
									rectangle(out,r,colors[count]);
									});
								count++;
								});
						imwrite(desfolder+name+".jpg", out);
					}
				} catch (Exception e) {
					cerr << e.msg << endl;
				}
				cout << "Please input image filename" << endl;
			}
		} else if (oper == "knncombo") {
			cout << "calucating vectors" << endl;
			string pcafn = argv[7];
			string vecoutfn = argv[8];
			ofstream fout(vecoutfn);

			string name;
			shared_ptr<KNNDetector> kd(new KNNDetector());
			shared_ptr<ExhaustiveCropper> ec(new ExhaustiveCropper());
			ec->setSize(128, 96);

			FileStorage pcafs(pcafn, FileStorage::READ);
			PCA pca;
			pcafs["mean"] >> pca.mean;
			pcafs["eigenvalues"] >> pca.eigenvalues;
			pcafs["eigenvectors"] >> pca.eigenvectors;
			cout << "PCA loaded" << endl;

			cout << "start loading index" << endl;
			kd->load(fsfn, indfn);


			vector<bool> gc(k, false);
			gc[14] = gc[15] = gc[19] = true;
			vector<string> files;

#ifndef _WIN32

			auto dp = opendir(srcfolder.c_str());

			struct dirent *fp;
			while ((fp = readdir(dp)) != NULL) {
				if (((string(fp->d_name)).find(".jpg")) != string::npos) {
					files.push_back(string(fp->d_name));
				}
			}
			closedir(dp);
#else


			WIN32_FIND_DATA FindFileData;
			HANDLE hFind = FindFirstFile(srcfolder.c_str(), &FindFileData);

			files.push_back(FindFileData.cFileName);

			while (FindNextFile(hFind, &FindFileData)){
				if((string(FindFileData.cFileName).find(".jpg"))!=string::npos){
					files.push_back(string(FindFileData.cFileName));
				}
			}

#endif


			sort(files.begin(), files.end());
			auto itend = files.rend();
			cout << "there are " << files.size() << " images" << endl;

#ifdef DEBUG
			itend = files.rbegin()+50;
#endif


			for_each(files.rbegin(), itend,
					[desfolder,srcfolder,&kd,&ec,k,&pca,&fout,&gc](string s) {
					auto fname = srcfolder+s;
					cout<<fname<<endl;
					ImageWrapper iw(kd,ec);
					Mat mat = imread(fname);

					iw.setImage(mat);
					iw.setBins(k);
					iw.collectPatches();

					iw.collectResult(pca);
					iw.calcClusHist();
					vector<int> vec = iw.histogram;
					for(int i=0;i<k;i++) {
					fout<<vec[i]<<",";
					}
					fout<<endl;
					Scalar colors[]= {Scalar(255,0,0),Scalar(0,255,0),Scalar(0,0,255),
					Scalar(0,255,255)};
					int count = 0;

					if (iw.match(gc)) {
						cout<<fname<<" matched!"<<endl;
						auto r= iw.matchArea(gc);
						Mat out = mat.clone();
						rectangle(out,r,Scalar(255,255,255));
						vector<vector<Rect>> debugs = iw.matchAreaDebug(gc);
						for_each(debugs.begin(), debugs.end(), [&out,&count,&colors](vector<Rect>& rs) {
								for_each(rs.begin(), rs.end(), [&out,&count,&colors](Rect r) {
									rectangle(out,r,colors[count]);
									});
								count++;
								});
						imwrite(desfolder+s, out);
					}

					});

			fout.close();
		}

	} else {
		cerr << "Number of args must > 3!" << endl;
	}
	double overall_diff = (clock() - overall_start) / (double) CLOCKS_PER_SEC;
	cout << "FINISHED!  " << overall_diff << " seconds used." << endl;
#if defined (_WIN32)
	system("pause");
#endif
	return 0;
}

