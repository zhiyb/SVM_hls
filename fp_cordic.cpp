#include "fp_type.h"
#include "fp_cordic.h"

#define N_ITER 12
#define K  0x1348        // 1.2073 initial value

void fp_cordic(data_t input, data_t *outputx, data_t *outputy) {

        // Controller for running the 4th iteration more than one time
        char controller = 0;

	data_t x = K;
	data_t y = 0;
	data_t x_new;
        data_t theta = input;

        // Stored values for 12 fix point tanh
        const data_t atanh[12] = { 0x08CA, 0x0416, 0x0202, 0x0100, 0x0080, 0x0064, 0x0032, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001 };

        // Cordic
fp_cordic: for (int i = 1; i <= N_ITER; i++) {
                // theta<0 anti-clockwise
                if (theta < 0) {
			x_new = x - (y >> i);
			y -= x >> i;
                        theta += atanh[i - 1];
		}
                // theta>0 clockwise
		else {
			x_new = x + (y >> i);
			y += (x >> i);
                        theta -= atanh[i - 1];
		}
		x = x_new;
                // 4th iteration repeated
		if ((i == 4)&& controller == 0 ) {
			controller++;
			i--;
		}

	}

	// Assignments of the outputs
	*outputx = x;
	*outputy = y;
}
