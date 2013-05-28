
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

ComplexArray *Operations::padCrop(ComplexArray const *a, int left, int top, int right, int bottom, Complex const& val)
{
    int const ww = a->width() + left + right;
    int const hh = a->height() + bottom + top;
    if (ww <= 0 || hh <= 0)
        return NULL;
    ComplexArray *nc = new ComplexArray(ww, hh);
    Complex *dst_ptr = nc->data();
    for (int p = 0; p < nc->width(); ++p, ++dst_ptr)
        memcpy(dst_ptr, &val, sizeof(Complex));
    for (int p = 1; p < nc->height(); ++p, dst_ptr += nc->width())
        memcpy(dst_ptr, nc->data(), nc->width() * sizeof(Complex));

    int const src_start_x = (left > 0) ? 0 : -left;
    int const dst_start_x = left > 0 ? left : 0;
    size_t const cpy_x_len = (a->width() + (right > 0 ? 0 : right) + (left > 0 ? 0 : left)) * sizeof(Complex);
    int const src_start_y = bottom > 0 ? 0 : -bottom;
    int const dst_start_y = bottom > 0 ? bottom : 0;
    int const cpy_y_len = a->height() + (top > 0 ? 0 : top) + (bottom > 0 ? 0 : bottom);

    dst_ptr = nc->data() + dst_start_x + nc->width() * dst_start_y;
    Complex const *src_ptr = a->data() + src_start_x + a->width() * src_start_y;
    for (int y = 0; y < cpy_y_len; ++y) {
        memcpy(dst_ptr, src_ptr, cpy_x_len);
        dst_ptr += nc->width();
        src_ptr += a->width();
    }
    nc->reset();
    return nc;
}
