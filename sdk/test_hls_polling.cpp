#include <stdio.h>
#include <xtime_l.h>
#include "test.h"

void test_cls_hls_polling_pre()
{
	// Disable interrupt
	interrupt_enable(false);
}

unsigned int test_cls_hls_polling()
{
	XTime tick[4], perf[3] = {0, 0, 0};
	unsigned int err = 0;
	int *label = &testDataLabel[0];
	data_t *x = &testDataI[0][0];
	static XClassifier_X_v data;
	for (size_t ix = ASIZE(testDataLabel); ix != 0; ix--) {
		XTime_GetTime(&tick[0]);
		u32 *p = &data.word_0;
		for (size_t i = N / 2; i != 0; i--) {
			*p++ = (*x) | (*(x + 1) << 16);
			x += 2;
		}
		XClassifier_Set_x_V(&cls, data);
		XTime_GetTime(&tick[1]);
		XClassifier_Start(&cls);
		XTime_GetTime(&tick[2]);
		while (!XClassifier_IsReady(&cls));
		XTime_GetTime(&tick[3]);
		err += (!XClassifier_Get_output_r(&cls)) != (!*label++);
		perf[0] += tick[1] - tick[0];
		perf[1] += tick[2] - tick[1];
		perf[2] += tick[3] - tick[2];
	}
	printf("Data %llu, starting %llu, result %llu\r\n", perf[0], perf[1], perf[2]);
	return err;
}
