#ifndef ARRAY_WINDOW_INCLUDE
#define ARRAY_WINDOW_INCLUDE

#include <global_defs.h>
#include <QWidget>
#include <display_info.h>
#include <ui_close_subwindow_dialog.h>

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
    void setScale(DisplayInfo::Scale s, int pow = -1);
    DisplayInfo::ComplexComponent getComponent() const;
    DisplayInfo::Scale getScale(int& pow) const;
    void setColourMap(DisplayInfo::ColourMap const& p);
    ComplexArray *getData();
    ComplexArray const *getData() const;
    virtual void mouseData(QWidget const *w, QMouseEvent *evt) = 0;
    void updateTitle();
    bool saveData();
 protected:
    ArrayWindow(ComplexArray *data, DisplayInfo::ComplexComponent c,
                DisplayInfo::Scale s);
    virtual void redraw() = 0;
    void closeEvent(QCloseEvent *event);
    ComplexArray *d;

    Plotter *colour_map;
    ScaledPlotter *left_plot;
    ScaledPlotter *right_plot;
    QWidget *plotLayout;
    QLineEdit **status;
    DisplayInfo::ComplexComponent cmp;
    DisplayInfo::Scale scl;
    int power;
    DisplayInfo::ColourMap pal;
 private:
    ArrayWindow(); // not imlemented
    ArrayWindow(ArrayWindow const&); // not imlemented
    ArrayWindow& operator=(ArrayWindow const&); // not imlemented
    static uint anon_count;
    Q_OBJECT
};

class CloseSubwindowDialog: public QDialog {
public:
    static QString const ask_on_unsaved;
    CloseSubwindowDialog(QWidget *p, QString const& name);
    ~CloseSubwindowDialog();
    bool saveRequested() const;
 private slots:
    void saveData();
private:
    CloseSubwindowDialog(); // not implemented
    CloseSubwindowDialog(CloseSubwindowDialog const&); // not implemented
    CloseSubwindowDialog&operator=(CloseSubwindowDialog const&); // not implemented
    Ui_closeSubwindowDialog ui;
    bool save_requested;
    Q_OBJECT
};

inline void ArrayWindow::setComponent(DisplayInfo::ComplexComponent c)
{
    if (cmp != c) {
        cmp = c;
        redraw();
    }
}

inline void ArrayWindow::setScale(DisplayInfo::Scale s, int pow)
{
    if (scl != s || (scl == DisplayInfo::POW && pow != power)) {
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

inline void CloseSubwindowDialog::saveData()
{
    save_requested = true;
    accept();
}

inline bool CloseSubwindowDialog::saveRequested() const
{
    return save_requested;
}

#endif /* ARRAY_WINDOW_INCLUDE */
