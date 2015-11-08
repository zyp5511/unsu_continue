#pragma once
#include <opencv2/opencv.hpp>
#include "Result.h"

using namespace std;
using namespace cv;

class Feature {
public:
  Feature(void);
  Feature(const vector<float> &aVec) { vec = aVec; };
  Feature(Mat &patch);
  Feature(Mat &patch, const PCA &pca);

  ~Feature(void);
  int getCategory() { return res.category; };
  float getScore() { return res.score; };
  Result getResult() { return res; }
  float l2(Feature b);
  void toHeadless();

  /*
   * For performance issue, the following members
   * are kept public.
   *
   */
  vector<float> vec; // should be private
  vector<float> orivec; // should be private
  Result res;
  Mat img;

private:
  //    int category=-1;
  //    float score=0;;
  vector<float> process(uchar *im, int &len, const int *dims, const int sbin);
};
