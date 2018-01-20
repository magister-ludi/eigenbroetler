
#include <complex_operations.h>
#include <complex_array.h>

struct scaleDataType {
    int newDim[2], nTiles[2];
    int paddedDim[2], offset[2];
    double scale, alpha[2];
    bool isFFT;
    bool x_direction;
};

static ComplexArray *scale_one_d(ComplexArray const *a, scaleDataType const& sc);

ComplexArray *Operations::chirpz_scale(ComplexArray const *a,
                                       int numX, int denomX, int numY, int denomY)
{
    ComplexArray *intrm = NULL;
    ComplexArray *newFits = NULL;

    /* horizontal scaling */
    if (numX != denomX) {
        scaleDataType sc;
        int const w = a->width();
        int const h = a->height();
        sc.newDim[0] = int(((double) w * numX) / denomX + 0.5);
        sc.newDim[1] = h;
        sc.nTiles[0] = int(ceil((double) sc.newDim[0] / w));
        sc.nTiles[1] = 1;
        sc.paddedDim[0] = 2 * w;
        sc.paddedDim[1] = h;
        sc.offset[0] = w / 2;
        sc.offset[1] = 0;
        sc.scale = sqrt(2.0);
        sc.alpha[0] = 2.0 * M_PI * denomX / (2.0 * w * numX);
        sc.alpha[1] = 0;
        sc.x_direction = true;
        sc.isFFT = a->isFFT();
        intrm = scale_one_d(a, sc);
    }
    else
        intrm = new ComplexArray(*a);

    if (intrm == NULL)
        return NULL;

    /* vertical scaling */
    if (numY != denomY) {
        scaleDataType sc;
        int w = intrm->width();
        int h = intrm->height();
        sc.newDim[0] = w;
        sc.newDim[1] = (int) (((double) h * numY) / denomY + 0.5);
        sc.nTiles[0] = 1;
        sc.nTiles[1] = (int) ceil((double) sc.newDim[1] / h);
        sc.paddedDim[0] = w;
        sc.paddedDim[1] = 2 * h;
        sc.offset[0] = 0;
        sc.offset[1] = h / 2;
        sc.alpha[0] = 0;
        sc.scale = sqrt(2.0);
        sc.alpha[1] = 2.0 * M_PI * denomY / (2.0 * h * numY);
        sc.x_direction = false;
        sc.isFFT = a->isFFT();
        newFits = scale_one_d(intrm, sc);
        delete intrm;
    }
    else
        newFits = intrm;
    newFits->setFFT(a->isFFT());
    return newFits;
}

template<typename T>
T SQR(T const& t)
{
    return t * t;
}

ComplexArray *scale_one_d(ComplexArray const *input, scaleDataType const& sc)
{
    ComplexArray *quadProd1 = NULL;
    ComplexArray *quadFFT1 = NULL;
    ComplexArray *inputFFT = NULL;
    ComplexArray *result = NULL;

    int const w = input->width();
    int const h = input->height();

    result = new ComplexArray(sc.newDim[0], sc.newDim[1]);
    if (result == NULL)
        return NULL;
    quadProd1 = new ComplexArray(sc.paddedDim[0], sc.paddedDim[1]);
    quadProd1->setAll(0.0);
    inputFFT = sc.x_direction ? input->xdft(true) : input->ydft(true);
    if (!quadProd1 || !inputFFT) {
        delete result;
        result = NULL;
        goto finished;
    }

    quadProd1->setFFT(!sc.isFFT);
    inputFFT->setFFT(sc.isFFT);

    for (int xx = 0; xx < w; xx++) {
        double const r2x = SQR(w / 2 - xx) * sc.alpha[0];
        for (int yy = 0; yy < h; yy++) {
            double const r2y = SQR(h / 2 - yy) * sc.alpha[1];
            quadProd1->value(xx + sc.offset[0], yy + sc.offset[1]) =
                inputFFT->value(xx, yy) * Complex(cos(r2x + r2y), -sin(r2x + r2y));
        }
    }
    delete inputFFT;
    inputFFT = NULL;
    quadFFT1 = sc.x_direction ? quadProd1->xdft(true) : quadProd1->ydft(true);
    if (quadFFT1 != NULL) {
        delete quadProd1;
        quadProd1 = NULL;
    }
    else {
        delete result;
        result = NULL;
        goto finished;
    }

    for (int deltaHor = 0; deltaHor < sc.nTiles[0]; ++deltaHor) {
        int const firstXpixel = (deltaHor == sc.nTiles[0] - 1 && sc.newDim[0] % w != 0) ?
            w - sc.newDim[0] % w : 0;
        for (int deltaVer = 0; deltaVer < sc.nTiles[1]; ++deltaVer) {
            int const firstYpixel = (deltaVer == sc.nTiles[1] - 1 && sc.newDim[1] % h != 0) ?
                h - sc.newDim[1] % h : 0;

            ComplexArray *quadProd2 = new ComplexArray(sc.paddedDim[0], sc.paddedDim[1]);
            if (quadProd2 == NULL) {
                delete result;
                result = NULL;
                goto finished;
            }
            quadProd2->setFFT(!sc.isFFT);
            for (int xx = 0; xx < sc.paddedDim[0]; xx++) {
                double const r2x = SQR(sc.paddedDim[0] / 2 - xx + deltaHor * w) * sc.alpha[0];
                for (int yy = 0; yy < sc.paddedDim[1]; yy++) {
                    double const r2y = SQR(sc.paddedDim[1] / 2 - yy + deltaVer * h) * sc.alpha[1];
                    quadProd2->value(xx, yy) = Complex(cos(r2x + r2y), sin(r2x + r2y));
                }
            }
            ComplexArray *quadFFT2 = sc.x_direction ? quadProd2->xdft(true) : quadProd2->ydft(true);
            if (quadFFT2 != NULL) {
                delete quadProd2;
                quadProd2 = NULL;
            }
            else {
                delete result;
                result = NULL;
                goto finished;
            }

            ComplexArray *fftProd = new ComplexArray(sc.paddedDim[0], sc.paddedDim[1]);
            if (fftProd == NULL) {
                delete result;
                delete quadProd2;
                result = quadProd2 = NULL;
                goto finished;
            }
            fftProd->setFFT(sc.isFFT);
            for (int xx = 0; xx < sc.paddedDim[0]; xx++)
                for (int yy = 0; yy < sc.paddedDim[1]; yy++)
                    fftProd->value(xx, yy) = quadFFT2->value(xx, yy) * quadFFT1->value(xx, yy);
            ComplexArray *conv = sc.x_direction ? fftProd->xdft(true) : fftProd->ydft(true);
            delete fftProd;
            fftProd = NULL;
            if (conv == NULL) {
                delete result;
                delete fftProd;
                delete quadProd2;
                result = fftProd = quadProd2 = NULL;
                goto finished;
            }
            for (int xx = firstXpixel; xx < w; xx++) {
                double const r2x = SQR(w / 2 - xx + deltaHor * w) * sc.alpha[0];
                for (int yy = firstYpixel; yy < h; yy++) {
                    double const r2y = SQR(h / 2 - yy + deltaVer * h) * sc.alpha[1];
                    result->value(
                                   (xx + (sc.newDim[0] - w) / 2 - deltaHor * w +
                                    sc.newDim[0]) % sc.newDim[0],
                                   (yy + (sc.newDim[1] - h) / 2 - deltaVer * h +
                                    sc.newDim[1]) % sc.newDim[1]) =
                        sc.scale * conv->value(xx + sc.offset[0], yy + sc.offset[1]) *
                        Complex(cos(r2x + r2y), -sin(r2x + r2y));
                }
            }
            delete conv;
            delete fftProd;
            delete quadFFT2;
            delete quadProd2;
            conv = fftProd = quadFFT2 = quadProd2 = NULL;
        }
    }
  finished:
    delete quadProd1;
    delete quadFFT1;
    delete inputFFT;

    return result;
}
