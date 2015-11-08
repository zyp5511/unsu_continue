#pragma once

#include <opencv2/opencv.hpp>
#include <fstream>
#include <ctime>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace cv;
using namespace Eigen;

class FeatureLoader {
public:
  FeatureLoader(void);
  ~FeatureLoader(void);

  Mat loadYAML(string fsfn); // load feature vecs from YAML, small
  Mat loadBigYAML(string fsfn); // load feature vecs from YAML, big
  Mat loadTab(string fn);
  void loadTab2Eigen(string fn, MatrixXf &feavec);
};
