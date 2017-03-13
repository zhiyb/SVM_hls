#include <stdio.h>

#include "fp_cordic.h"
#include "fp_tanh.h"
#include "fp_svm.h"
#include "fp_type.h"

#define NUMUNR 4

void fp_svm(data_t x[N],            // Input of the classifier
            data_t *output          // Output of the classifier
            )
{
#pragma HLS ARRAY_PARTITION variable=x complete dim=1
#pragma HLS INTERFACE s_axilite port=output
#pragma HLS INTERFACE s_axilite port=x
#pragma HLS INTERFACE s_axilite port=return

        // Dot prodcut accumulator
        int acc_dp[NUMUNR];

        // Result
	data_t result;

        // Registers for tanh
        data_t reg_tanh[NUMUNR];

        // Support Vectors
        ap_fixed_type acc_sv[NUMUNR];
        // Result
        ap_fixed_type svm_res = 0;


init_sv_loop:
        for (int sv_init_index = 0; sv_init_index < NUMUNR; sv_init_index++)
        {
#pragma HLS UNROLL
                acc_sv[sv_init_index] = 0;
	}
acc_loop:
        for (int index_sv = 0; index_sv < NSV; index_sv = index_sv + NUMUNR)
        {
#pragma HLS PIPELINE
init_dp_loop:
                for (int dp_init_index = 0; dp_init_index < NUMUNR; dp_init_index++)
                {
                        acc_dp[dp_init_index] = 0;
		}
acc_dp_loop:
                for (int dp_index = 0; dp_index < N; dp_index++)
                {
unroll_dp_loop:
                        for (int dp_index_unroll = 0; dp_index_unroll < NUMUNR; dp_index_unroll++)
                        {
                                acc_dp[dp_index_unroll] += (sv_x[index_sv + dp_index_unroll][dp_index] * x[dp_index]) >> 12;
			}
		}
unroll_acc_loop:
                for (int sv_unroll_index = 0; sv_unroll_index < NUMUNR; sv_unroll_index++)
                {
                        fp_tanh(acc_dp[sv_unroll_index] << 1, &reg_tanh[sv_unroll_index]);
                        acc_sv[sv_unroll_index] += (alpha[index_sv + sv_unroll_index] * (reg_tanh[sv_unroll_index]) >> 12);
		}
	}
unroll_sum_loop:
        for (int sum_index = 0; sum_index < NUMUNR; sum_index++)
        {
#pragma HLS UNROLL
                svm_res += acc_sv[sum_index];
	}

	// Result computation from the sign of the sum
        svm_res = svm_res + bias;
        //printf("svm_res + b: %f\n", (float)svm_res);
        result = ((svm_res) >= 0) ? 0 : 1;

	// Output assignment
	*output = result;
}
