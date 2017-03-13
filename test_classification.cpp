#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdint.h>

#include "alpha.h"
// #include "testData.h"
// #include "SV.h"
#include "fp_tanh.h"
#include "new_sv.h"
#include "refout.h"
#include "formattedData.h"

// #define Nsv 1050

int classify(int x[16]);

uint32_t Nsv = sizeof(sv_x)/(sizeof(sv_x[0][0]) * 16);
int main(){
	FILE *gold;
	int i;
	int x[16];
	uint32_t data_size = sizeof(testData) / (sizeof(testData[0][0]) * 16);
	uint32_t alpha_size = sizeof(alpha) / sizeof(alpha[0]);
	gold = fopen("golden.h", "w");
	printf("alpha: %u, \n", alpha_size);
	printf("Nsv: %u, \n", Nsv);
	printf("testData sets: %u, \n", data_size);
	fprintf(gold, "const int gold[%u] = {", data_size);
	int ClassResult[data_size];
	for (i=0; i<data_size; i++){
		int j;
		// printf("testpoint, %d\n", i);	
		// form input vector x
		for (j=0; j<16; j++)
		{	// x[j] = testData[i*16+j];
			//printf("testdata x, %d\n", testData[i][j]);	
			x[j] = testData[i][j];
		}
		// printf("testdata x\n");	
		

		// call the function
		ClassResult[i] = classify(x);

		if (i<20)
			// printf("Label: %d, Predicted: %d\n",testDataLabel[i], ClassResult[i]);
			printf("Label: %d, Predicted: %d\n",refout[i], ClassResult[i]);
	}

	// Summary statistics
	double error_rate = 0.0;
	int error = 0;
	for (i=0; i<data_size; i++){
		if(i != data_size -1)
			fprintf(gold, "%d,\n", ClassResult[i]);
		else
			fprintf(gold, "%d", ClassResult[i]);
		if (ClassResult[i] != refout[i])
		{
			// printf("error count %d", error);
			error++;
		}
	}
	fprintf(gold, "};");
	fclose(gold);
	error_rate = (double)error/data_size;

	printf("Classification Error Rate: %f\n", error_rate);
}


int classify(int x[16]){
	int i, j;
	double sum = 0.0;
	int tanhres;
	for (i=0; i<Nsv; i++){
		// calculate the dot product
		double temp;
		temp = 0.0;
		for (j=0; j<16; j++)
			temp = temp + x[j]*sv_x[i][j] / 4096;
		fp_tanh(2*temp, &tanhres);
		sum = sum + alpha[i]*(float)tanhres / 4096;
	}
	sum = sum + bias;

	if (sum>=0)
		return 0;
	else
		return 1;
}
