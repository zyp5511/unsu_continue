#include "FeaturePCA.h"
FeaturePCA::FeaturePCA(MatrixXf&fea, float retainedVar){
	int len = fea.cols();
	int in_count = fea.rows();

	mean = fea.colwise().mean();
	MatrixXf covar(len, len);
	fea.rowwise()-=mean;
	covar = fea.transpose()*fea;
	EigenSolver<MatrixXf> es(covar);
	auto tempval = es.eigenvalues();
	ev = MatrixXf(len,len);
	el = RowVectorXf(len);
	vector<int> position(len);
	for(int i=0;i<len;i++)
		position[i]=i;
	sort(position.begin(),position.end(),[&tempval](int i, int j)->bool{
			return tempval(i).real() >tempval(j).real();
			});
	for(size_t i=0;i<len;i++){
		el(i)=tempval(position[i]).real();
		ev.row(i)=es.eigenvectors().row(position[i]).real();
	}

	// compute the cumulative energy content for each eigenvector
	vector<float> g(len);
	g[0]=el(0);
	for(int ig = 1; ig < len; ig++) {
		g[ig] = g[ig-1]+el(ig);
	}

	int L;
	for(L = 0; L < len; L++) {
		double energy = g[L] / g[len-1];
		if(energy > retainedVar)
			break;
	}

	L = std::max(2, L);

	// use clone() to physically copy the data and thus deallocate the original matrices
	el.conservativeResize(L);
	ev.conservativeResize(L,len);
}
void FeaturePCA::project(MatrixXf&ori, MatrixXf&shorten){
	shorten = ori * ev.transpose();
}
void FeaturePCA::backProject(MatrixXf&shorten, MatrixXf&ori){
	ori = shorten * ev;
}
PCA FeaturePCA::getCVPCA(){
	PCA pca;
	pca.mean = Mat();
	pca.eigenvalues = Mat();
	pca.eigenvectors = Mat();
	eigen2cv(ev,pca.eigenvectors);
	eigen2cv(el,pca.eigenvalues);
	eigen2cv(mean,pca.mean);
	return pca;
}
