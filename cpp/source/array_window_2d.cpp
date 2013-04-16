
#include <array_window_2d.h>
#include <QLineEdit>
#include <QMouseEvent>
#include <complex_array.h>
#include <scaled_plotter.h>
#include <iostream>

ArrayWindow2D::ArrayWindow2D(ComplexArray *cdata):
    ArrayWindow(cdata)
{
    DisplayInfo::Palette img_palette = DisplayInfo::createPalette();
    for (int i = 0; i < DisplayInfo::PALETTE_SIZE; ++i)
        img_palette[i] = QColor(i, i, i).rgb();
    QImage img = cdata->constructImage(DisplayInfo::REAL, DisplayInfo::LIN, img_palette);
    left_plot->drawImage(0, 0, img);
    img = cdata->constructImage(DisplayInfo::IMAG, DisplayInfo::LIN, img_palette);
    right_plot->drawImage(0, 0, img);

    palette_plot->setBackground(192, 192, 192);
    palette_plot->clear();
    palette_plot->setForeground(0, 0, 0);
    for (int i = 0; i < DisplayInfo::PALETTE_SIZE; i += 64)
        palette_plot->drawLine(0, i, palette_plot->width(), i);
    for (int i = 0; i < DisplayInfo::PALETTE_SIZE; ++i) {
        palette_plot->setForeground(img_palette[DisplayInfo::PALETTE_SIZE - i - 1]);
        palette_plot->drawLine(10, i, palette_plot->width() - 10, i);
    }
}

ArrayWindow2D::~ArrayWindow2D()
{
}

void ArrayWindow2D::mouseData(QWidget const *w, QMouseEvent *evt)
{
    Complex const& val = d->value(evt->x(), w->height() - evt->y() + 1);
    int xx = evt->x() - w->width() / 2;
    int yy = w->height() / 2 - evt->y();

    status->setText(QString().sprintf("(%d, %d): %+.2g %c i * %.2g",
                                      xx, yy,
                                      val.real(),
                                      val.imag() < 0 ? '-' : '+',
                                      fabs(val.imag())));
}
