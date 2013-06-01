
#include <plotter.h>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <array_window.h>

//static QColor const maskColour(10, 36, 106);
static double const ft = 1.4;
static QColor const maskColour(int(ft * 10), int(ft * 36), int(ft * 106));

Plotter::Plotter(int w, int h, ArrayWindow *listener):
    QWidget(NULL),
    pixmap(w, h, QImage::Format_ARGB32),
    painter(&pixmap),
    owner(listener)
{
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setRenderHint(QPainter::TextAntialiasing);
    setFixedSize(w, h);
    if (owner)
        setMouseTracking(true);
    setVisibleRegion();
}

void Plotter::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    p.setBrush(maskColour);
    p.setPen(QPen(maskColour));
    p.drawRect(0, 0, width(), height());
    p.setClipRegion(mask);

    const QRect& r = e->rect();
    p.drawImage(r, pixmap, r);
}

void Plotter::mouseMoveEvent(QMouseEvent *evt)
{
    if (owner)
        owner->mouseData(this, evt);
    evt->accept();
}

void Plotter::leaveEvent(QEvent *)
{
    if (owner)
        owner->mouseData(this, NULL);
}

void Plotter::setVisibleRegion(QRegion const& m)
{
    //QRegion m_overlap = m.subtracted(QRegion(0, 0, width(), height()));
    mask = m;
}

void Plotter::setHiddenRegion(QRegion const& m)
{
    setVisibleRegion();
    //QRegion m_overlap = m.intersected(QRegion(0, 0, width(), height()));
    //mask = mask.subtracted(m_overlap);
    mask = mask.subtracted(m);
}

void Plotter::setVisibleRegion()
{
    mask = QRegion(0, 0, width(), height());
}
