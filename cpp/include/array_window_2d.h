// -*- c++ -*-
#ifndef ARRAY_WINDOW2D_INCLUDE
#define ARRAY_WINDOW2D_INCLUDE

#include <global_defs.h>
#include <array_window.h>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class ArrayWindow2D: public ArrayWindow {
 public:
    ArrayWindow2D(QList<ComplexArray *>& cdata, DisplayInfo::ComplexComponent c,
                  DisplayInfo::Scale s,
                  DisplayInfo::ColourMap const& p);
    ~ArrayWindow2D();
    void mouseData(QWidget const *w, QMouseEvent *evt);
 protected:
    virtual void redraw();
 private:
    ArrayWindow2D(); // not imlemented
    ArrayWindow2D(ArrayWindow2D const&); // not imlemented
    ArrayWindow2D& operator=(ArrayWindow2D const&); // not imlemented
    Q_OBJECT
};

#endif /* ARRAY_WINDOW2D_INCLUDE */
