// -*- c++ -*-
#ifndef COMPLEX_OPERATIONS_INCLUDE
#define COMPLEX_OPERATIONS_INCLUDE

#include <global_defs.h>

class ComplexArray;
namespace Operations {
ComplexArray *addDislocation(ComplexArray const *a, int order, double x, double y, bool use_abs);
ComplexArray *addPhase(ComplexArray const *a, int order, double x, double y);
ComplexArray *padCrop(ComplexArray const *a, int l, int t, int r, int b, Complex const& val = 0);
}
#endif /* COMPLEX_OPERATIONS_INCLUDE */
