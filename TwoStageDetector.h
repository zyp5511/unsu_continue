/*
 * TwoStageDetector.h
 *
 *  Created on: May 12, 2013
 *      Author: lichao
 */

#ifndef TWOSTAGEDETECTOR_H_
#define TWOSTAGEDETECTOR_H_

#include "PatchDetector.h"
#include <memory>

class TwoStageDetector : public PatchDetector {
public:
  TwoStageDetector();
  TwoStageDetector(shared_ptr<PatchDetector> aFirst,
                   shared_ptr<PatchDetector> aSecond);
  TwoStageDetector(shared_ptr<PatchDetector> aFirst,
                   shared_ptr<PatchDetector> aSecond,
                   shared_ptr<PatchDetector> aThird);
  virtual void detect(Feature &feature) override;
  virtual ~TwoStageDetector();

private:
  shared_ptr<PatchDetector> first, second, third;
};

#endif /* TWOSTAGEDETECTOR_H_ */
