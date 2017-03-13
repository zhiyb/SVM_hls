#include <stdio.h>
#include <math.h>
#include "fp_svm.h"
#include "fp_type.h"
#include "fp_tanh.h"
#include "fp_cordic.h"

#include "formattedData.h"
#include "refout.h"
#include "golden.h"

#define NSVM 2000 // Number of tests
int main() {
	// Declare the files to save the data
        FILE *fpsvm, *fpsvmg;
	// Error count
	int error = 0;
	int error_gold = 0;

	// Variables for the test
	data_t output;
	data_t x[N];

	// Accumulator for the result of the sum
	float acc_sv;
	// Accumulator for the dot product
	float acc_dp;

        fpsvm = fopen("outIP.dat", "w");
        fpsvmg = fopen("outgold.dat", "w");

        for (int test_index = 0; test_index < NSVM; test_index++) {
                fprintf(fpsvm, " svm Test %d\n", test_index);
                fprintf(fpsvmg, " svm Test %d\n", test_index);

                // Copy the input test vector to the input
		for (int j = 0; j < N; j++) {
                        x[j] = testData[test_index][j];
		}

                // Compute the output value with the svm IP
                fp_svm(x, &output);
                fprintf(fpsvm, "Result { %d }\n\n", output);
                if(output != refout[test_index])
			error++;

                if(output != gold[test_index])
		{
                        fprintf(stdout, "gold: %d\n", gold[test_index]);
			fprintf(stdout, "output: %d\n", output);
                        fprintf(stdout, "refout: %d\n", refout[test_index]);
                        fprintf(stdout, "index: %d\n", test_index);
			error_gold++;
		}

		// Print the gold output value
                output = gold[test_index];
                fprintf(fpsvmg, "Result { %d }\n\n", output);
	}

        fclose(fpsvm);
        fclose(fpsvmg);
	fprintf(stdout, "Error Count: %d\n", error);
        fprintf(stdout, "Error rate: %f\n", (float)error/NSVM);
	fprintf(stdout, "Error gold: %d\n", error_gold);

	if (system("diff -w outIP.dat outgold.dat")) {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
		fprintf(stdout, "*******************************************\n");
		return 0;
	} else {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden output!\n");
		fprintf(stdout, "*******************************************\n");
		return 0;
	}

}
