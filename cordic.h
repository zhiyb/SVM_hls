#ifndef CORDIC_H
#define CORDIC_H

#include "data.h"

void cordic_tanh(mdata_t theta, ldata_t *out);
float f_cordic_tanh(float theta);

#endif
