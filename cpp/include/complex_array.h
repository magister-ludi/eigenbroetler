#ifndef COMPLEX_ARRAY_INCLUDE
#define COMPLEX_ARRAY_INCLUDE

#include <complex>

typedef std::complex<double> Complex;

class ComplexArray {
 public:
    ComplexArray();
    ComplexArray(int w, int h = 0);
    ComplexArray(ComplexArray const& ca);
    ~ComplexArray();
    ComplexArray& operator=(ComplexArray const& ca);

    int width() const;
    int height() const;
    Complex const& value(int x, int y) const;
    Complex& value(int x, int y);
 private:
    void ensure_capacity();
    long mem;
    int w;
    int h;
    int mh;
    Complex *vals;
};

inline ComplexArray::~ComplexArray()
{
    delete [] vals;
}

inline int ComplexArray::width() const
{
    return w;
}

inline int ComplexArray::height() const
{
    return h;
}

Complex const& ComplexArray::value(int x, int y) const
{
    return vals[x * mh + y];
}

Complex& ComplexArray::value(int x, int y)
{
    return vals[x * mh + y];
}

#endif /* COMPLEX_ARRAY_INCLUDE */
