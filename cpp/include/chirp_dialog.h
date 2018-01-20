// -*- c++ -*-
#ifndef CHIRP_DIALOG_INCLUDE
#define CHIRP_DIALOG_INCLUDE

#include <global_defs.h>
#include <ui_chirp_dialog.h>
#include <QMutex>

class ComplexArray;
class ArrayWindow;

class ChirpDialog: public QDialog {
public:
    ChirpDialog(QWidget *p, ArrayWindow *a);
    QList<ComplexArray *> apply() const;
protected slots:
    void spin_changed();
    void check_changed();
    void accept();
    void reject();
private:
    ChirpDialog(); // not implemented
    ChirpDialog(ChirpDialog const&); // not implemented
    ChirpDialog& operator=(ChirpDialog const&); // not implemented
    Ui_ChirpDialog ui;
    ArrayWindow *a;
    Q_OBJECT
};

#endif /* CHIRP_DIALOG_INCLUDE */
