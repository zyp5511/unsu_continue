/*
* LCTransform.h
*
*  Created on: Apr 14, 2013
*      Author: lichao
*/

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include <cstdio>

using namespace std;
using namespace cv;

class LCTransform {
public:
  LCTransform(){};
  LCTransform(float axr, float ayr, float ar, int afrom, int ato)
      : xr(axr), yr(ayr), ratio(ar), from(afrom), to(ato) {}
  LCTransform(string str) {
    sscanf(str.c_str(), "%d=>%d\t%f:%f:%f", &from, &to, &xr, &yr, &ratio);
  }
  virtual ~LCTransform();
  Rect apply(const Rect &from) const {
    Rect res;
    res.x = from.x + xr * from.width;
    res.y = from.y + yr * from.height;
    res.height = from.height * ratio;
    res.width = from.width * ratio;
    return res;
  }
  string getString() {
    return to_string(from) + "=>" + to_string(to) + "\t" + to_string(xr) + ":" +
           to_string(yr) + ":" + to_string(ratio);
  }

  float xr;
  float yr;
  float ratio;
  int from;
  int to;
};

class LCTransformSet {
public:
  LCTransformSet(){};
  LCTransformSet(int n, string filename) {
    transforms = vector<LCTransform>(n);
    vb = vector<bool>(n, false);
    ifstream fin(filename);
    string line;
    while (getline(fin, line)) {
      auto tr = LCTransform(line);
      transforms[tr.from] = tr;
      vb[tr.from] = true;
    }
  }

  Rect apply(int fromidx, const Rect &from) const {
    if (vb[fromidx]) {
      return transforms[fromidx].apply(from);
    } else {
      return Rect();
    }
  }

private:
  vector<LCTransform> transforms;
  vector<bool> vb;
};
#endif /* TRANSFORM_H_ */
