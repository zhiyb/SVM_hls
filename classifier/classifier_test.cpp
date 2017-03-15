#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "classifier.h"
#include "testData.h"

#define ASIZE(a)	(sizeof(a) / sizeof((a)[0]))

void _cordic_report();

int main()
{
	FILE *fp = fopen("classifier.out", "w");
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
		fprintf(fp, "%u\t=> %d%s\n", i, res, *pl != res ? "\t<err>" : "");
		pl++;
	}
	fprintf(fp, "Error rate: %g\n", (double)err / (double)size);
	fprintf(stderr, "Error rate: %g\n", (double)err / (double)size);
	fflush(fp);
	fclose(fp);

	_cordic_report();

	puts("Comparing against reference data");
	if (system("diff -w classifier.out classifier.golden.out")) {
		puts("*********************************************");
		puts("FAIL: Output DOES NOT match the golden output");
		puts("*********************************************");
		return 1;
	} else {
		puts("*******************************************");
		puts("PASS: The output matches the golden output!");
		puts("*******************************************");
		return 0;
	}
}
