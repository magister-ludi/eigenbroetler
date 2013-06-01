
#include <eigenbroetler_window.h>
#include <QInputDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QSettings>
#include <about_dialog.h>
#include <array_window_2d.h>
#include <complex_operations.h>
#include <filter_dialog.h>
#include <formula_dialog.h>

void EigenbroetlerWindow::newWindow()
{
    bool stack;
    QList<ComplexArray *> a = FormulaDialog::create_image(this, stack);
    newWindow(a, stack);
}

void EigenbroetlerWindow::readData()
{
    QString fileName = getFileName(this);
    if (!fileName.isEmpty())
        loadImage(fileName);
}

void EigenbroetlerWindow::exportImages()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a)
        a->exportComponents();
}

void EigenbroetlerWindow::saveData()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        a->saveData();
        resetGUI();
    }
}

void EigenbroetlerWindow::setComponent()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        if (componentGroup->checkedAction() == riAction)
            a->setComponent(ComplexArray::REAL);
        else
            a->setComponent(ComplexArray::MAGNITUDE);
        resetGUI();
    }
}

void EigenbroetlerWindow::toggleComponents()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        if (a->getComponent() == ComplexArray::REAL)
            a->setComponent(ComplexArray::MAGNITUDE);
        else
            a->setComponent(ComplexArray::REAL);
        resetGUI();
    }
}

void EigenbroetlerWindow::setScale()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        if (scaleGroup->checkedAction() == linAction)
            a->setScale(DisplayInfo::LINEAR);
        else if (scaleGroup->checkedAction() == logAction)
            a->setScale(DisplayInfo::LOGARITHMIC);
        else if (scaleGroup->checkedAction() == powAction) {
            int	root = QInputDialog::getInt(this, tr("Select root to use"),
                                            tr("Root:"), 2, 2);
            a->setScale(DisplayInfo::POWER_LAW, root);
        }
        resetGUI();
    }
}

void EigenbroetlerWindow::setColourMap()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        if (colourMenu->exec(QCursor::pos()))
            a->setColourMap(DisplayInfo::instance().getColourMap(colourGroup->checkedAction()->text()));
    }
}

void EigenbroetlerWindow::fft()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        for (int i = 0; i < a->numDataSets(); ++i)
            da << a->getData()[i]->dft(true);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::fftx()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        for (int i = 0; i < a->numDataSets(); ++i)
            da << a->getData()[i]->xdft(true);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::ffty()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        for (int i = 0; i < a->numDataSets(); ++i)
            da << a->getData()[i]->ydft(true);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::about()
{
    AboutDialog dlg(this);
    dlg.exec();
}

void EigenbroetlerWindow::hilbert_x()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        QList<ComplexArray const *> const& aa = a->getData();
        for (int i = 0; i < a->numDataSets(); ++i)
            da << Operations::hilbert(aa[i], true);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::hilbert_y()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        QList<ComplexArray const *> const& aa = a->getData();
        for (int i = 0; i < a->numDataSets(); ++i)
            da << Operations::hilbert(aa[i], false);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::demod_add()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        QList<ComplexArray const *> const& aa = a->getData();
        for (int i = 0; i < a->numDataSets(); ++i)
            da << Operations::demod(aa[i], false);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::demod_sel()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        QList<ComplexArray const *> const& aa = a->getData();
        for (int i = 0; i < a->numDataSets(); ++i)
            da << Operations::demod(aa[i], true);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::demod_hilbert()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        QList<ComplexArray const *> const& aa = a->getData();
        for (int i = 0; i < a->numDataSets(); ++i)
            da << Operations::demod_hilbert(aa[i]);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::standard_energy()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        QList<ComplexArray const *> const& aa = a->getData();
        for (int i = 0; i < a->numDataSets(); ++i)
            da << Operations::calculateEnergy(aa[i], false);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::modified_energy()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        QList<ComplexArray const *> const& aa = a->getData();
        for (int i = 0; i < a->numDataSets(); ++i)
            da << Operations::calculateEnergy(aa[i], true);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::filter()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        FilterDialog dlg(this, a);
        if (dlg.exec() == QDialog::Accepted) {
            QList<ComplexArray *> da = dlg.apply();
            newWindow(da, true);
        }
    }
}
