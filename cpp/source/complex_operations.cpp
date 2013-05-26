
#include <complex_operations.h>
#include <complex_array.h>

ComplexArray *Operations::addDislocation(ComplexArray const *a, int order,
                                         double xOffset, double yOffset, bool use_abs)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreCol = (ww & 1) ? (ww - 1) / 2 : ww / 2;
    int const centreRow = (hh & 1) ? (hh - 1) / 2 : hh / 2;

    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; x++) {
        double const xx = x - centreCol - xOffset;
        for (int y = 0; y < hh; y++) {
            double const yy = y - centreRow - yOffset;
            double phase = order * atan2(yy, xx);
            if (use_abs)
                phase = fabs(phase);
            da->value(x, y) *= Complex(cos(phase), sin(phase));
        }
    }
    da->reset();
    return da;
}

ComplexArray *Operations::addPhase(ComplexArray const *a, int order, double x, double y)
{
    if (order < 0 || order > 2)
        return NULL;
    int const ww = a->width();
    int const hh = a->height();
    ComplexArray *da = new ComplexArray(*a);
    switch (order) {
    case 0:
        {
            double const dphase = 2.0 * M_PI * x;
            Complex const factor = Complex(cos(dphase), sin(dphase));
            for (int xx = 0; xx < ww; xx++)
                for (int yy = 0; yy < hh; yy++)
                    da->value(xx, yy) *= factor;
        }
        break;
    case 1:
    case 2:
        {
            int const centreCol = (ww & 1) ? (ww - 1) / 2 : ww / 2;
            int const centreRow = (hh & 1) ? (hh - 1) / 2 : hh / 2;
            for (int xx = 0; xx < ww; xx++) {
                double const c = (double) (xx - centreCol) / ww;
                for (int yy = 0; yy < hh; yy++) {
                    double const s = (double) (yy - centreRow) / hh;
                    double phase;
                    if (order == 2) {
                        double const r2 = x * c * c + y * s * s;
                        phase = 4.0 * M_PI * r2;
                    }
                    else
                        phase = 2.0 * M_PI * (x * c + y * s);
                    da->value(xx, yy) *= Complex(cos(phase), sin(phase));
                }
            }
        }
        break;
    }
    da->reset();
    return da;
}
