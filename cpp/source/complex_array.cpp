
#include <complex_array.h>
#include <limits>
#include <QFile>
#include <QImage>

#include <iostream>

// This #include must go last: otherwise it screws up UNICODE stuff
// in Visual Studio build.
#include <fitsio.h>

ComplexArray::ComplexArray():
    mem(0),
    w(0),
    h(0),
    mh(0),
    vals(NULL),
    fft(false),
    have_min_max(false)
{
}

ComplexArray::ComplexArray(int ww, int hh):
    mem(0),
    w(ww),
    h(hh),
    vals(NULL),
    fft(false),
    have_min_max(true),
    maxCmp(0),
    minCmp(0),
    maxMag(0),
    minMag(0)
{
    ensure_capacity();
}

inline bool testFilestring(char const *data, char const *test)
{
    return memcmp(data, test, strlen(test)) == 0;
}

ComplexArray::ComplexArray(QString const& file_name):
    mem(0),
    vals(NULL),
    file(QFile::decodeName(file_name.toAscii())),
    //file(QFile::decodeName(file_name.toUtf8())),
    fft(false),
    have_min_max(true)
{
    QFile datafile(file);
    if (!datafile.exists(file)) {
        errString = QString(QObject::tr("\"%1\" doesn't appear to exist!")).arg(datafile.fileName());
        return;
    }
    int const BUFFER_SIZE = 20;
    char buffer[BUFFER_SIZE];
    if (!datafile.open(QIODevice::ReadOnly)) {
        errString = QString(QObject::tr("Couldn't open \"%1\" for reading")).arg(file);
        return;
    }
    if (BUFFER_SIZE != datafile.read(buffer, BUFFER_SIZE)) {
        errString = QString(QObject::tr("Unable to determine file type of \"%1\"")).arg(file);
        return;
    }
    datafile.close();

    if (testFilestring(buffer, "SIMPLE  = "))
        readFITS();
    else
        readImage();
}

inline uchar rgb2grey(QRgb const& rgb)
{
    static double const RED_INTENSITY = 0.30;
    static double const GREEN_INTENSITY = 0.59;
    static double const BLUE_INTENSITY = 0.11;
    // rgb <-> #AARRGGBB
    return uchar(
                 ((rgb >> 16) & 0xff) *  RED_INTENSITY +
                 ((rgb >> 8) & 0xff) *  GREEN_INTENSITY +
                 ((rgb >> 0) & 0xff) *  BLUE_INTENSITY
                 );
}

void ComplexArray::readImage()
{
    QImage img;

    if (!img.load(file)) {
        errString = QString(QObject::tr("File \"%1\" load failed: unsupported format?")).arg(file);
        return;
    }
    w = img.width();
    h = img.height();
    ensure_capacity();

    maxCmp = -std::numeric_limits<double>::max();
    minCmp = 0;
    maxMag = -std::numeric_limits<double>::max();
    minMag = 0;
    Complex *ptr = vals;
    for (int y = h - 1; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            double const g = rgb2grey(img.pixel(x, y));
            *ptr = g;
            maxCmp = std::max(maxCmp, g);
            maxMag = std::max(maxMag, sqrt(g * g));
            ++ptr;
        }
    }
}

void ComplexArray::readFITS()
{
    long naxis;
    long bitpix;
    long *axes = NULL;
    double *lpData = NULL;
    fitsfile *f;
    int nulval = 0;
    int anynul;
    int status = 0;
    char comment[80];
    char key[80];
    double *real;
    double *imag;
    Complex *ptr;
    size_t ndata;

    if (fits_open_file(&f, file.toUtf8().constData(), READONLY, &status)) {
        errString = QString(QObject::tr("Can't open \"%1\" for reading")).arg(file);
        return;
    }
    strcpy(key, "NAXIS");
    if (fits_read_key_lng(f, key, &naxis, comment, &status)) {
        errString = QString(QObject::tr("\"%1\" has no NAXIS key")).arg(file);
        return;
    }
    strcpy(key, "BITPIX");
    if (fits_read_key_lng(f, key, &bitpix, comment, &status)) {
        errString = QString(QObject::tr("\"%1\" has no BITPIX key")).arg(file);
        return;
    }
    axes = new long[naxis];
    ndata = 1;
    for (long i = 0; i < naxis && i < 3; ++i) {
        snprintf(key, 80, "NAXIS%ld", i + 1);
        if (fits_read_key_lng(f, key, &axes[i], comment, &status)) {
            errString = QString(QObject::tr("Axis data missing for axis #%1 of %2")).arg(i + 1).arg(naxis);
            goto all_done;
        }
        switch (i) {
        case 0:
            w = int(axes[i]);
            break;
        case 1:
            h = int(axes[i]);
            break;
        }
        ndata *= axes[i];
    }
    ensure_capacity();
    lpData = new double[ndata];
    if (fits_read_img(f, TDOUBLE, 1, ndata, &nulval, lpData, &anynul, &status)) {
        errString = QString(QObject::tr("The data in \"%1\" are badly formatted")).arg(file);
        goto all_done;
    }

    maxCmp = -std::numeric_limits<double>::max();
    minCmp = std::numeric_limits<double>::max();
    maxMag = -std::numeric_limits<double>::max();
    minMag = std::numeric_limits<double>::max();
    real = lpData;
    imag = lpData + w * h;
    ptr = vals;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            *ptr = Complex(*real, axes[2] >= 2 ? *imag : 0.0);
            maxCmp = std::max(maxCmp, std::max(ptr->real(), ptr->imag()));
            minCmp = std::min(minCmp, std::min(ptr->real(), ptr->imag()));
            maxMag = std::max(maxMag, std::abs(*ptr));
            minMag = std::min(minMag, atan2(ptr->imag(), ptr->real()));
            ++ptr;
            ++real;
            ++imag;
        }
    }
    //TODO: setMinMax();
    strcpy(key, "ISFFT");
    int fftFlag;
    if (fits_read_key(f, TLOGICAL, key, &fftFlag, comment, &status)) {
        fft = fftFlag ? true : false;
        status = 0;
    }
  all_done:
    fits_close_file(f, &status);
    delete [] lpData;
    delete [] axes;
}

//    ComplexArray::ComplexArray(ComplexArray const& ca);

//    ComplexArray& ComplexArray::operator=(ComplexArray const& ca);

void ComplexArray::ensure_capacity()
{
    mh = h ? h : 1;
    long req = w * mh;
    if (req > mem) {
        delete [] vals;
        mem = req;
        //TODO: vals = (Complex *) fftw_malloc(a * sizeof(Complex));
        vals = new Complex[req];
    }
}

void ComplexArray::setMinMax()
{
    if (!have_min_max) {
        Complex const *ptr = vals;
        size_t const s = w * h;
        minCmp = std::numeric_limits<double>::max();
        maxCmp = -std::numeric_limits<double>::max();
        double const m0 = std::abs(*ptr);
        maxMag = m0;
        minMag = m0;
        for (size_t k = 0; k < s; ++k, ++ptr) {
            double const m = std::abs(*ptr);
            minCmp = std::min(std::min(minCmp, ptr->real()), ptr->imag());
            maxCmp = std::max(std::max(maxCmp, ptr->real()), ptr->imag());
            maxMag = std::max(maxMag, m);
            minMag = std::min(minMag, m);
        }
        have_min_max = true;
    }
}

inline double scaleLogarithmic(double x, double)
{
    return log10(x);
}

inline double scaleLinear(double x, double)
{
    return x;
}

inline double scaleRoot(double x, double p)
{
    return pow(x, p);
}

inline bool testEqual(double a, double b)
{
    static double const VERYSMALL = 1.0e-8;

    return ((a == b) ||
            (fabs(a - b) / std::max(fabs(a), fabs(b)) < VERYSMALL));
}

QImage ComplexArray::constructImage(DisplayInfo::ComplexComponent cmp, DisplayInfo::Scale /*scl*/,
                                    DisplayInfo::Palette const& palette, double power) const
{
    QImage img(w, h, QImage::Format_Indexed8);
    if (!have_min_max) {
        ComplexArray *tmp = const_cast<ComplexArray *>(this);
        tmp->setMinMax();
    }
    img.setColorTable(palette);

    //void	setPixel ( int x, int y, uint index_or_rgb )
    int const ymax = h; // TODO: adjust for 1D
    double minValue;
    double maxValue;
    double scaleFactor;
    double offset;
    switch(cmp) {
    case DisplayInfo::REAL:
        {
            double (*scaler)(double x, double p) = scaleLinear;
            minValue = minCmp;
            maxValue = maxCmp;
            if (testEqual(maxValue, minValue)) {
                scaleFactor = (DisplayInfo::PALETTE_SIZE >> 1) - 1;
                offset = 0;
                minValue -= offset;
                maxValue -= offset;
            }
            else {
                offset = minValue;
                scaleFactor = (DisplayInfo::PALETTE_SIZE - 1) /
                    (scaler(maxValue - offset, power) - scaler(minValue - offset, power));
            }
            Complex const *ptr = vals;
            for (int y = ymax - 1; y >= 0; --y) {
                for (int x = 0; x < w; ++x) {
                    double const& v = ptr->real();
                    int const k = int(scaler(v - offset, power) * scaleFactor);
                    img.setPixel(x, y, k);
                    ++ptr;
                }
            }
        }
        break;
    case DisplayInfo::IMAG:
        {
            double (*scaler)(double x, double p) = scaleLinear;
            minValue = minCmp;
            maxValue = maxCmp;
            if (testEqual(maxValue, minValue)) {
                scaleFactor = (DisplayInfo::PALETTE_SIZE >> 1) - 1;
                offset = 0;
                minValue -= offset;
                maxValue -= offset;
            }
            else {
                offset = minValue;
                scaleFactor = (DisplayInfo::PALETTE_SIZE - 1) /
                    (scaler(maxValue - offset, power) - scaler(minValue - offset, power));
            }
            Complex const *ptr = vals;
            for (int y = ymax - 1; y >= 0; --y) {
                for (int x = 0; x < w; ++x) {
                    double const& v = ptr->imag();
                    int const k = int(scaler(v - offset, power) * scaleFactor);
                    img.setPixel(x, y, k);
                    ++ptr;
                }
            }
        }
        break;
    case DisplayInfo::PHSE:
        break;
    case DisplayInfo::MAGN:
        break;
    }
    return img;
}
