#include <chirp_dialog.h>
#include <limits>
#include <QSettings>
#include <array_window.h>
#include <eigenbroetler_window.h>
#include <complex_array.h>
#include <complex_operations.h>

static QString const x_numerator_name("ChirpDialog/xNumerator");
static QString const x_denominator_name("ChirpDialog/xDenominator");
static QString const y_numerator_name("ChirpDialog/yNumerator");
static QString const y_denominator_name("ChirpDialog/yDenominator");
static QString const xy_equal_name("ChirpDialog/xyEqual");

ChirpDialog::ChirpDialog(QWidget *p, ArrayWindow *a_in):
    QDialog(p),
    a(a_in)
{
    ui.setupUi(this);
    EigenbroetlerWindow *eb = EigenbroetlerWindow::instance();
    QSettings settings(eb->app_owner, eb->app_name);

    int const x_num = settings.value(x_numerator_name, 1).toInt();
    int const x_den = settings.value(x_denominator_name, 2 * x_num).toInt();
    int const y_num = settings.value(y_numerator_name, 1).toInt();
    int const y_den = settings.value(y_denominator_name, 2 * y_num).toInt();
    ui.xNumSpinBox->setValue(x_num);
    ui.xDenomSpinBox->setValue(x_den);

    bool const xy_same = settings.value(xy_equal_name, true).toBool();
    ui.yNumSpinBox->setEnabled(!xy_same);
    ui.yDenomSpinBox->setEnabled(!xy_same);
    if (xy_same) {
        ui.xy_sameCheckBox->setChecked(Qt::Checked);
        ui.yNumSpinBox->setValue(x_num);
        ui.yDenomSpinBox->setValue(x_den);
    }
    else {
        ui.xy_sameCheckBox->setChecked(Qt::Unchecked);
        ui.yNumSpinBox->setValue(y_num);
        ui.yDenomSpinBox->setValue(y_den);
    }

    ui.xNumSpinBox->setRange(1, std::numeric_limits<int>::max());
    ui.xDenomSpinBox->setRange(1, std::numeric_limits<int>::max());
    ui.yNumSpinBox->setRange(1, std::numeric_limits<int>::max());
    ui.yDenomSpinBox->setRange(1, std::numeric_limits<int>::max());
}

void ChirpDialog::accept()
{
    EigenbroetlerWindow *eb = EigenbroetlerWindow::instance();
    QSettings settings(eb->app_owner, eb->app_name);
    settings.setValue(xy_equal_name, ui.xy_sameCheckBox->isChecked());
    settings.setValue(x_numerator_name, ui.xNumSpinBox->value());
    settings.setValue(x_denominator_name, ui.xDenomSpinBox->value());
    settings.setValue(y_numerator_name, ui.yNumSpinBox->value());
    settings.setValue(y_denominator_name, ui.yDenomSpinBox->value());
    QDialog::accept();
}

void ChirpDialog::reject()
{
    a->markFilter(0, 0, 0);
    QDialog::reject();
}

void ChirpDialog::spin_changed()
{
    int const x_num = ui.xNumSpinBox->value();
    int const x_den = ui.xDenomSpinBox->value();
    int const y_num = ui.xy_sameCheckBox->isChecked() ? x_num : ui.yNumSpinBox->value();
    int const y_den = ui.xy_sameCheckBox->isChecked() ? x_den : ui.yDenomSpinBox->value();
    if (ui.xy_sameCheckBox->isChecked()) {
        ui.yNumSpinBox->setValue(x_num);
        ui.yDenomSpinBox->setValue(x_den);
    }
    ui.xDescription->setText(tr("Scale in x-direction by %1").
                             arg(double(x_num) / x_den, 0, 'g', 4));
    ui.yDescription->setText(tr("Scale in y-direction by %1").
                             arg(double(y_num) / y_den, 0, 'g', 4));
}

void ChirpDialog::check_changed()
{
    ui.yNumSpinBox->setEnabled(!ui.xy_sameCheckBox->isChecked());
    ui.yDenomSpinBox->setEnabled(!ui.xy_sameCheckBox->isChecked());
    spin_changed();
}

QList<ComplexArray *> ChirpDialog::apply() const
{
    QList<ComplexArray const *> src = a->getData();
    QList<ComplexArray *> dest;
    int const x_num = ui.xNumSpinBox->value();
    int const x_den = ui.xDenomSpinBox->value();
    int const y_num = ui.yNumSpinBox->value();
    int const y_den = ui.yDenomSpinBox->value();
    for (QList<ComplexArray const *>::const_iterator i = src.begin(); i != src.end(); ++i) {
        dest << Operations::chirpz_scale(*i, x_num, x_den, y_num, y_den);
    }
    return dest;
}
