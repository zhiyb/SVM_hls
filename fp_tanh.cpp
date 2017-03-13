#include <stdio.h>

#include "fp_cordic.h"
#include "fp_tanh.h"

void fp_tanh(data_t input, data_t *output) {

        // Trigo index to extend range
        int trigo_index;
	// Input angle
        data_t theta = input;
        // Output of Cordic
        data_t outcosh, outsinh;

	const data_t cosh[6] = { 0x1000, 0x18B0, 0x3C31, 0xA115, 0x1B4EE, 0x4A35B };
	const data_t sinh[6] = { 0x0, 0x12CD, 0x3A07, 0xA049, 0x1B4A3, 0x4A340 };
	// Result to be assigned to the output
        data_t result, result_sinh, result_cosh;

        // Flag for determin whether the value is neg
        char neg;
        if (theta < 0) {
                neg = 1;
                theta = -theta;
	} else {
                neg = 0;
	}

        // Optimise the range of the cordic
        if (theta >= 24576) {
                result = 4096;
	} else {
                if (theta >= 20480) {
                        trigo_index = 5;
                } else if (theta >= 16384) {
                        trigo_index = 4;
                } else if (theta >= 12288) {
                        trigo_index = 3;
                } else if (theta >= 8192) {
                        trigo_index = 2;
                } else if (theta >= 4096) {
                        trigo_index = 1;
		} else {
                        trigo_index = 0;
		}
                theta = theta - trigo_index * 4096;

                // Call Cordic function
                fp_cordic(theta, &outcosh, &outsinh);

                // Trigo rules
                result_sinh = (sinh[trigo_index] * outcosh + cosh[trigo_index] * outsinh);
                result_cosh = (cosh[trigo_index] * outcosh + sinh[trigo_index] * outsinh) >> 12;
                result = result_sinh / result_cosh;
	}
	// Central symmetry correction
        result = neg ? -result : result;

	// Output assignment
	*output = result;
}
