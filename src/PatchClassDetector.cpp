/*
 * PatchClassDetector.cpp
 *
 *  Created on: May 12, 2013
 *      Author: lichao
 */

#include "PatchClassDetector.h"

PatchClassDetector::PatchClassDetector() {
	// TODO Auto-generated constructor stub

}

PatchClassDetector::~PatchClassDetector() {
	// TODO Auto-generated destructor stub
}

void PatchClassDetector::detect(const vector<float>& fea, int& c, float& score,
		bool&accepted) {
	classify(fea, c, score);
	if (c >= 0 && !gamecard.empty()&& gamecard[c])
		accepted = true;
	else
		accepted = false;
}

void PatchClassDetector::loadGC(const vector<bool>& gc) {
	gamecard = gc;
}
