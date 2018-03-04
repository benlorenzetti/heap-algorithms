#include <iostream>
#include <string>
#include <assert.h>
using namespace std;

class Decimal {
 public:
  string str;
  size_t dpt;
  Decimal();
  Decimal(const string &);
  int integral_trunc();
  int fraction_trunc();
  Decimal decimal_part() const;
  Decimal integer_part() const;
};
bool is_zero(const Decimal &);
ostream &operator<<(ostream &os, Decimal const &);
string carry_add(const string &, const string &, char);
pair <char, string> frac_add(const string &, const string &);
Decimal operator+(const Decimal &, const Decimal &);

Decimal::Decimal() { str.clear(), dpt=0; }
Decimal::Decimal(const string &s) {
  str = s;
  dpt = s.find('.');
  if(dpt == string::npos)
    dpt = str.size();
}

ostream &operator<<(ostream &os, Decimal const &d) {
  return os << d.str;
}

pair<char, string> frac_add(const string &d1, const string &d2) {
  pair<char, string> r;
  size_t u = max(d1.size(), d2.size());
  size_t i = min(d1.size(), d2.size());
  char carry = 0;
  if(d1.size() > d2.size())
    do {
      --u, r.second.push_back(d1[u]);
    } while(u > i);
  if(d1.size() < d2.size())
    do {
      --u, r.second.push_back(d2[u]);
    } while(u > i);
  if(i)
    do {
      --i;
      char x = d1[i] + d2[i] - '0' + carry;
      if(x > '9')
	carry = 1, x -= 10;
      else
	carry = 0;
      r.second.push_back(x);
    } while(i);
  r.first = carry;
  return r;
}

Decimal operator+(const Decimal &a, const Decimal &b) {
  Decimal d;
  pair<char, string> r = frac_add(a.decimal_part().str,
				  b.decimal_part().str);
  string l = carry_add(a.integer_part().str,
		       b.integer_part().str,
		       r.first);
  size_t n = l.size();
  while(n)
    d.str.push_back(l[--n]);
  if(n = r.second.size())
    d.dpt = d.str.size(), d.str.push_back('.');
  while(n)
    d.str.push_back(r.second[--n]);
  
  return move(d);
}

string carry_add(const string &d1, const string &d2, char carry) {
  string s;
  size_t i = d1.size();
  size_t j = d2.size();
  size_t intersect = min(i, j);
  if(intersect)
    do {
      --i, --j;
      char x = d1[i] + d2[j] + carry - '0';
      if(x > '9')
	carry = 1, x -= 10;
      else
	carry = 0;
      s.push_back(x);
    } while(--intersect);
  
  if(i)
    do {
      char x = d1[i] + carry;
      if(x > '9')
	carry = 1, x -= 10;
      else
	carry = 0;
      s.push_back(x);
    } while(--i);
  if(j)
    do {
      char x = d2[j] + carry;
      if(x > '9')
	carry = 1, x -= 10;
      else
	carry = 0;
      s.push_back(x);
    } while(--j);
  if(carry)
    s.push_back('1');
  return move(s);
}

bool is_zero(const Decimal &d) {
  for(int i=0; i<d.str.size(); i++)
    if(d.str[i] != '0' || d.str[i] != '.')
      return false;
  return true;
}

int Decimal::integral_trunc() {
  assert(!is_zero(*this));
  size_t trunc = 0;
  while(str[trunc] == '0' && str[trunc] != '.')
    ++trunc;
  str = str.substr(trunc, str.size() - trunc);
  dpt -= trunc;
  return dpt;
}

int Decimal::fraction_trunc() {
  if(dpt == str.size())
    return 0;
  while(str[str.size()-1] == '0' && str[str.size()-1] != '.')
    str.pop_back();
  return str.size() - 1 - dpt;
}

Decimal Decimal::decimal_part() const {
  Decimal d;
  if(dpt != str.size())
    d.str = str.substr(dpt+1, string::npos);
  d.dpt = d.str.size();
  return move(d);
}

Decimal Decimal::integer_part() const {
  Decimal d;
  cout << "integer_part(" << *this << ") = ";
  d.str = str.substr(0, dpt);
  d.dpt = d.str.size();
  cout << d << endl;
  return move(d);
}
