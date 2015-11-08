#include "FeatureLoader.h"

FeatureLoader::FeatureLoader(void) {}

FeatureLoader::~FeatureLoader(void) {}
void FeatureLoader::loadTab2Eigen(string fn, MatrixXf &feavec) {
  clock_t overall_start = clock();
  int mr, mc;
  ifstream fin(fn);
  fin >> mr;
  fin >> mc;
  feavec = MatrixXf(mr, mc);
  for (size_t i = 0; i < mr; i++) {

#ifdef FAST_TAB_LOAD
    string line;
    getline(fin, line);
    istringstream iss(line);
    for (size_t j = 0; j < mc; j++) {
      string item;
      iss >> feavec(i, j);
    }
#else
    for (size_t j = 0; j < mc; j++) {
      fin >> feavec(i, j);
    }
#endif
  }
  fin.close();
  double overall_diff = (clock() - overall_start) / (double)CLOCKS_PER_SEC;
  cout << "we use " << overall_diff << " seconds to load file!" << endl;
}

Mat FeatureLoader::loadTab(string fn) {
  clock_t overall_start = clock();
  int mr, mc;
  FILE *fp;
  fp = fopen(fn.c_str(), "r");
  // ifstream fin(fn);
  // fin>>mr;
  // fin>>mc;
  fscanf(fp, " %d ", &mr);
  fscanf(fp, " %d ", &mc);

  Mat feavec(mr, mc, CV_32F);
  for (size_t i = 0; i < mr; i++) {
    for (size_t j = 0; j < mc; j++) {
      // fin>>feavec.at<float>(i,j);
      float temp;
      fscanf(fp, " %f ", &temp);
      feavec.at<float>(i, j) = temp;
    }
  }
  fclose(fp);
  // fin.close();
  double overall_diff = (clock() - overall_start) / (double)CLOCKS_PER_SEC;
  cout << "we use " << overall_diff << " seconds to load file!" << endl;
  return feavec;
}

Mat FeatureLoader::loadYAML(string fsfn) {
  FileStorage fs;
  cout << "opening " << fsfn << endl;
  fs.open(fsfn, FileStorage::READ);
  cout << "loading feature matrix" << endl;
  Mat feavec;
  fs["feature"] >> feavec;
  fs.release();
  return feavec;
}

Mat FeatureLoader::loadBigYAML(string fsfn) {
  ifstream fin(fsfn);
  string line;
  getline(fin, line);
  cout << line << endl;
  getline(fin, line);
  cout << line << endl;
  int mr, mc;
  string k;
  fin >> k;
  fin >> mr;
  cout << "there are " << mr << " rows" << endl;
  fin >> k;
  fin >> mc;
  cout << "there are " << mc << " clos" << endl;
  fin >> k;
  fin >> k;
  fin >> k;
  fin >> k;
  int count = 5;
  Mat feavec = Mat(mr, mc, CV_32F); // Memory should be released by Index
  for (int i = 0; i < mr; i++) {
    if (!(i % 10000)) {
      cout << "have read " << i << " lines" << endl;
    }
    for (int j = 0; j < mc; j++) {
      string val;
      fin >> val;
      float fval = stof(val);
      if (count > 0 || (i == mr - 1 && j > mc - 5)) {
        cout << val << "\t" << fval << endl;
        count--;
      }
      feavec.at<float>(i, j) = fval;
    }
  }
  cout << "feature loaded" << endl;

  // close file
  fin.close();
  return feavec;
}
