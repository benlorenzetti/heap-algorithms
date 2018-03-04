#include "decimal.h"
#include <iostream>
using namespace std;

int main() {

	Decimal d1("777.9934");
	Decimal d2("222.01");
	Decimal d3 = d1 + d2;
	cout << d1 << " + " << d2 << " = " << d3 << endl;
}
