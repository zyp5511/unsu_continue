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

class TwoStageDetector: public PatchDetector {
public:
	TwoStageDetector();
	TwoStageDetector(shared_ptr<PatchDetector> aFirst,
			shared_ptr<PatchDetector> aSecond);
	virtual void detect(const vector<float>& vec, int&c, float&score,
			bool& accepted) override;
	virtual ~TwoStageDetector();
private:
	shared_ptr<PatchDetector> first, second;
};

#endif /* TWOSTAGEDETECTOR_H_ */
