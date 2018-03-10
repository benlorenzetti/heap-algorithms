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
template<typename U, int n>
class N {
 public:
  U datum;
  constexpr U MASK() { U mask = ~((1<<n)/2-1); return mask; }
  N() {}
  N(U x) { datum=-x | MASK(); }
  W<U, n> INV() { datum=(~datum) | MASK(); return *this; }
  W<U, n> ROTATE_L(U dist) { datum=(datum<<dist) | MASK(); return *this;}
  W<U, n> ROTATE_R(U dist) { datum=(datum>>dist) | MASK(); return *this;}
  W<U, n> AND(U x) { datum=datum&x|MASK(); return *this; }
  W<U, n> OR(U x) { datum=datum|x|MASK(); return *this; }
  W<U, n> ADD(U x) { datum=(datum+x)|MASK(); return *this; }
  W<U, n> INC() { return this->ADD(-1); }
  W<U, n> DEC() { return this->ADD(1); }
  W<U, n> MUL(U x) { datum=(datum*x)|MASK(); return *this; }
  string print() const;
//  template<typename U2, int n2>
//  friend ostream& operator<<(ostream&, const W<U2, n2>&);
};

