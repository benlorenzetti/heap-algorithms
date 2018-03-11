#include <string>
#include <cassert>
#include <iostream> // temp?
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

template<typename U, int n>
class N {
 public:
  U datum;
  N() {}
  N(U x) { datum=(-x)|MASK(); }
  template<typename U2, int n2>
  N(W<U2, n2> x) { datum=x.datum|MASK(); } 
  string print() const;
  
  constexpr U MASK() { U mask=~((1<<n)-1); return mask; }
  N<U, n> INV() { datum=(~datum) | MASK(); return *this; }
  N<U, n> INC() { return this->ADD(-1); }
  N<U, n> DEC() { return this->ADD(1); }
  W<unsigned int, sizeof(unsigned int)> LOG();
  N<U, n> ROTATE_L(U dist) { datum=(datum<<dist) | MASK(); return *this; }
  N<U, n> ROTATE_R(U dist) { datum=(datum>>dist) | MASK(); return *this; }
  N<U, n> AND(U x) { datum=(datum&x)|MASK(); return *this; }
  N<U, n> OR(U x) { datum=datum|x; }
  N<U, n> ADD(U x) { datum=(datum+x)|MASK(); return *this; }
  N<U, n> MUL(U x) { datum=(datum*x)|MASK(); return *this; }
  N<U, n> DIV(U x);
  
  template<typename U2, int n2>
    N<U, n> ROTATE_L(W<U2, n2> dist) { ROTATE_L(dist.datum); return *this; }
  template<typename U2, int n2>
    N<U, n> ROTATE_R(W<U2, n2> dist) { ROTATE_R(dist.datum); return *this; }
  template<typename U2, int n2>
    N<U, n> AND(N<U2, n2> op2) { AND(op2.datum); return *this; }
  template<typename U2, int n2>
    N<U, n> OR(N<U2, n2> op2) { OR(op2.datum); return *this; }
  template<typename U2, int n2>
    N<U, n> ADD(N<U2, n2> op2) { ADD(op2.datum); return *this; }
  template<typename U2, int n2>
    N<U, n> MUL(N<U2, n2> op2) { MUL(op2.datum); return *this; }
};

template<typename U, int n>
  string N<U, n>::print() const {
  if(datum == MASK())
    return "N";
  W<U, n> w(*this);
  w = -w;
  return w.print();
}

template<typename U, int n>
  W<unsigned int, sizeof(unsigned int)> N<U, n>::LOG() {
  W<unsigned int, sizeof(unsigned int)> characteristic = 0;
  datum = ~datum;
  if(!datum)
    return characteristic;
  do characteristic.INC();
  while(datum >>= 1);
  return characteristic;
}
