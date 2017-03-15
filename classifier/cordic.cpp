#include "cordic.h"

#define SGN(v)	(((v >> 31u) & 1u) ? -1 : 1)

#define KP	3392	// 0.8281593609602f
#define KPI	4946	// 1.207497067763f

static data_t lut[] = {
	//0.55, 0.26, 0.13, 0.063, 0.031, 0.016, 0.0078, 0.0039, 0.002, 0.00098, 0.00049, 0.00024, 0.00012,
	//1,	2,	3,	4,	5,	6,	7,
	2250,	1046,	515,	256,	128,	64,	32,
	//8,	9,	10,	11,	12,	13,	14,
	16,	8,	4,	2,	1,	1,	0,
};

static data_t shift[] = {
	1, 1, 1, 2, 3, 4, 4, 5, 6, 7, 8//, 9, 10, 11, 12, 13, 13,
};

static data_t min = 0, max = 0;

void _cordic_tanh(data_t theta, data_t *out)
{
	data_t x = KPI, y = 0;

	if (theta < min)
		min = theta;
	if (theta > max)
		max = theta;

loop:	for (int i = 0; i != sizeof(shift) / sizeof(shift[0]); i++) {
		int it = shift[i];
		int d = SGN(theta);
		data_t nx = x + ((d * y) >> it);	// mu = -1
		y = y + ((d * x) >> it);
		x = nx;
		theta = theta - d * lut[it - 1u];
	}

	*out = y * (data_t)RATIO / x;
}

#include <stdio.h>
void _cordic_report()
{
	printf("%s min: %d(%g), max: %d(%g)\n", __func__, min, ItoF(min), max, ItoF(max));
}
