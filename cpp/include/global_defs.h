#ifndef GLOBAL_DEFS_INCLUDE
#define GLOBAL_DEFS_INCLUDE

#include <complex>
#include <iostream>

typedef std::complex<double> Complex;

namespace Constants {
    extern Complex const E;
    extern Complex const I;
    extern Complex const PI;
}

/* Provide some functions not supplied in std::complex... */
namespace NSComplex {
    Complex asin(Complex const &z);
    Complex acos(const Complex & z);
    Complex atan(const Complex & z);
    Complex atan2(const Complex & x, const Complex & y);
};

#endif /* GLOBAL_DEFS_INCLUDE */
