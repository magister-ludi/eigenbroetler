// -*- c++ -*-
#ifndef ARRAY_WINDOW_INCLUDE
#define ARRAY_WINDOW_INCLUDE

#include <global_defs.h>
#include <QWidget>
#include <display_info.h>
#include <complex_array.h>

QT_BEGIN_NAMESPACE
class QKeyEvent;
class QLineEdit;
QT_END_NAMESPACE

class Plotter;
class ScaledPlotter;

class ArrayWindow: public QWidget {
public:
    static ArrayWindow *createWindow(QList<ComplexArray *>& data,
                                     ComplexArray::Component c,
                                     DisplayInfo::Scale s,
                                     DisplayInfo::ColourMap const& p);
    virtual ~ArrayWindow();
    void setComponent(ComplexArray::Component c);
    void setScale(DisplayInfo::Scale s, int pow = -1);
    ComplexArray::Component getComponent() const;
    DisplayInfo::Scale getScale(int& pow) const;
    void setColourMap(DisplayInfo::ColourMap const& p);
    QList<ComplexArray const *> getData() const;
    virtual void mouseData(QWidget const *w, QMouseEvent *evt) = 0;
    void updateTitle();
    bool saveData();
    void exportComponents();
    int numDataSets() const;
    QString const& baseTitle() const;
    int currentIndex() const;
protected:
    class DataSet {
    public:
        DataSet();
        ComplexArray *d;
        QImage left;
        QImage right;
    private:
        DataSet(DataSet const&); // not implemented
        DataSet& operator=(DataSet const&); // not implemented
    };
    ArrayWindow(QList<ComplexArray *>& data, ComplexArray::Component c,
                DisplayInfo::Scale s);
    virtual void redraw() = 0;
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void setViewIndex(int idx, bool force = false);

    ComplexArray::Component cmp;
    DisplayInfo::Scale scl;
    int index;
    int power;
    QString title_base;
    DisplayInfo::ColourMap pal;
    Plotter *colour_map_display;
    QWidget *plotLayout;
    ScaledPlotter *left_plot;
    ScaledPlotter *right_plot;
    QLineEdit *status[2];
    QList<DataSet *> dlist;
    QList<ComplexArray const *> alist;
private:
    ArrayWindow(); // not imlemented
    ArrayWindow(ArrayWindow const&); // not imlemented
    ArrayWindow& operator=(ArrayWindow const&); // not imlemented
    static uint anon_count;
    Q_OBJECT
};

inline void ArrayWindow::setComponent(ComplexArray::Component c)
{
    if (cmp != c) {
        cmp = c;
        redraw();
    }
}

inline void ArrayWindow::setScale(DisplayInfo::Scale s, int pow)
{
    if (scl != s || (scl == DisplayInfo::POWER_LAW && pow != power)) {
        scl = s;
        power = pow > 0 ? pow : power;
        redraw();
    }
}

inline DisplayInfo::Scale ArrayWindow::getScale(int& pow) const
{
    pow = power;
    return scl;
}

inline ComplexArray::Component ArrayWindow::getComponent() const
{
    return cmp;
}

inline QList<ComplexArray const *> ArrayWindow::getData() const
{
    return alist;
}

inline int ArrayWindow::numDataSets() const
{
    return dlist.length();
}

inline ArrayWindow::DataSet::DataSet():
    d(NULL)
{
}

inline QString const& ArrayWindow::baseTitle() const
{
    return title_base;
}

inline int ArrayWindow::currentIndex() const
{
    return index;
}

#endif /* ARRAY_WINDOW_INCLUDE */
