#ifndef COMPLEX_ARRAY_INCLUDE
#define COMPLEX_ARRAY_INCLUDE

#include <complex>
#include <QString>
#include <QTextStream>

typedef std::complex<double> Complex;

class ComplexArray {
 public:
    ComplexArray();
    ComplexArray(int w, int h = 0);
    ComplexArray(QString const& source);
    ComplexArray(ComplexArray const& ca);
    ~ComplexArray();
    ComplexArray& operator=(ComplexArray const& ca);

    int width() const;
    int height() const;
    bool isValid() const;
    QString const& errorString() const;
    Complex const& value(int x, int y) const;
    Complex& value(int x, int y);
 private:
    void ensure_capacity();
    void readFITS();
    void readImage();

    long mem;
    int w;
    int h;
    int mh;
    Complex *vals;
    QString file;
    QString errString;
    bool fft;
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

inline bool ComplexArray::isValid() const
{
    return errorString().isEmpty();
}

inline QString const& ComplexArray::errorString() const
{
    return errString;
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
