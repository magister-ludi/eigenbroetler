
#include <formula_dialog.h>
#include <QRegExp>
#include <QSettings>
#include <QVector>
#include <calculator.h>
#include <complex_array.h>
#include <eigenbrot_window.h>

QString const FormulaDialog::sq_name("FormulaDialog/square_image");
QString const FormulaDialog::dim_name("FormulaDialog/image_dimension");
QString const FormulaDialog::width_name("FormulaDialog/image_width");
QString const FormulaDialog::height_name("FormulaDialog/image_height");
QString const FormulaDialog::form_name("FormulaDialog/image%1");
QString const FormulaDialog::multi_name("FormulaDialog/stacked");
QString const FormulaDialog::start_name("FormulaDialog/first");
QString const FormulaDialog::stop_name("FormulaDialog/last");
QString const FormulaDialog::incr_name("FormulaDialog/incr");

static const int num_formulae = 10;

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
    ui.formulaComboBox->setMaxVisibleItems(num_formulae);

    ui.firstNLineEdit->setText(settings.value(start_name, QString().setNum(1)).toString());
    ui.finalNLineEdit->setText(settings.value(stop_name, QString().setNum(5)).toString());
    ui.incrNLineEdit->setText(settings.value(incr_name, QString().setNum(1)).toString());
    ui.multiviewCheckBox->setCheckState(settings.value(multi_name, false).toBool() ? Qt::Checked : Qt::Unchecked);
    // TODO: fix calculations, and remove next lines
    ui.multiGroupBox->setEnabled(false);
    ui.multiviewCheckBox->setEnabled(false);
    ui.oneDRadioButton->setEnabled(false);
    ui.twoDRadioButton->setChecked(true);

    for (int i = 0; i < num_formulae; ++i) {
        QString f = settings.value(form_name.arg(i), QString()).toString();
        if (f.isEmpty())
            break;
        ui.formulaComboBox->addItem(f);
    }
    ui.formulaComboBox->setCurrentIndex(0);
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

static QString zip(QString const& s)
{
    QString z(s);
    return z.remove(QRegExp("\\s"));
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

    settings.setValue(start_name, ui.firstNLineEdit->text());
    settings.setValue(stop_name, ui.finalNLineEdit->text());
    settings.setValue(incr_name, ui.incrNLineEdit->text());
    settings.setValue(multi_name, ui.multiviewCheckBox->checkState() ? true : false);

    std::map<QString, QString> frms;
    frms[zip(ui.formulaComboBox->currentText())] = ui.formulaComboBox->currentText();
    int item = 0;
    settings.setValue(form_name.arg(item++), ui.formulaComboBox->currentText());
    for (int i = 0; i < num_formulae && i < ui.formulaComboBox->count(); ++i) {
        QString const key = zip(ui.formulaComboBox->itemText(i));
        if (frms.find(key) == frms.end()) {
            frms[key] = ui.formulaComboBox->itemText(i);
            settings.setValue(form_name.arg(item++), ui.formulaComboBox->itemText(i));
        }
    }
}

ComplexArray *FormulaDialog::construct()
{
    extern Calculator calculator;
    if (calculator.setFormula(ui.formulaComboBox->currentText().toAscii().constData())) {
        ComplexArray *d = new ComplexArray(calculator,
                                           ui.widthLineEdit->text().toInt(),
                                           ui.heightLineEdit->text().toInt(),
                                           // TODO: fix n evaluation
                                           0);
        if (d) {
            if (d->isValid())
                return d;
            else
                delete d;
        }
    }
    return NULL;
}

ComplexArray *FormulaDialog::create_image(QWidget *p)
{
    FormulaDialog dlg(p);
    if (dlg.exec() == QDialog::Accepted)
        return dlg.construct();
    else
        return NULL;
}
