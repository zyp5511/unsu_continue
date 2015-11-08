#pragma once
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>
#include <ctime>
using namespace std;
using namespace cv;
using namespace Eigen;

class FeatureWriter {
public:
  FeatureWriter(void);
  ~FeatureWriter(void);
  void saveYAML(string fsfn, const Mat &feavec);
  void saveTab(string fname, const Mat &feavec);
  void saveEigen2Tab(string fname, const MatrixXf &feavec);
};
