
#include <plotter.h>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <array_window.h>

Plotter::Plotter(int w, int h, ArrayWindow *listener):
    QWidget(NULL),
    pixmap(w, h, QImage::Format_ARGB32),
    painter(new QPainter(&pixmap)),
    owner(listener)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setBrush(Qt::NoBrush);
    setFixedSize(w, h);
    setForeground(Qt::white);
    setBackground(Qt::black);
    clear();

    if (owner)
        setMouseTracking(true);
}

Plotter::~Plotter()
{
    delete painter;
}

void Plotter::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    const QRect& r = e->rect();
    p.drawImage(r, pixmap, r);
}

void Plotter::setForeground(QColor const& c)
{
    QPen p(painter->pen());
    if (p.color() != c) {
        p.setColor(c);
        painter->setPen(p);
    }
}

void Plotter::setBackground(QColor const& c)
{
    bg = c;
}

void Plotter::clear()
{
    QColor fg = painter->pen().color();
    setForeground(bg);
    fillRectangle(0, 0, width(), height());
    setForeground(fg);
}

void Plotter::drawRectangle(int xx, int yy, int w, int h)
{
    painter->drawRect(xx, yy, w, h);
}

void Plotter::fillRectangle(int xx, int yy, int w, int h)
{
    painter->setBrush(getForeground());
    drawRectangle(xx, yy, w, h);
    painter->setBrush(Qt::NoBrush);
}

QColor Plotter::getForeground() const
{
    return painter->pen().color();
}

QColor Plotter::getBackground() const
{
    return bg;
}

void Plotter::drawLine(int x1, int y1, int x2, int y2)
{
    painter->drawLine(x1, y1, x2, y2);
}

void Plotter::drawImage(int xx, int yy, QImage const& img, bool centred)
{
    if (centred) {
        xx -= img.width();
        yy -= img.height();
    }
    QRectF i_r(0, 0, img.width(), img.height());
    QRectF to_r(xx, yy, img.width(), img.height());
#if 0
    // TODO: check whether adjustments are necessary
    if (xx < 0) {
        i_r.setLeft(-xx);
        i_r.setWidth(i_r.getWidth() - xx);
        to_r.setLeft(0);
        to_r.setWidth(to_r.getWidth() - xx);
        xx = 0;
    }
    if (yy < 0) {
        i_r.setTop(-yy);
        i_r.setHeight(i_r.getHeight() - yy);
        to_r.setTop(0);
        to_r.setHeight(to_r.getHeight() - yy);
        yy = 0;
    }
#endif
    painter->drawImage(to_r, img, i_r);
}

void Plotter::mouseMoveEvent(QMouseEvent *evt)
{
    if (owner)
        owner->mouseData(this, evt);
    evt->accept();
}
