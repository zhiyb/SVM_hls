#include "test.h"

void test_cls_hls_interrupt_pre()
{
	// Disable interrupt
	interrupt_enable(true);
}

unsigned int test_cls_hls_interrupt()
{
	unsigned int err = 0;
	int *label = &testDataLabel[0];
	data_t *x = &testDataI[0][0];
	static XClassifier_X_v data;
	for (size_t ix = ASIZE(testDataLabel); ix != 0; ix--) {
		u32 *p = &data.word_0;
		for (size_t i = N / 2; i != 0; i--) {
			*p++ = (*x) | (*(x + 1) << 16);
			x += 2;
		}
		XClassifier_Set_x_V(&cls, data);
		XClassifier_Start(&cls);
		interrupt_wait();
		err += (!XClassifier_Get_output_r(&cls)) != (!*label++);
	}
	return err;
}
