//
//  distance.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 09/26/13
//  Copyright (c) 2013 Lichao Chen. All rights reserved.
//

#include <eigen3/Eigen/Dense>
#include <iostream>
#include <opencv2/opencv.hpp>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "Feature.h"
#include "FeatureLoader.h"
#include "FeatureWriter.h"

using namespace std;
using namespace cv;
using namespace Eigen;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

#pragma mark main

int main(int argc, const char *argv[]) {

  // default value
  string pcafn;
  string config;
  string fromfn;
  string tofn;
  int k;

  po::options_description desc("General options");
  po::options_description detectdesc("Detection options");

  desc.add_options()("help", "produce help message")(
      "configuration,K", po::value<string>(&config), "configuration file")(
      "from,F", po::value<string>(&fromfn),
      "from image file")("to,T", po::value<string>(&tofn), "to image file")(
      "PCA,P", po::value<string>(&pcafn), "set PCA file");

  po::variables_map vm;

  po::store(po::parse_command_line(argc, argv, desc), vm);

  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  } else if (vm.count("configuration")) {
    cout << config << endl;
    ifstream fconf(config);
    po::store(po::parse_config_file(fconf, desc), vm);
    po::notify(vm);
    fconf.close();
  }
  FileStorage pcafs(pcafn, FileStorage::READ);
  PCA pca;
  pcafs["mean"] >> pca.mean;
  pcafs["eigenvalues"] >> pca.eigenvalues;
  pcafs["eigenvectors"] >> pca.eigenvectors;
  cout << "PCA loaded" << endl;

  Mat from = imread(fromfn);
  Mat to = imread(tofn);

  Feature fea_from(from, pca);
  Feature fea_to(to, pca);
  cout << "l2 distance is " << fea_from.l2(fea_to) << endl;
}
