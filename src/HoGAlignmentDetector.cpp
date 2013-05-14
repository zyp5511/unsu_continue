/*
 * HoGAlignmentDetector.cpp
 *
 *  Created on: May 13, 2013
 *      Author: lichao
 */

#include "HoGAlignmentDetector.h"

HoGAlignmentDetector::HoGAlignmentDetector(){
	tlx=0;
	tly=0;
}

HoGAlignmentDetector::~HoGAlignmentDetector(){
}

void HoGAlignmentDetector::detect(const vector<float>& vec, int&c, float&score,
		bool& accepted){
	accepted = false;
	int w=2;
	int h=2;
	int step=1;
	int pw=10;
	int ph=14;

	vector<float> subvec(w*h*32,0);

	int seg = pw*ph;
	for(int x=0;x<pw-w+1;x++){
		for(int y=0;y<ph-h+1;y++){
			//subvector extraction
			for(int j=0;j<32;j++){
				subvec[j*4+0]=vec[j*seg+x*ph+y];
				subvec[j*4+1]=vec[j*seg+x*ph+y+1];
				subvec[j*4+2]=vec[j*seg+(x+1)*ph+y];
				subvec[j*4+3]=vec[j*seg+(x+1)*ph+y+1];
			}
			int csub;
			float ssub;
			bool asub;
			referenceFinder->detect(subvec,csub,ssub,asub);
			if (asub){
				c = csub;
				score = ssub;
				accepted = true;
				tlx = x*8;
				tly = y*8;
				return;
			}
		}
	}
}

void HoGAlignmentDetector::setFinder(shared_ptr<KNNDetector> rf){
	referenceFinder = rf;
}
