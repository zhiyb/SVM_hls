#ifndef FP_SVM_
#define FP_SVM_

#include "alpha.h"
#include "new_sv.h"

#define NSV 1050 // Number of support vectors in new_sv
#define N 16 // Data number in one set of Data


void fp_svm(data_t x[N], data_t *output);

#endif // FP_SVM_
