
#include <complex_array.h>
#include <limits>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <calculator.h>

// This #include must go last: otherwise it screws up UNICODE stuff
// in Visual Studio build.
#include <fitsio.h>

Calculator calculator;

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

ComplexArray::ComplexArray(Calculator& calc, int ww, int hh, int n):
    mem(0),
    w(ww),
    h(hh),
    vals(NULL),
    fft(false),
    have_min_max(true),
    maxCmp(-std::numeric_limits<double>::max()),
    minCmp(std::numeric_limits<double>::max()),
    maxMag(-std::numeric_limits<double>::max()),
    minMag(std::numeric_limits<double>::max())
{
    ensure_capacity();
    int const x_offs = ww >> 1;
    int const y_offs = mh >> 1;
    Complex *ptr = vals;
    for (int j = 0; j < mh; ++j) {
        int const y = j - y_offs;
        for (int i = 0; i < ww; ++i) {
            int const x = i - x_offs;
            *ptr = calc.eval(x, y, n);
            maxCmp = std::max(maxCmp, std::max(ptr->real(), ptr->imag()));
            minCmp = std::min(minCmp, std::min(ptr->real(), ptr->imag()));
            maxMag = std::max(maxMag, std::abs(*ptr));
            minMag = std::min(minMag, std::abs(*ptr));
            ++ptr;
        }
    }
}

inline bool testFilestring(char const *data, char const *test)
{
    return memcmp(data, test, strlen(test)) == 0;
}

ComplexArray::ComplexArray(QString const& file_name):
    mem(0),
    vals(NULL),
    //TODO: check which of these is correct.
    //file(QFile::decodeName(file_name.toAscii())),
    file(QFileInfo(QFile::decodeName(file_name.toUtf8())).absoluteFilePath()),
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

ComplexArray::ComplexArray(ComplexArray const& ca):
    mem(0),
    w(ca.w),
    h(ca.h),
    vals(NULL),
    fft(ca.fft),
    have_min_max(ca.have_min_max),
    maxCmp(ca.maxCmp),
    minCmp(ca.minCmp),
    maxMag(ca.maxMag),
    minMag(ca.minMag)
{
    ensure_capacity();
    memcpy(vals, ca.vals, mem * sizeof(Complex));
}

//    ComplexArray& ComplexArray::operator=(ComplexArray const& ca);

inline uchar rgb2grey(QRgb const& rgb)
{
    static double const RED_INTENSITY = 0.30;
    static double const GREEN_INTENSITY = 0.59;
    static double const BLUE_INTENSITY = 0.11;
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

void ComplexArray::ensure_capacity()
{
    mh = h ? h : 1;
    long req = w * mh;
    if (req > mem) {
        delete [] vals;
        mem = req;
        vals = (Complex *) fftw_malloc(req * sizeof(Complex));
    }
}

bool ComplexArray::save(QString const& filename)
{
    // save FITS format
    fitsfile *aFITSfile;
    int status = 0;
    long axes[3];
    char key[80];
    char comment[80];
    double *lpData = NULL;
    double *real;
    double *imag;
    bool result = true;
    Complex *ptr;
    int fft_val;

    axes[0] = w;
    axes[1] = mh;
    axes[2] = 2;
    lpData = new double[2 * w * mh];
    if (lpData == NULL)
        return false;
    real = lpData;
    imag = lpData + w * mh;
    ptr = vals;
    // TODO: This looks strange. Check it.
    for (int y = 0; y < mh; ++y) {
        for (int x = 0; x < w; ++x) {
            *(real++) = ptr->real();
            *(imag++) = ptr->imag();
            ++ptr;
        }
    }
    QFile ff(filename);
    ff.remove();

    if (fits_create_file(&aFITSfile, filename.toUtf8().constData(), &status)) {
        result = false;
        goto all_done;
    }

    if (fits_create_img(aFITSfile, DOUBLE_IMG, 3, axes, &status)) {
        result = false;
        goto all_done;
    }

    if (fits_write_img(aFITSfile, TDOUBLE, 1, w * mh * 2, lpData, &status)) {
        result = false;
        goto all_done;
    }

    strcpy(key, "ISFFT");
    strcpy(comment, "image in Fourier space?");
    fft_val = fft ? 1 : 0;
    if (fits_write_key(aFITSfile, TLOGICAL, key, &fft_val, comment, &status)) {
        result = false;
        goto all_done;
    }
    strcpy(comment, "CREATED BY AN EIGENBROETLER");
    if (fits_write_comment(aFITSfile, comment, &status)) {
        result = false;
        goto all_done;
    }

    if (fits_close_file(aFITSfile, &status)) {
        result = false;
        goto all_done;
    }

  all_done:
    delete [] lpData;
    if (result)
        file = filename;
    return result;
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

QImage ComplexArray::constructImage(DisplayInfo::ComplexComponent cmp, DisplayInfo::Scale scl,
                                    DisplayInfo::ColourMap const& colour_map, int inv_power) const
{
    QImage img(w, h, QImage::Format_Indexed8);
    if (!have_min_max) {
        ComplexArray *tmp = const_cast<ComplexArray *>(this);
        tmp->setMinMax();
    }
    img.setColorTable(colour_map);

    double (*scaler)(double x, double p) = scaleLinear;
    if (inv_power == 0)
        inv_power = 1;
    double power = 1.0 / fabs(inv_power);
    if (cmp == DisplayInfo::MAGN)
        switch (scl) {
        case DisplayInfo::LIN:
            scaler = scaleLinear;
            break;
        case DisplayInfo::LOG:
            scaler = scaleLogarithmic;
            break;
        case DisplayInfo::POW:
            scaler = scaleRoot;
            break;
        }
    int const ymax = h; // TODO: adjust for 1D
    double minValue;
    double maxValue;
    double scaleFactor;
    double offset;
    switch(cmp) {
    case DisplayInfo::REAL:
        {
            minValue = minCmp;
            maxValue = maxCmp;
            if (testEqual(maxValue, minValue)) {
                scaleFactor = (DisplayInfo::COLOURMAP_SIZE >> 1) - 1;
                offset = 0;
                minValue -= offset;
                maxValue -= offset;
            }
            else {
                offset = minValue;
                scaleFactor = (DisplayInfo::COLOURMAP_SIZE - 1) /
                    (scaler(maxValue - offset, power) - scaler(minValue - offset, power));
            }
            Complex const *ptr = vals;
            for (int y = ymax - 1; y >= 0; --y) {
                for (int x = 0; x < w; ++x) {
                    double const& v = ptr->real();
                    int const k = int(scaler(v - offset, power) * scaleFactor) & 0xff;
                    img.setPixel(x, y, k);
                    ++ptr;
                }
            }
        }
        break;
    case DisplayInfo::IMAG:
        {
            minValue = minCmp;
            maxValue = maxCmp;
            if (testEqual(maxValue, minValue)) {
                scaleFactor = (DisplayInfo::COLOURMAP_SIZE >> 1) - 1;
                offset = 0;
                minValue -= offset;
                maxValue -= offset;
            }
            else {
                offset = minValue;
                scaleFactor = (DisplayInfo::COLOURMAP_SIZE - 1) /
                    (scaler(maxValue - offset, power) - scaler(minValue - offset, power));
            }
            Complex const *ptr = vals;
            for (int y = ymax - 1; y >= 0; --y) {
                for (int x = 0; x < w; ++x) {
                    double const& v = ptr->imag();
                    int const k = int(scaler(v - offset, power) * scaleFactor) & 0xff;
                    img.setPixel(x, y, k);
                    ++ptr;
                }
            }
        }
        break;
    case DisplayInfo::PHSE:
        {
            scaler = scaleLinear;
            minValue = -M_PI;
            maxValue = +M_PI;
            offset = minValue;
            scaleFactor = (DisplayInfo::COLOURMAP_SIZE - 1) /
                (scaler(maxValue - offset, power) - scaler(minValue - offset, power));

            Complex const *ptr = vals;
            for (int y = ymax - 1; y >= 0; --y) {
                for (int x = 0; x < w; ++x) {
                    double const& v = atan2(ptr->imag(), ptr->real());
                    int const k = int(scaler(v - offset, power) * scaleFactor) & 0xff;
                    img.setPixel(x, y, k);
                    ++ptr;
                }
            }
        }
        break;
    case DisplayInfo::MAGN:
        {
            minValue = minMag;
            maxValue = maxMag;
            offset = scl == DisplayInfo::LOG ? 1 : 0;
            if (testEqual(maxValue, minValue)) {
                scaleFactor = (DisplayInfo::COLOURMAP_SIZE >> 1) - 1;
                offset = 0;
                maxValue = minValue;
            }
            else {
                offset = minValue - offset;
                scaleFactor = (DisplayInfo::COLOURMAP_SIZE - 1) /
                    (scaler(maxValue - offset, power) - scaler(minValue - offset, power));
            }
            Complex const *ptr = vals;
            for (int y = ymax - 1; y >= 0; --y) {
                for (int x = 0; x < w; ++x) {
                    double const& v = std::abs(*ptr);
                    int const k = int(scaler(v - offset, power) * scaleFactor) & 0xff;
                    img.setPixel(x, y, k);
                    ++ptr;
                }
            }
        }
        break;
    }
    return img;
}

ComplexArray *ComplexArray::dft(bool recentre) const
{
    ComplexArray *trf = new ComplexArray(*this);
    trf->have_min_max = false;
    double const scale = 1.0 / sqrt(double(mh * w));
    Complex *ptr = trf->vals;
    if (recentre) {
        for (int y = 0; y < mh; ++y) {
            for (int x = 0; x < w; ++x) {
                if ((y & 1) == (x & 1))
                    *ptr *= -1.0;
                ++ptr;
            }
        }
    }
    fftw_plan p = fftw_plan_dft_2d(trf->mh, trf->w,
                                   (fftw_complex *) trf->vals,
                                   (fftw_complex *) trf->vals,
                                   fft ? FFTW_FORWARD : FFTW_BACKWARD,
                                   FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    ptr = trf->vals;
    for (int y = 0; y < mh; ++y) {
        for (int x = 0; x < w; ++x) {
            *ptr *= (recentre && (y & 1) == (x & 1)) ? -scale : scale;
            ++ptr;
        }
    }
    trf->fft = !fft;
    trf->setMinMax();
    return trf;
}

ComplexArray *ComplexArray::xdft(bool recentre) const
{
    ComplexArray *trf = new ComplexArray(*this);
    double const scale = 1.0 / sqrt(double(w));
    Complex *ptr = trf->vals;
    if (recentre) {
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                //std::cout << "(" << x << ", " << y << "): " << (
                if (x & 1)
                    *ptr *= -1;
                ++ptr;
            }
        }
    }
    int const rank = 1;                 /* Tranform dimension is 1 */
    int const nd = trf->w;              /* Size of tranform */
    int const howmany = trf->h;         /* Number of transforms is h */
    int const stride = 1;
    int const dist = trf->w;
    fftw_plan p = fftw_plan_many_dft(rank, &nd, howmany,
                                     /*  in -> */ (fftw_complex *) trf->vals, NULL, stride, dist,
                                     /* out -> */ (fftw_complex *) trf->vals, NULL, stride, dist,
                                     fft ? FFTW_FORWARD : FFTW_BACKWARD,
                                     FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    trf->fft = !fft;
    ptr = trf->vals;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            *ptr *= ((x & 1) && recentre) ? -scale : scale;
            ++ptr;
        }
    }
    trf->have_min_max = false;
    trf->setMinMax();
    return trf;
}

ComplexArray *ComplexArray::ydft(bool recentre) const
{
    ComplexArray *trf = new ComplexArray(*this);
    if (h > 1) {
        double const scale = 1.0 / sqrt(double(h));
        Complex *ptr = trf->vals;
        if (recentre) {
            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    if (y & 1)
                        *ptr *= -1;
                    ++ptr;
                }
            }
        }
        int const rank = 1;                  /* Tranform dimension is 1 */
        int const nd = trf->h;               /* Size of tranform */
        int const howmany = trf->w;          /* Number of transforms is w */
        int const stride = trf->w;
        int const dist = 1;
        fftw_plan p = fftw_plan_many_dft(rank, &nd, howmany,
                                         /*  in -> */ (fftw_complex *) trf->vals, NULL, stride, dist,
                                         /* out -> */ (fftw_complex *) trf->vals, NULL, stride, dist,
                                         fft ? FFTW_FORWARD : FFTW_BACKWARD,
                                         FFTW_ESTIMATE);
        fftw_execute(p);
        fftw_destroy_plan(p);
        trf->fft = !fft;
        ptr = trf->vals;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                *ptr *= ((y & 1) && recentre) ? -scale : scale;
                ++ptr;
            }
        }
        trf->have_min_max = false;
        trf->setMinMax();
    }
    return trf;
}
