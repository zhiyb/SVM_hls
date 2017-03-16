#ifndef TEST_H_
#define TEST_H_

#include <xclassifier.h>
#include "data.h"

#define TEST_SIZE	2000
#define SV_SIZE		1050

#define ASIZE(a)	(sizeof(a) / sizeof((a)[0]))

extern double SVs[SV_SIZE * N];
extern data_t SVsI[ASIZE(SVs) / N][N];
extern double alpha[SV_SIZE];
extern data_t alphaI[ASIZE(alpha)];
extern double bias;
extern data_t biasI;

extern double testData[TEST_SIZE * N];
extern int16_t testDataI[ASIZE(testData) / N][N];
extern int testDataLabel[TEST_SIZE];

extern XClassifier cls;

void interrupt_enable(bool e);
void interrupt_wait();

unsigned int test_cls_double();
unsigned int test_cls_fixed();
void test_cls_hls_polling_pre();
unsigned int test_cls_hls_polling();
void test_cls_hls_interrupt_pre();
unsigned int test_cls_hls_interrupt();

#endif /* TEST_H_ */
