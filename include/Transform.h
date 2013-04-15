/*
 * LCTransform.h
 *
 *  Created on: Apr 14, 2013
 *      Author: lichao
 */

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include<opencv2/opencv.hpp>
#include<string>


using namespace std;
using namespace cv;

class LCTransform {
	public:
		LCTransform(float axr, float ayr, float ar, int afrom, int ato) :
			xr(axr), yr(ayr), ratio(ar), from(afrom), to(ato) {
			}
		virtual ~LCTransform();
		Rect apply(const Rect& from) {
			Rect res;
			res.x = from.x + xr*from.width;
			res.y = from.y + yr*from.height;
			res.height = from.height * ratio;
			res.width = from.width * ratio;
			return res;
		}
		string getString(){
			return to_string(from)+"=>"+to_string(to)+"\t"+to_string(xr)+":"+to_string(yr)+":"+to_string(ratio);
		}

		float xr;
		float yr;
		float ratio;
		int from;
		int to;
};

#endif /* TRANSFORM_H_ */
