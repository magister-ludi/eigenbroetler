// -*- c++ -*-
#ifndef FILTER_DIALOG_INCLUDE
#define FILTER_DIALOG_INCLUDE

#include <global_defs.h>
#include <ui_filter_dialog.h>
#include <QMutex>

class ComplexArray;
class ArrayWindow;

class FilterDialog: public QDialog {
public:
    FilterDialog(QWidget *p, ArrayWindow *a);
    QList<ComplexArray *> apply() const;
protected slots:
    void radius_spin_changed();
    void radius_slider_changed();
    void filter_changed();
    void window_changed();
    void updateArrayWindow();
    void accept();
    void reject();
private:
    FilterDialog(); // not implemented
    FilterDialog(FilterDialog const&); // not implemented
    FilterDialog& operator=(FilterDialog const&); // not implemented
    Ui_filterDialog ui;
    ArrayWindow *a;
    int pm;
    QMutex mutex;
    QList<QRadioButton *> filters;
    Q_OBJECT
};

#endif /* FILTER_DIALOG_INCLUDE */
