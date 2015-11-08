/*
 * HoGAlignmentDetector.cpp
 *
 *  Created on: May 13, 2013
 *      Author: lichao
 */

#include "HoGAlignmentDetector.h"

HoGAlignmentDetector::HoGAlignmentDetector() {
  tlx = 0;
  tly = 0;
}

HoGAlignmentDetector::~HoGAlignmentDetector() {}

void HoGAlignmentDetector::detect(Feature &feature) {
  feature.res.accepted = false;
  int w = 2;
  int h = 2;
  int step = 1;
  int pw = 10;
  int ph = 14;

  vector<float> subvec(w * h * 32, 0);

  int seg = pw * ph;
  for (int x = 0; x < pw - w + 1; x++) {
    for (int y = 0; y < ph - h + 1; y++) {
      // subvector extraction
      for (int j = 0; j < 32; j++) {
        subvec[j * 4 + 0] = feature.orivec[j * seg + x * ph + y];
        subvec[j * 4 + 1] = feature.orivec[j * seg + x * ph + y + 1];
        subvec[j * 4 + 2] = feature.orivec[j * seg + (x + 1) * ph + y];
        subvec[j * 4 + 3] = feature.orivec[j * seg + (x + 1) * ph + y + 1];
      }
      int csub;
      float ssub;
      bool asub;
      Mat temp = pca.project(subvec);
      auto finalvec = vector<float>(temp.begin<float>(), temp.end<float>());
      referenceFinder->detect(finalvec, csub, ssub, asub);
      if (asub) {
        feature.res.category = csub;
        feature.res.score = ssub;
        feature.res.accepted = true;
        tlx = x * 8;
        tly = y * 8;
        return;
      }
    }
  }
}

void HoGAlignmentDetector::setPCA(PCA &aPCA) { pca = aPCA; }
void HoGAlignmentDetector::setFinder(shared_ptr<KNNDetector> rf) {
  referenceFinder = rf;
}
