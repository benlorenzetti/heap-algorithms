#include <string>
#include <cassert>

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
class W;
class Wimalis;
class N;

class Decimal {
 public:
  std::string str;
  Decimal();
  Decimal(const std::string &);
};

class W {
 public:
  unsigned int datum;
  unsigned int width;
  W();
  W(unsigned int);
  W(const W&);
};
class Wimalis : W {
 public:
  W frac;
  Wimalis();
  Wimalis(unsigned int, unsigned int);
  Wimalis(const W&, const W&);
};
class N : W {
 public:
  N();
  N(unsigned int);
  N(const W&);
};

Decimal::Decimal() { str.clear(); }
Decimal::Decimal(const std::string &s) { str = s; }
W::W() { width = 8 * sizeof(datum); }
W::W(unsigned int w) { width=w; }
W::W(const W &w) : W(w.datum) {}
Wimalis::Wimalis() : W() { frac.width = 8 * sizeof(frac.datum); }
Wimalis::Wimalis(unsigned int i_width, unsigned int f_width) : W(i_width) {
  frac.width = f_width;
}
Wimalis::Wimalis(const W &i_width, const W &f_width) : W(i_width) {
  frac.width = f_width.datum;
}
N::N() : W() {}
N::N(unsigned int w) : W(w) {}
N::N(const W &w) : W(w) {}
