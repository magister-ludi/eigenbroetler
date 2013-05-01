#ifndef ARRAY_WINDOW_INCLUDE
#define ARRAY_WINDOW_INCLUDE

#include <global_defs.h>
#include <QWidget>
#include <display_info.h>

class ComplexArray;

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class Plotter;
class ScaledPlotter;

class ArrayWindow: public QWidget {
 public:
    static ArrayWindow *createWindow(ComplexArray *data,
                                     DisplayInfo::ComplexComponent c,
                                     DisplayInfo::Scale s,
                                     DisplayInfo::ColourMap const& p);
    virtual ~ArrayWindow();
    void setComponent(DisplayInfo::ComplexComponent c);
    void setScale(DisplayInfo::Scale s);
    DisplayInfo::ComplexComponent getComponent() const;
    DisplayInfo::Scale getScale() const;
    void setColourMap(DisplayInfo::ColourMap const& p);
    ComplexArray *getData();
    ComplexArray const *getData() const;
    virtual void mouseData(QWidget const *w, QMouseEvent *evt) = 0;
 protected:
    ArrayWindow(ComplexArray *data, DisplayInfo::ComplexComponent c,
                DisplayInfo::Scale s);
    virtual void redraw() = 0;
    ComplexArray *d;

    Plotter *colour_map;
    ScaledPlotter *left_plot;
    ScaledPlotter *right_plot;
    QWidget *plotLayout;
    QLineEdit *status;
    DisplayInfo::ComplexComponent cmp;
    DisplayInfo::Scale scl;
    DisplayInfo::ColourMap pal;
 private:
    ArrayWindow(); // not imlemented
    ArrayWindow(ArrayWindow const&); // not imlemented
    ArrayWindow& operator=(ArrayWindow const&); // not imlemented
    static uint anon_count;
    Q_OBJECT
};

inline void ArrayWindow::setComponent(DisplayInfo::ComplexComponent c)
{
    if (cmp != c) {
        cmp = c;
        redraw();
    }
}

inline void ArrayWindow::setScale(DisplayInfo::Scale s)
{
    if (scl != s) {
        scl = s;
        redraw();
    }
}

inline DisplayInfo::Scale ArrayWindow::getScale() const
{
    return scl;
}

inline DisplayInfo::ComplexComponent ArrayWindow::getComponent() const
{
    return cmp;
}

inline ComplexArray *ArrayWindow::getData()
{
    return d;
}

inline ComplexArray const *ArrayWindow::getData() const
{
    return d;
}

#endif /* ARRAY_WINDOW_INCLUDE */
