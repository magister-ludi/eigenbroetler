// -*- c++ -*-
#ifndef GLOBAL_DEFS_INCLUDE
#define GLOBAL_DEFS_INCLUDE

#include <complex>
#include <iostream>
#include <QString>

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

// TODO: for debugging, will be removed
inline std::ostream& operator<<(std::ostream& o, QString const& s)
{
    return o << s.toLatin1().data();
}

#endif /* GLOBAL_DEFS_INCLUDE */
