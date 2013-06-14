// -*- c++ -*-
#ifndef PLOTTER_INCLUDE
#define PLOTTER_INCLUDE

#include <global_defs.h>
#include <QWidget>
#include <QPainter>

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
class QRegion;
QT_END_NAMESPACE

class ArrayWindow;

class Plotter: public QWidget {
public:
    Plotter(int w, int h, ArrayWindow *listener = NULL);
    ~Plotter();
    void setHiddenRegion(QRegion const& m);
    void setVisibleRegion(QRegion const& m);
    void setVisibleRegion();
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
    QRegion mask;

    Q_OBJECT
};

inline Plotter::~Plotter()
{
}

#endif /* PLOTTER_INCLUDE */
