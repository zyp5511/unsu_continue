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

void PatchClassDetector::detect(const vector<float> &fea, int &c, float &score,
                                bool &accepted) {
  classify(fea, c, score);
  if (c >= 0 && !gamecard.empty() && gamecard[c])
    accepted = true;
  else
    accepted = false;
}


void PatchClassDetector::detect(const vector<float> &fea, int &c, float &score,
                                bool &accepted,Mat &neighborResponses,Mat &dists) {
  classify(fea, c, score, neighborResponses, dists);
  if (c >= 0 && !gamecard.empty() && gamecard[c])
    accepted = true;
  else
    accepted = false;
}


void PatchClassDetector::classify(const vector<float> &fea, int &c, float &score) {
  cerr << "Not implemented." << endl;
}

void PatchClassDetector::classify(const vector<float> &fea, int &c, float &score, Mat &neighborResponses, Mat &dists) {
  cerr << "Not implemented." << endl;
}




void PatchClassDetector::loadGC(const vector<bool> &gc) { gamecard = gc; }
