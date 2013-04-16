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
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "SVMDetector.h"
#include "KNNDetector.h"
#include "KMeanDetector.h"
#include "LatentDetector.h"
#include "RandomCropper.h"
#include "ExhaustiveCropper.h"
#include "Feature.h"
#include "Image.h"
#include "FeatureLoader.h"
#include "FeatureWriter.h"
#include "FeaturePartitioner.h"
#include "FeaturePCA.h"
#include "Cluster.h"
#include "Transform.h"

using namespace std;
using namespace cv;
using namespace Eigen;

vector<string> loadFolder(string srcfolder);

void classify(shared_ptr<PatchDetector> kd, shared_ptr<ExhaustiveCropper> ec,
		string srcfolder, string desfolder, int k, PCA& pca, string s,
		const vector<bool>& gc, const vector<bool>&core_gc, ostream& fout, const LCTransformSet& ts=LCTransformSet()) ;
vector<bool> buildGameCard(string gcfn, int k) {
	auto res = vector<bool>(k, false);
	ifstream fin(gcfn);
	istream_iterator<int> eos;
	istream_iterator<int> iit(fin);
	while (iit != eos) {
		res[*iit] = true;
		iit++;
	}
	return res;
}
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
		if (oper == "daemon" || oper == "kmeandaemon") {
			string pcafn = argv[7];
			string portn = argv[8];
			string gcfn = argv[9];
			string coregcfn = argv[10];
			string transfn = argv[11];

			//set up patch cropper
			shared_ptr<PatchDetector> kd(new KNNDetector());
			if (oper == "kmeandaemon")
				kd = make_shared<KMeanDetector>();
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
			vector<bool> gc = buildGameCard(gcfn, k);
			vector<bool> core_gc = buildGameCard(coregcfn, k);
			LCTransformSet ts(k,transfn);

			//set up socket
			int sockfd, newsockfd, portno;
			socklen_t clilen;
			char buffer[256];
			struct sockaddr_in serv_addr, cli_addr;
			int n;
			sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (sockfd < 0) {
				fprintf(stderr, "ERROR opening socket");
				exit(1);
			}
			bzero((char *) &serv_addr, sizeof(serv_addr));
			portno = stoi(portn);
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = htonl(INADDR_ANY );
			serv_addr.sin_port = htons(portno);
			if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
					< 0) {
				fprintf(stderr, "ERROR on binding");
				exit(1);
			}
			listen(sockfd, 1024);
			clilen = sizeof(cli_addr);

			string name;
			cout << "Please input image filename" << endl;

			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if (newsockfd < 0) {
				cerr << ("ERROR on accept") << endl;
				return -1;
			}
			bzero(buffer, 256);

			n = read(newsockfd, buffer, 255);
			if (n < 0) {
				cerr << ("ERROR reading from socket") << endl;
				return -1;
			}

			while (n >= 0) {
				stringstream iss(buffer);
				getline(iss, name);
				printf("Here is the message: %s\n", buffer);
				ostringstream ss;
				try {
					classify(kd, ec, srcfolder, desfolder, k, pca, name, gc,core_gc,
							ss,ts);
				} catch (Exception& e) {
					cerr << e.msg << endl;
				}
				n = write(newsockfd, ss.str().c_str(), ss.str().size());
				if (n < 0) {
					cerr << ("ERROR writing to socket") << endl;
					return -1;
				}
				close(newsockfd);
				cout << "Please input image filename" << endl;
				newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
						&clilen);
				if (newsockfd < 0) {
					cerr << ("ERROR on accept") << endl;
					return -1;
				}
				bzero(buffer, 256);
				n = read(newsockfd, buffer, 255);
			}

			close(sockfd);
			return 0;
		} else if (oper == "clusteranalysis") {
			auto fl = FeatureLoader();
			auto feavec = fl.loadTab(fsfn);
			ifstream fin(indfn);
			int numvec = feavec.rows;
			vector<int> ind(numvec);
			for (int i = 0; i < numvec; i++) {
				fin >> ind[i];
			}
			fin.close();

			//Mat feavec;
			//vector<int> ind(k);
			//FileStorage fs(fsfn,FileStorage::READ);
			//fs["feature"]>>feavec;
			//fs["index"]>>ind;
			//fs.release();
			//cout<<"Feature Loaded"<<endl;

			vector<Cluster> clus = Cluster::makeClusters(feavec, ind, k);
			string resfn = argv[7];
			string resfn2 = argv[8];
			ofstream fout(resfn);
			ofstream fout2(resfn2);
			fout << "Min\tMax\tAvg" << endl;
			for (int i = 0; i < k; i++) {
				float mindis = clus[i].getMinDistance();
				float maxdis = clus[i].getMaxDistance();
				float avgdis = clus[i].getAvgDistance();
				fout << mindis << "\t" << maxdis << "\t" << avgdis << endl;
			}
			fout.close();
			for (size_t i = 0; i < k; i++) {
				for (size_t j = 0; j < k - 1; j++) {
					fout2 << clus[i].distance(clus[j]) << ",";
				}
				fout2 << clus[i].distance(clus[k - 1]) << endl;
			}
			fout2.close();

		} else if (oper == "kmean") {

			auto fp = FeaturePartitioner();
			auto fl = FeatureLoader();
			auto fea = fl.loadTab(fsfn);
			vector<int> category(fea.rows);
			fp.kmean(fea, category, k);

			ofstream fout(indfn);
			for (auto i : category) {
				fout << i << "\n";
			}
			fout.close(); //use indfn as the destination.
		} else if (oper == "pca") {
#ifndef OLD_PCA
			string evfn = argv[7];
			auto fl = FeatureLoader();

			MatrixXf fea;
			fl.loadTab2Eigen(fsfn, fea);
			auto pca = FeaturePCA(fea, 0.95);
			cout << "there are " << pca.el.size() << " components in PCA"
				<< endl;
			auto a = pca.getCVPCA();
			MatrixXf shortfea;
#ifdef DEBUG_PCA
			cout<<"fea:"<<endl<<fea<<endl;
#endif
			pca.projectZeroMean(fea, shortfea);
			FileStorage fs(evfn, FileStorage::WRITE);
			fs << "mean" << a.mean;
			fs << "eigenvalues" << a.eigenvalues;
			fs << "eigenvectors" << a.eigenvectors;
			fs.release();
			cout << "eigenvalue written in " << evfn << endl;
			auto fw = FeatureWriter();
			fw.saveEigen2Tab(indfn, shortfea);	//use indfn as the destination.
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
			fw.saveTab(indfn, shortfea); //use indfn as the destination.
#endif
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
		} else if (oper == "latent") {
			string pcafn = argv[7];
			string gcfn = argv[8];

			//set up patch cropper
			shared_ptr<LatentDetector> kd;
			shared_ptr<ExhaustiveCropper> ec(new ExhaustiveCropper());
			ec->setSize(128, 96);

			FileStorage pcafs(pcafn, FileStorage::READ);
			PCA pca;
			pcafs["mean"] >> pca.mean;
			pcafs["eigenvalues"] >> pca.eigenvalues;
			pcafs["eigenvectors"] >> pca.eigenvectors;
			cout << "PCA loaded" << endl;

			vector<bool> gc = buildGameCard(gcfn, k);

			cout << "start loading index" << endl;
			kd = make_shared<LatentDetector>(fsfn, indfn, gc);

			string name;
			cout << "Please input image filename" << endl;

			//gc[14] = gc[15] = gc[19] = true;

			while (getline(cin, name)) {
				try {
					//Read Image
					auto fname = srcfolder + name + ".jpg";
					Mat mat = imread(fname);
					cout << "loading file " << fname << endl;
					ImageWrapper iw(kd, ec);

					//Prepare Image Wrapper
					iw.setImage(mat);
					iw.setBins(k);

					iw.collectPatches();    //cropping
					iw.collectResult(pca);    //kNN matching

					Scalar colors[] = { Scalar(255, 0, 0), Scalar(0, 255, 0),
						Scalar(0, 0, 255), Scalar(0, 255, 255) };
					Mat out = mat.clone();
					vector<Result> debugs = iw.getBestResults(10);
					int dsize = debugs.size();
					rectangle(out, debugs[0].rect, colors[1]);
					for (size_t i = 1; i < dsize; i++) {
						rectangle(out, debugs[i].rect, colors[0]);
					}
					imwrite(desfolder + name + ".jpg", out);
				} catch (Exception& e) {
					cerr << e.msg << endl;
				}
				cout << "Please input image filename" << endl;
			}
		} else if (oper == "knnclassify") {
			string pcafn = argv[7];
			string vecoutfn = argv[8];
			string gcfn = argv[9];
			ofstream fout(vecoutfn);

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

			vector<bool> gc = buildGameCard(gcfn, k);

			while (getline(cin, name)) {
				try {
					classify(kd, ec, srcfolder, desfolder, k, pca, name, gc,vector<bool>(),
							fout);
				} catch (Exception& e) {
					cerr << e.msg << endl;
				}
				cout << "Please input image filename" << endl;
			}
			fout.close();
		} else if (oper == "knncombo") {
			cout << "calucating vectors" << endl;
			string pcafn = argv[7];
			string vecoutfn = argv[8];
			string gcfn = argv[9];
			string coregcfn = argv[10];
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

			vector<bool> gc = buildGameCard(gcfn, k);
			vector<bool> coregc = buildGameCard(coregcfn, k);

			vector<string> files = loadFolder(srcfolder);

			for (auto& s : files) {
				classify(kd, ec, srcfolder, desfolder, k, pca, s, gc,coregc, fout);
			}

			fout.close();
		} else if (oper == "kmeancombo") {
			cout << "calucating vectors" << endl;
			string pcafn = argv[7];
			string vecoutfn = argv[8];
			string gcfn = argv[9];
			ofstream fout(vecoutfn);

			string name;
			shared_ptr<KMeanDetector> kd(new KMeanDetector());
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

			vector<bool> gc = buildGameCard(gcfn, k);

			vector<string> files = loadFolder(srcfolder);

			for (auto s : files) {
				classify(kd, ec, srcfolder, desfolder, k, pca, s, gc,vector<bool>(), fout);
			}

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

void classify(shared_ptr<PatchDetector> kd, shared_ptr<ExhaustiveCropper> ec,
		string srcfolder, string desfolder, int k, PCA& pca, string s,
		const vector<bool>& gc, const vector<bool>&core_gc, ostream& fout, const LCTransformSet& ts) {
	auto fname = srcfolder + s;
	ImageWrapper iw(kd, ec);
	Mat raw = imread(fname);
	Mat mat;
	cout << fname << "\t" << raw.size() << endl;
	if (raw.rows > 800) {
		float ratio = 800. / raw.rows;
		resize(raw, mat, Size(), ratio, ratio);
		cout << "resized to \t" << mat.size() << endl;
	} else {
		mat = raw;
	}

	iw.setImage(mat);
	iw.setBins(k);
	iw.collectPatches();
	iw.collectResult(pca);
	iw.calcClusHist();
	vector<int> vec = iw.histogram;

	fout << s << endl;
	fout << "vector:\t";
	for (int i = 0; i < k - 1; i++) {
		fout << vec[i] << ",";
	}
	fout << vec[k - 1] << endl;

	auto goodRes = iw.getGoodResults();
	for (const Result& r : goodRes) {
		fout << r.category << "\t" << r.score << "\t";
		fout << r.rect.x << ":" << r.rect.y << ":" << r.rect.width << ":"
			<< r.rect.height << endl;
	}

	Scalar colors[] = { Scalar(128, 0, 0), Scalar(0, 128, 0), Scalar(0, 0, 128),
		Scalar(255, 0, 0), Scalar(0, 255, 0), Scalar(0, 0, 255), Scalar(0,
				255, 255), Scalar(255, 0, 255), Scalar(255, 255, 0), Scalar(
					0, 128, 128), Scalar(128, 0, 128), Scalar(128, 128, 0),
				Scalar(64, 64, 64), Scalar(128, 128, 128), Scalar(255, 255, 255) };
	int count = 0;

	//if(!core_gc.empty()){
	//	vector<LCTransform> trans = iw.getLCTransforms(gc, core_gc);
	//	for(LCTransform& t :trans){
	//		fout<<t.getString()<<endl;
	//	}
	//}

	if (iw.match(gc)) {
		cout << fname << " matched!" << endl;
		Mat out = mat.clone();
		vector<vector<Result>> debugs = iw.getMatchedResults(gc);
		for (auto&rs : debugs) {
			for (auto&r : rs) {
				if (!core_gc[r.category])
					rectangle(out, ts.apply(r.category,r.rect), colors[r.category % 15]);
					//rectangle(out, r.rect, colors[r.category % 15]);
				else
					rectangle(out, r.rect, colors[r.category % 15]);

			}
		}
		imwrite(desfolder + s, out);
	}
}

vector<string> loadFolder(string srcfolder) {
	vector<string> files;
#ifndef _WIN32

	auto dp = opendir(srcfolder.c_str());

	struct dirent *fp;
	while ((fp = readdir(dp)) != NULL) {
		if (((string(fp->d_name)).find(".png")) != string::npos
				|| ((string(fp->d_name)).find(".jpg")) != string::npos) {
			files.push_back(string(fp->d_name));
		}
	}
	closedir(dp);
#else

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(srcfolder.c_str(), &FindFileData);

	files.push_back(FindFileData.cFileName);

	while (FindNextFile(hFind, &FindFileData)) {
		if((string(FindFileData.cFileName).find(".jpg"))!=string::npos) {
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
	return files;
}
