
#include <plotter.h>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <array_window.h>

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
}

void Plotter::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
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
