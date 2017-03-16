#include "cordic.h"

static const mdata_t lut_cosh[6] = { 1.0, 1.54296875, 3.761962891, 10.06762695, 27.30810547, 74.2097168 };
static const mdata_t lut_sinh[6] = { 0.0, 1.175048828, 3.626708984, 10.01782227, 27.28979492, 74.203125 };

void fp_tanh(mdata_t theta, ldata_t *output)
{
#pragma HLS PIPELINE
//#pragma HLS INLINE
	// Result to be assigned to the output
	ldata_t result;
	// Flag for determine whether the value is negative
	int neg = theta < 0;
	if (neg)
		theta = -theta;

	// Optimise the range of the cordic
	if (theta >= (mdata_t)6.0) {
		result = 1.0;
	} else {
		// Trigo index to extend range
		int trigo_index = theta;
		theta = theta - trigo_index;
		// Call Cordic function
		ldata_t outcosh, outsinh;
		cordic(theta, &outcosh, &outsinh);
		// Trigo rules
		ldata_t sc, ss, cs, cc;
		sc = lut_sinh[trigo_index] * outcosh;
		ss = lut_sinh[trigo_index] * outsinh;
		cs = lut_cosh[trigo_index] * outsinh;
		cc = lut_cosh[trigo_index] * outcosh;
		mdata_t result_sinh, result_cosh;
		result_sinh = sc + cs;
		result_cosh = cc + ss;
		//result_sinh = (sinh[trigo_index] * outcosh + cosh[trigo_index] * outsinh);
		//result_cosh = (cosh[trigo_index] * outcosh + sinh[trigo_index] * outsinh);
		result = result_sinh / result_cosh;
	}

	// Central symmetry correction
	*output = neg ? (ldata_t)-result : result;
}
