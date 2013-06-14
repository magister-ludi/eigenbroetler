
#include <complex_operations.h>
#include <complex_array.h>

ComplexArray *Operations::low_pass_rectangular(ComplexArray const *a, int xc, int yc, int r)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2 + xc;
    int const centreY = hh / 2 + yc;
    int const r2 = r * r;
    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        int const x0 = x - centreX;
        for (int y = 0; y < hh; ++y) {
            int const y0 = y - centreY;
            if (x0 * x0 + y0 * y0 > r2)
                da->value(x, y) = 0;
        }
    }
    return da;
}

ComplexArray *Operations::high_pass_rectangular(ComplexArray const *a, int xc, int yc, int r)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2 + xc;
    int const centreY = hh / 2 + yc;
    int const r2 = r * r;
    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        int const x0 = x - centreX;
        for (int y = 0; y < hh; ++y) {
            int const y0 = y - centreY;
            if (x0 * x0 + y0 * y0 <= r2)
                da->value(x, y) = 0;
        }
    }
    return da;
}

ComplexArray *Operations::gaussian_filter(ComplexArray const *a, int xc, int yc, int r)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2 + xc;
    int const centreY = hh / 2 + yc;
    int const r2 = r * r;
    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        int const x2 = (x - centreX) * (x - centreX);
        for (int y = 0; y < hh; ++y) {
            int const y2 = (y - centreY) * (y - centreY);
            da->value(x, y) *= exp(-(x2 + y2) / double(r2));
        }
    }
    return da;
}

ComplexArray *Operations::low_pass_butterworth(ComplexArray const *a, int xc, int yc, int r, int order)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2 + xc;
    int const centreY = hh / 2 + yc;
    int const r2 = r * r;
    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        int const x2 = (x - centreX) * (x - centreX);
        for (int y = 0; y < hh; ++y) {
            int const y2 = (y - centreY) * (y - centreY);
            da->value(x, y) *= 1.0 / (1.0 + pow((x2 + y2) / double(r2), order));
        }
    }
    return da;
}

ComplexArray *Operations::high_pass_butterworth(ComplexArray const *a, int xc, int yc, int r, int order)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2 + xc;
    int const centreY = hh / 2 + yc;
    int const r2 = r * r;
    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        int const x2 = (x - centreX) * (x - centreX);
        for (int y = 0; y < hh; ++y) {
            int const y2 = (y - centreY) * (y - centreY);
              if (x2 == 0 && y2 == 0)
                  da->value(x, y) = 0;
              else
                  da->value(x, y) *= 1.0 / (1.0 + pow(double(r2) / (x2 + y2), order));
        }
    }
    return da;
}

ComplexArray *Operations::low_pass_parzen(ComplexArray const *a, int xc, int yc, int r)
{
     int const ww = a->width();
     int const hh = a->height();
     int const centreX = ww / 2 + xc;
     int const centreY = hh / 2 + yc;
     int const r2 = r * r;
     ComplexArray *da = new ComplexArray(*a);
     for (int x = 0; x < ww; ++x) {
         int const x2 = (x - centreX) * (x - centreX);
         for (int y = 0; y < hh; ++y) {
             int const y2 = (y - centreY) * (y - centreY);
             int const rr2 = x2 + y2;
             if (rr2 > r2)
                 da->value(x, y) = 0;
             else
                 da->value(x, y) *= (r - sqrt(rr2)) / r;
         }
     }
     return da;
}

ComplexArray *Operations::high_pass_parzen(ComplexArray const *a, int xc, int yc, int r)
{
     int const ww = a->width();
     int const hh = a->height();
     int const centreX = ww / 2 + xc;
     int const centreY = hh / 2 + yc;
     int const r2 = r * r;
     ComplexArray *da = new ComplexArray(*a);
     for (int x = 0; x < ww; ++x) {
         int const x2 = (x - centreX) * (x - centreX);
         for (int y = 0; y < hh; ++y) {
             int const y2 = (y - centreY) * (y - centreY);
             int const rr2 = x2 + y2;
             if (rr2 <= r2)
                 da->value(x, y) *= sqrt(rr2) / r;
         }
     }
     return da;
}

ComplexArray *Operations::low_pass_welch(ComplexArray const *a, int xc, int yc, int r)
{
     int const ww = a->width();
     int const hh = a->height();
     int const centreX = ww / 2 + xc;
     int const centreY = hh / 2 + yc;
     int const r2 = r * r;
     ComplexArray *da = new ComplexArray(*a);
     for (int x = 0; x < ww; ++x) {
         int const x2 = (x - centreX) * (x - centreX);
         for (int y = 0; y < hh; ++y) {
             int const y2 = (y - centreY) * (y - centreY);
             int const rr2 = x2 + y2;
             if (rr2 > r2)
                 da->value(x, y) = 0;
             else {
                 double mul = (r - sqrt(rr2)) / r;
                 da->value(x, y) *= mul * mul;
             }
         }
     }
     return da;
}

ComplexArray *Operations::high_pass_welch(ComplexArray const *a, int xc, int yc, int r)
{
     int const ww = a->width();
     int const hh = a->height();
     int const centreX = ww / 2 + xc;
     int const centreY = hh / 2 + yc;
     int const r2 = r * r;
     ComplexArray *da = new ComplexArray(*a);
     for (int x = 0; x < ww; ++x) {
         int const x2 = (x - centreX) * (x - centreX);
         for (int y = 0; y < hh; ++y) {
             int const y2 = (y - centreY) * (y - centreY);
             int const rr2 = x2 + y2;
             if (rr2 <= r2) {
                 double mul = sqrt(rr2) / r;
                 da->value(x, y) *= mul * mul;
             }
         }
     }
     return da;
}

ComplexArray *Operations::low_pass_hanning(ComplexArray const *a, int xc, int yc, int r)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2 + xc;
    int const centreY = hh / 2 + yc;
    int const r2 = r * r;
    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        int const x2 = (x - centreX) * (x - centreX);
        for (int y = 0; y < hh; ++y) {
            int const y2 = (y - centreY) * (y - centreY);
            if (x2 + y2 > r2)
                da->value(x, y) = 0;
            else
                da->value(x, y) *= 0.5 * (1.0 - cos(M_PI * (r - sqrt(x2 + y2)) / r));
         }
     }
     return da;
}

ComplexArray *Operations::high_pass_hanning(ComplexArray const *a, int xc, int yc, int r)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2 + xc;
    int const centreY = hh / 2 + yc;
    int const r2 = r * r;
    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        int const x2 = (x - centreX) * (x - centreX);
        for (int y = 0; y < hh; ++y) {
            int const y2 = (y - centreY) * (y - centreY);
            if (x2 + y2 <= r2)
               da->value(x, y) *= 0.5 * (1.0 - cos(M_PI * sqrt(x2 + y2) / r));
         }
     }
     return da;
}

ComplexArray *Operations::low_pass_hamming(ComplexArray const *a, int xc, int yc, int r)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2 + xc;
    int const centreY = hh / 2 + yc;
    int const r2 = r * r;
    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        int const x2 = (x - centreX) * (x - centreX);
        for (int y = 0; y < hh; ++y) {
            int const y2 = (y - centreY) * (y - centreY);
            if (x2 + y2 > r2)
                da->value(x, y) = 0;
            else
                da->value(x, y) *= 0.54 - 0.46 * cos(M_PI * (r - sqrt(x2 + y2)) / r);
         }
     }
     return da;
}

ComplexArray *Operations::high_pass_hamming(ComplexArray const *a, int xc, int yc, int r)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2 + xc;
    int const centreY = hh / 2 + yc;
    int const r2 = r * r;
    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        int const x2 = (x - centreX) * (x - centreX);
        for (int y = 0; y < hh; ++y) {
            int const y2 = (y - centreY) * (y - centreY);
            if (x2 + y2 <= r2)
               da->value(x, y) *= 0.54 - 0.46 * cos(M_PI * sqrt(x2 + y2) / r);
         }
     }
     return da;
}
