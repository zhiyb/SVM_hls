#include <stdio.h>
#include <math.h>
#include "classifier.h"
#include "testData.h"

#define ASIZE(a)	(sizeof(a) / sizeof((a)[0]))

int main()
{
	static const size_t size = ASIZE(testData) / N;
	double *pd = testData;
	int *pl = testDataLabel;
	size_t err = 0;
	for (unsigned int i = 0; i != size; i++) {
		data_t x[N];
		for (unsigned int j = 0; j != N; j++)
			x[j] = FtoI(*pd++);

		data_t res;
		classifier(x, &res);
		if (*pl != res)
			err++;
		printf("%u\t=> %d%s\n", i, res, *pl != res ? "\t<err>" : "");
		pl++;
	}
	printf("Error rate: %g\n", (double)err / (double)size);
	fprintf(stderr, "Error rate: %g\n", (double)err / (double)size);
	return 0;
}
