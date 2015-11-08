#include "Feature.h"

using namespace cv;

#define eps 0.0001

static inline float min(float x, float y) { return (x <= y ? x : y); }
static inline float max(float x, float y) { return (x <= y ? y : x); }

static inline int min(int x, int y) { return (x <= y ? x : y); }
static inline int max(int x, int y) { return (x <= y ? y : x); }

Feature::Feature(void) {}

Feature::Feature(Mat &patch, const PCA &pca) {
  Mat patchcl = patch.clone();
  uchar *data = patchcl.data;
  int dims[2] = {patch.rows, patch.cols};
  int len;
  orivec = (process(data, len, dims, 8));
  Mat temp = pca.project(orivec);
  vec = vector<float>(temp.begin<float>(), temp.end<float>());
}
Feature::Feature(Mat &patch) {
  Mat patchcl = patch.clone();
  uchar *data = patchcl.data;
  int dims[2] = {patch.rows, patch.cols};
  int len;
  vec = (process(data, len, dims, 8));
}

Feature::~Feature(void) {}

float Feature::l2(Feature b) { return norm(vec, b.vec); }

void Feature::toHeadless() {
  if (vec.size() == 4480) {
    cout << "already headless patch" << endl;
  } else if (vec.size() != 5760) {
    cerr << "error" << endl;
    return;
  } else {
    int i = 0;
    int j = 0;
    for (i = 0; i < 5760; i++) {
      if (i % 18 >= 4) {
        vec[j] = vec[i];
        j++;
      }
    }
    vec.resize(j);
    cout << j << endl;
  }
}

vector<float> Feature::process(uchar *im, int &len, const int *dims,
                               const int sbin) {
  float uu[9] = {1.0000,  0.9397,  0.7660,  0.500,  0.1736,
                 -0.1736, -0.5000, -0.7660, -0.9397};
  float vv[9] = {0.0000, 0.3420, 0.6428, 0.8660, 0.9848,
                 0.9848, 0.8660, 0.6428, 0.3420};

  int blocks[2];
  blocks[0] = (int)(((float)dims[0] / (float)sbin) + 0.5f);
  blocks[1] = (int)(((float)dims[1] / (float)sbin) + 0.5f);
  float *hist = (float *)malloc(blocks[0] * blocks[1] * 18 * sizeof(float));
  for (int i = 0; i < blocks[0] * blocks[1] * 18; i++)
    *(hist + i) = 0;
  float *norm = (float *)malloc(blocks[0] * blocks[1] * sizeof(float));
  for (int i = 0; i < blocks[0] * blocks[1]; i++)
    *(norm + i) = 0;

  float *imd = (float *)malloc(dims[0] * dims[1] * 3 * sizeof(float));
  for (int i = 0; i < dims[0]; i++) {
    for (int j = 0; j < dims[1]; j++) {
      for (int k = 0; k < 3; k++)
        imd[k * dims[0] * dims[1] + j * dims[0] + i] =
            im[i * dims[1] * 3 + j * 3 + 2 - k];
    }
  }

  int out[3];
  out[0] = max(blocks[0] - 2, 0);
  out[1] = max(blocks[1] - 2, 0);
  out[2] = 27 + 4 + 1;
  len = out[0] * out[1] * out[2];
  float *feat = (float *)malloc(len * sizeof(float));

  int visible[2];
  visible[0] = blocks[0] * sbin;
  visible[1] = blocks[1] * sbin;
  for (int x = 1; x < visible[1] - 1; x++) {
    for (int y = 1; y < visible[0] - 1; y++) {
      float *s = imd + (x > dims[1] - 2 ? dims[1] - 2 : x) * dims[0] +
                 (y > dims[0] - 2 ? dims[0] - 2 : y);
      float dy = *(s + 1) - *(s - 1);
      float dx = *(s + dims[0]) - *(s - dims[0]);
      float v = dx * dx + dy * dy;

      s += dims[0] * dims[1];
      float dy2 = *(s + 1) - *(s - 1);
      float dx2 = *(s + dims[0]) - *(s - dims[0]);
      float v2 = dx2 * dx2 + dy2 * dy2;

      s += dims[0] * dims[1];
      float dy3 = *(s + 1) - *(s - 1);
      float dx3 = *(s + dims[0]) - *(s - dims[0]);
      float v3 = dx3 * dx3 + dy3 * dy3;

      if (v2 > v) {
        v = v2;
        dx = dx2;
        dy = dy2;
      }
      if (v3 > v) {
        v = v3;
        dx = dx3;
        dy = dy3;
      }

      float best_dot = 0;
      int best_o = 0;
      for (int o = 0; o < 9; o++) {
        float dot = uu[o] * dx + vv[o] * dy;
        if (dot > best_dot) {
          best_dot = dot;
          best_o = o;
        } else if (-dot > best_dot) {
          best_dot = -dot;
          best_o = o + 9;
        }
      }

      float xp = ((float)x + 0.5) / (float)sbin - 0.5;
      float yp = ((float)y + 0.5) / (float)sbin - 0.5;
      int ixp = (int)floor(xp);
      int iyp = (int)floor(yp);
      float vx0 = xp - ixp;
      float vy0 = yp - iyp;
      float vx1 = 1.0 - vx0;
      float vy1 = 1.0 - vy0;
      v = sqrt(v);

      if (ixp >= 0 && iyp >= 0) {
        *(hist + ixp * blocks[0] + iyp + best_o * blocks[0] * blocks[1]) +=
            vx1 * vy1 * v;
      }

      if (ixp + 1 < blocks[1] && iyp >= 0) {
        *(hist + (ixp + 1) * blocks[0] + iyp +
          best_o * blocks[0] * blocks[1]) += vx0 * vy1 * v;
      }

      if (ixp >= 0 && iyp + 1 < blocks[0]) {
        *(hist + ixp * blocks[0] + (iyp + 1) +
          best_o * blocks[0] * blocks[1]) += vx1 * vy0 * v;
      }

      if (ixp + 1 < blocks[1] && iyp + 1 < blocks[0]) {
        *(hist + (ixp + 1) * blocks[0] + (iyp + 1) +
          best_o * blocks[0] * blocks[1]) += vx0 * vy0 * v;
      }
    }
  }

  for (int o = 0; o < 9; o++) {
    float *src1 = hist + o * blocks[0] * blocks[1];
    float *src2 = hist + (o + 9) * blocks[0] * blocks[1];
    float *dst = norm;
    float *end = norm + blocks[1] * blocks[0];
    while (dst < end) {
      *(dst++) += (*src1 + *src2) * (*src1 + *src2);
      src1++;
      src2++;
    }
  }

  for (int x = 0; x < out[1]; x++) {
    for (int y = 0; y < out[0]; y++) {
      float *dst = feat + x * out[0] + y;
      float *src, *p, n1, n2, n3, n4;

      p = norm + (x + 1) * blocks[0] + y + 1;
      n1 = 1.0 /
           sqrt(*p + *(p + 1) + *(p + blocks[0]) + *(p + blocks[0] + 1) + eps);
      p = norm + (x + 1) * blocks[0] + y;
      n2 = 1.0 /
           sqrt(*p + *(p + 1) + *(p + blocks[0]) + *(p + blocks[0] + 1) + eps);
      p = norm + x * blocks[0] + y + 1;
      n3 = 1.0 /
           sqrt(*p + *(p + 1) + *(p + blocks[0]) + *(p + blocks[0] + 1) + eps);
      p = norm + x * blocks[0] + y;
      n4 = 1.0 /
           sqrt(*p + *(p + 1) + *(p + blocks[0]) + *(p + blocks[0] + 1) + eps);

      float t1 = 0;
      float t2 = 0;
      float t3 = 0;
      float t4 = 0;

      // contrast-sensitive features
      src = hist + (x + 1) * blocks[0] + (y + 1);
      for (int o = 0; o < 18; o++) {
        float h1 = min(*src * n1, 0.2);
        float h2 = min(*src * n2, 0.2);
        float h3 = min(*src * n3, 0.2);
        float h4 = min(*src * n4, 0.2);
        *dst = 0.5 * (h1 + h2 + h3 + h4);
        t1 += h1;
        t2 += h2;
        t3 += h3;
        t4 += h4;
        dst += out[0] * out[1];
        src += blocks[0] * blocks[1];
      }

      // contrast-insensitive features
      src = hist + (x + 1) * blocks[0] + (y + 1);
      for (int o = 0; o < 9; o++) {
        float sum = *src + *(src + 9 * blocks[0] * blocks[1]);
        float h1 = min(sum * n1, 0.2);
        float h2 = min(sum * n2, 0.2);
        float h3 = min(sum * n3, 0.2);
        float h4 = min(sum * n4, 0.2);
        *dst = 0.5 * (h1 + h2 + h3 + h4);
        dst += out[0] * out[1];
        src += blocks[0] * blocks[1];
      }

      // texture features
      *dst = 0.2357 * t1;
      dst += out[0] * out[1];
      *dst = 0.2357 * t2;
      dst += out[0] * out[1];
      *dst = 0.2357 * t3;
      dst += out[0] * out[1];
      *dst = 0.2357 * t4;

      // truncation feature
      dst += out[0] * out[1];
      *dst = 0;
    }
  }
  free(hist);
  free(norm);
  free(imd);
  vector<float> res(feat, feat + len);
  free(feat);
  return res;
}
