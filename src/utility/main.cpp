//
//  main.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 11/26/12.
//  Copyright (c) 2012 Lichao Chen. All rights reserved.
//

#include <ctime>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _WIN32
#else
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "ExhaustiveCropper.h"
#include "RandomCropper.h"

#include "Cluster.h"
#include "Feature.h"
#include "FeatureLoader.h"
#include "FeaturePCA.h"
#include "FeaturePartitioner.h"
#include "FeatureWriter.h"
#include "Image.h"
#include "Transform.h"

#include "HoGAlignmentDetector.h"
#include "KNNDetector.h"
#include "LatentDetector.h"
#include "TwoStageDetector.h"
#include "VoronoiDetector.h"

using namespace std;
using namespace cv;
using namespace Eigen;

namespace fs = boost::filesystem;
namespace po = boost::program_options;
namespace al = boost::algorithm;

using std::cout;

vector<string> loadFolder(string srcfolder, string prefix);

void classify(shared_ptr<PatchDetector> kd, shared_ptr<ExhaustiveCropper> ec,
              string srcfolder, string desfolder, int k, PCA &pca, string s,
              const vector<bool> &gc, const vector<bool> &core_gc,
              ostream &fout, const LCTransformSet &ts,
              const po::variables_map &vm);

vector<bool> buildGameCard(string gcfn, int k) {
  auto res = vector<bool>(k, false);
  ifstream fin(gcfn);
  istream_iterator<int> eos;
  istream_iterator<int> iit(fin);
  while (iit != eos) {
    res[*iit] = true;
    iit++;
  }
  return res;
}
#pragma mark main

int main(int argc, const char *argv[]) {

  // default value
  string srcfolder;
  string desfolder;
  string fsfn;
  string indfn;
  string oper;
  string pcafn;
  string portn;
  string gcfn;
  string coregcfn;
  string transfn;
  string vecoutfn;
  string auxfn;
  string config;
  string prefix;
  int k;

  po::options_description desc("General options");
  po::options_description cropdesc("Patch cropping options");
  po::options_description detectdesc("Detection options");
  po::options_description transdesc("Transform options");
  po::options_description kmeansdesc("Kmeans clustering options");
  po::options_description cvdesc("OpenCV stock classifier options");
  po::options_description casdesc("Cascading classifier options");
  po::options_description distdesc("Distributed Computing options");
  po::options_description voronoidesc("kmeans class purification options");

  desc.add_options()("help", "produce help message")(
      "config,K", po::value<string>(&config), "configuration file")(
      "cluster,C", po::value<int>(&k), "set Number of Clusters")(
      "operation,O", po::value<string>(&oper),
      "set operation")("batch,B", "set batch/single")("daemon", "set daemon")(
      "list", po::value<string>(), "set file list to scan")(
      "export-feature", "Export feature vector instead of rect position")(
      "co-occurrence", "set co-occurrence rule detection")(
      "aux-result,A", po::value<string>(&auxfn),
      "set aux result file")("port", po::value<string>(&portn), "set port");

  kmeansdesc.add_options()("maxiter", po::value<int>(),
                           "max iterations of kmeans")(
      "eps", po::value<double>(), "desired precision of kmeans");

  distdesc.add_options()("paroffset", po::value<int>()->default_value(0),
                         "set parallel offset")(
      "parstride", po::value<int>()->default_value(0),
      "set parallel stride")("parallel", "set parallel or not");

  cropdesc.add_options()("height", po::value<int>()->default_value(128),
                         "set patch height")(
      "maxheight", po::value<int>()->default_value(800),
      "set max image height")("width", po::value<int>()->default_value(96),
                              "set patch width")(
      "stride", po::value<int>()->default_value(32), "set cropping stride")(
      "patch-per-image", po::value<int>()->default_value(10),
      "set cropping density")("onelevel", "set prymaid or not");

  detectdesc.add_options()("src,S", po::value<string>(&srcfolder),
                           "set source folder")(
      "des,D", po::value<string>(&desfolder), "set destination folder")(
      "feature,F", po::value<string>(&fsfn), "set feature file")(
      "index,I", po::value<string>(&indfn), "set index file")(
      "result,R", po::value<string>(&vecoutfn), "set result file")(
      "gamecard", po::value<string>(&gcfn),
      "set gamecard file")("prefix", po::value<string>(),
                           "set filename prefix for task distribution")(
      "PCA,P", po::value<string>(&pcafn), "set PCA file");

  transdesc.add_options()("corecard", po::value<string>(&coregcfn),
                          "set core gamecard file")(
      "transform", po::value<string>(&transfn), "set transform file");
  cvdesc.add_options()("model-file", po::value<string>(),
                       "set CV classifier model");
  casdesc.add_options()("2ndfeature", po::value<string>(),
                        "set 2nd feature file")("2ndindex", po::value<string>(),
                                                "set 2nd index file")(
      "2ndgamecard", po::value<string>(), "set 2nd gamecard file")(
      "2ndcluster", po::value<int>(), "set 2nd number of clusters")(
      "2ndPCA", po::value<string>(), "set 2nd PCA file");

  voronoidesc.add_options()("members", po::value<string>(),
                            "left nodes and membership file");

  desc.add(distdesc)
      .add(cropdesc)
      .add(detectdesc)
      .add(transdesc)
      .add(cvdesc)
      .add(casdesc)
      .add(kmeansdesc)
      .add(voronoidesc);

  po::variables_map vm;

  po::store(po::parse_command_line(argc, argv, desc), vm);

  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  } else if (vm.count("config")) {
    cout << config << endl;
    ifstream fconf(config);
    po::store(po::parse_config_file(fconf, desc), vm);
    po::notify(vm);
    fconf.close();
  }
  if (vm.count("des") && !fs::is_directory(desfolder)) {
    cout << "creating destination folder..." << endl;
    fs::create_directory(desfolder);
  }

  if (vm.count("prefix")) {
    cout << "The filename prefix is " << vm["prefix"].as<string>() << endl;
    prefix = vm["prefix"].as<string>();
  } else {
    prefix = "";
  }

  clock_t overall_start = clock();

  if (oper == "clusteranalysis") {
    auto fl = FeatureLoader();
    auto feavec = fl.loadTab(fsfn);
    ifstream fin(indfn);
    int numvec = feavec.rows;
    vector<int> ind(numvec);
    for (int i = 0; i < numvec; i++) {
      fin >> ind[i];
    }
    fin.close();

    vector<Cluster> clus = Cluster::makeClusters(feavec, ind, k);
    ofstream fout(vecoutfn);
    ofstream fout2(auxfn);
    fout << "Min\tMax\tAvg\tRadius\tCenter" << endl;
    for (int i = 0; i < k; i++) {
      float mindis = clus[i].getMinDistance();
      float maxdis = clus[i].getMaxDistance();
      float avgdis = clus[i].getAvgDistance();
      float avgr = clus[i].getAvgRadius();
      float center = clus[i].getCenterNorm();
      fout << mindis << "\t" << maxdis << "\t" << avgdis << "\t" << avgr << "\t"
           << center << endl;
    }
    fout.close();
    for (size_t i = 0; i < k; i++) {
      for (size_t j = 0; j < k - 1; j++) {
        fout2 << clus[i].distance(clus[j]) << ",";
      }
      fout2 << clus[i].distance(clus[k - 1]) << endl;
    }
    fout2.close();

  } else if (oper == "cos-network") {
    // construct a network by evaluating l2 btw all pair
    auto fl = FeatureLoader();
    MatrixXf fea;
    fl.loadTab2Eigen(fsfn, fea);
    fea.transposeInPlace();

    int start = 0;
    int end = fea.cols();
    int stride = 0;
    int overallEnd = fea.cols();
    cout << "there are " << fea.rows() << " rows" << endl;
    cout << "there are " << fea.cols() << " cols" << endl;
    if (vm.count("parallel")) {
      cout << "Parallel mode started:" << endl;
      start = vm["paroffset"].as<int>();
      stride = vm["parstride"].as<int>();
      end = (start + stride) > fea.cols() ? fea.cols() : (start + stride);
    } else {
      cout << "Test Mode started:" << endl;
    }
    cout << "start calculating from " << start << " to " << end << endl;
    ofstream fout(vecoutfn);

    for (int i = start; i < end; i++) {
      auto inorm = fea.col(i).norm();
      auto nume =
          fea.col(i).transpose() * fea.block(0, i, fea.rows(), overallEnd - i);
      auto deno =
          fea.block(0, i, fea.rows(), overallEnd - i).colwise().norm() * inorm;
      auto temp = nume.cwiseQuotient(deno);
      fout << temp << endl;
    }
    fout.close(); // use vecoutfn as the destination.
  } else if (oper == "network") {
    // construct a network by evaluating l2 btw all pair
    auto fl = FeatureLoader();
    MatrixXf fea;
    fl.loadTab2Eigen(fsfn, fea);
    fea.transposeInPlace();

    int start = 0;
    int end = fea.cols();
    int stride = 0;
    int overallEnd = fea.cols();
    cout << "there are " << fea.rows() << " rows" << endl;
    cout << "there are " << fea.cols() << " cols" << endl;
    if (vm.count("parallel")) {
      cout << "Parallel mode started:" << endl;
      start = vm["paroffset"].as<int>();
      stride = vm["parstride"].as<int>();
      end = (start + stride) > fea.cols() ? fea.cols() : (start + stride);
    } else {
      cout << "Test Mode started:" << endl;
    }
    cout << "start calculating from " << start << " to " << end << endl;
    ofstream fout(vecoutfn);

    for (int i = start; i < end; i++) {
      auto temp =
          (fea.block(0, i, fea.rows(), overallEnd - i).colwise() - fea.col(i))
              .colwise()
              .norm();
      fout << temp << endl;
    }
    fout.close(); // use vecoutfn as the destination.
  } else if (oper == "kmeans" || oper == "kmean" // historical typo
  ) {
    int mi = 1;
    int ep = 0;
    int mode = 0;
    if (vm.count("maxiter")) {
      mode += TermCriteria::MAX_ITER;
      mi = vm["maxiter"].as<int>();
      cout << "max iteration specified" << endl;
    }

    if (vm.count("eps")) {
      mode += TermCriteria::EPS;
      ep = vm["eps"].as<double>();
      cout << "epsilon specified" << endl;
    }
    auto fp = FeaturePartitioner();
    auto fl = FeatureLoader();
    auto fea = fl.loadTab(fsfn);
    vector<int> category(fea.rows);
    fp.kmean(fea, category, k, TermCriteria(mode, mi, ep));

    ofstream fout(indfn);
    for (auto i : category) {
      fout << i << "\n";
    }
    fout.close(); // use indfn as the destination.
  } else if (oper == "pca") {
    auto fl = FeatureLoader();
    MatrixXf fea;
    fl.loadTab2Eigen(fsfn, fea);
    auto pca = FeaturePCA(fea, 0.95);
    cout << "there are " << pca.el.size() << " components in PCA\n";
    auto a = pca.getCVPCA();
    MatrixXf shortfea;

    pca.projectZeroMean(fea, shortfea);
    FileStorage fs(pcafn, FileStorage::WRITE);
    fs << "mean" << a.mean;
    fs << "eigenvalues" << a.eigenvalues;
    fs << "eigenvectors" << a.eigenvectors;
    fs.release();
    cout << "eigenvalue written in " << pcafn << endl;
    auto fw = FeatureWriter();
    fw.saveEigen2Tab(vecoutfn, shortfea);
  } else if (oper == "randomcrop") {
    // set up patch cropper
    string seperator_fn = vecoutfn;
    int h = vm["height"].as<int>();
    int w = vm["width"].as<int>();
    int ppi = vm["patch-per-image"].as<int>();
    cout << "we are cropping " << ppi << " patches per image" << endl;
    cout << "the height is " << h << "\t the width is " << w << endl;
    auto nc = RandomCropper(ppi);
    if (vm.count("onelevel")) {
      cout << " we are using one-level cropping approach" << endl;
      nc.setPrymaid(false);
    } else {
      nc.setPrymaid(true);
    }
    nc.setSize(h, w);
    nc.collectSrcDir(srcfolder);
    cout << "Patches created!" << endl;
    nc.exportFeatures(fsfn);
    nc.exportPatches(desfolder);
    nc.exportSeperators(seperator_fn);
  } else if (oper == "latent") {
    // set up patch cropper
    shared_ptr<LatentDetector> kd;
    shared_ptr<ExhaustiveCropper> ec(new ExhaustiveCropper());
    ec->setSize(128, 96);
    ec->setStride(vm["stride"].as<int>());

    FileStorage pcafs(pcafn, FileStorage::READ);
    PCA pca;
    pcafs["mean"] >> pca.mean;
    pcafs["eigenvalues"] >> pca.eigenvalues;
    pcafs["eigenvectors"] >> pca.eigenvectors;
    cout << "PCA loaded" << endl;

    vector<bool> gc = buildGameCard(gcfn, k);

    cout << "start loading index" << endl;
    kd = make_shared<LatentDetector>(fsfn, indfn, gc);

    string name;
    cout << "Please input image filename" << endl;

    while (getline(cin, name)) {
      try {
        // Read Image
        auto fname = srcfolder + name + ".jpg";
        Mat mat = imread(fname);
        cout << "loading file " << fname << endl;
        ImageWrapper iw(kd, ec);

        // Prepare Image Wrapper
        iw.setImage(mat);
        iw.setBins(k);

        iw.collectPatches();          // cropping
        iw.collectResult(pca, false); // kNN matching

        Scalar colors[] = {Scalar(255, 0, 0), Scalar(0, 255, 0),
                           Scalar(0, 0, 255), Scalar(0, 255, 255)};
        Mat out = mat.clone();
        vector<Result> debugs = iw.getBestResults(10);
        size_t dsize = debugs.size();
        rectangle(out, debugs[0].rect, colors[1]);
        for (size_t i = 1; i < dsize; i++) {
          rectangle(out, debugs[i].rect, colors[0]);
        }
        imwrite(desfolder + name + ".jpg", out);
      } catch (Exception &e) {
        cerr << e.msg << endl;
      }
      cout << "Please input image filename" << endl;
    }
  } else if (oper == "opencv") {
    string modelname = vm["model-file"].as<string>();
    ofstream fout(vecoutfn);
    ImageWrapper iw;
    vector<string> files;
    if (vm.count("list")) {
      string listfn = vm["list"].as<string>();
      ifstream listfin(listfn);
      string line;
      while (getline(listfin, line))
        files.push_back(line);
      listfin.close();
    } else {
      files = loadFolder(srcfolder, prefix);
    }
    iw.loadCVModel(modelname);
    for (auto &s : files) {
      float ratio = 1;
      try {
        auto fname = srcfolder + s;
        Mat raw = imread(fname);
        Mat mat;
        cout << fname << "\t" << raw.size() << endl;
        if (raw.rows > 500) {
          ratio = 500. / raw.rows;
          resize(raw, mat, Size(), ratio, ratio);
          cout << "resized to \t" << mat.size() << endl;
        } else {
          mat = raw;
        }
        iw.setImage(mat);
        auto result = iw.getocvresult();

        if (result.size() > 0) {
          cout << fname << " matched!" << endl;
          fout << s << endl;
          Mat out = mat.clone();
          for (const Rect &r : result) {
            fout << (int)(r.x / ratio) << ":" << (int)(r.y / ratio) << ":"
                 << (int)(r.width / ratio) << ":" << (int)(r.height / ratio)
                 << endl;
            rectangle(out, r, Scalar(255, 0, 0));
          }
          imwrite(desfolder + s, out);
        }
      } catch (Exception &e) {
        cerr << e.msg << endl;
      }
    }
    fout.close();
  } else if (oper == "knn" || oper == "voronoi" || oper == "cascading") {
    string name;
    shared_ptr<PatchDetector> kd;

    vector<bool> gc = vector<bool>();
    if (vm.count("gamecard")) {
      gc = buildGameCard(gcfn, k);
    }

    if (oper == "knn") {
      auto temp = make_shared<KNNDetector>();
      temp->load(fsfn, indfn);
      temp->loadGC(gc);
      kd = temp;
    } else if (oper == "voronoi") {
      auto temp = make_shared<VoronoiDetector>();
      temp->load(fsfn, indfn);
      temp->loadGC(gc);
      kd = temp;

    } else if (oper == "cascading") {
      shared_ptr<PatchClassDetector> kdfirst = make_shared<VoronoiDetector>();
      shared_ptr<PatchClassDetector> kdsecond = make_shared<KNNDetector>();
      shared_ptr<HoGAlignmentDetector> kdthird =
          make_shared<HoGAlignmentDetector>();
      kdfirst->load(fsfn, indfn);
      kdfirst->loadGC(gc);
      kdsecond->load(fsfn, indfn);
      kdsecond->loadGC(gc);

      kd = make_shared<TwoStageDetector>(kdfirst, kdsecond);
    } else if (oper == "3cascading") {
      shared_ptr<PatchClassDetector> kdfirst = make_shared<VoronoiDetector>();
      shared_ptr<PatchClassDetector> kdsecond = make_shared<KNNDetector>();
      shared_ptr<HoGAlignmentDetector> kdthird =
          make_shared<HoGAlignmentDetector>();
      kdfirst->load(fsfn, indfn);
      kdfirst->loadGC(gc);
      kdsecond->load(fsfn, indfn);
      kdsecond->loadGC(gc);

      shared_ptr<KNNDetector> rf = make_shared<KNNDetector>();
      rf->load(vm["2ndfeature"].as<string>(), vm["2ndindex"].as<string>());
      vector<bool> thirdgc = buildGameCard(vm["2ndgamecard"].as<string>(),
                                           vm["2ndcluster"].as<int>());
      rf->loadGC(thirdgc);
      kdthird->setFinder(rf);
      FileStorage pcafs(vm["2ndPCA"].as<string>(), FileStorage::READ);
      PCA thirdpca;
      pcafs["mean"] >> thirdpca.mean;
      pcafs["eigenvalues"] >> thirdpca.eigenvalues;
      pcafs["eigenvectors"] >> thirdpca.eigenvectors;
      kdthird->setPCA(thirdpca);
      kd = make_shared<TwoStageDetector>(kdfirst, kdsecond, kdthird);
    }
    shared_ptr<ExhaustiveCropper> ec(new ExhaustiveCropper());
    int h = vm["height"].as<int>();
    int w = vm["width"].as<int>();
    int ss = vm["stride"].as<int>();
    ec->setSize(h, w);
    ec->setStride(ss);

    FileStorage pcafs(pcafn, FileStorage::READ);
    PCA pca;
    pcafs["mean"] >> pca.mean;
    pcafs["eigenvalues"] >> pca.eigenvalues;
    pcafs["eigenvectors"] >> pca.eigenvectors;
    cout << "PCA loaded" << endl;

    cout << "start loading index" << endl;

    // load standard head clusters, for rule observation,
    // and rule applying
    vector<bool> coregc = vector<bool>();
    if (vm.count("corecard")) {
      coregc = buildGameCard(coregcfn, k);
    }

    // load geometric transfrom rules
    LCTransformSet ts;
    if (vm.count("transform")) {
      ts = LCTransformSet(k, transfn);
    }

    if (vm.count("daemon")) {
#ifndef _WIN32
      // set up socket
      int sockfd, newsockfd, portno;
      socklen_t clilen;
      char buffer[256];
      struct sockaddr_in serv_addr, cli_addr;
      int n;
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) {
        fprintf(stderr, "ERROR opening socket");
        exit(1);
      }
      bzero((char *)&serv_addr, sizeof(serv_addr));
      portno = stoi(portn);
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      serv_addr.sin_port = htons(portno);
      bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
      // if (bind(sockfd, (struct sockaddr *) &serv_addr,
      // sizeof(serv_addr))<0) {
      //	fprintf(stderr, "ERROR on binding");
      //	exit(1);
      //}
      cout << "socket server created at " << portno << endl;
      listen(sockfd, 1024);
      clilen = sizeof(cli_addr);

      string name;
      cout << "Waiting for detection request" << endl;

      newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
      if (newsockfd < 0) {
        cerr << ("ERROR on accept") << endl;
        return -1;
      }
      bzero(buffer, 256);

      n = read(newsockfd, buffer, 255);
      if (n < 0) {
        cerr << ("ERROR reading from socket") << endl;
        return -1;
      }

      while (n >= 0) {
        stringstream iss(buffer);
        getline(iss, name);
        printf("Here is the message: %s\n", buffer);
        ostringstream ss;
        try {
          classify(kd, ec, srcfolder, desfolder, k, pca, name, gc, coregc, ss,
                   ts, vm);
        } catch (Exception &e) {
          cerr << e.msg << endl;
        }
        n = write(newsockfd, ss.str().c_str(), ss.str().size());
        if (n < 0) {
          cerr << ("ERROR writing to socket") << endl;
          return -1;
        }
        close(newsockfd);
        cout << "Waiting for detection request" << endl;
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
          cerr << ("ERROR on accept") << endl;
          return -1;
        }
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
      }
      close(sockfd);
#endif
    } else { // end if daemon
      ofstream fout(vecoutfn);
      if (vm.count("batch")) {
        vector<string> files;
        if (vm.count("list")) {
          // parse all files in the list.
          string listfn = vm["list"].as<string>();
          ifstream listfin(listfn);
          string line;
          while (getline(listfin, line))
            files.push_back(line);
          listfin.close();
        } else {
          // parse all files in the folder.
          files = loadFolder(srcfolder, prefix);
        }

        for (auto &s : files) {
          classify(kd, ec, srcfolder, desfolder, k, pca, s, gc, coregc, fout,
                   ts, vm);
        }
      } else {
        cout << "Please input image filename" << endl;
        while (getline(cin, name)) {
          try {
            classify(kd, ec, srcfolder, desfolder, k, pca, name, gc, coregc,
                     fout, ts, vm);
          } catch (Exception &e) {
            cerr << e.msg << endl;
          }
          cout << "Please input image filename" << endl;
        }
      }
      fout.close();
    }
  } else if (oper == "clusterpurify") {
    if (!vm.count("members")) {
      cerr << "members parameter not specified" << endl;
      return -1;
    }
    auto memfn = vm["members"].as<string>();
    ifstream memfin(memfn);
    string line;
    map<int, set<int>> members;
    while (getline(memfin, line)) {
      stringstream ss(line);
      int clu;
      int idx;
      ss >> clu;
      ss >> idx;
      // cout<<clu<<"\t:\t"<<idx<<"\t inserted"<<endl;
      members[clu].insert(idx);
    }
    memfin.close();

    auto temp = make_shared<VoronoiDetector>();
    temp->load(fsfn, indfn);
    auto res = temp->diffCenters(members);
    for (size_t i = 0; i < res.first.size(); i++)
      cout << i << "\t" << res.first[i] << "\t" << res.second[i] << endl;
  };
  double overall_diff = (clock() - overall_start) / (double)CLOCKS_PER_SEC;
  cout << "FINISHED!  " << overall_diff << " seconds used." << endl;
  return 0;
}

void classify(shared_ptr<PatchDetector> kd, shared_ptr<ExhaustiveCropper> ec,
              string srcfolder, string desfolder, int k, PCA &pca, string s,
              const vector<bool> &gc, const vector<bool> &core_gc,
              ostream &fout, const LCTransformSet &ts,
              const po::variables_map &vm) {
  auto fname = srcfolder + s;
  ImageWrapper iw(kd, ec);
  Mat raw = imread(fname);
  Mat mat;
  cout << fname << "\t" << raw.size() << endl;
  float ratio = 1;
  int maxheight = vm["maxheight"].as<int>();
  if (raw.rows > maxheight) {
    ratio = (maxheight + 0.0) / raw.rows;
    resize(raw, mat, Size(), ratio, ratio);
    cout << "resized to \t" << mat.size() << endl;
  } else {
    mat = raw;
  }

  iw.setImage(mat);
  iw.setBins(k);
  iw.collectPatches();
  bool feat_vec_collected = vm.count("export-feature") && !core_gc.empty();
  iw.collectResult(pca, feat_vec_collected);
  if(true){
    iw.export_Patches("/home/yipeng/Desktop/project/validation_build/EC_patches/");

  }
  // Calculating viewlet histogram.
  iw.calcClusHist();
  // vector<int> vec = iw.histogram;

  // fout << s << endl;
  // fout << "vector:\t";
  // for (int i = 0; i < k - 1; i++) {
  //  fout << vec[i] << ",";
  //}
  // fout << vec[k - 1] << endl;

  if (!feat_vec_collected) {
    // Export rect from core_gc viewlets.
    auto goodRes = iw.getGoodResults();
    fout << s << endl;
    for (const Result &r : goodRes) {
      fout << r.category << "\t" << r.score << "\t";
      fout << (int)(r.rect.x / ratio) << ":" << (int)(r.rect.y / ratio) << ":"
           << (int)(r.rect.width / ratio) << ":" << (int)(r.rect.height / ratio)
           <<"\t"<<r.neighborResponses << "\t"<< r.dists<<
           endl;
    }
  }

  Scalar colors[] = {
      Scalar(128, 0, 0),   Scalar(0, 128, 0),     Scalar(0, 0, 128),
      Scalar(255, 0, 0),   Scalar(0, 255, 0),     Scalar(0, 0, 255),
      Scalar(0, 255, 255), Scalar(255, 0, 255),   Scalar(255, 255, 0),
      Scalar(0, 128, 128), Scalar(128, 0, 128),   Scalar(128, 128, 0),
      Scalar(64, 64, 64),  Scalar(128, 128, 128), Scalar(255, 255, 255)};

  // record transformations from head related patches to standard head patches
  // (deprecated): using ruby script instead.
  // if (vm.count("co-occurrence") && !core_gc.empty()) {
  //  vector<LCTransform> trans = iw.getLCTransforms(gc, core_gc);
  //  for (LCTransform &t : trans) {
  //    fout << t.getString() << endl;
  //  }
  //}

  // for matching using active set(game card) and transforms to target set(core
  // card )
  if (vm.count("transform") && !gc.empty() && iw.match(gc)) {
    cout << fname << " matched!" << endl;
    Mat out = mat.clone();
    Mat inferred_out;
    if (!core_gc.empty()) {
      inferred_out = mat.clone();
    }
    vector<vector<Result>> res_decks = iw.getMatchedResults(gc);
    for (auto &rs : res_decks) {
      for (auto &r : rs) {
        rectangle(out, r.rect, colors[r.category % 15]);
        if (!core_gc.empty()) {
          if (!core_gc[r.category])
            rectangle(inferred_out, ts.apply(r.category, r.rect),
                      colors[r.category % 15]);
          else
            rectangle(inferred_out, r.rect, colors[r.category % 15]);
        }
      }
    }
    imwrite(desfolder + "detected_" + s, out);
    imwrite(desfolder + "inferred_" + s, inferred_out);
  } else if (feat_vec_collected) {
    // Export feature vectors from core_gc viewlets.
    Mat out = mat.clone();
    vector<vector<Result>> res_decks = iw.getMatchedResults(core_gc);
    bool viewlet_detected = false;
    for (auto &rs : res_decks) {
      for (auto &r : rs) {
        fout << r.category << "\t" << r.score << "\t";
        fout << format(r.feature.t(), Formatter::FMT_CSV) <<"\t"<<r.neighborResponses << "\t"<< r.dists<<endl;
        rectangle(out, r.rect, colors[r.category % 15]);
        viewlet_detected = true;
      }
    }
    if (viewlet_detected)
      imwrite(desfolder + "picked_viewlet_" + s, out);
  }
}

vector<string> loadFolder(string srcfolder, string prefix) {

  vector<string> files;
  vector<fs::directory_entry> entries;

  copy_if(fs::directory_iterator(srcfolder), fs::directory_iterator(),
          back_inserter(entries),
          [&prefix](const fs::directory_entry &e) -> bool {
            string ext = al::to_lower_copy(e.path().extension().string());
            string fn = al::to_lower_copy(e.path().filename().string());
            bool condition = (ext == ".png" || ext == ".jpg" || ext == ".jp2" ||
                              ext == ".jpeg");
            if (prefix != "") {
              condition = condition && al::starts_with(fn, prefix);
            }
            return condition;
          });
  transform(entries.begin(), entries.end(), back_inserter(files),
            [](const fs::directory_entry &e) {
              return e.path().filename().string();
            });
  sort(files.begin(), files.end());
  return files;
}
