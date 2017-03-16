#include "test.h"
#include "cordic.h"

static inline data_t dotp(data_t *x, data_t *y)
{
	data_t sum = 0;
	for (size_t i = N; i != 0; i--)
		sum += (*x++ * (int16_t)(*y++)) >> FRAC;
	return sum;
}

static inline data_t fp_tanh(data_t theta)
{
	static const data_t cosh[6] = { 0x1000, 0x18B0, 0x3C31, 0xA115, 0x1B4EE, 0x4A35B };
	static const data_t sinh[6] = { 0x0, 0x12CD, 0x3A07, 0xA049, 0x1B4A3, 0x4A340 };

	// Result to be assigned to the output
	data_t result;
	// Flag for determine whether the value is negative
	char neg = theta < 0;
	if (neg)
		theta = -theta;

	// Optimise the range of the cordic
	if (theta >= 24576) {
		result = 4096;
	} else {
		// Trigo index to extend range
		int trigo_index;
		if (theta >= 20480) {
			trigo_index = 5;
		} else if (theta >= 16384) {
			trigo_index = 4;
		} else if (theta >= 12288) {
			trigo_index = 3;
		} else if (theta >= 8192) {
			trigo_index = 2;
		} else if (theta >= 4096) {
			trigo_index = 1;
		} else {
			trigo_index = 0;
		}
		theta = theta - (trigo_index << FRAC);
		// Call Cordic function
		data_t outcosh, outsinh;
		cordic(theta, &outcosh, &outsinh);
		// Trigo rules
		data_t result_sinh, result_cosh;
		result_sinh = (sinh[trigo_index] * outcosh + cosh[trigo_index] * outsinh);
		result_cosh = (cosh[trigo_index] * outcosh + sinh[trigo_index] * outsinh) >> 12;
		result = result_sinh / result_cosh;
	}

	// Central symmetry correction
	result = neg ? -result : result;
	// Output assignment
	return result;
}

static inline data_t k(data_t *psv, data_t *x)
{
	data_t dot = dotp(psv, x) * 2.d;
	return fp_tanh(dot);
	//return cordic_tanh(dot);
}

unsigned int test_cls_fixed()
{
	unsigned int err = 0;
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
