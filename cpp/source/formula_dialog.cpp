
#include <formula_dialog.h>
#include <QSettings>
#include <eigenbrot_window.h>

QString const FormulaDialog::sq_name("FormulaDialog/square_image");
QString const FormulaDialog::dim_name("FormulaDialog/image_dimension");
QString const FormulaDialog::width_name("FormulaDialog/image_width");
QString const FormulaDialog::height_name("FormulaDialog/image_height");

FormulaDialog::FormulaDialog(QWidget *p):
    QDialog(p)
{
    ui.setupUi(this);
    EigenbrotWindow *eb = (EigenbrotWindow *) p;
    QSettings settings(eb->app_owner, eb->app_name);
    ui.squareCheckBox->setCheckState(settings.value(sq_name, false).toBool() ? Qt::Checked : Qt::Unchecked);
    ui.twoDRadioButton->setChecked(settings.value(dim_name, true).toBool());
    ui.widthLineEdit->setText(settings.value(width_name, QString().setNum(256)).toString());
    ui.heightLineEdit->setText(settings.value(height_name, QString().setNum(256)).toString());
    updateControls();
}

void FormulaDialog::updateControls()
{
    if (ui.twoDRadioButton->isChecked()) {
        ui.heightLabel->show();
        ui.heightLineEdit->show();
        ui.squareCheckBox->show();
        if (ui.squareCheckBox->checkState() == Qt::Checked) {
            ui.heightLineEdit->setEnabled(false);
            ui.heightLineEdit->setText(ui.widthLineEdit->text());
        }
        else {
            ui.heightLineEdit->setEnabled(true);
        }
    }
    else {
        ui.heightLabel->hide();
        ui.heightLineEdit->hide();
        ui.squareCheckBox->hide();
    }
}

void FormulaDialog::formulaChanged()
{
}

void FormulaDialog::accept()
{
    QDialog::accept();
    EigenbrotWindow *eb = (EigenbrotWindow *) parent();
    QSettings settings(eb->app_owner, eb->app_name);
    settings.setValue(sq_name, ui.squareCheckBox->checkState() ? true : false);
    settings.setValue(dim_name, ui.twoDRadioButton->isChecked());
    settings.setValue(width_name, ui.widthLineEdit->text());
    settings.setValue(height_name, ui.heightLineEdit->text());
}

ArrayWindow *FormulaDialog::create_image(QWidget *p)
{
    FormulaDialog dlg(p);
    dlg.exec();
    return NULL;
}
