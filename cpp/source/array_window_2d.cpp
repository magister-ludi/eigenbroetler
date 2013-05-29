
#include <array_window_2d.h>
#include <QLineEdit>
#include <QMouseEvent>
#include <complex_array.h>
#include <scaled_plotter.h>

ArrayWindow2D::ArrayWindow2D(QList<ComplexArray *>& cdata, ComplexArray::Component c,
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
        Complex const& val = dlist.at(index)->d->value(evt->x(), w->height() - evt->y() - 1);
        status[1]->setText(QString().sprintf("(%d, %d): (%f, %f)",
                                             p.x(), p.y(),
                                             val.real(), val.imag()));
    }
    else
        status[1]->setText("");
}

void ArrayWindow2D::redraw()
{
    QList<DataSet *>::iterator it;
    for (it = dlist.begin(); it != dlist.end(); ++it) {
        ComplexArray *d = (*it)->d;
        (*it)->left = d->constructImage(cmp == ComplexArray::REAL
                                       ? ComplexArray::REAL
                                       : ComplexArray::MAGNITUDE,
                                       scl, pal, power);
        (*it)->right = d->constructImage(cmp == ComplexArray::REAL
                                ? ComplexArray::IMAGINARY
                                : ComplexArray::PHASE,
                                DisplayInfo::LINEAR, pal, power);
    }
    setViewIndex(index, true);
    QString componentStr;
    if (cmp == ComplexArray::REAL)
        componentStr = tr("real/imaginary");
    else
        componentStr = tr("magnitude/phase");
    QString scaleStr;
    switch(scl) {
    case DisplayInfo::LINEAR:
        scaleStr = tr("linear");
        break;
    case DisplayInfo::LOGARITHMIC:
        scaleStr = tr("logarithmic");
        break;
    case DisplayInfo::POWER_LAW:
        scaleStr = QString(tr("root (1/%1)").arg(power));
        break;
    }
    status[0]->setText(QString("(%1x%2): %3, %4").
                       arg(left_plot->width()).
                       arg(left_plot->height()).
                       arg(componentStr).
                       arg(scaleStr));
}
