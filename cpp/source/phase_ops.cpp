
#include <eigenbroetler_window.h>
#include <QMdiArea>
#include <QMessageBox>
#include <array_window.h>
#include <limits>
#include <complex_operations.h>
#include <phase_ops.h>

DislocationDialog::DislocationDialog(QWidget *p):
    QDialog(p)
{
    ui.setupUi(this);
    setWindowTitle(tr("Add spiral dislocation"));
    ui.xLabel->setText(tr("&X location"));
    ui.yLabel->setText(tr("&Y location"));

    ui.orderSpinBox->setRange(-std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    ui.orderSpinBox->setValue(1);
    ui.xSpinBox->setRange(-std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    ui.xSpinBox->setValue(0);
    ui.ySpinBox->setRange(-std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    ui.ySpinBox->setValue(0);
}

PhaseDialog::PhaseDialog(QWidget *p, int order):
    QDialog(p)
{
    ui.setupUi(this);
    ui.orderLabel->hide();
    ui.orderSpinBox->hide();
    ui.absCheckBox->hide();
    if (order < 0 || order > 2) {
        QMessageBox::critical(p, tr("Internal error"),
                              QString(tr("Incorrect value (%1) passed to PhaseDialog constructor\n")).arg(order) +
                              QString(tr("Using Quadratic phase")));
        order = 2;
    }
    switch (order) {
    case 0:
        setWindowTitle(tr("Phase offset"));
        ui.xLabel->setText(QString(tr("Phase (fraction of 2%1)")).arg(QChar(0x3c0)));
        ui.xSpinBox->setValue(0.5);
        ui.xSpinBox->setRange(-std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        ui.ySpinBox->setValue(0);
        ui.yLabel->hide();
        ui.ySpinBox->hide();
        break;
    case 1:
        setWindowTitle(tr("Linear phase"));
        ui.xSpinBox->setValue(1);
        ui.ySpinBox->setValue(1);
        ui.xSpinBox->setRange(-std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        ui.ySpinBox->setRange(-std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        break;
    case 2:
        setWindowTitle(tr("Quadratic phase"));
        ui.xSpinBox->setValue(1);
        ui.ySpinBox->setValue(1);
        ui.xSpinBox->setRange(-std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        ui.ySpinBox->setRange(-std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        break;
    }
}

void EigenbroetlerWindow::dislocation()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        DislocationDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted) {
            QList<ComplexArray *> da;
            for (int i = 0; i < a->numDataSets(); ++i)
                da << Operations::addDislocation(a->getData()[i], dlg.order(),
                                                 dlg.xpos(), dlg.ypos(), dlg.useAbs());
            newWindow(da, true);
        }
    }
}

static QList<ComplexArray *> addPhase(QWidget *p, int order, QList<ComplexArray const *> data)
{
    PhaseDialog dlg(p, order);
    QList<ComplexArray *> da;
    if (dlg.exec() == QDialog::Accepted) {
        for (int i = 0; i < data.length(); ++i)
            da << Operations::addPhase(data[i], order, dlg.xval(), dlg.yval());
    }
    return da;
}

void EigenbroetlerWindow::addConstPhase()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da = addPhase(this, 0, a->getData());
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::addLinearPhase()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da = addPhase(this, 1, a->getData());
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::addQuadPhase()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da = addPhase(this, 2, a->getData());
        newWindow(da, true);
    }
}
