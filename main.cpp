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
#include <dirent.h>
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
			string evfn = argv[7];
			auto fl = FeatureLoader();

			clock_t start = clock();
			MatrixXf fea;
			fl.loadTab2Eigen(fsfn,fea);
			double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
			cout << "we use " << diff << " seconds to load file!" << endl;
			auto pca = FeaturePCA(fea,0.95);
			cout << "there are " << pca.ev.size() << " components in PCA"
					<< endl;
			auto a = pca.getCVPCA();
			MatrixXf shortfea;
			pca.project(fea,shortfea);
			FileStorage fs(evfn, FileStorage::WRITE);
			fs << "mean" << a.mean;
			fs << "eigenvalues" << a.eigenvalues;
			fs << "eigenvectors" << a.eigenvectors;
			fs.release();
			cout << "eigenvalue written in " << evfn << endl;
			auto fw = FeatureWriter();
			fw.saveEigen2Tab(indfn, shortfea);		//use indfn as the destination.
		} else if (oper == "randomcrop") {
			//set up patch cropper
			string seperator_fn = argv[7];
			auto nc = RandomCropper(k);
			nc.setSize(128, 96);
			nc.collectSrcDir(srcfolder);
			cout << "Patches created!" << endl;
			nc.exportFeatures(fsfn);
			nc.exportPatches(desfolder);
			nc.exportSeperators(seperator_fn);
		} else if (oper == "knnclassify") {
			//set up patch cropper
			auto nc = ExhaustiveCropper();
			nc.setSize(128, 96);
			string name;
			KNNDetector kd;
			cout << "start loading index" << endl;
			kd.loadYAML(fsfn, indfn);
			cout << "Please input image filename" << endl;

			while (getline(cin, name)) {
				try {
					auto fname = srcfolder + name + ".jpg";
					cout << "loading file " << fname << endl;

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
			clock_t start = clock();
			kd->load(fsfn, indfn);
			double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
			cout << "we use " << diff << " seconds to load file!" << endl;


			vector<bool> gc(k, false);
			gc[14] = gc[15] = gc[19] = true;

			auto dp = opendir(srcfolder.c_str());
			vector<string> files;
			struct dirent *fp;
			while ((fp = readdir(dp)) != NULL) {
				if (((string(fp->d_name)).find(".jpg")) != string::npos) {
					files.push_back(string(fp->d_name));
				}
			}
			closedir(dp);
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
						if (mat.rows>600) {
							resize(mat, mat, Size(),600.0/mat.rows,600.0/mat.rows);
						}

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

#if defined (_WIN32)
	system("pause");

#endif
	return 0;
}

