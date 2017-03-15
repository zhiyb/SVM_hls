#include "test.h"

static inline double dotp(double *x, double *y)
{
	double sum = 0;
	for (size_t i = N; i != 0; i--)
		sum += *x++ * *y++;
	return sum;
}

static inline double k(double *psv, double *x)
{
	double dot = dotp(psv, x) * 2.d;
	return tanh(dot);
}

unsigned int test_cls_double()
{
	unsigned int err = 0;
	int *label = &testDataLabel[0];
	double *x = &testData[0];
	for (size_t ix = ASIZE(testData)  / N; ix != 0; ix--) {
		double sum = bias;
		double *psv = &SVs[0], *pa = &alpha[0];
		for (size_t i = ASIZE(SVs) / N; i != 0; i--) {
			sum += k(psv, x) * *pa++;
			psv += 16;
		}
		err += (sum < 0) != *label++;
		x += N;
	}
	return err;
}
