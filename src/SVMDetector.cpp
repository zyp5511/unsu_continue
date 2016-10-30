#include "SVMDetector.h"
#include <fstream>
#include <opencv2/opencv.hpp>
#include <sstream>

using namespace cv;
using namespace cv::ml;
using namespace std;

SVMDetector::SVMDetector(string fname) {
  ifstream fin(fname);
  string line;
  getline(fin, line);

  b = stof(line);

  getline(fin, line);
  stringstream ss(line);
  string item;
  for (int i = 0; i < vlen; i++) {
    getline(ss, item, ',');
    svm[i] = stof(item);
  }
  fin.close();
}

SVMDetector::SVMDetector(const Mat &feavec, const vector<int> &label) {
  classifier = SVM::create();
  classifier->setType(SVM::C_SVC);
  classifier->setKernel(SVM::LINEAR);
  classifier->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

  Mat labelsMat(label);
  // Train the SVM
  // TODO: Maybe this is COL_SAMPLE
  classifier->train(TrainData::create(feavec, ROW_SAMPLE, labelsMat));
}

void SVMDetector::detect(const vector<float> &vec, int &cat, float &score,
                         bool &accepted) {
  score = classifier->predict(Mat(vec).t());
  cat = score > 0 ? 1 : -1;
  accepted = score > 0 ? true : false;
}
