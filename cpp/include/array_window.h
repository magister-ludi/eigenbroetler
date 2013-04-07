#ifndef ARRAY_WINDOW_INCLUDE
#define ARRAY_WINDOW_INCLUDE

#include <QWidget>

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class ArrayWindow: public QWidget {
 public:
    ArrayWindow();
    ~ArrayWindow();
 private:
    ArrayWindow(ArrayWindow const&); // not imlemented
    ArrayWindow& operator=(ArrayWindow const&); // not imlemented
    Q_OBJECT
};

#endif /* ARRAY_WINDOW_INCLUDE */
