#include "heap-algorithms.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


bool intgt(const int& a, const int& b) { return (a>b); }
bool intlt(const int& a, const int& b) { return (a<b); }
bool inteq(const int& a, const int& b) { return (a==b); }

void save(vector<int>& v, string filename) {
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
      cout << "(" << i << "," << *it << "), ";
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

const int POWER = 16;
const int SEED = 0;
const int RUNS = 1;

int main() {

  vector<int> v(1<<POWER);
  W<POWER> a = 0;
  W<POWER> m = 1 << (POWER-1);
  W<POWER> b = 0;

  vector<int> median_estimate_difs, partition_difs;

  int seed = SEED;
  for(int i=0; i<RUNS; i++, seed++) {
    generate(v, seed);
    sort(v.begin(), v.end());
    int true_median = v[m.datum];
    generate(v, seed);
    //heap_median<POWER>(v.begin());
    winnow_heap(N<POWER>(a), m, b, v.begin(), intgt);
    median_estimate_difs.push_back(estimate_dif(a, b, v.begin(), intlt, inteq, v[m.datum]));
    partition_difs.push_back(partition_dif(a, m, v.begin(), intgt, true_median).datum);
  }
  int max_med_est_dif = -(*min_element(median_estimate_difs.begin(), median_estimate_difs.end()));
  max_med_est_dif = max(max_med_est_dif, *min_element(median_estimate_difs.begin(), median_estimate_difs.end()));
  float noverlogn = (1<<POWER)/(float)POWER;
  int max_part_dif = *max_element(partition_difs.begin(), partition_difs.end());
  cout << "max_med_est_dif_pct = " << max_med_est_dif / noverlogn << endl;
  cout << "   max_part_dif_pct = " << max_part_dif / noverlogn << endl;
  save(median_estimate_difs, to_string(POWER) + "median-estimate-difs.csv");
  save(partition_difs, to_string(POWER) + "partition-difs.csv");
  
  /*
  N<POWER> n(0);
  W<POWER> w(-1);
  do {
    ++n;
    --w;
    if(!verify_heap(n, N<POWER>(0), v.begin(), intleq)) {
      cout << "N heap violation at " << n << "(" << w << "): \n";
    }
    if(!verify_heap(W<POWER>(0), w, v.begin(), intleq)) {
      cout << "W heap violation at " << n << "(" << w << "): \n";
    }
  } while(n != N<POWER>(0));
  */  

  save(v, "output.csv");
}

