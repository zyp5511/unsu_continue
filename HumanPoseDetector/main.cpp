//
//  main.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 11/26/12.
//  Copyright (c) 2012 Lichao Chen. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "HumanDetector.h"
#include "KNNDetector.h"
#include "NegativeCollector.h"
#include "ImageCropper.h"
#include "Feature.h"


using namespace std;
using namespace cv;

#pragma mark declaration
void headlessDetection(void);
void celebrityDetection(void);

#pragma mark main
int main(int argc, const char * argv[])
{
	//set up patch cropper
	auto nc = NegativeCollector();
	nc.setSize(128, 96);
    
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
    if (toTrain){
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
        string name;
        KNNDetector kd;
        kd.load(fsfn, indfn);
        while(getline(cin, name)){
            try{
                auto fname = srcfolder+name+".jpg";
                Mat img = imread(fname);
                
                ImageCropper ic=ImageCropper();
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
                        stringstream ss;
                        ss<<temp.getCategory()<<":"<<temp.getScore();
                        putText(out,ss.str(),(it->br()-Point(50,10)),FONT_HERSHEY_COMPLEX_SMALL,1,Scalar(255,0,0));
                    }
                }
                imwrite(desfolder+name+".jpg", out);

            } catch(Exception e){
                cerr<<e.msg<<endl;
            }
        }
    }
    
    
	return 0;
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
            
			ImageCropper ic=ImageCropper();
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
	HumanDetector hd=HumanDetector();
	//KNNDetector kd("C:\\data\\temp\\picked_fea.txt","C:\\data\\temp\\num.txt");
	KNNDetector kd;
	kd.loadText("/Users/lichao/data/112012/purifiedclusters.txt","/Users/lichao/data/112012/purifiedclustersnum.txt");
	while(getline(cin, name)){
		try{
			//auto fname = "C:\\data\\data_archive\\goodimages\\2106514165466612.jpg";
			auto fname = "/Users/lichao/data/data_archive/goodimages/"+name+".jpg";
			Mat img = imread(fname);
			namedWindow("display", CV_WINDOW_AUTOSIZE);
			ImageCropper ic=ImageCropper();
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

