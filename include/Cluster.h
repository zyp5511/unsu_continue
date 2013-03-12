/*
 * Cluster.h
 *
 *  Created on: Feb 25, 2013
 *      Author: lichao
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Cluster {
public:
	static vector<Cluster> makeClusters(const Mat& feavecs, const vector<int>& ind, int k);
	Cluster();
	Cluster(const Mat& feavec);
	virtual ~Cluster();
	float getAvgDistance();
	float getMaxDistance();
	float getMinDistance();

private:
	void init();
	float avgDistance = -1;
	float maxDistance = -1;
	float minDistance = -1;
	Mat feavec;

};

#endif /* CLUSTER_H_ */
