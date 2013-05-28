// -*- c++ -*-
#ifndef PLOTTER_INCLUDE
#define PLOTTER_INCLUDE

#include <global_defs.h>
#include <QWidget>
#include <QPainter>

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

class ArrayWindow;

class Plotter: public QWidget {
public:
    Plotter(int w, int h, ArrayWindow *listener = NULL);
    ~Plotter();
protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    QImage pixmap;
public:
    QPainter painter;
private:
    Plotter(); // not implemented
    Plotter(Plotter const&); // not implemented
    Plotter& operator=(Plotter const&); // not implemented
    ArrayWindow *owner;

    Q_OBJECT
};

inline Plotter::~Plotter()
{
}

#endif /* PLOTTER_INCLUDE */
