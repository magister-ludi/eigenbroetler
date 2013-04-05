
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
    if (!QFile::exists(file)) {
        QTextStream err(&errString);
        err << "\"" << file << "\" doesn't appear to exist!";
        return;
    }
    QFile datafile(file);
    if (!datafile.exists(file)) {
        QTextStream err(&errString);
        err << "\"" << datafile.fileName() << "\" doesn't appear to exist!";
        return;
    }
    int const BUFFER_SIZE = 20;
    char buffer[BUFFER_SIZE];
    if (!datafile.open(QIODevice::ReadOnly)) {
        QTextStream err(&errString);
        err << "Couldn't open \"" << file.constData() << "\" for reading";
    }
    if (BUFFER_SIZE != datafile.read(buffer, BUFFER_SIZE)) {
        std::cout << "line " << __LINE__ << std::endl;
        QTextStream err(&errString);
        err << "\"" << file << "\", unable to determine file type";
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
        QTextStream err(&errString);
        err << "File \"" << file << "\" load failed: unsupported format?";
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

    errString.clear();
    QTextStream err(&errString);
    if (fits_open_file(&f, file.toUtf8().constData(), READONLY, &status)) {
        err << "Can't open " << file << " for reading";
        return;
    }
    strcpy(key, "NAXIS");
    if (fits_read_key_lng(f, key, &naxis, comment, &status)) {
        err << file << " has no NAXIS key";
        return;
    }
    strcpy(key, "BITPIX");
    if (fits_read_key_lng(f, key, &bitpix, comment, &status)) {
        err << file << " has no BITPIX key";
        return;
    }
    axes = new long[naxis];
    ndata = 1;
    for (long i = 0; i < naxis && i < 3; i++) {
        snprintf(key, 80, "NAXIS%ld", i + 1);
        if (fits_read_key_lng(f, key, &axes[i], comment, &status)) {
            err << "Axis data missing for axis #" << i + 1 << " of " << naxis;
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
        err << "The data in " << file << " are badly formatted";
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
