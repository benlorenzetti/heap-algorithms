#ifndef INTEGERS_H
#define INTEGERS_H
#include "whole.h"
#include "natural.h"
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

template<int n> class W;
template<int n> class N;

template<typename Z>
  W<sizeof(unsigned int)> log2(Z arg) {
  return arg.LOG();
}

template<int n>
  ostream& operator<<(ostream& os, const W<n>& w) {
  os << w.print();
  return os;
}

template<int n>
  ostream& operator<<(ostream& os, const N<n>& z) {
  os << z.print();
  return os;
}

template<class Z1, class Z2>
Z1 operator&(Z1 z1, Z2 z2) { return z1.AND(z2); }

template<class Z>
Z operator~(Z z) { return z.INV(); }

template<class Z1, class Z2>
Z1 operator*(Z1 z1, Z2 z2) { return z1.MUL(z2); }
/*
template<class Z1, class Z2>
  Z1 operator<<(Z1 z1, Z2 z2) { return z1.ROTATE_L(z2); }

template<class Z1, class Z2>
  Z1 operator>>(Z1 z1, Z2 z2) { return z1.ROTATE_R(z2); }
*/

template<class Z1, class Z2>
  bool operator==(const Z1& z1, const Z2& z2) {
  return (z1.datum == z2.datum);
}

#endif
