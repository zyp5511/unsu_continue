#pragma once
#include "PatchDetector.h"
#include <opencv2/ml.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;
const int vlen = 4480;
class SVMDetector : public PatchDetector {
public:
  SVMDetector(string fname);
  virtual ~SVMDetector(void){};
  SVMDetector(const Mat &feavec, const vector<int> &label);
  virtual void detect(const vector<float> &vec, int &c, float &score,
                      bool &accepted) override;

private:
  Ptr<SVM> classifier;
  bool load(string fname);
  float b; // constant term in svm
  float svm[vlen]; // svm coeff;
};
