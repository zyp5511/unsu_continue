#pragma once
#include "PatchClassDetector.h"
#include "FeatureLoader.h"
#include <fstream>
#include <memory>
#include <set>

using namespace cv;

class VoronoiDetector : public PatchClassDetector
/*classify patches by comaparing distances from centers of clusters, instead
 of all training samples*/
{
public:
  VoronoiDetector(void) {}
  VoronoiDetector(string vecfname, string clusfname);
  VoronoiDetector(const Mat &featurevec, vector<int> &clusvec)
      : feavec(featurevec), clus(clusvec) {}
  ~VoronoiDetector(void) {}
  virtual void classify(const vector<float> &vec, int &c,
                        float &score) override;
  void load(string fsfn, string clusfn) override; // load feature and indice
                                                  // list
  Mat voronoiCenters();
  pair<Mat, vector<int>> purifiedVoronoiCenters(map<int, set<int>> members);
  pair<vector<float>, vector<int>> diffCenters(map<int, set<int>> members);

protected:
  Mat centers;
  vector<int> clus; // cluster tag
  Mat feavec; // feature index, hold memory
};
