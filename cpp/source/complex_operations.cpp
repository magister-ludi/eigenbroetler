
#include <complex_operations.h>
#include <complex_array.h>

ComplexArray *Operations::addDislocation(ComplexArray const *a, int order,
                                         double xOffset, double yOffset, bool use_abs)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = (ww & 1) ? (ww - 1) / 2 : ww / 2;
    int const centreY = (hh & 1) ? (hh - 1) / 2 : hh / 2;

    ComplexArray *da = new ComplexArray(*a);
    for (int x = 0; x < ww; ++x) {
        double const xx = x - centreX - xOffset;
        for (int y = 0; y < hh; ++y) {
            double const yy = y - centreY - yOffset;
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
            for (int xx = 0; xx < ww; ++xx)
                for (int yy = 0; yy < hh; ++yy)
                    da->value(xx, yy) *= factor;
        }
        break;
    case 1:
    case 2:
        {
            int const centreX = (ww & 1) ? (ww - 1) / 2 : ww / 2;
            int const centreY = (hh & 1) ? (hh - 1) / 2 : hh / 2;
            for (int xx = 0; xx < ww; ++xx) {
                double const c = (double) (xx - centreX) / ww;
                for (int yy = 0; yy < hh; ++yy) {
                    double const s = (double) (yy - centreY) / hh;
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

ComplexArray *Operations::resize(ComplexArray const *a, int ww, int hh)
{
    if (ww > a->width() || hh > a->height()) {
        int dw = (ww - a->width());
        int l = dw / 2;
        int r = l;
        if (r + l != dw)
            r += 1;
        int dh = (hh - a->height());
        int t = dh / 2;
        int b = l;
        if (b + t != dh)
            b += 1;
        return Operations::padCrop(a, l, t, r, b);
    }
    else
        return new ComplexArray(*a);
}

ComplexArray *Operations::arithmetic(ComplexArray const *a,
                                     int a_num, int a_den, int a_offs,
                                     ArithOp op,
                                     ComplexArray const *b,
                                     int b_num, int b_den, int b_offs)
{
    ComplexArray const *a_op;
    ComplexArray const *b_op;
    if (a->width() != b->width() || a->height() != b->height()) {
        a_op = resize(a, std::max(a->width(), b->width()), std::max(a->height(), b->height()));
        b_op = resize(b, std::max(a->width(), b->width()), std::max(a->height(), b->height()));
    }
    else {
        a_op = a;
        b_op = b;
    }
    ComplexArray *nc = new ComplexArray(a_op->width(), a_op->height());
    Complex *ptr = nc->data();
    Complex const *a_ptr = a_op->data();
    Complex const *b_ptr = b_op->data();
    int const count = a_op->width() * a_op->height();
    Complex const a_factor = Complex(double(a_num) / double(a_den));
    Complex const b_factor = Complex(double(b_num) / double(b_den));
    Complex const a_add = Complex(a_offs);
    Complex const b_add = Complex(b_offs);

    switch (op) {
    default:
    case 0:
        for (int i = 0; i < count; ++i, ++ptr, ++a_ptr, ++b_ptr)
            *ptr = ((*a_ptr) * a_factor + a_add) + ((*b_ptr) * b_factor + b_add);
        break;
    case 1:
        for (int i = 0; i < count; ++i, ++ptr, ++a_ptr, ++b_ptr)
            *ptr = ((*a_ptr) * a_factor + a_add) - ((*b_ptr) * b_factor + b_add);
        break;
    case 2:
        for (int i = 0; i < count; ++i, ++ptr, ++a_ptr, ++b_ptr)
            *ptr = ((*a_ptr) * a_factor + a_add) * ((*b_ptr) * b_factor + b_add);
        break;
    case 3:
        for (int i = 0; i < count; ++i, ++ptr, ++a_ptr, ++b_ptr) {
            Complex const div = (*b_ptr * b_factor + b_add);
            if (std::abs(div) < 1e-6)
                *ptr = 0;
            else
                *ptr = (*a_ptr * a_factor + a_add) / div;
        }
        break;
    }
    if (a->width() != b->width() || a->height() != b->height()) {
        delete a_op;
        delete b_op;
    }
    nc->reset();
    return nc;
}

ComplexArray *Operations::hilbert(ComplexArray const *d, bool x)
{
    ComplexArray *nd = new ComplexArray(*d);

    int const ww = nd->width();
    int const hh = nd->height();
    int const centre_x = (ww & 1) ? (ww - 1) / 2 : ww / 2;
    int const centre_y = (hh & 1) ? (hh - 1) / 2 : hh / 2;
    int sign;
    for (int yy = 0; yy < hh; ++yy) {
        for (int xx = 0; xx < ww; ++xx) {
            if (xx == centre_x)
                sign = (yy > centre_y) ? 1 : -1;
            else if (yy == centre_y)
                sign = (xx > centre_x) ? 1 : -1;
            else if (x)
                sign = (xx > centre_x) ? 1 : -1;
            else
                sign = (yy > centre_y) ? 1 : -1;
            nd->value(xx, yy) *= Complex(0, sign);
        }
    }
    nd->reset();
    return nd;
}

ComplexArray *Operations::extract_component(ComplexArray const *a, ComplexArray::Component component)
{
    int ww = a->width();
    int hh = a->height();
    ComplexArray *nd = new ComplexArray(ww, hh);

    for (int x = 0; x < ww; ++x) {
        for (int y = 0; y < hh; ++y) {
            switch (component) {
            case ComplexArray::REAL:
                nd->value(x, y) = Complex(a->value(x, y).real(), 0.0);
                break;
            case ComplexArray::IMAGINARY:
                nd->value(x, y) = Complex(a->value(x, y).imag(), 0.0);
                break;
            case ComplexArray::MAGNITUDE:
                nd->value(x, y) = Complex(std::abs(a->value(x, y)), 0.0);
                break;
            case ComplexArray::PHASE:
                nd->value(x, y) = Complex(arg(a->value(x, y)), 0.0);
                break;
            }
        }
    }
    nd->reset();
    return nd;
}

ComplexArray *Operations::calculateEnergy(ComplexArray const *a, bool modified)
{
    int const ww = a->width();
    int const hh = a->height();
    bool const ft = a->isFFT();
    int centreX = ww / 2;
    int centreY = hh / 2;
    ComplexArray *result[2];
    for (int cmp = 0; cmp < 2; ++cmp) {
        ComplexArray *component = (cmp == 0) ?
            extract_component(a, ComplexArray::REAL) :
            extract_component(a, ComplexArray::IMAGINARY);

        ComplexArray *noDC = removeDC(component);
        delete component;

        ComplexArray *h = new ComplexArray(ww, hh);
        for (int x = 0; x < ww; ++x) {
            double const iFactor = x - centreX;
            for (int y = 0; y < hh; ++y) {
                double const rFactor = y - centreY;
                double const pha = atan2(iFactor, rFactor);
                double const c = cos(pha);
                double const s = sin(pha);
                h->value(x, y) = (modified ? 1.0 : sqrt(iFactor * iFactor + rFactor * rFactor)) * Complex(c, s);
            }
        }

        ComplexArray *fs = noDC->dft(true);
        delete noDC;
        ComplexArray *hfs = arithmetic(h, 1, 1, 0, MULTIPLY, fs, 1, 1, 0);
        hfs->setFFT(!ft);
        ComplexArray *fhfs = hfs->dft(true);
        ComplexArray *hhfs = arithmetic(h, 1, 1, 0, MULTIPLY, hfs, 1, 1, 0);
        hhfs->setFFT(!ft);

        delete h;        // don't need this again
        delete hfs;      // don't need this again
        ComplexArray *fhhfs = hhfs->dft(true);
        delete hhfs;

        // junk DC component
        fs->value(centreX, centreY) = 0;
        ComplexArray *ss = fs->dft(true);
        delete fs;       // don't need this again

        hfs = arithmetic(fhfs, 1, 1, 0, MULTIPLY, fhfs, 1, 1, 0); // (Ds)^2
        delete fhfs;
        hfs->setFFT(ft);
        fs = arithmetic(ss, 1, 1, 0, MULTIPLY, fhhfs, 1, 1, 0); // s(D^2)s
        delete ss;
        delete fhhfs;
        fs->setFFT(ft);
        // (Ds)^2 - s(D^2)s
        result[cmp] = arithmetic(hfs, 1, 1, 0, SUBTRACT, fs, 1, 1, 0);

        delete fs;
        delete hfs;
    }

    ComplexArray *nd = arithmetic(result[0], 1, 1, 0, ADD, result[1], 1, 1, 0);
    delete result[0];
    delete result[1];
    nd->reset();
    return nd;
}

ComplexArray *Operations::demod(ComplexArray const *a, bool use_sel)
{
    int const ww = a->width();
    int const hh = a->height();
    ComplexArray *noDC = removeDC(a);
    ComplexArray *energy = calculateEnergy(a, true);

    ComplexArray *nd = new ComplexArray(ww, hh);

    for (int x = 0; x < ww; ++x) {
        for (int y = 0; y < hh; ++y) {
            double phase = 0.5 * arg(energy->value(x, y));
            nd->value(x, y) = Complex(cos(phase), sin(phase));
        }
    }
    delete energy;

    ComplexArray *ff = noDC->dft(true);
    ComplexArray *h2 = addDislocation(ff, 1, 0, 0, false);

    delete ff;
    ff = h2->dft(true);

    delete h2;
    h2 = arithmetic(ff, 1, 1, 0, MULTIPLY, nd, 1, 1, 0);

    delete ff;

    if (use_sel) {
        for (int x = 0; x < ww; ++x)
            for (int y = 0; y < hh; ++y)
                nd->value(x, y) = Complex(noDC->value(x, y).real(), h2->value(x, y).imag());
    }
    else {
        delete nd;
        nd = arithmetic(noDC, 1, 1, 0, ADD, h2, 1, 1, 0);
    }
    delete noDC;
    delete h2;

    nd->reset();
    return nd;
}

ComplexArray *Operations::demod_hilbert(ComplexArray const *a)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2;
    int const centreY = hh / 2;
    ComplexArray *ft = a->dft(true);

    for (int y = 0; y < hh; ++y) {
        for (int x = 0; x < ww; ++x) {
            int mul;
            if (x == centreX)
                mul = (y > centreY) ? 1 : 0;
            else if (y == centreY)
                mul = (x > centreX) ? 1 : 0;
            else
                mul = (x > centreX) ? 1 : 0;
            ft->value(x, y) = Complex(-mul * ft->value(x, y).imag(), mul * ft->value(x, y).real());
        }
    }
    ComplexArray *nd = ft->dft(true);
    delete ft;
    nd->reset();
    return nd;
}

ComplexArray *Operations::removeDC(ComplexArray const *a)
{
    int const ww = a->width();
    int const hh = a->height();
    int const centreX = ww / 2;
    int const centreY = hh / 2;

    ComplexArray *ft = a->dft(true);
    ft->value(centreX, centreY) = 0;
    ComplexArray *nd = ft->dft(true);
    delete ft;
    nd->reset();
    return nd;
}
