//
//  Image.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 2/1/13.
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#include "Image.h"

using namespace std;
ImageWrapper::ImageWrapper(shared_ptr<PatchDetector> detector,
		shared_ptr<ImageCropper> cropper) {
	pd = detector;
	ic = cropper;
}
void ImageWrapper::setImage(Mat image) {
	img = image;

}
void ImageWrapper::setImage(string imgFilename) {
	img = imread(imgFilename);

}
void ImageWrapper::collectPatches() {
	ic->setUp(img);
	cout << "We collected " << ic->size() << " patches." << endl;
}

void ImageWrapper::collectResult(const PCA& pca) {
#ifndef SEQ_IMG 
	auto mat_count = ic->all_mats.size();
	results = concurrent_vector<Result>(mat_count);
	parallel_for(size_t(0), mat_count, [&](size_t i) {
		Mat temp = ic->all_mats[i].clone();
		Feature fea(temp,pca);
		pd->detect(fea);
		Result tempres = fea.getResult();
		tempres.rect = ic->all_rects[i];
		results[i]=(tempres);
	});
#else
	auto it_mats = ic->getMats();
	auto end_mats = ic->getMatsEnd();
	int c =0;
	for(;it_mats!=end_mats;it_mats++) {
		Mat temp = it_mats -> clone();
		Feature fea(temp,pca);
		fea.detect(*pd);
		results.push_back(fea.getResult());
		c++;
	}
#endif
}
void ImageWrapper::collectResult() {
	auto it_mats = ic->getMats();
	auto end_mats = ic->getMatsEnd();
	int c = 0;
	for (; it_mats != end_mats; it_mats++) {
		Mat temp = it_mats->clone();
		Feature fea(temp);
		pd->detect(fea);
		results.push_back(fea.getResult());
		c++;
	}
}
void ImageWrapper::calcClusHist() {
	auto it_rects = ic->getRects();
	auto end_rects = ic->getRectsEnd();
	int counter = 0;
	for (; it_rects != end_rects; it_rects++, counter++) {
		if (results[counter].category > -1) {
			histogram[results[counter].category]++;
			rtb[results[counter].category].push_back(results[counter]);
		}
	}
}

void ImageWrapper::setBins(int n) {
	histogram = vector<int>(n, 0);
	results = concurrent_vector<Result>();
	rtb = vector<vector<Result>>(n, vector<Result>());
}

bool ImageWrapper::match(const vector<bool>& gamecard) {
	bool matched = false;
	size_t len = histogram.size();
	for (int i = 0; i < len; i++) {
		if ((gamecard[i] && histogram[i] > 0)) {
			matched = true;
			break;
		}
	}
	return matched;
}

Rect ImageWrapper::matchArea(const vector<bool>& gamecard) {
	size_t len = histogram.size();
	vector<Point> points;
	for (int i = 0; i < len; i++) {
		if (gamecard[i]) {
			for_each(rtb[i].begin(), rtb[i].end(), [&points](Result r) {
				points.push_back(r.rect.tl());
				points.push_back(r.rect.br());
			});
		}
	}
	return boundingRect(points);
}

vector<vector<Result>> ImageWrapper::getMatchedResults(
		const vector<bool>& gamecard) {
	auto res = vector<vector<Result>>();
	size_t len = histogram.size();
	for (int i = 0; i < len; i++) {
		if (gamecard[i]) {
			
			res.push_back(rtb[i]);
		}
	}
	return res;
}

vector<Result> ImageWrapper::getBestResults(int len) {
	auto mat_count = ic->all_mats.size();
	vector<int> a = vector<int>(mat_count);
	for (size_t i = 0; i < mat_count; i++) {
		a[i] = i;
	}
	sort(a.begin(), a.end(), [&](int x, int y) {
		return results[x].score<results[y].score;
	});
	auto res = vector<Result>();
	int reslen = len > mat_count ? mat_count : len;
	for (int i = 0; i < len; i++) {
		res.push_back(results[a[i]]);
	}
	return res;
}
vector<LCTransform> ImageWrapper::getLCTransforms(const vector<bool>& gc,
		const vector<bool>& core_gc) {
	if (transforms.empty() && match(gc) && match(core_gc)) {
		vector<Result> froms;
		vector<Result> tos;
		size_t len = histogram.size();
		for (int i = 0; i < len; i++) {
			if (core_gc[i]) {
				tos.insert(tos.end(), rtb[i].begin(), rtb[i].end());
			} else if (gc[i]) {
				froms.insert(froms.end(), rtb[i].begin(), rtb[i].end());
			}
		}
		if (tos.size() > 0 && froms.size() > 0) {
			int len_to = tos.size();
			int len_from = froms.size();
			cout << "there are " << len_to << " true patches" << endl;
			cout << "there are " << len_from << " anchor patches" << endl;
			int c = 0;
			for (int i = 0; i < len_to; i++)
				for (int j = 0; j < len_from; j++) {
					if (tos[i].overlapped(froms[j])) {
						transforms.push_back(froms[j].getLCTransform(tos[i]));
						c++;
					}
				}
			cout << "find " << c << " transforms" << endl;
		}
	}
	return transforms;
}
vector<Result> ImageWrapper::getGoodResults() {
	auto mat_count = ic->all_mats.size();
	auto res = vector<Result>();
	for (int i = 0; i < mat_count; i++) {
		if (results[i].category > -1)
			res.push_back(results[i]);
	}
	return res;
}

void ImageWrapper::loadCVModel(string modelfn) {
	if (!face_cascade.load(modelfn)) {
		printf("--(!)Error loading\n");
	};
}

vector<Rect> ImageWrapper::getocvresult(void) {

	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(img, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2,
			0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	return faces;
}
