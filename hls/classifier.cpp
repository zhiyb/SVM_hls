#include <ap_fixed.h>
#include <ap_cint.h>
#include "data.h"
#include "classifier.h"
#include "cordic.h"

static ldata_t SVs[1050][16] = {
#include "data_SVs.txt"
};

static mdata_t alpha[1050] = {
#include "data_alpha.txt"
};

static hdata_t bias = 231.344509;	//947587;

#define ASIZE(a)	(sizeof(a) / sizeof((a)[0]))

static void dotp(ldata_t x[N], ldata_t y[N], mdata_t *output)
{
//#pragma HLS ARRAY_PARTITION variable=x cyclic factor=16 dim=1
//#pragma HLS ARRAY_PARTITION variable=y cyclic factor=16 dim=1
//#pragma HLS PIPELINE
#pragma HLS INLINE
	mdata_t acc = 0, accs[N];
#pragma HLS ARRAY_PARTITION variable=accs cyclic factor=16 dim=1
loop:	for (int i = 0; i != N; i++) {
//#pragma HLS PIPELINE
#pragma HLS UNROLL
		accs[i] = x[i] * y[i];
		//acc += x[i] * y[i];
	}
	for (int i = 0; i != N; i++) {
#pragma HLS UNROLL
		acc += accs[i];
	}
	*output = acc;
}

static void k(ldata_t u[N], ldata_t v[N], ldata_t *output)
{
//#pragma HLS PIPELINE
#pragma HLS INLINE
	mdata_t res;
	dotp(u, v, &res);
	res = res << 1u;	// * 2
	//ldata_t cosh, sinh;
	//cordic(res, &cosh, &sinh);
	//*output = sinh / cosh;
	fp_tanh(res, output);
}

void classifier(ldata_t x[N], int *output)
{
//#pragma HLS ARRAY_PARTITION variable=x cyclic factor=16 dim=1
#pragma HLS ARRAY_RESHAPE variable=x cyclic factor=16 dim=1
#pragma HLS ARRAY_PARTITION variable=SVs cyclic factor=10 dim=1
//#pragma HLS ARRAY_PARTITION variable=SVs cyclic factor=16 dim=2
#pragma HLS ARRAY_RESHAPE variable=SVs cyclic factor=16 dim=2
#pragma HLS ARRAY_PARTITION variable=alpha cyclic factor=10 dim=1
#pragma HLS INTERFACE s_axilite port=output
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=x
	hdata_t sum = 0, sums[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#pragma HLS ARRAY_PARTITION variable=sums cyclic factor=10 dim=1
loop:	for (int i = 0; i != ASIZE(alpha); i++) {
#pragma HLS PIPELINE
#pragma HLS UNROLL factor=10
		ldata_t res;
		k(SVs[i], x, &res);
		sums[i % 10] += res * alpha[i];
		//sum += res * alpha[i];
	}
	for (int i = 0; i != 10; i++) {
#pragma HLS PIPELINE
#pragma HLS UNROLL
		sum += sums[i];
	}
	*output = sum + bias >= 0 ? 0 : 1;
}
