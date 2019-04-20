#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc < 4) {
    cerr << "input and output filename required" << endl;
  } else {
    // open input and outfile files;
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    double threshold = strtod(argv[3], NULL);
    string line;

    // iterate by line
    int i = 0;
    while (getline(fin, line)) {
      i++;
      stringstream ss(line);
      int j = i;
      float dis;
      ss >> dis;
      while (ss >> dis) {
        j++;
        if ((dis) > threshold) {
          fout << i << "\t" << j << "\t" << dis << endl;
        }
      }
    }
    fin.close();
    fout.close();
  }
  return 0;
}
