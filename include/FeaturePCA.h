#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Eigenvalues>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>

using namespace Eigen;
using namespace std;
using namespace cv;

class FeaturePCA {
public:
  RowVectorXf mean;
  VectorXf el;
  MatrixXf ev;

  FeaturePCA(MatrixXf &fea, float retainedVar);
  void projectZeroMean(MatrixXf &ori, MatrixXf &shorten);
  void project(MatrixXf &ori, MatrixXf &shorten);
  void backProject(MatrixXf &shorten, MatrixXf &ori);
  PCA getCVPCA();
};
