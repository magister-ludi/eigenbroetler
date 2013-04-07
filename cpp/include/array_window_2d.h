#ifndef ARRAY_WINDOW2D_INCLUDE
#define ARRAY_WINDOW2D_INCLUDE

#include <array_window.h>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class Plotter;
class ScaledPlotter;

class ArrayWindow2D: public ArrayWindow {
 public:
    ArrayWindow2D();
    ~ArrayWindow2D();
 private:
    ArrayWindow2D(ArrayWindow2D const&); // not imlemented
    ArrayWindow2D& operator=(ArrayWindow2D const&); // not imlemented

    Plotter *palette_plot;
    ScaledPlotter *left_plot;
    ScaledPlotter *right_plot;
    Q_OBJECT
};

#endif /* ARRAY_WINDOW2D_INCLUDE */
