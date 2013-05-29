// -*- c++ -*-
#ifndef COMPLEX_OPERATIONS_INCLUDE
#define COMPLEX_OPERATIONS_INCLUDE

#include <global_defs.h>
#include <complex_array.h>

namespace Operations {
    enum ArithOp {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE
    };
ComplexArray *addDislocation(ComplexArray const *a, int order, double x, double y, bool use_abs);
ComplexArray *addPhase(ComplexArray const *a, int order, double x, double y);
ComplexArray *padCrop(ComplexArray const *a, int l, int t, int r, int b, Complex const& val = 0);
ComplexArray *resize(ComplexArray const *a, int ww, int hh);
ComplexArray *arithmetic(ComplexArray const *a,
                         int a_num, int a_den, int a_offs,
                         ArithOp op,
                         ComplexArray const *b,
                         int b_num, int b_den, int b_offs);
ComplexArray *hilbert(ComplexArray const *a, bool x_dir);
ComplexArray *demod(ComplexArray const *a, bool use_sel);
ComplexArray *demod_hilbert(ComplexArray const *a);
ComplexArray *removeDC(ComplexArray const *a);
ComplexArray *calculateEnergy(ComplexArray const *a, bool modified);
ComplexArray *extract_component(ComplexArray const *a, ComplexArray::Component component);
}
#endif /* COMPLEX_OPERATIONS_INCLUDE */
