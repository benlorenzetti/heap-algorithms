#include "integers.h"

#include <iostream>
using namespace std;

int main() {
	W<unsigned int, 6> w1(7);
	W<unsigned int, 8> w2(1);
        N<unsigned int, 8> n1(37);
	N<unsigned int, 7> n2(128);
	cout << "w1=" << w1 << ", w2=" << w2 << ", n1=" << n1;
	cout << ", n2=" << n2 << endl;
	cout << (w1<<w2) << endl;
	
}
