#ifndef PLOTTER_INCLUDE
#define PLOTTER_INCLUDE

#include <global_defs.h>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

class ArrayWindow;

class Plotter: public QWidget {
 public:
    Plotter(int w, int h, ArrayWindow *listener = NULL);
    ~Plotter();
    void setForeground(int r, int g, int b);
    void setForeground(QColor const& c);
    void setBackground(int r, int g, int b);
    void setBackground(QColor const& c);
    QColor getForeground() const;
    QColor getBackground() const;
    void clear();
    void drawRectangle(int xx, int yy, int w, int h);
    void fillRectangle(int xx, int yy, int w, int h);

    void drawLine(int x1, int y1, int x2, int y2);
    void drawImage(int x, int y, QImage const& img, bool centred = false);
 protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *event);
    QImage pixmap;
    QPainter *painter;
 private:
    Plotter(); // not implemented
    Plotter(Plotter const&); // not implemented
    Plotter& operator=(Plotter const&); // not implemented

    QColor bg;
    ArrayWindow *owner;

    Q_OBJECT
};

inline void Plotter::setForeground(int r, int g, int b)
{
    setForeground(QColor(r, g, b));
}

inline void Plotter::setBackground(int r, int g, int b)
{
    setBackground(QColor(r, g, b));
}

#endif /* PLOTTER_INCLUDE */
