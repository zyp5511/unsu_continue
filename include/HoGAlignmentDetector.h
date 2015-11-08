/*
 * HoGAlignmentDetector.h
 *
 *  Created on: May 13, 2013
 *      Author: lichao
 */

#pragma once

#include "KNNDetector.h"
#include "VoronoiDetector.h"

#include <memory>

class HoGAlignmentDetector : public PatchDetector {
public:
  HoGAlignmentDetector();
  virtual ~HoGAlignmentDetector();
  virtual void detect(Feature &feature) override;
  void setFinder(shared_ptr<KNNDetector> rf);
  void setPCA(PCA &aPCA);

  int tlx;
  int tly;

private:
  shared_ptr<KNNDetector> referenceFinder;
  PCA pca;
};
