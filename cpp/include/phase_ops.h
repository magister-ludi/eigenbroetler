/* -*- c++ -*- */
#ifndef PHASE_OPS_INCLUDE
#define PHASE_OPS_INCLUDE

#include <global_defs.h>
#include <ui_phase_dialog.h>

class DislocationDialog: public QDialog {
public:
    DislocationDialog(QWidget *p);
    int order() const;
    double xpos() const;
    double ypos() const;
    bool useAbs() const;
private:
    DislocationDialog(); /* not implemented */
    DislocationDialog(DislocationDialog const&); /* not implemented */
    DislocationDialog& operator=(DislocationDialog const&); /* not implemented */

    Ui_phaseDialog ui;
    Q_OBJECT
};

class PhaseDialog: public QDialog {
public:
    PhaseDialog(QWidget *p, int order);
    double xval() const;
    double yval() const;
private:
    PhaseDialog(); /* not implemented */
    PhaseDialog(PhaseDialog const&); /* not implemented */
    PhaseDialog& operator=(PhaseDialog const&); /* not implemented */

    Ui_phaseDialog ui;
    Q_OBJECT
};

inline int DislocationDialog::order() const
{
    return ui.orderSpinBox->value();
}

inline double DislocationDialog::xpos() const
{
    return ui.xSpinBox->value();
}

inline double DislocationDialog::ypos() const
{
    return ui.ySpinBox->value();
}

inline bool DislocationDialog::useAbs() const
{
    return ui.absCheckBox->isChecked();
}

inline double PhaseDialog::xval() const
{
    return ui.xSpinBox->value();
}

inline double PhaseDialog::yval() const
{
    return ui.ySpinBox->value();
}

#endif /* PHASE_OPS_INCLUDE */
