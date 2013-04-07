#ifndef PLOTTER_INCLUDE
#define PLOTTER_INCLUDE

#include <QWidget>

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
class QPaintEvent;
QT_END_NAMESPACE

class Plotter: public QWidget {
 public:
    Plotter(int w, int h);
    ~Plotter();
   void setForeground(QColor const& c);
   void setBackground(QColor const& c);
   QColor getForeground() const;
   QColor getBackground() const;
   void clear();
   void drawRectangle(int xx, int yy, int w, int h);
   void fillRectangle(int xx, int yy, int w, int h);

   void drawLine(int x1, int y1, int x2, int y2);
 protected:
    void paintEvent(QPaintEvent *e);
    QImage pixmap;
    QPainter *painter;
 private:
    Plotter(); // not implemented
    Plotter(Plotter const&); // not implemented
    Plotter& operator=(Plotter const&); // not implemented

    QColor bg;

    Q_OBJECT
};

#endif /* PLOTTER_INCLUDE */
