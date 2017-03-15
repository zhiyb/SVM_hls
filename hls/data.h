#ifndef DATA_H
#define DATA_H

#include <stdint.h>
#include <ap_cint.h>
#include <ap_fixed.h>

#define N	16ul

//#define FRAC	12ul
//#define RATIO	(1ul << FRAC)

//#define FtoI(f)	((data_t)round((double)(f) * (double)RATIO))
//#define ItoF(i)	((double)((double)(i) / (double)RATIO))

typedef ap_fixed<22, 10> hdata_t;
typedef ap_fixed<19, 7> mdata_t;
typedef ap_fixed<16, 4> data_t;
typedef ap_fixed<16, 4> ldata_t;

#endif
