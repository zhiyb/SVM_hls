#ifndef CORDIC_H
#define CORDIC_H

#include "data.h"

void _cordic_tanh(data_t theta, data_t *out);
float cordic_tanh(float theta);

#endif
