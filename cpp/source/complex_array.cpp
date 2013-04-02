
#include <complex_array.h>
#include <iostream>

ComplexArray::ComplexArray():
    mem(0),
    w(0),
    h(0),
    mh(0),
    vals(NULL)
{
}

ComplexArray::ComplexArray(int ww, int hh):
    mem(0),
    w(ww),
    h(hh),
    mh(hh ? hh : 1),
    vals(NULL)
{
    ensure_capacity();
}

//    ComplexArray(ComplexArray const& ca);

//    ComplexArray& operator=(ComplexArray const& ca);

void ComplexArray::ensure_capacity()
{
    long req = w * mh;
    if (req > mem) {
        delete [] vals;
        mem = req;
        vals = new Complex[req];
    }
}

//    long mem;
//    int w;
//    int h;
//    Complex *vals;
