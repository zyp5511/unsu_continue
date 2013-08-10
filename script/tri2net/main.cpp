#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;

int main(int argc, char** argv){
	if(argc<3){
		cerr<<"input and output filename required"<<endl;
	} else{
		// open input and outfile files;
		ifstream fin(argv[1]);
		ofstream fout(argv[2]);
		string line;

		//iterate by line
		int i = 0;
		while(getline(fin, line)){
			i++;
			stringstream ss(line);
			int j=i;
			float dis;
			ss>>dis;
			while(ss>>dis){
				j++;
				if (dis<47){
					fout<<i<<"\t"<<j<<"\t"<<dis<<endl;
				}
			}
		}	
		fin.close();
		fout.close();
	}
	return 0;
}
