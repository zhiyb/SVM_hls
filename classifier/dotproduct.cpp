#include <stdint.h>
#include "dotproduct.h"

void dotProduct(data_t x[N], data_t y[N],
		data_t *output)
{
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE s_axilite port=output
#pragma HLS INTERFACE s_axilite port=y
#pragma HLS INTERFACE s_axilite port=x
	data_t acc = 0;
	uint32_t i;
loop:	for (i = 0; i != N; i++)
		acc += x[i] * y[i];
	*output = acc;
}
