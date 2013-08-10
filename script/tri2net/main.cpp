#include<fstream>
using namespace std;
int main(int argc, char** argv){
	if(argc<3){
		cerr<<"input and output filename required"<<endl;
	} else{
		// open input and outfile files;
		ifstream fin(argv[1]);
		ofstream fout(argv[2]);
		string line;

		iterate by line
		while(getline(fin, line)){
		}	

	}
	return 0;
}
