
#include <array_window_dialogs.h>
#include <QSettings>

#include <eigenbroetler_window.h>

QString const CloseSubwindowDialog::ask_on_unsaved("ask_on_unsaved");

CloseSubwindowDialog::CloseSubwindowDialog(QWidget *p, QString const& name):
    QDialog(p),
    save_requested(false)
{
    ui.setupUi(this);
    QSettings const settings(EigenbroetlerWindow::app_owner, EigenbroetlerWindow::app_name);
    ui.dontaskCheckBox->setChecked(!settings.value("ask_on_unsaved", true).toBool());
    ui.instructionLabel->setText(QString(tr("Window <b>%1</b> has not been saved. Save now?")).arg(name));
}

CloseSubwindowDialog::~CloseSubwindowDialog()
{
    QSettings settings(EigenbroetlerWindow::app_owner, EigenbroetlerWindow::app_name);
    settings.setValue("ask_on_unsaved", !ui.dontaskCheckBox->isChecked());
}

ExportDialog::ExportDialog(QWidget *p, ComplexArray::Component cmp):
    QDialog(p)
{
    ui.setupUi(this);
    QString const& left = cmp == ComplexArray::REAL ? tr("&Real") : tr("&Magnitude");
    QString const& right = cmp == ComplexArray::REAL ? tr("&Imaginary") : tr("&Phase");
    ui.leftCheckBox->setText(left);
    ui.rightCheckBox->setText(right);
    QString leftClean = left.toLower().remove('&');
    QString rightClean = right.toLower().remove('&');
    ui.bothCheckBox->setText(QString(tr("%1 and %2 &combined")).
                             arg(leftClean.left(1).toUpper() + leftClean.mid(1)).arg(rightClean));
    left_name = QString("ExportDialog/") + (cmp == ComplexArray::REAL ? "real" : "magnitude");
    right_name = QString("ExportDialog/") + (cmp == ComplexArray::REAL ? "imaginary" : "phase");
    both_name = QString("ExportDialog/") + (cmp == ComplexArray::REAL ? "real-imaginary" : "magnitude-phase");
    QSettings settings(EigenbroetlerWindow::app_owner, EigenbroetlerWindow::app_name);
    ui.leftCheckBox->setChecked(settings.value(left_name, true).toBool());
    ui.rightCheckBox->setChecked(settings.value(right_name, true).toBool());
    ui.bothCheckBox->setChecked(settings.value(both_name, true).toBool());
}

void ExportDialog::accept()
{
    QDialog::accept();
    QSettings settings(EigenbroetlerWindow::app_owner, EigenbroetlerWindow::app_name);
    settings.setValue(left_name, ui.leftCheckBox->isChecked());
    settings.setValue(right_name, ui.rightCheckBox->isChecked());
    settings.setValue(both_name, ui.bothCheckBox->isChecked());
}
