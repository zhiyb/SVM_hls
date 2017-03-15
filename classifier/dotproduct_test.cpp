#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dotproduct.h"

int main()
{
	FILE *fp = fopen("result.dat", "w");
	FILE *fpg = fopen("result.golden.dat", "w");
	data_t x[N], y[N];
	int i, j;
	srand(666);
	for (i = 0; i != 600; i++) {
		for (j = 0; j != N; j++) {
			x[j] = rand();
			y[j] = rand();
		}
		data_t output;
		// Execute the function with latest input
		dotProduct(x, y, &output);
		// Save the results.
		fprintf(fp, "%i %d\n", i, output);
		// Generate reference result
		output = 0;
		for (j = 0; j != N; j++)
			output += x[j] * y[j];
		// Save the results.
		fprintf(fpg, "%i %d\n", i, output);
	}
	fclose(fp);
	fclose(fpg);

	printf("Comparing against output data \n");
	if (system("diff -w result.dat result.golden.dat")) {

		fprintf(stdout,
			"*******************************************\n");
		fprintf(stdout,
			"FAIL: Output DOES NOT match the golden output\n");
		fprintf(stdout,
			"*******************************************\n");
		return 1;
	} else {
		fprintf(stdout,
			"*******************************************\n");
		fprintf(stdout,
			"PASS: The output matches the golden output!\n");
		fprintf(stdout,
			"*******************************************\n");
		return 0;
	}
}
