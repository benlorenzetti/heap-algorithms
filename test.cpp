#include "heap-algorithms.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

void save(vector<int>& v, const char* filename) {
  ofstream filestream;
  filestream.open(filename);
  N<32> i(0);
  vector<int>::iterator it = v.end();
  if(v.size())
    do {
      i = i + N<1>(1);
      --it;
      filestream << i << "\t" << *it << "\n";
    } while(it != v.begin());
}

const int POWER = 5;

int main() {
  vector<int> v(1<<POWER);
  srand(0);
  generate(v.begin(), v.end(), rand);
  save(v, "input.csv");

  N<POWER> a(-v.size());
  W<POWER> m, b;
  m = v.size() / 2;
  b = 0;
  build_dual_heap(v.begin(), a, m, b);
}
