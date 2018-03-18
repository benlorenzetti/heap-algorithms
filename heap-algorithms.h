#include "integers.h"
#include <cstdio>
#include <iostream>
#include <algorithm> // for swap()
using namespace std;

template<typename Z>
Z lchild(Z i) { return (i<<1).ADD(1); }

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
  void nwjunction(N<n>, W<n>, Iterator, bool (*)(const T&, const T&));

template<typename Iterator, typename T, int n>
  void wnjunction(N<n>, W<n>, Iterator, bool (*)(const T&, const T&));

template<typename Iterator, typename T, int n>
  void wdrift(W<n> j, W<n> b, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "wdrift(w" << j << ", w" << b << ") ";
  W<n> p = parent(j);
  while(p >= b && invr(base[j.datum], base[p.datum])) {
    cout << "swap(w" << j << ","<< base[j.datum] << ")<->(w";
    cout << p << "," << base[p.datum] << ") ";
    iter_swap(base+p.datum, base+j.datum);
    j = p;
    p = parent(j);
  }
}

template<typename Iterator, typename T, int n>
  void ndrift(N<n> a, W<n> i, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "ndrift(" << a << ", w" << i << ") ";
  W<n> p = W<n>(parent(N<n>(i)));
  while(a < N<n>(p) && invr(base[p.datum], base[i.datum])) {
    cout << "swap(w" << parent(i) << "," << base[p.datum] << ")<->(w";
    cout << i << "," << base[i.datum] << ") ";
    iter_swap(base+p.datum, base+i.datum);
    i = p;
    p = W<n>(parent(N<n>(i)));
  }
}

template<typename Iterator, typename T, int n>
  void wdiffuse(N<n> a, W<n> m, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "wdiffuse(" << a << ", " << m << ", w" << j << ") ";
  W<n> l = lchild(j);
  W<n> r = rchild(j);
  if(m == (r&m))
    wnjunction(a, j, base, invr);
  else if(invr(base[l.datum], base[r.datum])) {
    if(invr(base[l.datum], base[j.datum])) {
      cout << "lswap(w" << l << "," << base[l.datum] << ")<->(w";
      cout << j << "," << base[j.datum] << ") ";
      iter_swap(base+l.datum, base+j.datum);
      wdiffuse(a, m, l, base, invr);
    }
  }
  else {
    if(invr(base[r.datum], base[j.datum])) {
      cout << "rswap(w" << r << "," << base[r.datum] << ")<->(w";
      cout << j << "," << base[j.datum] << ") ";
      iter_swap(base+r.datum, base+j.datum);
      wdiffuse(a, m, r, base, invr);
    }
  }
}

template<typename Iterator, typename T, int n>
  void ndiffuse(N<n> i, W<n> m, W<n> b, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "ndiffuse(" << i << ", " << m << ", w" << b << ") ";
  W<n> p = W<n>(i);
  W<n> l = W<n>(lchild(i));
  W<n> r = W<n>(rchild(i));
  if(m != (r&m))
    nwjunction(i, b, base, invr);
  else if(invr(base[r.datum], base[l.datum])) {
    if(invr(base[p.datum], base[l.datum])) {
      cout << "lswap(" << i << "," << base[p.datum] << ")<->(";
      cout << lchild(i) << "," << base[l.datum] << ") ";
      iter_swap(base+l.datum, base+p.datum);
      ndiffuse(lchild(i), m, b, base, invr);
    }
  }
  else {
    if(invr(base[p.datum], base[r.datum])) {
      cout << "rswap(" << i << "," << base[p.datum] << ")<->(";
      cout << rchild(i) << "," << base[r.datum] << ") ";
      iter_swap(base+r.datum, base+p.datum);
      ndiffuse(rchild(i), m, b, base, invr);
    }
  }
}

template<typename Iterator, typename T, int n>
  void wnjunction(N<n> a, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "wnjunction(" << a << ", w" << j << ") ";
  W<n> l = lchild(j);
  W<n> r = rchild(j);
  bool fall_left = false;
  bool fall_right = false;
  if(!(a < N<n>(r)) && a !=N<n>(0)) {
    cout << "rchild bounds return. ";
    return;
  }
  if(invr(base[r.datum], base[j.datum])) {
    cout << "rswap(w" << r << "," << base[r.datum] << ")<->(w";
    cout << j << "," << base[j.datum] << ") ";
    iter_swap(base+r.datum, base+j.datum);
    fall_right = true;
  }
  if((a < N<n>(l) || a == N<n>(0)) && invr(base[l.datum], base[j.datum])) {
    cout << "lswap(w" << l << "," << base[l.datum] << ")<->(w";
    cout << j << "," << base[j.datum] << ") ";
    iter_swap(base+l.datum, base+j.datum);
    fall_left = true;
  }
  if(fall_left)
    ndrift(a, l, base, invr);
  else if(fall_right)
    ndrift(a, r, base, invr);
  else {}
}

template<typename Iterator, typename T, int n>
  void nwjunction(N<n> i, W<n> b, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "nwjunction(" << i << ", w" << b << ") ";
  W<n> p = W<n>(i);
  W<n> l = W<n>(lchild(i));
  W<n> r = W<n>(rchild(i));
  bool fall_left = false;
  bool fall_right = false;
  if(!(l>=b)) {
    cout << "lchild bounds return.";
    return;
  }
  if(invr(base[p.datum], base[l.datum])) {
    cout << "lswap(" << i << "," << base[p.datum] << ")<->(";
    cout << lchild(i) << "," << base[l.datum] << ") ";
    iter_swap(base+l.datum, base+p.datum);
    fall_left = true;
  }
  if(r >= b && invr(base[p.datum], base[r.datum])) {
    cout << "rswap(" << i << "," << base[p.datum] << ")<->(";
    cout << rchild(i) << "," << base[r.datum] << ") ";
    iter_swap(base+p.datum, base+r.datum);
    fall_right = true;
  }
  
  if(fall_right)
    wdrift(r, b, base, invr);
  else if(fall_left)
    wdrift(l, b, base, invr);
  else
    return;
}

template<typename Iterator, typename T, int n>
  void build_dual_heap(Iterator base, N<n> a, W<n> m, W<n> b, bool (*invr)(const T&, const T&)) {
  cout << "dual_heapify(base, " << a << ", " << m << ", w" << b;
  cout << ");\n";
  N<n> i = N<n>(m);
  W<n> j = m - W<1>(1);
  do {
    wdiffuse(a, m, j, base, invr);
    cout << endl;
    ndiffuse(i, m, b, base, invr);
    cout << endl;
    --i;
    --j;
  } while(a != i);
  
}
