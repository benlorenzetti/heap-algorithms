#include "heap-algorithms.h"
#include <vector>
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

void print(vector<int>& v) {
  vector<int>::iterator it = v.end();
  N<32> i(0);
  W<32> j(v.size());
  if(v.size())
    do {
      ++i;
      --j;
      --it;
      cout << "(" << j << "," << *it << "), ";
    } while(it != v.begin());
  cout << endl;
}

void generate(vector<int>& v, int seed) {
  srand(seed);
  int i = v.size();
  do {
    v[i] = rand() % v.size();
  } while(--i);
}

template<typename T>
bool lt(const T& a, const T& b) {return (a<b);}
template<typename T>
bool gt(const T& a, const T& b) {return (a>b);}
bool intgt(const int& a, const int& b) { return (a>b); }

const int POWER = 18;

int main() {
  vector<int> v(1<<POWER);
  generate(v, 1);
  save(v, "input.csv");

  N<POWER> a(1<<POWER);
  W<POWER> m, b;
  m = v.size() / 2;
  b = 0;
  build_dual_heap(v.begin(), a, m, b, intgt);
  save(v, "output.csv");
}
