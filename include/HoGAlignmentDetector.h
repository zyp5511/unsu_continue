/*
 * HoGAlignmentDetector.h
 *
 *  Created on: May 13, 2013
 *      Author: lichao
 */


#include "KNNDetector.h"
#include "VoronoiDetector.h"

#include <memory>

class HoGAlignmentDetector: public PatchDetector {
public:
	HoGAlignmentDetector();
	virtual ~HoGAlignmentDetector();
	virtual void detect(const vector<float>& vec, int&c, float&score,
			bool& accepted) override;
	void setFinder(shared_ptr<KNNDetector> rf);

	int tlx;
	int tly;
private:
	shared_ptr<KNNDetector> referenceFinder;
};

