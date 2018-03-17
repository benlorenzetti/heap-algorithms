#include "integers.h"
#include <cstdio>
#include <iostream>
using namespace std;

template<typename Z>
Z lchild(Z i) { return ++(i<<1); }

template<typename Z>
Z rchild(Z i) { return i<<1; }

template<typename Z>
Z parent(Z i) { return i>>1; }

template<typename Iterator, int n>
void W_heapify(Iterator base, W<n> a, W<n> m, W<n> j) {
  if(m == (m | j)) {
    if(a > lchild(j)) {
      matchx
    }

  }
  cout << "j=" << j << ", lchild="<<lchild(j);
  cout << ", rchild=" << rchild(j) << ", parent=" << wparent(j) << endl;
  if(
}

template<typename Iterator, int n>
void build_dual_heap(Iterator base, N<n> a, W<n> m, W<n> b) {
  cout << "dual_heapify(base, " << a << ", " << m << ", " << b;
  cout << ");\n";
  N<n> i = N<n>(m);
  ++i;
  W<n> j = m - 1;
  cout << "i=" << i << ", j=" << j << endl;
  W_heapify(base, W<n>(a), m, j);
  
}
