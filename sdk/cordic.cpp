#include "cordic.h"

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
	/*1, 1, 1, 1,*/ 1, 2, 3, 4, 4, //5, //6, 7, 8//, 9, 10, 11, 12, 13, 13,
};

void cordic(data_t theta, data_t *cosh, data_t *sinh)
{
	data_t x = KPI, y = 0;

	for (int i = 0; i != sizeof(shift) / sizeof(shift[0]); i++) {
		int it = shift[i];
		int d = theta >= 0 ? 1 : -1;
		data_t nx = x + ((d * y) >> it);	// mu = -1
		y = y + ((d * x) >> it);
		x = nx;
		theta = theta - d * lut[it - 1u];
	}

	*cosh = x;
	*sinh = y;
	//return (y << FRAC) / x;
}
