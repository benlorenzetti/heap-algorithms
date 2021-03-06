#include <string>
#include <cassert>
#include <iostream> // temp
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

template<int n> class N;

template<int n>
class W {
 public:
  unsigned int datum;
  
  string print() const;
  template<int n2>
  friend ostream& operator<<(ostream&, const W<n2>&);
  
  W() {}
  W(unsigned int x) { datum=x & MASK(); }
  template<int n2>
  W(N<n2> x) { datum=x.datum&MASK(); }
  
  constexpr unsigned int MASK() {
    if(n == 8*sizeof(unsigned int))
      return -1;
    else if(n == (8*sizeof(unsigned int))-1)
      return ~(1<<(8*sizeof(unsigned int)-2));
    else {
      unsigned int mask = (1<<n)-1;
      return mask;
    }
  }
  void INV() { datum=(~datum) & MASK(); }
  void NEG() { datum=(-datum) & MASK(); }
  void INC() { this->ADD(1); }
  void DEC() { this->ADD(-1); }
  W<sizeof(unsigned int)> LOG();
  void LEFT(unsigned int dist) { datum=(datum<<dist) & MASK(); }
  void RIGHT(unsigned int dist) { datum=(datum>>dist); }
  void AND(unsigned int x) { datum=datum&x; }
  void OR(unsigned int x) { datum=(datum|x)&MASK();  }
  void ADD(unsigned int x) { datum=(datum+x)&MASK(); }
  void SUB(unsigned int x) { datum=(datum-x)&MASK(); }
  void MUL(unsigned int x) { datum=(datum*x)&MASK(); }

  W<n>& operator++() { INC(); return *this; }
  W<n>& operator--() { DEC(); return *this; }
  template<int n2>
    W<n> operator<<(W<n2> dist) { W<n>r=*this; r.LEFT(dist.datum); return r; }
  template<int n2>
    W<n> operator>>(W<n2> dist) { W<n>r=*this; r.RIGHT(dist.datum); return r; }
  W<n> operator<<(unsigned int dist) { W<n>r=*this; r.LEFT(dist); return r; }
  W<n> operator>>(unsigned int dist) { W<n>r=*this; r.RIGHT(dist); return r; }
  template<int n2>
    W<n> operator+(const W<n2>& b) const {
    W<n> r = *this;
    r.ADD(b);
    return r;
  }
  template<int n2>
    W<n> operator-(const W<n2>& b) const {
    W<n> r = *this;
    r.SUB(b);
    return r;
  }
  W<n> operator-() const {
    W<n> r = *this;
    r.NEG();
    return r;
  }
  template<int n2>
    W<n> operator|(const W<n2>& x) const {
    W<n> r = *this;
    r.OR(x.datum);
    return r;
  }
  template<int n2>
    W<n> operator&(const W<n2>& x) const {
    W<n> r = *this;
    r.AND(x.datum);
    return r;
  }
  
  template<int n2>
    bool operator<(const W<n2>& b) const {return (datum < b.datum);}
  template<int n2>
    bool operator>(const W<n2>& b) const {return (datum > b.datum);}
  template<int n2>
    bool operator==(const W<n2>& b) const {return (datum == b.datum); }
  template<int n2>
    bool operator!=(const W<n2>& b) const {return (datum != b.datum); }
  template<int n2>
    bool operator<=(const W<n2>& b) const {return (datum <= b.datum); }
  template<int n2>
    bool operator>=(const W<n2>& b) const {return (datum >= b.datum); }
  
  template<int n2>
  W<n> LEFT(W<n2> dist) { LEFT(dist.datum); return *this; }
  template<int n2>
  W<n> RIGHT(W<n2> dist) { RIGHT(dist.datum); return *this; }
  template<int n2>
  W<n> AND(W<n2> x) { AND(x.datum); return *this; }
  template<int n2>
  W<n> OR(W<n2> x) { OR(x.datum); return *this; }
  template<int n2>
  W<n> ADD(W<n2> x) { ADD(x.datum); return *this; }
  template<int n2>
    W<n> SUB(W<n2> x) { SUB(x.datum); return *this; }
  template<int n2>
  W<n> MUL(W<n2> x) { MUL(x.datum); return *this; }
};

template<int n>
string W<n>::print() const {
  string s;
  W<n> w = *this;
  do {
    char c = '0' + w.datum % 10;
    w.datum /= 10;
    s.insert(s.begin(), c);
  } while (w.datum);
  return s;
}

template<int n>
  W<sizeof(unsigned int)> W<n>::LOG() {
  W<sizeof(unsigned int)> characteristic = 0;
  while(datum >>= 1)
    characteristic.INC();
  return characteristic;
}
