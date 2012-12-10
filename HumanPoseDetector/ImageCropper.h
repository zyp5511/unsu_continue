#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
class ImageCropper
{
public:
	ImageCropper(void);
	~ImageCropper(void);
	virtual void setUp(Mat img);
	virtual void setSize(int r, int c);
    virtual vector<Rect>::const_iterator getRects(){
        return all_candidates.begin();
    }
    virtual vector<Mat>::const_iterator getMats(){
        return all_candidate_mats.begin();
    }
    virtual vector<Rect>::const_iterator getRectsEnd(){
        return all_candidates.end();
    }

protected:
	int patch_r;
	int patch_c;
	int stride;
    vector<Mat> pyr;
    vector<Rect> all_candidates;
	vector<Mat> all_candidate_mats;

};
