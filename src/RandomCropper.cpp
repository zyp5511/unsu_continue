//
//  NegativeCollector.cpp
//  HumanPoseDetector
//
//  Created by Lichao Chen on 12/3/12.
//  Copyright (c) 2012 Lichao Chen. All rights reserved.
//

#include "RandomCropper.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

namespace fs = boost::filesystem;
namespace al = boost::algorithm;

void RandomCropper::collectSrcDir(string fname) {

  vector<string> files;
  vector<fs::directory_entry> entries;

  copy_if(fs::directory_iterator(fname), fs::directory_iterator(),
          back_inserter(entries), [](const fs::directory_entry &e) -> bool {
            string ext = al::to_lower_copy(e.path().extension().string());
            return (ext == ".png" || ext == ".jpg");
          });
  transform(entries.begin(), entries.end(), back_inserter(files),
            [](const fs::directory_entry &e) {
              return e.path().filename().string();
            });
  sort(files.begin(), files.end());
  auto itend = files.rend();

#ifdef DEBUG
  itend = files.rbegin() + 50;
#endif

  for_each(files.rbegin(), itend, [this, fname](string s) {
    Mat img = imread(fname + s);
    cout << fname + s << endl;
    this->setUp(img);
  });
  auto it = all_mats.begin();
  Feature f(*it);
  int numcol = static_cast<int>(f.vec.size());
  int numrow = static_cast<int>(all_mats.size());

  feavec = Mat(numrow, numcol, CV_32F);

  for (int i = 0; i < numrow; i++) {
    feavec.row(i) = Mat(Feature(*it++).vec).t();
  }
  cout << "Feature Matrix Created" << endl;
  cout << "Rows:\t" << numrow << endl;
  cout << "Cols:\t" << numcol << endl;
}

void RandomCropper::kmean() { kmean(100); }

void RandomCropper::kmean(int k) {
  auto category = vector<int>(all_mats.size());
  kmeans(feavec, k, category, TermCriteria(TermCriteria::MAX_ITER, 30, 0), 5,
         KMEANS_PP_CENTERS);
  for_each(category.begin(), category.end(), [](int i) { cout << i << endl; });
}
void RandomCropper::pca() {
  PCA a(feavec, noArray(), PCA::DATA_AS_ROW, 0.95);
  auto shortfea = a.project(feavec);

  cout << "PCA done" << endl;
  cout << "Rows:\t" << shortfea.rows << endl;
  cout << "Cols:\t" << shortfea.cols << endl;
  feavec = shortfea;
}

void RandomCropper::exportFeatures(string fname) {
  //	FileStorage fs(fname, FileStorage::WRITE);
  //	fs<<"feature"<<feavec;
  //	fs.release();
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
}
void RandomCropper::exportSeperators(string fname) {
  ofstream fout(fname);
  for_each(seperators.begin(), seperators.end(),
           [&fout](int i) { fout << i << endl; });
  fout.close();
}
void RandomCropper::exportPatches(string fname) {

  if (*fname.rbegin() != '/') {
    cout << (int)*fname.rbegin() << endl;
    fname = fname + "/";
  }
  for (size_t i = 0; i < all_mats.size(); i++) {
    imwrite(fname + to_string(i + 1) + ".jpg", all_mats[i]);
  }
}
void RandomCropper::setUp(Mat img) {
  seperators.push_back(all_mats.size());
  vector<double> level_scale;
  // cout<<"windows ratio:"<<patch_r<<"\t"<<patch_c<<endl;

  double scale = 1.;
  double scale0 = 1.2;
  int levels = 0;
  int maxl = 40;
  if (!toprymaid) {
    maxl = 1;
  }
  for (levels = 0; levels < maxl; levels++) {
    level_scale.push_back(scale);
    if (cvRound(img.cols / scale) < patch_c ||
        cvRound(img.rows / scale) < patch_r || scale0 <= 1)
      break;
    scale *= scale0;
  }

  levels = max(levels, 1);
  level_scale.resize(levels);
  size_t i;
  for (i = 0; i < level_scale.size(); i++) {
    scale = level_scale[i];
    int imgs = patchesPerImage / scale / scale;
    Size sz(cvRound(img.cols / scale), cvRound(img.rows / scale));
    Mat smaller_img;

    if (sz == img.size())
      smaller_img = img;
    else {
      resize(img, smaller_img, sz);
    }
    // cout<<"windows scale:"<<smaller_img.rows<<"\t"<<smaller_img.cols<<endl;
    if (smaller_img.rows - patch_r > -1 && smaller_img.cols - patch_c > -1 &&
        imgs > 0) {
      vector<int> rowvs(imgs), colvs(imgs);
      cout << imgs << endl;
      cv::randu(rowvs, 0, smaller_img.rows - patch_r + 1);
      cv::randu(colvs, 0, smaller_img.cols - patch_c + 1);
      Size scaled_win_size(cvRound(patch_c * scale), cvRound(patch_r * scale));
      for (auto rs = rowvs.begin(), cs = colvs.begin(); rs != rowvs.end();
           rs++, cs++) {
        // rect list
        all_rects.push_back(Rect(Point2d(*cs, *rs) * scale, scaled_win_size));
        cout << *cs << "\t" << *rs << endl;
        // sub mat list
        Mat temp =
            smaller_img(Range(*rs, *rs + patch_r), Range(*cs, *cs + patch_c))
                .clone();
        all_mats.push_back(temp);
      }
    }
  }
}
