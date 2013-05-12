
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
    if (evt) {
        QPoint p(evt->x() - w->width() / 2, w->height() / 2 - evt->y() - 1);
        Complex const& val = d->value(evt->x(), w->height() - evt->y() - 1);
        status[1]->setText(QString().sprintf("(%d, %d): (%f, %f)",
                                             p.x(), p.y(),
                                             val.real(), val.imag()));
    }
    else
        status[1]->setText("");
}

void ArrayWindow2D::redraw()
{
    QImage img = d->constructImage(cmp == DisplayInfo::REAL
                                   ? DisplayInfo::REAL
                                   : DisplayInfo::MAGN,
                                   scl, pal, power);
    left_plot->drawImage(0, 0, img);
    img = d->constructImage(cmp == DisplayInfo::REAL
                            ? DisplayInfo::IMAG
                            : DisplayInfo::PHSE,
                            DisplayInfo::LIN, pal, power);
    right_plot->drawImage(0, 0, img);
    left_plot->repaint();
    right_plot->repaint();
    QString componentStr;
    if (cmp == DisplayInfo::REAL)
        componentStr = tr("real/imaginary");
    else
        componentStr = tr("magnitude/phase");
    QString scaleStr;
    switch(scl) {
    case DisplayInfo::LIN:
        scaleStr = tr("linear");
        break;
    case DisplayInfo::LOG:
        scaleStr = tr("logarithmic");
        break;
    case DisplayInfo::POW:
        scaleStr = QString(tr("root (1/%1)").arg(power));
        break;
    }
    status[0]->setText(QString("(%1x%2): %3, %4").
                       arg(left_plot->width()).
                       arg(left_plot->height()).
                       arg(componentStr).
                       arg(scaleStr));
}
