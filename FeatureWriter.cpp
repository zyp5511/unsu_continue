#include "FeatureWriter.h"
#include<fstream>


FeatureWriter::FeatureWriter(void)
{
}


FeatureWriter::~FeatureWriter(void)
{
}

void FeatureWriter::saveYAML(string fsfn, const Mat& feavec){
	//save to FileStorage
	FileStorage fs(fsfn, FileStorage::WRITE);
	fs << "feature" << feavec;
	fs.release();
}
void FeatureWriter::saveTab(string fname, const Mat& feavec){
	ofstream fout(fname);
	fout<<feavec.rows<<endl;
	fout<<feavec.cols<<endl;

	for(int i = 0;i<feavec.rows;i++){
		Mat tempr = feavec.row(i);
		for(auto j = tempr.begin<float>();j!=tempr.end<float>();j++){
			fout<<*j<<"\t";
		}
		fout<<"\n";//I think endl is slow, haven't tested :)
	}
	fout.close();
}
