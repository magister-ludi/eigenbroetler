#include <arithmetic.h>
#include <eigenbroetler_window.h>
#include <limits>
#include <QFileInfo>
#include <QSettings>
#include <array_window.h>
#include <complex_array.h>
#include <complex_operations.h>

static QString const op_name("ArithmeticDialog/operator");
static QString const lnum_name("ArithmeticDialog/leftNumerator");
static QString const lden_name("ArithmeticDialog/leftDenominator");
static QString const loff_name("ArithmeticDialog/leftOffset");
static QString const rnum_name("ArithmeticDialog/rightNumerator");
static QString const rden_name("ArithmeticDialog/rightDenominator");
static QString const roff_name("ArithmeticDialog/rightOffset");

ArithmeticDialog::ArithmeticDialog(QWidget *w):
    QDialog(w),
    currentCombo(NULL)
{
    ui.setupUi(this);
    ui.leftComboBox->installEventFilter(this);
    ui.rightComboBox->installEventFilter(this);
    ui.okButton->setEnabled(false);

    ui.leftNum->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    ui.leftDen->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    ui.leftOffs->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    ui.rightNum->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    ui.rightDen->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    ui.rightOffs->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    EigenbroetlerWindow *eb = EigenbroetlerWindow::instance();
    QSettings settings(eb->app_owner, eb->app_name);
    ui.leftNum->setValue(settings.value(lnum_name, 1).toInt());
    ui.leftDen->setValue(settings.value(lden_name, 1).toInt());
    ui.leftOffs->setValue(settings.value(loff_name, 0).toInt());
    ui.rightNum->setValue(settings.value(rnum_name, 1).toInt());
    ui.rightDen->setValue(settings.value(rden_name, 1).toInt());
    ui.rightOffs->setValue(settings.value(roff_name, 0).toInt());
    op = abs(settings.value(op_name, 0).toInt()) % 4;
    buttons << ui.plusRadioButton;
    buttons << ui.minusRadioButton;
    buttons << ui.timesRadioButton;
    buttons << ui.divideRadioButton;
    buttons[op]->setChecked(true);
    comboSelection(); // This won't do anything except test whether
                       // okButton can be re-enabled
}

void ArithmeticDialog::comboSelection()
{
    if (currentCombo && currentCombo->currentText() == DataSelector::chooseFileText()) {
        QString filename = EigenbroetlerWindow::getFileName(this);
        if (!filename.isEmpty()) {
            QFileInfo file(filename);
            QString name = file.fileName();
            int count = 0;
            for (int k = 0; k < currentCombo->count(); ++k) {
                QVariant v = currentCombo->itemData(k);
                if (v.canConvert(QVariant::String)) {
                    if (filename == v.toString()) {
                        currentCombo->setCurrentIndex(k);
                        return;
                    }
                    else {
                        QFileInfo cmp(v.toString());
                        if (name == cmp.fileName())
                            ++count;
                    }
               }
            }
            if (count > 0)
                name += QString(" (%1)").arg(count + 1);
            int n = currentCombo->count();
            ui.leftComboBox->insertItem(n - 1, name, filename);
            ui.rightComboBox->insertItem(n - 1, name, filename);
            currentCombo->setCurrentIndex(n - 1);
        }
    }
    if (ui.leftComboBox->currentText() == DataSelector::chooseFileText() ||
        ui.rightComboBox->currentText() == DataSelector::chooseFileText())
        return;
    ui.okButton->setEnabled(true);
}

void ArithmeticDialog::accept()
{
    EigenbroetlerWindow *eb = EigenbroetlerWindow::instance();
    QSettings settings(eb->app_owner, eb->app_name);
    settings.setValue(lnum_name, ui.leftNum->value());
    settings.setValue(lden_name, ui.leftDen->value());
    settings.setValue(loff_name, ui.leftOffs->value());
    settings.setValue(rnum_name, ui.rightNum->value());
    settings.setValue(rden_name, ui.rightDen->value());
    settings.setValue(roff_name, ui.rightOffs->value());
    for (int i = 0; i < buttons.length(); ++i)
        if (buttons[i]->isChecked()) {
            op = i;
            break;
        }
    settings.setValue(op_name, op);
    QDialog::accept();
}

QList<ComplexArray const *> ArithmeticDialog::getData(DataSelector *s, bool& is_owned) const
{
    QVariant v = s->itemData(s->currentIndex());
    if (v.canConvert(QVariant::String)) {
        // file name
        is_owned = true;
        QString errStr;
        QList<ComplexArray *> arr = ComplexArray::readFileData(v.toString(), errStr, true);
        QList<ComplexArray const *> c_arr;
        for(QList<ComplexArray *>::iterator a = arr.begin(); a != arr.end(); ++a)
            c_arr << *a;
        return c_arr;
    }
    else {
        // window data
        ArrayWindow const *w = dynamic_cast<ArrayWindow const *>(v.value<QWidget*>());
        is_owned = false;
        return w->getData();
    }
}

QList<ComplexArray *> ArithmeticDialog::doArithmetic() const
{
    bool l_owned;
    QList<ComplexArray const *> left_operand = getData(ui.leftComboBox, l_owned);
    bool r_owned;
    QList<ComplexArray const *> right_operand = getData(ui.rightComboBox, r_owned);
    QList<ComplexArray *> arith_result;

    QSize sz(left_operand[0]->width(), left_operand[0]->height());
    bool same_size = true;
    for (int i = 0; i < left_operand.length(); ++i) {
        same_size &= (sz.width() == left_operand[i]->width());
        same_size &= (sz.height() == left_operand[i]->height());
        sz.setWidth(std::max(sz.width(), left_operand[i]->width()));
        sz.setHeight(std::max(sz.height(), left_operand[i]->height()));
    }
    for (int i = 0; i < right_operand.length(); ++i) {
        same_size &= (sz.width() == right_operand[i]->width());
        same_size &= (sz.height() == right_operand[i]->height());
        sz.setWidth(std::max(sz.width(), right_operand[i]->width()));
        sz.setHeight(std::max(sz.height(), right_operand[i]->height()));
    }
    if (!same_size) {
        for (int i = 0; i < left_operand.length(); ++i) {
            ComplexArray const *a = Operations::resize(left_operand[i], sz.width(), sz.height());
            if (l_owned)
                delete left_operand[i];
            left_operand[i] = a;
        }
        for (int i = 0; i < right_operand.length(); ++i) {
            ComplexArray const *a = Operations::resize(right_operand[i], sz.width(), sz.height());
            if (r_owned)
                delete right_operand[i];
            right_operand[i] = a;
        }
        l_owned = r_owned = true;
    }
    Operations::ArithOp arith_op = Operations::ArithOp(-1);
    switch (op) {
    case 0:
        arith_op = Operations::ADD;
        break;
    case 1:
        arith_op = Operations::SUBTRACT;
        break;
    case 2:
        arith_op = Operations::MULTIPLY;
        break;
    case 3:
        arith_op = Operations::DIVIDE;
        break;
    };
    if (left_operand.length() == right_operand.length()) {
        for (int i = 0; i < left_operand.length(); ++i)
            arith_result << Operations::arithmetic(left_operand[i],
                                                   ui.leftNum->value(), ui.leftDen->value(), ui.leftOffs->value(),
                                                   arith_op,
                                                   right_operand[i],
                                                   ui.rightNum->value(), ui.rightDen->value(), ui.rightOffs->value());

    }
    else if (left_operand.length() == 1) {
        for (int i = 0; i < right_operand.length(); ++i)
            arith_result << Operations::arithmetic(left_operand[0],
                                                   ui.leftNum->value(), ui.leftDen->value(), ui.leftOffs->value(),
                                                   arith_op,
                                                   right_operand[i],
                                                   ui.rightNum->value(), ui.rightDen->value(), ui.rightOffs->value());

    }
    else if (right_operand.length() == 1) {
        for (int i = 0; i < left_operand.length(); ++i)
            arith_result << Operations::arithmetic(left_operand[i],
                                                   ui.leftNum->value(), ui.leftDen->value(), ui.leftOffs->value(),
                                                   arith_op,
                                                   right_operand[0],
                                                   ui.rightNum->value(), ui.rightDen->value(), ui.rightOffs->value());

    }
    else {
        int const n = std::min(left_operand.length(), right_operand.length());
        for (int i = 0; i < n; ++i)
            arith_result << Operations::arithmetic(left_operand[i],
                                                   ui.leftNum->value(), ui.leftDen->value(), ui.leftOffs->value(),
                                                   arith_op,
                                                   right_operand[i],
                                                   ui.rightNum->value(), ui.rightDen->value(), ui.rightOffs->value());

    }

    if (l_owned)
        for (int i = 0; i < left_operand.length(); ++i)
            delete left_operand[i];
    if (r_owned)
        for (int i = 0; i < right_operand.length(); ++i)
            delete right_operand[i];
    return arith_result;
}

bool ArithmeticDialog::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::FocusIn)
        currentCombo = dynamic_cast<DataSelector *>(obj);
    return QDialog::eventFilter(obj, evt);
}

void EigenbroetlerWindow::arithmetic()
{
    ArithmeticDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        QList<ComplexArray *> da = dlg.doArithmetic();
        newWindow(da, true);
    }
}
