#ifndef COMPLEX_ARRAY_INCLUDE
#define COMPLEX_ARRAY_INCLUDE

#include <global_defs.h>
#include <QString>
#include <display_info.h>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

class Calculator;

class ComplexArray {
 public:
    ComplexArray(int w, int h = 0);
    ComplexArray(QString const& source);
    ComplexArray(Calculator& calc, int ww, int hh, int n);
    ComplexArray(ComplexArray const& ca);
    ~ComplexArray();
    ComplexArray& operator=(ComplexArray const& ca);

    int width() const;
    int height() const;
    bool isValid() const;
    QString const& source() const;
    QString const& errorString() const;
    Complex const& value(int x, int y) const;
    Complex& value(int x, int y);
    QImage constructImage(DisplayInfo::ComplexComponent cmp, DisplayInfo::Scale scl,
                          DisplayInfo::Palette const& palette, double power = 0) const;
 private:
    ComplexArray();
    void ensure_capacity();
    void readFITS();
    void readImage();
    void setMinMax();

    long mem;
    int w;
    int h;
    int mh;
    Complex *vals;
    QString file;
    QString errString;
    bool fft;
    bool have_min_max;
    double maxCmp;
    double minCmp;
    double maxMag;
    double minMag;
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

inline Complex const& ComplexArray::value(int x, int y) const
{
    return vals[x * mh + y];
}

inline Complex& ComplexArray::value(int x, int y)
{
    return vals[x * mh + y];
}

inline QString const& ComplexArray::source() const
{
    return file;
}

#endif /* COMPLEX_ARRAY_INCLUDE */
