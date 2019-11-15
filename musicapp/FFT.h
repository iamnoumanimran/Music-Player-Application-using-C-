#pragma once
#include <complex>
using namespace std;

class FastFourierTransform {
	friend class Spectrum;
private:
	int count; 
public:
	FastFourierTransform(int c) { count = c; }
	void process(complex<float>*Data, int Log2N, int sign);
};


