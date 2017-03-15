#include <math.h>
#include <stdio.h>
#include "cordic.h"

int main()
{
	// Test bench
	float ftheta;
	for (ftheta = -2.f; ftheta <= 2.f; ftheta += 0.05f) {
		data_t theta = FtoI(ftheta);
		data_t out = 0;
		_cordic_tanh(theta, &out);
		data_t fout = FtoI(tanh(ftheta));
		printf("%6d/%5.2f\t%7.4f\t%d/%.2f\t%d/%.2f\n", theta, ftheta, \
				ItoF(out - fout), fout, ItoF(fout), out, ItoF(out));
	}
	return 0;
}
