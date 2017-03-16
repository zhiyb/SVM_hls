#ifndef CORDIC_H
#define CORDIC_H

#include "data.h"

void cordic(mdata_t theta, ldata_t *cosh, ldata_t *sinh);
float f_cordic_tanh(float theta);
void fp_tanh(mdata_t theta, ldata_t *output);

#endif
