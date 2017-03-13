#include <ap_fixed.h>
#include <ap_cint.h>
#include "cordic.h"

#define KP	3392	// 0.8281593609602f
#define KPI	/*4946*/	1.207497067763f

void cordic_tanh(mdata_t theta, ldata_t *out)
{
#pragma HLS INLINE
	static const ldata_t lut[] = {
		0.55, 0.26, 0.13, 0.063, 0.031, 0.016, 0.0078, 0.0039, 0.002, 0.00098, 0.00049, 0.00024, 0.00012,
		//1,	2,	3,	4,	5,	6,	7,
		//2250,	1046,	515,	256,	128,	64,	32,
		//8,	9,	10,	11,	12,	13,	14,
		//16,	8,	4,	2,	1,	1,	0,
	};

	static const int shift[] = {
		1, 1, 1, 1, 1, 2, 3, 4, 4, 5, 6, 7, 8
	};

	data_t x = KPI, y = 0;

loop:	for (int i = 0; i != sizeof(shift) / sizeof(shift[0]); i++) {
//#pragma HLS PIPELINE
#pragma HLS UNROLL
		int it = shift[i];
		int d = theta.is_neg() ? -1 : 1;
		ldata_t nx = x + ((d * y) >> it);	// mu = -1
		y = y + ((d * x) >> it);
		x = nx;
		theta = theta - (d * lut[it - 1u]);
	}

	*out = y / x;
}
