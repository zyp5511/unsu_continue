#include "VoronoiDetector.h"

VoronoiDetector::VoronoiDetector(string vecfn, string clusfn) {
  load(vecfn, clusfn);
}

void VoronoiDetector::load(string vecfn, string clusfn) {
  // Load vector matrix
  auto fl = FeatureLoader();
  feavec = fl.loadTab(vecfn);
  int in_count = feavec.rows;

  // Load cluster index
  ifstream fin(clusfn);
  int cmax = 0;
  clus = vector<int>(in_count);
  for (int i = 0; i < in_count; i++) {
    fin >> clus[i];
    if (clus[i] > cmax)
      cmax = clus[i];
  }
  fin.close();

  centers = Mat(cmax + 1, feavec.cols, CV_32F, Scalar(0));
  vector<int> vec_count(cmax + 1, 0);

  for (int i = 0; i < in_count; i++) {
    centers.row(clus[i]) += feavec.row(i);
    vec_count[clus[i]]++;
  }
  for (int i = 0; i < cmax + 1; i++) {
    centers.row(i) /= vec_count[i];
  }
}

Mat VoronoiDetector::voronoiCenters() { return centers; }
pair<Mat, vector<int>>
VoronoiDetector::purifiedVoronoiCenters(map<int, set<int>> members) {
  Mat pcenters = Mat(centers.rows, feavec.cols, CV_32F, Scalar(0));
  vector<int> vec_count(centers.rows, 0);

  for (int i = 0; i < feavec.rows; i++) {
    if (members[clus[i]].find(i) != members[clus[i]].end()) {
      pcenters.row(clus[i]) += feavec.row(i);
      vec_count[clus[i]]++;
    }
  }
  for (int i = 0; i < pcenters.rows; i++) {
    if (vec_count[i] != 0) {
      pcenters.row(i) /= vec_count[i];
    }
  }
  return make_pair(pcenters, vec_count);
}
pair<vector<float>, vector<int>>
VoronoiDetector::diffCenters(map<int, set<int>> members) {
  auto apair = purifiedVoronoiCenters(members);
  Mat pcenters = apair.first;
  vector<int> pcount = apair.second;
  vector<float> res(pcenters.rows, 0);
  for (int i = 0; i < pcenters.rows; i++) {
    if (pcount[i] != 0) {
      res[i] = cv::norm(pcenters.row(i), centers.row(i));
    }
  }
  return make_pair(res, pcount);
}
void VoronoiDetector::classify(const vector<float> &vec, int &c, float &score) {
  int n = 5;
  c = -1;
  vector<int> ind(n);
  vector<float> dis(n, FLT_MAX);
  // fill(dis.begin(),dis.end(),FLT_MAX);
  auto len = centers.rows;
  for (int i = 0; i < len; i++) {
    auto temp = norm(centers.row(i), vec);
    int j = n;
    while (j > 0 && temp < dis[j - 1]) {
      if (j < n) {
        dis[j] = dis[j - 1];
      }
      j--;
    }
    if (j < n) {
      dis[j] = temp;
      ind[j] = i;
    }
  }
  if (dis[0] < 12) {
    c = ind[0];
    score = dis[0]; // not good.. not real distance.
  } else {
    c = -1;
  }
}
