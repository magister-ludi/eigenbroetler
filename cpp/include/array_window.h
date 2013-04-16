#ifndef ARRAY_WINDOW_INCLUDE
#define ARRAY_WINDOW_INCLUDE

#include <QWidget>

class ComplexArray;

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class Plotter;
class ScaledPlotter;

class ArrayWindow: public QWidget {
 public:
    static ArrayWindow *createWindow(ComplexArray *data);
    virtual ~ArrayWindow();
    ComplexArray *getData();
    ComplexArray const *getData() const;
    virtual void mouseData(QWidget const *w, QMouseEvent *evt) = 0;
 protected:
    ArrayWindow(ComplexArray *data);
    ComplexArray *d;

    Plotter *palette_plot;
    ScaledPlotter *left_plot;
    ScaledPlotter *right_plot;
    QWidget *plotLayout;
    QLineEdit *status;
 private:
    ArrayWindow(); // not imlemented
    ArrayWindow(ArrayWindow const&); // not imlemented
    ArrayWindow& operator=(ArrayWindow const&); // not imlemented
    static uint anon_count;
    Q_OBJECT
};

inline ComplexArray *ArrayWindow::getData()
{
    return d;
}

inline ComplexArray const *ArrayWindow::getData() const
{
    return d;
}

#endif /* ARRAY_WINDOW_INCLUDE */
