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
	int16_t *x = &testDataI[0][0];
	for (size_t ix = ASIZE(testDataLabel); ix != 0; ix--) {
		XTime_GetTime(&tick[0]);
		XClassifier_Set_x_V(&cls, *(XClassifier_X_v *)x);
		XTime_GetTime(&tick[1]);
		XClassifier_Start(&cls);
		XTime_GetTime(&tick[2]);
		while (!XClassifier_IsReady(&cls));
		XTime_GetTime(&tick[3]);
		err += (!XClassifier_Get_output_r(&cls)) != (!*label++);
		perf[0] += tick[1] - tick[0];
		perf[1] += tick[2] - tick[1];
		perf[2] += tick[3] - tick[2];
		x += N;
	}
	printf("Data %llu, starting %llu, result %llu\r\n", perf[0], perf[1], perf[2]);
	return err;
}
