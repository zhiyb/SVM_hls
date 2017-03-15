#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define FRAC	12
#define RATIO	(1ul << FRAC)
#define IT	15
#define SGN(v)	(-(int)(((v >> 30u) & 2u) - 1u))

#define KP	0.8281593609602f
#define KPI	1.207497067763f

typedef int32_t data_t;

static data_t lut[] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
};

static data_t shift[] = {
	1, 1, 1, 1, 2, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 13, 14, 15,
};

void cordicTanh(data_t theta, data_t *out)
{
	const data_t kpi = round(KPI * (float)RATIO);
	data_t x = kpi, y = 0;

	for (int i = 0; i != IT; i++) {
		int it = shift[i];
		//printf("iteration %d: step %d\n", i, it);
		int32_t d = SGN(theta);
		//printf("i%d: %d, %d, %d:%d\n", i, x, y, theta, d);
		data_t nx = x + ((d * y) >> it);	// mu = -1
		y = y + ((d * x) >> it);
		x = nx;
		theta = theta - d * lut[it - 1u];
	}

	*out = y * (data_t)RATIO / x;
}

int main()
{
	// Generate look-up table
	for (int i = 0; i != IT; i++) {
		float e = atanh(pow(2.f, -(float)i));
		lut[i] = round(e * (float)RATIO);
		printf("%.6g, ", e);
	}
	putchar('\n');
	for (int i = 0; i != IT; i++)
		printf("%d, ", lut[i]);
	putchar('\n');

	// Test bench
	float ftheta;
	for (ftheta = -10.f; ftheta < 10.f; ftheta += 0.5f) {
		data_t theta = round(ftheta * (float)RATIO);
		data_t out = 0;
		cordicTanh(theta, &out);
		data_t fout = round(tanh(ftheta) * (float)RATIO);
		printf("%6d/%5.2f\t%7.4f\t%d/%.2f\t%d/%.2f\n", theta, ftheta, \
				(float)(out - fout) / (float)RATIO,
				fout, (float)fout / (float)RATIO,
				out, (float)out / (float)RATIO);
	}
	return 0;
}
