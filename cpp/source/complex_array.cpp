
#include <complex_array.h>
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
    fft(false)
{
}

ComplexArray::ComplexArray(int ww, int hh):
    mem(0),
    w(ww),
    h(hh),
    vals(NULL),
    fft(false)
{
    ensure_capacity();
}

inline bool testFilestring(char const *data, char const *test)
{
    return memcmp(data, test, strlen(test)) == 0;
}

ComplexArray::ComplexArray(QString const& source):
    mem(0),
    vals(NULL),
    file(QFile::decodeName(source.toAscii())),
    //file(QFile::decodeName(source.toUtf8())),
    fft(false)
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
    if (!img.isGrayscale())
        img = img.convertToFormat(QImage::Format_Mono);
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            value(x, y) = 0xff & img.pixel(x, y);
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
    for (long i = 0; i < naxis && i < 3; i++) {
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

    real = lpData;
    imag = lpData + w * h;
    ptr = vals;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            *ptr = Complex(*real, axes[2] >= 2 ? *imag : 0.0);
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

//    long mem;
//    int w;
//    int h;
//    Complex *vals;
