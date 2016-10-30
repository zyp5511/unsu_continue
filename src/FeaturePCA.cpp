#include "FeaturePCA.h"
FeaturePCA::FeaturePCA(MatrixXf &fea, float retainedVar) {

  size_t len = fea.cols();
  int in_count = fea.rows();
#ifdef DEBUG_PCA
  cout << "fea" << endl;
  cout << fea << endl;
#endif
  mean = fea.colwise().mean();
  MatrixXf covar(len, len);
  fea.rowwise() -= mean;

  covar = fea.transpose() * fea / in_count;
#ifdef DEBUG_PCA
  cout << "cor" << endl;
  cout << covar << endl;
#endif

  SelfAdjointEigenSolver<MatrixXf> es(covar);
  auto tempval = es.eigenvalues();
#ifdef DEBUG_PCA
  cout << "eigen values" << endl;
  cout << es.eigenvalues() << endl;
  cout << "eigen vector:" << endl;
  cout << es.eigenvectors() << endl;
#endif
  ev = MatrixXf(len, len);
  el = VectorXf(len);
  vector<int> position(len);
  for (size_t i = 0; i < len; i++)
    position[i] = i;
  sort(position.begin(), position.end(),
       [&tempval](int i, int j) -> bool { return tempval(i) > tempval(j); });
  for (size_t i = 0; i < len; i++) {
    el(i) = tempval(position[i]);
    ev.col(i) = es.eigenvectors().col(position[i]);
  }

  // compute the cumulative energy content for each eigenvector
  vector<double> g(len);
  g[0] = el(0);
  for (size_t ig = 1; ig < len; ig++) {
    g[ig] = g[ig - 1] + el(ig);
#ifdef DEBUG_PCA
    cout << g[ig] << "\t";
#endif
  }
#ifdef DEBUG_PCA
  cout << endl;
#endif

  int L;
  for (L = 0; L < len; L++) {
    double energy = g[L] / g[len - 1];
    if (energy > retainedVar)
      break;
  }

  L = std::max(2, L);
#ifdef DEBUG_PCA
  cout << "L=" << L << endl;
#endif

  el.conservativeResize(L);
  ev.conservativeResize(len, L);
#ifdef DEBUG_PCA
  cout << "eigen values" << endl;
  cout << el << endl;
  cout << "eigen vector:" << endl;
  cout << ev << endl;
#endif
}
void FeaturePCA::projectZeroMean(MatrixXf &ori, MatrixXf &shorten) {
  shorten = ori * ev;
}
void FeaturePCA::project(MatrixXf &ori, MatrixXf &shorten) {
  ori.rowwise() -= mean;
  shorten = ori * ev;
}
void FeaturePCA::backProject(MatrixXf &shorten, MatrixXf &ori) {
  ori = shorten * ev.transpose();
}
PCA FeaturePCA::getCVPCA() {
  PCA pca;
  pca.mean = Mat();
  pca.eigenvalues = Mat();
  pca.eigenvectors = Mat();
  auto temp = ev.transpose().eval();
  eigen2cv(temp, pca.eigenvectors);
  eigen2cv(el, pca.eigenvalues);
  eigen2cv(mean, pca.mean);
  return pca;
}
