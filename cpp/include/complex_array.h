#ifndef COMPLEX_ARRAY_INCLUDE
#define COMPLEX_ARRAY_INCLUDE

#include <global_defs.h>
#include <QList>
#include <QString>
#include <fftw3.h>
#include <display_info.h>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

class Calculator;

class ComplexArray {
 public:
    ComplexArray(int w, int h = 0);
    ComplexArray(Calculator& calc, int ww, int hh, int n);
    ComplexArray(ComplexArray const& ca);
    ~ComplexArray();
    ComplexArray& operator=(ComplexArray const& ca);
    static QList<ComplexArray *> readFileData(QString const& source, QString& err, bool single = false);

    int width() const;
    int height() const;
    bool isValid() const;
    bool isFFT() const;
    QString const& source() const;
    QString const& errorString() const;
    Complex const& value(int x, int y) const;
    Complex& value(int x, int y);
    // TODO: Rewrite this method that can save
    // 2- or 3-dimensional FITS data
    bool save(QString const& filename);
    QImage constructImage(DisplayInfo::ComplexComponent cmp, DisplayInfo::Scale scl,
                          DisplayInfo::ColourMap const& colour_map, int inv_power = -1) const;
    // Mark min/max calculations as invalid
    void reset();
    // Operations
    ComplexArray *dft(bool recentre) const;
    ComplexArray *xdft(bool recentre) const;
    ComplexArray *ydft(bool recentre) const;
 private:
    ComplexArray();
    // TODO: Rewrite this constructor as a method that can read
    // 2- or 3-dimensional FITS data
    ComplexArray(QString const& file_name);
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
    fftw_free(vals);
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

inline bool ComplexArray::isFFT() const
{
    return fft;
}

inline QString const& ComplexArray::errorString() const
{
    return errString;
}

inline Complex const& ComplexArray::value(int x, int y) const
{
    return vals[x + y * w];
}

inline Complex& ComplexArray::value(int x, int y)
{
    return vals[x + y * w];
}

inline QString const& ComplexArray::source() const
{
    return file;
}

inline void ComplexArray::reset()
{
    have_min_max = false;
}

#endif /* COMPLEX_ARRAY_INCLUDE */
