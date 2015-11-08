#include "FeatureWriter.h"
#include <fstream>

FeatureWriter::FeatureWriter(void) {}

FeatureWriter::~FeatureWriter(void) {}

void FeatureWriter::saveYAML(string fsfn, const Mat &feavec) {
  // save to FileStorage
  FileStorage fs(fsfn, FileStorage::WRITE);
  fs << "feature" << feavec;
  fs.release();
}
void FeatureWriter::saveEigen2Tab(string fname, const MatrixXf &feavec) {
  clock_t overall_start = clock();
  ofstream fout(fname);
  auto nr = feavec.rows();
  auto nc = feavec.cols();
  fout << nr << endl;
  fout << nc << endl;

  for (int i = 0; i < nr; i++) {
    auto tempr = feavec.row(i);
    for (int j = 0; j < nc; j++) {
      fout << tempr(j) << "\t";
    }
    fout << "\n"; // I think endl is slow, haven't tested :)
  }
  fout.close();

  double overall_diff = (clock() - overall_start) / (double)CLOCKS_PER_SEC;
  cout << "we use " << overall_diff << " seconds to write file!" << endl;
}

void FeatureWriter::saveTab(string fname, const Mat &feavec) {
  clock_t overall_start = clock();
  ofstream fout(fname);
  fout << feavec.rows << endl;
  fout << feavec.cols << endl;

  for (int i = 0; i < feavec.rows; i++) {
    Mat tempr = feavec.row(i);
    for (auto j = tempr.begin<float>(); j != tempr.end<float>(); j++) {
      fout << *j << "\t";
    }
    fout << "\n"; // I think endl is slow, haven't tested :)
  }
  fout.close();
  double overall_diff = (clock() - overall_start) / (double)CLOCKS_PER_SEC;
  cout << "we use " << overall_diff << " seconds to write file!" << endl;
}
