#include "cordic.h"

float cordic_tanh(float theta)
{
	data_t t = round(theta * (float)RATIO);
	_cordic_tanh(t, &t);
	return (float)t / (float)RATIO;
}
