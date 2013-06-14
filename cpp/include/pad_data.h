// -*- c++ -*-
#ifndef PAD_DIALOG_INCLUDE
#define PAD_DIALOG_INCLUDE

#include <global_defs.h>
#include <ui_pad_dialog.h>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QDoubleSpinBox;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE

class ComplexArray;
class ScaledPlotter;

class PadDialog: public QDialog {
public:
    PadDialog(QWidget *p, ComplexArray const *a);
    int left() const;
    int top() const;
    int right() const;
    int bottom() const;
    Complex padValue() const;
 protected slots:
    void updateControls();
    void padChanged();
    void accept();
private:
    PadDialog(); // not implemented
    PadDialog(PadDialog const&); // not implemented
    PadDialog& operator=(PadDialog const&); // not implemented

    Ui_padDialog ui;
    QRect const d;
    Q_OBJECT
};

inline int PadDialog::left() const
{
    return ui.leftSpinBox->value();
}

inline int PadDialog::top() const
{
    return ui.topSpinBox->value();
}

inline int PadDialog::right() const
{
    return ui.rightSpinBox->value();
}

inline int PadDialog::bottom() const
{
    return ui.bottomSpinBox->value();
}

inline Complex PadDialog::padValue() const
{
    return Complex(ui.realSpinBox->value(), ui.imagSpinBox->value());
}

#endif /* PAD_DIALOG_INCLUDE */
