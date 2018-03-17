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

template<int n> class W;

template<int n>
class N {
 public:
  unsigned int datum;
  N() {}
  N(unsigned int x) { datum=(-x)|MASK(); }
  template<int n2>
  N(W<n2> x) { datum=x.datum|MASK(); } 
  string print() const;
  
  constexpr unsigned int MASK() {
    if(n == 8*sizeof(unsigned int))
      return 0;
    else if(n == 8*sizeof(unsigned int) - 1)
      return 1 << (8*sizeof(unsigned int)-2);
    else {
      unsigned int mask = ~((1<<n)-1);
      return mask;
    }
  }
  N<n> INV() { datum=(~datum) | MASK(); return *this; }
  N<n> INC() { return this->ADD(-1); }
  N<n> DEC() { return this->ADD(1); }
  W<sizeof(unsigned int)> LOG();
  N<n> LEFT(unsigned int dist) { datum=(datum<<dist) | MASK(); return *this; }
  N<n> RIGHT(unsigned int dist) { datum=(datum>>dist) | MASK(); return *this; }
  N<n> AND(unsigned int x) { datum=(datum&x)|MASK(); return *this; }
  N<n> OR(unsigned int x) { datum=datum|x; }
  N<n> ADD(unsigned int x) { datum=(datum+x)|MASK(); return *this; }
  N<n> SUB(unsigned int x) { datum=(datum-x)|MASK(); return *this; }
  N<n> MUL(unsigned int x) { datum=(datum*x)|MASK(); return *this; }
  N<n> DIV(unsigned int x);

  N<n>& operator++() { INC(); return *this; }
  N<n>& operator--() { DEC(); return *this; }
  template<int n2>
    N<n> operator<<(W<n2> dist) { return LEFT(dist.datum); }
  template<int n2>
    N<n> operator>>(W<n2> dist) { return RIGHT(dist.datum); }
  N<n> operator<<(unsigned int dist) { return LEFT(dist); }
  N<n> operator>>(unsigned int dist) { return RIGHT(dist); }

  template<int n2>
    bool operator<(const N<n2>& b) const {return (datum > b.datum);}
  template<int n2>
    bool operator>(const N<n2>& b) const {return (datum < b.datum);}
  template<int n2>
    bool operator==(const N<n2>& b) const {return (datum == b.datum);}
  template<int n2>
    bool operator<=(const N<n2>& b) const {return (datum >= b.datum);}
  template<int n2>
    bool operator>=(const N<n2>& b) const {return (datum <= b.datum);}
  
  template<int n2>
    N<n> LEFT(W<n2> dist) { LEFT(dist.datum); return *this; }
  template<int n2>
    N<n> RIGHT(W<n2> dist) { RIGHT(dist.datum); return *this; }
  template<int n2>
    N<n> AND(N<n2> op2) { AND(op2.datum); return *this; }
  template<int n2>
    N<n> OR(N<n2> op2) { OR(op2.datum); return *this; }
  template<int n2>
    N<n> ADD(N<n2> op2) { ADD(op2.datum); return *this; }
  template<int n2>
    N<n> SUB(N<n2> op2) { SUB(op2.datum); return *this; }
  template<int n2>
    N<n> MUL(N<n2> op2) { MUL(op2.datum); return *this; }
};

template<int n>
  string N<n>::print() const {
  if(datum == MASK())
    return "N";
  W<n> w(*this);
  w = -w;
  return w.print();
}

template<int n>
W<sizeof(unsigned int)> N<n>::LOG() {
  W<sizeof(unsigned int)> characteristic = 0;
  datum = ~datum;
  if(!datum)
    return characteristic;
  do characteristic.INC();
  while(datum >>= 1);
  return characteristic;
}
