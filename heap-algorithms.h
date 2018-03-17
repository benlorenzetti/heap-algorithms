#include "integers.h"
#include <cstdio>
#include <iostream>
#include <algorithm> // for swap()
using namespace std;

template<typename Z>
Z lchild(Z i) { return (i<<1).OR(1); }

template<typename Z>
Z rchild(Z i) { return i<<1; }

template<typename Z>
Z parent(Z i) { return i>>1; }

/* The procedure match(a, tmp, b, r) takes references to objects a and b
and temporary "a" storage location tmp and a relational operator invr.
If invr(tmp,b), then match return false and element 'a' is updated to
the value of 'b'. Otherwise r(tmp,b) was already true so match()
returns true.
*/
template<typename T>
bool match(T& a, const T& tmp, const T& b, bool (*invr)(const T&, const T&)) {
  if(invr(tmp, b)) {
    a = b;
    return false;
  }
  else
    return true;
}

template<typename Iterator, typename T, int n>
  void diffuse(W<n> j, W<n> b, Iterator base, bool (*invr)(const T&, const T&)) {
  T tmp = base[j];
  while(!match(base+j, tmp, base+parent(j), invr))
	j = parent(j);
  base[j] = tmp;
}

template<typename Iterator, typename T, int n>
  void junction(N<n> i, W<n> b, Iterator base, bool (*invr)(const T&, const T&)) {
  W<n> p = W(i);
  W<n> l = W(lchild(i));
  W<n> r = W(rchild(i));
  bool diffusion_left = false;
  bool diffusion_right = false;
  if(b > l)
    return;
  else if(invr(base[l], base[p])) {
    swap(base+l, base+p);
    diffusion_left = true;
  }
  else if(r > b && invr(base[p], base[r])) {
    swap(base+p, base+r);
    diffusion_right = true;
  }
  else
    return;
  
  if(diffusion_right)
    diffuse(r, b, base, invr);
  else if(diffusion_left)
    diffuse(l, b, base, invr);
  else
    return;
}

template<typename Iterator, int n>
void W_heapify(Iterator base, W<n> a, W<n> m, W<n> j) {
  if(m == (m | j)) {
    

  }
  cout << "j=" << j << ", lchild="<<lchild(j);
  cout << ", rchild=" << rchild(j) << ", parent=" << parent(j) << endl;
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
