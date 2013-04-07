
#include <plotter.h>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>

Plotter::Plotter(int w, int h):
    QWidget(NULL),
    pixmap(w, h, QImage::Format_ARGB32),
    painter(new QPainter(&pixmap))
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setBrush(Qt::NoBrush);
    setFixedSize(w, h);
    setForeground(Qt::white);
    setBackground(Qt::black);
    clear();

    drawLine(0, 0, w, h);
    drawLine(0, h, w, 0);
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
