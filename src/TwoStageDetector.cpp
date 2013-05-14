/*
 * TwoStageDetector.cpp
 *
 *  Created on: May 12, 2013
 *      Author: lichao
 */

#include "TwoStageDetector.h"

TwoStageDetector::TwoStageDetector() {
	// TODO Auto-generated constructor stub

}

TwoStageDetector::TwoStageDetector(shared_ptr<PatchDetector> aFirst,
		shared_ptr<PatchDetector> aSecond) :
		first(aFirst), second(aSecond) {
}

void TwoStageDetector::detect(const vector<float>& vec, int& c, float& score,
		bool& accepted) {
	first->detect(vec, c, score, accepted);
	if (accepted) {
		second->detect(vec, c, score, accepted);
		if (accepted) {
			return;
		} else {
			cout << "caution, entering rescue mode" << endl;
			third->detect(vec, c, score, accepted);
		}
	}
}

TwoStageDetector::TwoStageDetector(shared_ptr<PatchDetector> aFirst,
		shared_ptr<PatchDetector> aSecond, shared_ptr<PatchDetector> aThird) :
		first(aFirst), second(aSecond), third(aThird) {
}

TwoStageDetector::~TwoStageDetector() {
	// TODO Auto-generated destructor stub
}

