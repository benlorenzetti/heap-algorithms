#include "whole.h"

#include <iostream>
using namespace std;

int main() {
	W<unsigned int, 10> w6(0);
	W<unsigned int, 8> w3(255);
	cout << "AND(w3, w6) = " << (w3 & w6) << "\n";
}
