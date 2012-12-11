//
//  NegativeCollector.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 12/3/12.
//  Copyright (c) 2012 Lichao Chen. All rights reserved.
//

#include "NegativeCollector.h"
#include <dirent.h>

void NegativeCollector::collectSrcDir(string fname){
    auto dp = opendir(fname.c_str());
    vector<string> files;
    struct dirent *fp;
    while ((fp = readdir(dp)) != NULL) {
        if(((string(fp->d_name)).find(".jpg"))!=string::npos){
            files.push_back(string(fp->d_name));
        }
    }
    closedir(dp);
    sort(files.begin(), files.end());
    auto itend = files.rend();
    
    
#ifdef DEBUG
    itend = files.rbegin()+50;
#endif
    
    for_each(files.rbegin(), itend, [this,fname](string s){
        Mat img = imread(fname+s);
        cout<<fname+s<<endl;
        this->setUp(img);
        
    });
}
void NegativeCollector::kmean(){
    kmean(100);
}

void NegativeCollector::kmean(int k){
    auto it = all_candidate_mats.begin();
    Feature f(*it);
    int numFea = static_cast<int>(f.vec.size());
    
    Mat dat(static_cast<int>(all_candidate_mats.size()), numFea,CV_32F);
    category = vector<int>(all_candidate_mats.size());
    
    for(int i = 0;i<dat.rows;i++){
        dat.row(i)=Mat(Feature(*it++).vec).t();
    }
    kmeans(dat, k, category, TermCriteria(CV_TERMCRIT_ITER, 30, 0), 5, KMEANS_PP_CENTERS);
    for_each(category.begin(),category.end() , [](int i) {cout<<i<<endl;});
    
}
void NegativeCollector::exportPatches(string fname){
    
    if (*fname.rbegin()!='/'){
        cout<<(int)*fname.rbegin()<<endl;
        fname = fname+"/";
    }
    
    
    for(int i=0;i<all_candidate_mats.size();i++){
        stringstream ss;
        ss<<fname<<category[i]<<"/";
        ss<<(i+1)<<".jpg";
        cout<<ss.str()<<endl;
        imwrite(ss.str(), all_candidate_mats[i]);
    }
}
void NegativeCollector::setUp(Mat img){
	vector<double> level_scale;
    cout<<"windows ratio:"<<patch_r<<"\t"<<patch_c<<endl;
    
	double scale = 1.;
	double scale0 = 1.2;
	int levels = 0;
	for (levels = 0; levels < 40; levels++)
	{
		level_scale.push_back(scale);
		if (cvRound(img.cols/scale) < patch_c ||
			cvRound(img.rows/scale) < patch_r || scale0 <= 1)
			break;
		scale *= scale0;
	}
    
	levels = std::max(levels, 1);
	level_scale.resize(levels);
	size_t i;
	for (i = 0; i < level_scale.size(); i++)
	{
        int imgs = patchesPerImage/scale/scale;
        
		scale = level_scale[i];
		Size sz(cvRound(img.cols / scale), cvRound(img.rows / scale));
		Mat smaller_img;
        
		if (sz == img.size())
			smaller_img = img;
		else
		{
			resize(img,smaller_img, sz);
		}
        cout<<"windows scale:"<<smaller_img.rows<<"\t"<<smaller_img.cols<<endl;
        if(smaller_img.rows-patch_r>-1&&smaller_img.cols-patch_c> -1&&imgs>1){
            vector<int> rowvs(imgs),colvs(imgs);
            cout<<imgs<<endl;
            cv::randu(rowvs, 0, smaller_img.rows-patch_r+1);
            cv::randu(colvs, 0, smaller_img.cols-patch_c+1);
            Size scaled_win_size(cvRound(patch_c * scale), cvRound(patch_r * scale));
            for (auto rs = rowvs.begin(), cs = colvs.begin(); rs!=rowvs.end(); rs++,cs++) {
                all_candidates.push_back(Rect(Point2d(*cs,*rs) * scale, scaled_win_size));
                cout<<*cs<<"\t"<<*rs<<endl;
                Mat temp = smaller_img(Range(*rs,*rs+patch_r),Range(*cs,*cs+patch_c));
                all_candidate_mats.push_back(temp);
            }
        }
	}
}