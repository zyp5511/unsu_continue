#pragma once
#include "ImageCropper.h"
using namespace std;
using namespace cv;
class ExhaustiveCropper : public ImageCropper {
public:
  ExhaustiveCropper(void);
  ~ExhaustiveCropper(void);
  virtual void exportPatches(string fname) override;
  virtual void setUp(Mat img) override;
};
