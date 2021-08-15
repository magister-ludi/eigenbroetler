#include <filter_dialog.h>
#include <limits>
#include <QSettings>
#include <array_window.h>
#include <eigenbroetler_window.h>
#include <complex_array.h>
#include <complex_operations.h>

static QString const pass_name("FilterDialog/low_pass");
static QString const filter_name("FilterDialog/filter");
static QString const x_name("FilterDialog/x_pos");
static QString const y_name("FilterDialog/y_pos");
static QString const order_name("FilterDialog/butterworth_order");

FilterDialog::FilterDialog(QWidget *p, ArrayWindow *a_in):
    QDialog(p),
    a(a_in)
{
    ui.setupUi(this);
    filters
        << ui.rectangularButton
        << ui.hanningButton
        << ui.butterworthButton
        << ui.hammingButton
        << ui.parzenButton
        << ui.welchButton
        << ui.gaussianButton;
    EigenbroetlerWindow *eb = EigenbroetlerWindow::instance();
    QSettings settings(eb->app_owner, eb->app_name);

    if (settings.value(pass_name, true).toBool())
        ui.lowpassButton->setChecked(true);
    else
        ui.highpassButton->setChecked(true);

    int filter = abs(settings.value(filter_name, 0).toInt()) % filters.length();
    filters[filter]->setChecked(true);
    if (!ui.lowpassButton->isChecked() && ui.gaussianButton->isChecked())
        filters[0]->setChecked(true);
    ui.gaussianButton->setEnabled(ui.lowpassButton->isChecked());
    pm = ui.lowpassButton->isChecked() ? 1 : -1;
    ui.orderSpinBox->setValue(abs(settings.value(order_name, 1).toInt()));

    ComplexArray const *d = a_in->getData()[0];
    int const ww = d->width();
    int const hh = d->height();
    int const r = int(sqrt(1.0 * ww * ww + hh * hh) + 0.5);

    ui.orderSpinBox->setRange(1, std::numeric_limits<int>::max());
    ui.xSpinBox->setRange(-r, r);
    ui.ySpinBox->setRange(-r, r);

    ui.radiusSpinBox->setRange(0, r / 2);
    ui.radiusSlider->setRange(0, r / 2);
    ui.radiusSpinBox->setValue(std::min(ww, hh) / 5);

    ui.xSpinBox->setValue(settings.value(x_name, 0).toInt());
    ui.ySpinBox->setValue(settings.value(y_name, 0).toInt());

    window_changed();
}

void FilterDialog::accept()
{
    a->markFilter(0, 0, 0);
    EigenbroetlerWindow *eb = EigenbroetlerWindow::instance();
    QSettings settings(eb->app_owner, eb->app_name);
    settings.setValue(pass_name, ui.lowpassButton->isChecked());
    settings.setValue(x_name, ui.xSpinBox->value());
    settings.setValue(y_name, ui.ySpinBox->value());
    settings.setValue(order_name, ui.orderSpinBox->value());
    for (int f = 0; f < filters.length(); ++f)
        if (filters[f]->isChecked())
            settings.setValue(filter_name,  f);
    QDialog::accept();
}

void FilterDialog::reject()
{
    a->markFilter(0, 0, 0);
    QDialog::reject();
}

void FilterDialog::updateArrayWindow()
{
    a->markFilter(ui.xSpinBox->value(), ui.ySpinBox->value(), pm * ui.radiusSpinBox->value());
}

void FilterDialog::radius_spin_changed()
{
    if (mutex.tryLock()) {
        ui.radiusSlider->setValue(ui.radiusSpinBox->value());
        updateArrayWindow();
        mutex.unlock();
    }
}

void FilterDialog::radius_slider_changed()
{
    if (mutex.tryLock()) {
        ui.radiusSpinBox->setValue(ui.radiusSlider->value());
        updateArrayWindow();
        mutex.unlock();
    }
}

void FilterDialog::filter_changed()
{
    pm = ui.lowpassButton->isChecked() ? 1 : -1;
    ui.gaussianButton->setEnabled(ui.lowpassButton->isChecked());
    updateArrayWindow();
}

void FilterDialog::window_changed()
{
    ui.orderSpinBox->setEnabled(ui.butterworthButton->isChecked());
}

QList<ComplexArray *> FilterDialog::apply() const
{
    QList<ComplexArray const *> src = a->getData();
    QList<ComplexArray *> dest;
    int const r = ui.radiusSpinBox->value();
    int const xc = ui.xSpinBox->value();
    int const yc = ui.ySpinBox->value();
    //int const o = ui.orderSpinBox->value();
    for (QList<ComplexArray const *>::const_iterator i = src.begin(); i != src.end(); ++i) {
        if (ui.rectangularButton->isChecked()) {
            if (ui.lowpassButton->isChecked())
                dest << Operations::low_pass_rectangular(*i, xc, yc, r);
            else
                dest << Operations::high_pass_rectangular(*i, xc, yc, r);
        }
        else if (ui.butterworthButton->isChecked()) {
            if (ui.lowpassButton->isChecked())
                dest << Operations::low_pass_butterworth(*i, xc, yc, r, ui.orderSpinBox->value());
            else
                dest << Operations::high_pass_butterworth(*i, xc, yc, r, ui.orderSpinBox->value());
        }
        else if (ui.hanningButton->isChecked()) {
            if (ui.lowpassButton->isChecked())
                dest << Operations::low_pass_hanning(*i, xc, yc, r);
            else
                dest << Operations::high_pass_hanning(*i, xc, yc, r);
        }
        else if (ui.hammingButton->isChecked()) {
            if (ui.lowpassButton->isChecked())
                dest << Operations::low_pass_hamming(*i, xc, yc, r);
            else
                dest << Operations::high_pass_hamming(*i, xc, yc, r);
        }
        else if (ui.parzenButton->isChecked()) {
            if (ui.lowpassButton->isChecked())
                dest << Operations::low_pass_parzen(*i, xc, yc, r);
            else
                dest << Operations::high_pass_parzen(*i, xc, yc, r);
        }
        else if (ui.welchButton->isChecked()) {
            if (ui.lowpassButton->isChecked())
                dest << Operations::low_pass_welch(*i, xc, yc, r);
            else
                dest << Operations::high_pass_welch(*i, xc, yc, r);
        }
        else if (ui.gaussianButton->isChecked()) {
            if (ui.lowpassButton->isChecked())
                dest << Operations::gaussian_filter(*i, xc, yc, r);
        }
    }
    return dest;
}
