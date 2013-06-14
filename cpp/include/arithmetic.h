// -*- c++ -*-
#ifndef ARITHMETIC_INCLUDE
#define ARITHMETIC_INCLUDE

#include <global_defs.h>
#include <ui_arithmetic_dialog.h>

class ComplexArray;

class ArithmeticDialog: public QDialog {
public:
    ArithmeticDialog(QWidget *w);
    QList<ComplexArray *> doArithmetic() const;
protected slots:
    void comboSelection();
    void accept();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    ArithmeticDialog(); // not implemented
    ArithmeticDialog(ArithmeticDialog const&); // not implemented
    ArithmeticDialog& operator=(ArithmeticDialog const&); // not implemented
    QList<ComplexArray const *> getData(DataSelector *s, bool& is_owned) const;
    Ui_arithmeticDialog ui;
    DataSelector *currentCombo;
    QList<QRadioButton *> buttons;
    int op;
    Q_OBJECT
};

#endif /* ARITHMETIC_INCLUDE */
