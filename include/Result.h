#pragma once
#include <opencv2/opencv.hpp>
#include "Transform.h"

using namespace std;
using namespace cv;
class Result {
public:
	Result(int c, float s) {
		category = c;
		score = s;
	}
	Result() {
		category = -1;
		score = 0;
	}
	LCTransform getLCTransform(const Result& to) {
		auto br = to.rect;
		int x = br.x - rect.x;
		int y = br.y - rect.y;
		float r = (br.height + 0.0) / rect.height;
		float xr = (x+0.0)/rect.width;
		float yr = (y+0.0)/rect.height;
		return LCTransform(xr,yr,r,category, to.category);

	}
	bool overlapped(const Result& b){
		return (rect&b.rect)==Rect()?false:true;
	}
	int category;
	float score;
	Rect rect;
	Rect real_rect;
};

