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
                                   shared_ptr<PatchDetector> aSecond)
    : first(aFirst), second(aSecond) {}

void TwoStageDetector::detect(Feature &feature) {
  first->detect(feature);
  Result orires = feature.res;
  if (orires.accepted) {
    second->detect(feature);
    if (feature.res.accepted) {
      return;
    } else if (third) {
      cout << "caution, entering rescue mode" << endl;
      third->detect(feature);
      if (!feature.res.accepted) {
        feature.res.category = -1;
        feature.res.score = 0;
      } else {
        feature.res = orires;
      }
    }
  }
}

TwoStageDetector::TwoStageDetector(shared_ptr<PatchDetector> aFirst,
                                   shared_ptr<PatchDetector> aSecond,
                                   shared_ptr<PatchDetector> aThird)
    : first(aFirst), second(aSecond), third(aThird) {}

TwoStageDetector::~TwoStageDetector() {
  // TODO Auto-generated destructor stub
}
