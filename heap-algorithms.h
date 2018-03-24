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
  bool verify_heap(W<n> a, W<n> b, Iterator base, bool (*rel)(const T&, const T&)) {
  W<n> l = lchild(b);
  W<n> r = rchild(b);
  if(r < b)
    return true;
  else if(a > lchild(b) || a == W<n>(0)) {
    if(rel(base[l.datum], base[b.datum]) && rel(base[r.datum], base[b.datum]))
      return true;
    else
      return false;
  }
  else if(a == rchild(b)) {
    if(rel(base[r.datum], base[b.datum]))
      return true;
    else
      return false;
  }
  else
    return true;
}

template<typename Iterator, typename T, int n>
  bool verify_heap(N<n> a, N<n> b, Iterator base, bool (*rel)(const T&, const T&)) {
  W<n> p = W<n>(a);
  W<n> l = W<n>(lchild(a));
  W<n> r = W<n>(rchild(a));
  if(a > lchild(a))
    return true;
  else if(rchild(a) <= b) {
    if(rel(base[p.datum], base[l.datum]) && rel(base[p.datum], base[r.datum]))
      return true;
    else
      return false;
  }
  else if(lchild(a) == b) {
    if(rel(base[p.datum], base[l.datum]))
      return true;
    else
      return false;
  }
  else
    return true;
}

template<typename Iterator, typename T, int n>
  void nwjunction(N<n>, W<n>, W<n>, Iterator, bool (*)(const T&, const T&));

template<typename Iterator, typename T, int n>
  void wnjunction(N<n>, N<n>, W<n>, Iterator, bool (*)(const T&, const T&));

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
  void wdiffuse(N<n> i, W<n> m, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "wdiffuse(" << i << ", " << m << ", w" << j << ") ";
  W<n> l = lchild(j);
  W<n> r = rchild(j);
  if(m == (r&m)) {
    wnjunction(i, j, base, invr);
  }
  else if(invr(base[l.datum], base[r.datum])) {
    if(invr(base[l.datum], base[j.datum])) {
      cout << "lswap(w" << l << "," << base[l.datum] << ")<->(w";
      cout << j << "," << base[j.datum] << ") ";
      iter_swap(base+l.datum, base+j.datum);
      wdiffuse(i, m, l, base, invr);
    }
  }
  else {
    if(invr(base[r.datum], base[j.datum])) {
      cout << "rswap(w" << r << "," << base[r.datum] << ")<->(w";
      cout << j << "," << base[j.datum] << ") ";
      iter_swap(base+r.datum, base+j.datum);
      wdiffuse(i, m, r, base, invr);
    }
  }
  ndrift(i, j, base, invr);
}

template<typename Iterator, typename T, int n>
  void ndiffuse(N<n> i, W<n> m, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "ndiffuse(" << i << ", " << m << ", w" << j << ") ";
  W<n> p = W<n>(i);
  W<n> l = W<n>(lchild(i));
  W<n> r = W<n>(rchild(i));
  if(m != (r&m)) {
    nwjunction(i, j, base, invr);
  }
  else if(invr(base[r.datum], base[l.datum])) {
    if(invr(base[p.datum], base[l.datum])) {
      cout << "lswap(" << i << "," << base[p.datum] << ")<->(";
      cout << lchild(i) << "," << base[l.datum] << ") ";
      iter_swap(base+l.datum, base+p.datum);
      ndiffuse(lchild(i), m, j, base, invr);
    }
  }
  else {
    if(invr(base[p.datum], base[r.datum])) {
      cout << "rswap(" << i << "," << base[p.datum] << ")<->(";
      cout << rchild(i) << "," << base[r.datum] << ") ";
      iter_swap(base+r.datum, base+p.datum);
      ndiffuse(rchild(i), m, j, base, invr);
    }
  }
  wdrift(p, j, base, invr);
}

template<typename Iterator, typename T, int n>
  void wnjunction(N<n> i, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "wnjunction(" << i << ", w" << j << ") ";
  W<n> l = lchild(j);
  W<n> r = rchild(j);
  if(!(i < N<n>(r)) && i !=N<n>(0)) {
    cout << "rchild bounds, return. ";
    return;
  }
  if(invr(base[r.datum], base[j.datum])) {
    cout << "rswap(w" << r << "," << base[r.datum] << ")<->(w";
    cout << j << "," << base[j.datum] << ") ";
    iter_swap(base+r.datum, base+j.datum);
    ndrift(i, r, base, invr);
  }
  if((i < N<n>(l) || i == N<n>(0)) && invr(base[l.datum], base[j.datum])) {
    cout << "lswap(w" << l << "," << base[l.datum] << ")<->(w";
    cout << j << "," << base[j.datum] << ") ";
    iter_swap(base+l.datum, base+j.datum);
    ndrift(i, l, base, invr);
  }
}

template<typename Iterator, typename T, int n>
  void nwjunction(N<n> i, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  cout << "nwjunction(" << i << ", w" << j << ") ";
  W<n> p = W<n>(i);
  W<n> l = W<n>(lchild(i));
  W<n> r = W<n>(rchild(i));
  if(!(l>=j)) {
    cout << "lchild bounds, return.";
    return;
  }
  if(invr(base[p.datum], base[l.datum])) {
    cout << "lswap(" << i << "," << base[p.datum] << ")<->(";
    cout << lchild(i) << "," << base[l.datum] << ") ";
    iter_swap(base+l.datum, base+p.datum);
    wdrift(l, j, base, invr);
  }
  if(r >= j && invr(base[p.datum], base[r.datum])) {
    cout << "rswap(" << i << "," << base[p.datum] << ")<->(";
    cout << rchild(i) << "," << base[r.datum] << ") ";
    iter_swap(base+p.datum, base+r.datum);
    wdrift(r, j, base, invr);
  }
}

template<typename Iterator, typename T, int n>
  void build_dual_heap(Iterator base, N<n> a, W<n> m, W<n> b, bool (*invr)(const T&, const T&)) {
  cout << "dual_heapify(base, " << a << ", " << m << ", w" << b;
  cout << ");\n";
  N<n> i = N<n>(m);
  W<n> j = m - W<1>(1);
  do {
    wdiffuse(i, m, j, base, invr);
    cout << endl;
    ndiffuse(i, m, j, base, invr);
    cout << endl;
    --j;
    --i;
  } while(i != a);
  
}
