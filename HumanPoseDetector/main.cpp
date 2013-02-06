//
//  main.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 11/26/12.
//  Copyright (c) 2012 Lichao Chen. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "SVMDetector.h"
#include "KNNDetector.h"
#include "RandomCropper.h"
#include "ExhaustiveCropper.h"
#include "Feature.h"
#include "Image.h"


using namespace std;
using namespace cv;

#pragma mark declaration
void headlessDetection(void);
void celebrityDetection(void);
int *processFile(PatchDetector&kd, string fname, int nc);

#pragma mark main
int main(int argc, const char * argv[])
{
	

	//default value
	string srcfolder = "/Users/lichao/data/122012/goodimages/training/";
	string desfolder = "/Users/lichao/data/122012/clneg/";
	string fsfn = "/Users/lichao/data/122012/kNNfea.yml";
	string indfn = "/Users/lichao/data/122012/kNNind.txt";
	bool toTrain = false;
	int k =100;
	if (argc>3) {
		if (string(argv[1])=="train"){
			toTrain = true;
		}
		k = stoi(argv[2]);
		srcfolder = argv[3];
		desfolder = argv[4];
		fsfn = argv[5];
		indfn = argv[6];
	}
	if (argc>7){
		cout<<"calucating vectors"<<endl;
		string vecsrcfn = argv[7];
		string vecdesfn = argv[8];
		ifstream fin(vecsrcfn);

		string name;
		shared_ptr<KNNDetector> kd(new KNNDetector());
		shared_ptr<ExhaustiveCropper> ec(new ExhaustiveCropper());
        ec->setSize(128, 96);
		cout<<"start loading index"<<endl;
		kd->loadYAML(fsfn, indfn);
        vector<bool> gc(k,false);
        gc[855]=gc[678]=gc[523]=true;
        
		while(getline(fin, name)){
//			try{
				auto fname = srcfolder+name;
				cout<<"loading file "<<fname<<endl;
            
                ImageWrapper iw(kd,ec);
                Mat mat = imread(fname);

                iw.setImage(mat);
                iw.setBins(k);
                iw.collectPatches();

                iw.collectResult();
                iw.calcClusHist();
                if (iw.match(gc)){
                    cout<<fname<<" matched!"<<endl;
                    auto  r= iw.matchArea(gc);
                    Mat out = mat.clone();
                    rectangle(out,r,Scalar(0,0,255));
                    imwrite(desfolder+fname, out);
                }
                
                
//			} catch(Exception e){
//				cerr<<e.msg<<endl;
//			}
		}
		fin.close();
//		fout.close();
	} else{
		if (toTrain){
            //set up patch cropper
            auto nc = RandomCropper();
            nc.setSize(128, 96);
			nc.collectSrcDir(srcfolder);
			cout<<"Patches created!"<<endl;

			nc.kmean(k);
			cout<<"K Mean Finished!"<<endl;
			KNNDetector kd(nc.feas,nc.category);
			cout<<"KNN index created"<<endl;
			kd.save(fsfn, indfn);
			cout<<"KNN index saved"<<endl;
			int i;
			cin>>i;
			nc.exportPatches(desfolder);
		} else {
            //set up patch cropper
            auto nc = ExhaustiveCropper();
            nc.setSize(128, 96);
			string name;
			KNNDetector kd;
			cout<<"start loading index"<<endl;
			kd.loadYAML(fsfn, indfn);
			cout<<"Please input image filename"<<endl;

			while(getline(cin, name)){
				try{
					auto fname = srcfolder+name+".jpg";
					cout<<"loading file "<<fname<<endl;

				} catch(Exception e){
					cerr<<e.msg<<endl;
				}
				cout<<"Please input image filename"<<endl;
			}
		}
	}
	return 0;
}

int *processFile(PatchDetector&kd, string fname, int nc){
	int *vec = new int[nc]();
	Mat img = imread(fname);
	ExhaustiveCropper ic=ExhaustiveCropper();
	ic.setSize(128, 96);
	ic.setUp(img);
	//Mat out = img.clone();

	auto it = ic.getRects();
	auto itm=ic.getMats();
	auto itend = ic.getRectsEnd();
	int count=0;

	for( ;it!=itend;it++,itm++){
		Feature temp = Feature(*itm);
		temp.detect(kd);
		if (temp.getCategory() >-1)
		{
			vec[temp.getCategory()]++;
			//	count++;
			//	rectangle(out,*it,Scalar(0,0,255));
			//	stringstream ss;
			//	ss<<temp.getCategory()<<":"<<temp.getScore();
			//	putText(out,ss.str(),(it->br()-Point(50,10)),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(255,0,0));
			//	if (count % 5==0)
			//	{
			//		imwrite(desfolder+name+"_"+to_string(count)+".jpg", out);
			//		out = img.clone();
			//	}
		}
	}
	return vec;
}


#pragma mark implementation

//detect headless patches
void headlessDetection(void){
	string name;
	KNNDetector kd;
	kd.loadText("/Users/lichao/data/112012/purifiedclusters.txt","/Users/lichao/data/112012/purifiedclustersnum.txt");
	while(getline(cin, name)){
		try{
			auto fname = "/Users/lichao/data/data_archive/goodimages/"+name+".jpg";
			Mat img = imread(fname);
			//            namedWindow("display", CV_WINDOW_AUTOSIZE);

			ExhaustiveCropper ic=ExhaustiveCropper();
			ic.setSize(128, 96);
			ic.setUp(img);
			Mat out = img.clone();

			auto it = ic.getRects();
			auto itm=ic.getMats();
			auto itend = ic.getRectsEnd();

			for( ;it!=itend;it++,itm++){
				Feature temp = Feature(*itm);
				temp.detect(kd);
				if (temp.getCategory() >-1)
				{
					rectangle(out,*it,Scalar(0,0,255));

					//old c++ compatibility code
					stringstream ss;
					ss<<temp.getCategory()<<
						":"<<temp.getScore();
					putText(out,ss.str(),(it->br()-Point(50,10)),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(255,0,0));
				}
			}
			imwrite("/Users/lichao/data/112012/hlknnres/"+name+".jpg", out);
			//            imshow("display", out);
			//            waitKey(0);
			//            destroyAllWindows();
		} catch(Exception e){
			cerr<<e.msg<<endl;
			//			destroyAllWindows();
		}
	}
}


//detect celebrity patches
void celebrityDetection(void){
	string name;
	SVMDetector hd=SVMDetector();
	//KNNDetector kd("C:\\data\\temp\\picked_fea.txt","C:\\data\\temp\\num.txt");
	KNNDetector kd;
	kd.loadText("/Users/lichao/data/112012/purifiedclusters.txt","/Users/lichao/data/112012/purifiedclustersnum.txt");
	while(getline(cin, name)){
		try{
			//auto fname = "C:\\data\\data_archive\\goodimages\\2106514165466612.jpg";
			auto fname = "/Users/lichao/data/data_archive/goodimages/"+name+".jpg";
			Mat img = imread(fname);
			namedWindow("display", CV_WINDOW_AUTOSIZE);
			ExhaustiveCropper ic=ExhaustiveCropper();
			ic.setUp(img);
			Mat out = img.clone();

			auto it = ic.getRects();
			auto itm=ic.getMats();
			auto itend = ic.getRectsEnd();

			for( ;it!=itend;it++,itm++){
				Feature temp = Feature(*itm);
				temp.detect(hd);
				if (temp.getCategory() ==1)
				{
					rectangle(out,*it,Scalar(0,0,255));

					//old c++ compatibility code
					stringstream ss;
					ss<<temp.getScore();
					putText(out,ss.str(),(it->br()-Point(50,10)),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(255,0,0));


					//                    putText(out,to_string(temp.getScore()),(it->br()-Point(50,10)),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(255,0,0));
					temp.toHeadless();
					temp.detect(kd);


					//old c++ compatibility code
					ss.clear();
					ss<<temp.getScore();
					putText(out,ss.str(),(it->br()-Point(50,10)),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(255,0,0));

					//                    putText(out,to_string(temp.getScore()),(it->tl()+Point(20,20)),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(0,255,0));
				}
			}
			imwrite("/Users/lichao/data/112012/res/"+name+".jpg", out);
			imshow("display", out);
			waitKey(0);
			destroyAllWindows();
		} catch(Exception e){
			cerr<<e.msg<<endl;
			destroyAllWindows();
		}
	}
}

