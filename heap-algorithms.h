#include "integers.h"
#include <cstdio>
#include <iostream>
#include <algorithm> // for swap()
using namespace std;

template<typename Z>
Z lchild(Z i) { i=(i<<1); i.ADD(1); return i; }

template<typename Z>
Z rchild(Z i) { i=(i<<1); return i; }

template<typename Z>
Z parent(Z i) { i=i>>1; return i; }

template<typename Iterator, typename T, int n>
  W<n> countif(W<n> a, W<n> b, Iterator base, bool (*rel)(const T&, const T&), T rel2) {
  W<n> count = 0;
  do {
    --a;
    if(rel(base[a.datum], rel2)) {
      ++count;
    }
  } while (a != b);
  return count;
}

template<typename Iterator, typename T, int n>
  int estimate_dif(W<n> a, W<n> b, Iterator base, bool (*lt)(const T&, const T&), bool (*eq)(const T&, const T&), T rel2) {
  W<n> ltc = countif(a, b, base, lt, rel2);
  W<n> eqc = countif(a, b, base, eq, rel2);
  W<n> gtc = (a-b) - ltc - eqc;
  int dif = gtc.datum - ltc.datum;
  if(dif < 0)
    dif += eqc.datum;
  else if(dif > 0)
    dif -= eqc.datum;
  else {}
  return dif;
}

template<typename Iterator, typename T, int n>
  W<n> partition_dif(W<n> a, W<n> m, Iterator base, bool (*invr)(const T&, const T&), T true_med_val) {
  return countif(a, m, base, invr, true_med_val);
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
  //cout << "wdrift(w" << j << ", w" << b << ") ";
  W<n> p = parent(j);
  while(p >= b && invr(base[j.datum], base[p.datum])) {
    //cout << "swap(w" << j << ","<< base[j.datum] << ")<->(w";
    //cout << p << "," << base[p.datum] << ") ";
    iter_swap(base+p.datum, base+j.datum);
    j = p;
    p = parent(j);
  }
}

template<typename Iterator, typename T, int n>
  void ndrift(N<n> a, W<n> i, Iterator base, bool (*invr)(const T&, const T&)) {
  //cout << "ndrift(" << a << ", w" << i << ") ";
  W<n> p = W<n>(parent(N<n>(i)));
  while(a < N<n>(p) && invr(base[p.datum], base[i.datum])) {
    //cout << "swap(w" << parent(i) << "," << base[p.datum] << ")<->(w";
    //cout << i << "," << base[i.datum] << ") ";
    iter_swap(base+p.datum, base+i.datum);
    i = p;
    p = W<n>(parent(N<n>(i)));
  }
}

template<typename Iterator, typename T, int n>
  void wdiffuse(N<n> i, W<n> m, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  //cout << "wdiffuse(" << i << ", " << m << ", w" << j << ") ";
  W<n> l = lchild(j);
  W<n> r = rchild(j);
  if(m == (r&m)) {
    wnjunction(i, j, base, invr);
  }
  else if(invr(base[l.datum], base[r.datum])) {
    if(invr(base[l.datum], base[j.datum])) {
      //cout << "lswap(w" << l << "," << base[l.datum] << ")<->(w";
      //cout << j << "," << base[j.datum] << ") ";
      iter_swap(base+l.datum, base+j.datum);
      wdiffuse(i, m, l, base, invr);
    }
  }
  else {
    if(invr(base[r.datum], base[j.datum])) {
      //cout << "rswap(w" << r << "," << base[r.datum] << ")<->(w";
      //cout << j << "," << base[j.datum] << ") ";
      iter_swap(base+r.datum, base+j.datum);
      wdiffuse(i, m, r, base, invr);
    }
  }
  ndrift(i, j, base, invr);
}

template<typename Iterator, typename T, int n>
  void ndiffuse(N<n> i, W<n> m, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  //cout << "ndiffuse(" << i << ", " << m << ", w" << j << ") ";
  W<n> p = W<n>(i);
  W<n> l = W<n>(lchild(i));
  W<n> r = W<n>(rchild(i));
  if(m != (r&m)) {
    nwjunction(i, j, base, invr);
  }
  else if(invr(base[r.datum], base[l.datum])) {
    if(invr(base[p.datum], base[l.datum])) {
      //cout << "lswap(" << i << "," << base[p.datum] << ")<->(";
      //cout << lchild(i) << "," << base[l.datum] << ") ";
      iter_swap(base+l.datum, base+p.datum);
      ndiffuse(lchild(i), m, j, base, invr);
    }
  }
  else {
    if(invr(base[p.datum], base[r.datum])) {
      //cout << "rswap(" << i << "," << base[p.datum] << ")<->(";
      //cout << rchild(i) << "," << base[r.datum] << ") ";
      iter_swap(base+r.datum, base+p.datum);
      ndiffuse(rchild(i), m, j, base, invr);
    }
  }
  wdrift(p, j, base, invr);
}

template<typename Iterator, typename T, int n>
  void wnjunction(N<n> i, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  //cout << "wnjunction(" << i << ", w" << j << ") ";
  W<n> l = lchild(j);
  W<n> r = rchild(j);
  if(!(i < N<n>(r)) && i !=N<n>(0)) {
    //cout << "rchild bounds, return. ";
    return;
  }
  if(invr(base[r.datum], base[j.datum])) {
    //cout << "rswap(w" << r << "," << base[r.datum] << ")<->(w";
    //cout << j << "," << base[j.datum] << ") ";
    iter_swap(base+r.datum, base+j.datum);
    ndrift(i, r, base, invr);
  }
  if((i < N<n>(l) || i == N<n>(0)) && invr(base[l.datum], base[j.datum])) {
    //cout << "lswap(w" << l << "," << base[l.datum] << ")<->(w";
    //cout << j << "," << base[j.datum] << ") ";
    iter_swap(base+l.datum, base+j.datum);
    ndrift(i, l, base, invr);
  }
}

template<typename Iterator, typename T, int n>
  void nwjunction(N<n> i, W<n> j, Iterator base, bool (*invr)(const T&, const T&)) {
  //cout << "nwjunction(" << i << ", w" << j << ") ";
  W<n> p = W<n>(i);
  W<n> l = W<n>(lchild(i));
  W<n> r = W<n>(rchild(i));
  if(!(l>=j)) {
    //cout << "lchild bounds, return.";
    return;
  }
  if(invr(base[p.datum], base[l.datum])) {
    //cout << "lswap(" << i << "," << base[p.datum] << ")<->(";
    //cout << lchild(i) << "," << base[l.datum] << ") ";
    iter_swap(base+l.datum, base+p.datum);
    wdrift(l, j, base, invr);
  }
  if(r >= j && invr(base[p.datum], base[r.datum])) {
    //cout << "rswap(" << i << "," << base[p.datum] << ")<->(";
    //cout << rchild(i) << "," << base[r.datum] << ") ";
    iter_swap(base+p.datum, base+r.datum);
    wdrift(r, j, base, invr);
  }
}

template<typename Iterator, typename T, int n>
  void build_dual_heap(N<n> a, W<n> m, W<n> b, Iterator base, bool (*invr)(const T&, const T&)) {
  //cout << "dual_heapify(base, " << a << ", " << m << ", w" << b;
  //cout << ");\n";
  N<n> i = N<n>(m);
  W<n> j = m - W<1>(1);
  do {
    wdiffuse(i, m, j, base, invr);
    ndiffuse(i, m, j, base, invr);
    --j;
    --i;
  } while(i != a);
}

template<typename Iterator, typename T, int n>
  void winnow_heap(N<n> a, W<n> m, W<n> b, Iterator base, bool (*invr)(const T&, const T&)) {
  build_dual_heap(a, m, b, base, invr);
  N<n-1> a2(0);
  W<n-1> m2 = 1 << (n-2);
  W<n-1> b2(0);
  winnow_heap(a2, m2, b2, base + m2.datum, invr);
}

template<typename Iterator, typename T>
  void winnow_heap(N<1> a, W<1> m, W<1> b, Iterator base, bool (*invr)(const T&, const T&)) {
  return;
}
