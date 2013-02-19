#include "FeatureWriter.h"


FeatureWriter::FeatureWriter(void)
{
}


FeatureWriter::~FeatureWriter(void)
{
}

void FeatureWriter::saveYAML(string fsfn, Mat& feavec){
	//save to FileStorage
	FileStorage fs(fsfn, FileStorage::WRITE);
	fs << "feature" << feavec;
	fs.release();
}