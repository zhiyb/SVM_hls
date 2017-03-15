#include <stdio.h>
#include <math.h>
#include "alpha.h"
#include "SV.h"
#include "data.h"

#define ASIZE(a)	(sizeof(a) / sizeof((a)[0]))

int main()
{
	static const size_t size = ASIZE(alpha);

	printf("static data_t alpha[%llu] = {\n", size);
	double *p = alpha;
	for (size_t s = size; s != 0; s--)
		printf("\t%d,\n", FtoI(*p++));
	puts("};\n");

	printf("static data_t bias = %lu;\n\n", FtoI(bias));

	printf("static data_t SVs[%llu] = {\n", size * N);
	p = SVs;
	for (size_t s = size * N; s != 0; s--)
		printf("\t%d,\n", FtoI(*p++));
	puts("};\n");
	return 0;
}
