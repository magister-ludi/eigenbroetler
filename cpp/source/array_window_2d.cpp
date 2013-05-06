
#include <array_window_2d.h>
#include <QLineEdit>
#include <QMouseEvent>
#include <complex_array.h>
#include <scaled_plotter.h>

ArrayWindow2D::ArrayWindow2D(ComplexArray *cdata, DisplayInfo::ComplexComponent c,
                             DisplayInfo::Scale s,
                             DisplayInfo::ColourMap const& p):
    ArrayWindow(cdata, c, s)
{
    setColourMap(p);
}

ArrayWindow2D::~ArrayWindow2D()
{
}

void ArrayWindow2D::mouseData(QWidget const *w, QMouseEvent *evt)
{
    QPoint p(evt->x() - w->width() / 2, w->height() / 2 - evt->y() - 1);
    Complex const& val = d->value(evt->x(), w->height() - evt->y() - 1);
    status->setText(QString().sprintf("(%d, %d): RI: (%f, %f) MP: (%f, %f)",
        p.x(), p.y(),
        val.real(), val.imag(),
        abs(val), atan2(val.imag(), val.real())));
}

void ArrayWindow2D::redraw()
{
    QImage img = d->constructImage(cmp == DisplayInfo::REAL
                                   ? DisplayInfo::REAL
                                   : DisplayInfo::MAGN,
                                   scl, pal);
    left_plot->drawImage(0, 0, img);
    img = d->constructImage(cmp == DisplayInfo::REAL
                            ? DisplayInfo::IMAG
                            : DisplayInfo::PHSE,
                            DisplayInfo::LIN, pal);
    right_plot->drawImage(0, 0, img);
    left_plot->repaint();
    right_plot->repaint();
}
