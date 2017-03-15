#ifndef DATA_H
#define DATA_H

#include <stdint.h>

#define N	16ul

#define FRAC	12ul
#define RATIO	(1ul << FRAC)

#define FtoI(f)	((data_t)round((double)(f) * (double)RATIO))
#define ItoF(i)	((double)((double)(i) / (double)RATIO))

typedef int32_t data_t;

#endif
