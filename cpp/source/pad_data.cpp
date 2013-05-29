#include <eigenbroetler_window.h>
#include <pad_data.h>
#include <limits>
#include <QMdiArea>
#include <QSettings>
#include <array_window.h>
#include <complex_array.h>
#include <complex_operations.h>

static QString const real_name("PadDialog/real");
static QString const imag_name("PadDialog/imag");
static QString const left_name("PadDialog/left");
static QString const top_name("PadDialog/top");
static QString const right_name("PadDialog/right");
static QString const bottom_name("PadDialog/bottom");
static QString const lr_name("PadDialog/lr_symmetric");
static QString const tb_name("PadDialog/tb_symmetric");

PadDialog::PadDialog(QWidget *p, ComplexArray const *a):
    QDialog(p),
    d(0, 0, a->width(), a->height())
{
    ui.setupUi(this);
    ui.realSpinBox->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    ui.imagSpinBox->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
    ui.rightSpinBox->setRange(-d.width(), std::numeric_limits<int>::max());
    ui.leftSpinBox->setRange(-d.width(), std::numeric_limits<int>::max());
    ui.bottomSpinBox->setRange(-d.height(), std::numeric_limits<int>::max());
    ui.topSpinBox->setRange(-d.height(), std::numeric_limits<int>::max());
    ui.infoLabel->setTextFormat(Qt::AutoText);

    EigenbroetlerWindow *eb = EigenbroetlerWindow::instance();
    QSettings settings(eb->app_owner, eb->app_name);
    ui.realSpinBox->setValue(settings.value(real_name, 0).toDouble());
    ui.imagSpinBox->setValue(settings.value(imag_name, 0).toDouble());
    ui.leftSpinBox->setValue(settings.value(left_name, 0).toInt());
    ui.topSpinBox->setValue(settings.value(top_name, 0).toInt());
    ui.rightSpinBox->setValue(settings.value(right_name, 0).toInt());
    ui.bottomSpinBox->setValue(settings.value(bottom_name, 0).toInt());
    ui.lrCheckBox->setChecked(settings.value(lr_name, false).toBool());
    ui.tbCheckBox->setChecked(settings.value(tb_name, false).toBool());
    updateControls();
}

void PadDialog::accept()
{
    QDialog::accept();
    EigenbroetlerWindow *eb = EigenbroetlerWindow::instance();
    QSettings settings(eb->app_owner, eb->app_name);
    settings.setValue(real_name, ui.realSpinBox->value());
    settings.setValue(imag_name, ui.imagSpinBox->value());
    settings.setValue(left_name, ui.leftSpinBox->value());
    settings.setValue(top_name, ui.topSpinBox->value());
    settings.setValue(right_name, ui.rightSpinBox->value());
    settings.setValue(bottom_name, ui.bottomSpinBox->value());
    settings.setValue(lr_name, ui.lrCheckBox->isChecked());
    settings.setValue(tb_name, ui.tbCheckBox->isChecked());
}

void PadDialog::updateControls()
{
    if (ui.lrCheckBox->isChecked())
        ui.rightSpinBox->setEnabled(false);
    else
        ui.rightSpinBox->setEnabled(true);
    if (ui.tbCheckBox->isChecked())
        ui.bottomSpinBox->setEnabled(false);
    else
        ui.bottomSpinBox->setEnabled(true);
    padChanged();
}

void PadDialog::padChanged()
{
    if (ui.lrCheckBox->isChecked())
        ui.rightSpinBox->setValue(ui.leftSpinBox->value());
    if (ui.tbCheckBox->isChecked())
        ui.bottomSpinBox->setValue(ui.topSpinBox->value());
    bool pad = (ui.leftSpinBox->value() > 0 || ui.topSpinBox->value() > 0 ||
                ui.rightSpinBox->value() > 0 || ui.bottomSpinBox->value() > 0);
    ui.realSpinBox->setEnabled(pad);
    ui.imagSpinBox->setEnabled(pad);
    QRect const new_rect(d.x() - ui.leftSpinBox->value(),
                         d.y() - ui.topSpinBox->value(),
                         d.width() + ui.leftSpinBox->value() + ui.rightSpinBox->value(),
                         d.height() + ui.topSpinBox->value() + ui.bottomSpinBox->value());
    QString err;
    if (new_rect.width() <= 0 || new_rect.height() <= 0) {
        err += "<p><span style=\" color:#ff0000;\">";
        err += tr("Invalid rectangle");
        err += "</span></p>";
    }
    ui.okButton->setEnabled(err.isEmpty());
    QString info("<html><head/><body><p>");
    info += tr("New data set:");
    info += " <b>";
    if (!err.isEmpty())
        info += "<span style=\" color:#ff0000;\">";
    info += QString("%1x%2").arg(new_rect.width()).arg(new_rect.height());
    if (!err.isEmpty())
        info += "</span>";
    info += "</b>";
    info += tr(" pixels");
    info += "</p>";
    info += err;
    ui.infoLabel->setText(info);
}

void EigenbroetlerWindow::padData()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        PadDialog dlg(this, a->getData()[0]);
        if (dlg.exec() == QDialog::Accepted) {
            QList<ComplexArray *> da;
            for (int i = 0; i < a->numDataSets(); ++i)
                da << Operations::padCrop(a->getData()[i],
                                          dlg.left(), dlg.top(), dlg.right(), dlg.bottom(),
                                          dlg.padValue());
            newWindow(da, true);
        }
    }
}
