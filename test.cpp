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

template<typename T>
bool lt(const T& a, const T& b) {return (a<b);}
template<typename T>
bool gt(const T& a, const T& b) {return (a>b);}

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

  W<32> x, y, t;
  x = 125;
  y = 250;
  t = 0;
  cout << "match(" << x << ", " << y << ") = " << match(t, x,y, gt);
  cout << "\n new x=" << x << ", y=" << y << endl;
}
