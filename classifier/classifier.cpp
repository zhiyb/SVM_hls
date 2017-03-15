#include "data.h"
#include "classifier.h"
#include "cordic.h"
#include "svm_data.h"

#define ASIZE(a)	(sizeof(a) / sizeof((a)[0]))

static inline void dotProduct(data_t x[N], data_t y[N], data_t *output)
{
	data_t acc = 0;
	uint32_t i;
loop:	for (i = 0; i != N; i++)
		acc += x[i] * y[i] / RATIO;
	*output = acc;
}

#include <math.h>
static inline void k(data_t u[N], data_t v[N], data_t *out)
{
	data_t res;
	dotProduct(u, v, &res);
	res = res << 1u;	// * 2
	//*out = FtoI(tanh(ItoF(res)));
	_cordic_tanh(res, out);
}

void classifier(data_t x[N], int *out)
{
	data_t res, *pa = alpha, *psv = SVs;
	data_t sum = 0;
loop:	for (int i = 0; i != ASIZE(alpha); i++) {
		k(psv, x, &res);
		sum += res * *pa++ / RATIO;
		psv += N;
	}
	*out = (sum + bias) >= 0 ? 0 : 1;
}
