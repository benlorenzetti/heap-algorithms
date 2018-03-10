#include <string>
#include <cassert>
#include <iostream>
using namespace std;

/* Defines two complementary, portable unsigned integer classes,
 * whole numbers W = {0, 1, 2..., N-1} and natural numbers
 * N = {1, 2, 3..., N}, where N = 2^width.
 *
 * Both are represented in a datum of specified width in bits,
 * and the Natural representation is the 2's complement of the
 * usual Whole, unsigned integers. For width = 4:
 *
 *         (0)1001 =  Whole 9  =    + (8 + 1)
 *         (1)0111 = Natural 9 = 16 - (4 + 2 + 1)
*/

template<typename U, int n> class W;
template<typename U, int n> class N;

template<class Z1, class Z2>
Z1 operator&(Z1 z1, Z2 z2) { return z1.AND(z2); }

template<class Z1, class Z2>
Z1 operator|(Z1 z1, Z2 z2) { return z1.OR(z2); }

template<class Z>
Z operator~(Z z) { return z.INV(); }

template<class Z>
Z operator++(Z z) { return z.INC(); }

template<class Z>
Z operator--(Z z) { return z.DEC(); }

template<class Z1, class Z2>
Z1 operator+(Z1 z1, Z2 z2) { return z1.ADD(z2); }

template<class Z>
Z operator-(Z z) { return (~z + 1); }

template<class Z1, class Z2>
Z1 operator-(Z1 z1, Z2 z2) { return z1 +(-z2); }

template<class Z1, class Z2>
Z1 operator*(Z1 z1, Z2 z2) { return z1.MUL(z2); }

template<typename U, int n>
class N {
 public:
  U datum;
  N() {}
  N(U x) { datum=(-x)|MASK(); }
  template<typename U2, int n2>
  N(W<U2, n2> x) { datum=x.datum|MASK(); } 
  
  constexpr U MASK() { U mask=-1; mask<<=n; return mask; }
  N<U, n> INV() { datum=(~datum) | MASK(); return *this; }
  N<U, n> INC() { return this->ADD(-1); }
  N<U, n> DEC() { return this->ADD(1); }
  W<U, n> ROTATE_L(U dist) { datum=(datum<<dist) | MASK(); return *this; }
  W<U, n> ROTATE_R(U dist) { datum=(datum>>dist) | MASK(); return *this; }
  W<U, n> AND(U x) { datum=(datum&x)|MASK(); return *this; }
  W<U, n> OR(U x) { datum=datum|x; }
  W<U, n> ADD(U x) { datum=(datum+x)|MASK(); return *this; }
  W<U, n> MUL(U x) { datum=(datum*x)|MASK(); return *this; }
};

template<typename U, int n>
class W {
 public:
  U datum;
  
  string print() const;
  template<typename U2, int n2>
  friend ostream& operator<<(ostream&, const W<U2, n2>&);
  
  W() {}
  W(U x) { datum=x & MASK(); }
  template<typename U2, int n2>
  W(N<U2, n2> x) { datum=x.datum&MASK(); }
  
  constexpr U MASK() { U mask = (1<<n)/2-1; return mask; }
  W<U, n> INV() { datum=(~datum) & MASK(); return *this; }
  W<U, n> INC() { return this->ADD(1); }
  W<U, n> DEC() { return this->ADD(-1); }
  W<U, n> ROTATE_L(U dist) { datum=(datum<<dist) & MASK(); return *this;}
  W<U, n> ROTATE_R(U dist) { datum=(datum>>dist); return *this;}
  W<U, n> AND(U x) { datum=datum&x; return *this; }
  W<U, n> OR(U x) { datum=(datum|x)&MASK(); return *this; }
  W<U, n> ADD(U x) { datum=(datum+x)&MASK(); return *this; }
  W<U, n> MUL(U x) { datum=(datum*x)&MASK(); return *this; }

  template<typename U2, int n2>
  W<U, n> ROTATE_L(W<U2, n2> dist) { ROTATE_L(dist.datum); return *this; }
  template<typename U2, int n2>
  W<U, n> ROTATE_R(W<U2, n2> dist) { ROTATE_R(dist.datum); return *this; }
  template<typename U2, int n2>
  W<U, n> AND(W<U2, n2> x) { AND(x.datum); return *this; }
  template<typename U2, int n2>
  W<U, n> OR(W<U2, n2> x) { OR(x.datum); return *this; }
  template<typename U2, int n2>
  W<U, n> ADD(W<U2, n2> x) { ADD(x.datum); return *this; }
  template<typename U2, int n2>
  W<U, n> MUL(W<U2, n2> x) { MUL(x.datum); return *this; }
};

template<typename U, int n>
string W<U, n>::print() const {
  string s;
  W<U, n> w = *this;
  do {
    char c = '0' + w.datum % 10;
    w.datum /= 10;
    s.insert(s.begin(), c);
  } while (w.datum);
  return s;
}

template<typename U, int n>
ostream& operator<<(ostream& os, const W<U, n>& w) {
  os << w.print();
  return os;
}
