#include "test.h"
#include "cordic.h"

static inline data_t dotp(data_t *x, data_t *y)
{
	data_t sum = 0;
	for (size_t i = N; i != 0; i--)
		sum += (*x++ * *y++) >> FRAC;
	return sum;
}

static inline data_t k(data_t *psv, data_t *x)
{
	data_t dot = dotp(psv, x) * 2.d;
	return cordic_tanh(dot);
}

unsigned int test_cls_fixed()
{
	int err = 0;
	int *label = &testDataLabel[0];
	data_t *x = &testDataI[0][0];
	for (size_t ix = ASIZE(testData)  / N; ix != 0; ix--) {
		data_t sum = biasI;
		data_t *psv = &SVsI[0][0], *pa = &alphaI[0];
		for (size_t i = ASIZE(SVs) / N; i != 0; i--) {
			sum += (k(psv, x) * *pa++) >> FRAC;
			psv += 16;
		}
		err += (sum < 0) != *label++;
		x += N;
	}
	return err;
}
