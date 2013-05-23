
#include <get_images.h>
#include <QButtonGroup>
#include <QComboBox>
#include <QEvent>
#include <QFileInfo>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExp>
#include <QVBoxLayout>
#include <array_window.h>
#include <complex_array.h>
#include <eigenbroetler_window.h>
#include <data_selector.h>

static const int base_width = 380;
static const int base_height = 40;
static const int margin = 10;
static const int layout_height = 80;

ImageLayout::ImageLayout(QWidget *p, int id):
    QGroupBox(p)
{
    setTitle(QString(tr("Image #%1")).arg(id));
    QWidget *horizontalLayoutWidget = new QWidget(this);
    horizontalLayoutWidget->setGeometry(QRect(margin, margin, base_width - 2 * margin, layout_height));
    QHBoxLayout *hbox = new QHBoxLayout(horizontalLayoutWidget);
    hbox->setContentsMargins(0, 0, 0, 0);
    fileCombo = new DataSelector(horizontalLayoutWidget);
    hbox->addWidget(fileCombo);
    QGridLayout *grid = new QGridLayout();
    truncate = new QRadioButton(tr("T&runcate"), horizontalLayoutWidget);
    grid->addWidget(truncate, 0, 0, 1, 1);
    wrapX = new QRadioButton(tr("Wrap &X"), horizontalLayoutWidget);
    grid->addWidget(wrapX, 0, 1, 1, 1);
    wrapY = new QRadioButton(tr("Wrap &Y"), horizontalLayoutWidget);
    grid->addWidget(wrapY, 1, 0, 1, 1);
    wrapBoth = new QRadioButton(tr("Wrap &both"), horizontalLayoutWidget);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(truncate);
    buttonGroup->addButton(wrapX);
    buttonGroup->addButton(wrapY);
    buttonGroup->addButton(wrapBoth);
    truncate->setChecked(true);

    grid->addWidget(wrapBoth, 1, 1, 1, 1);
    hbox->addLayout(grid);
}

GetImageDialog::GetImageDialog(EigenbroetlerWindow *w, Calculator& calc):
    QDialog(w),
    c(calc),
    currentCombo(NULL)
{
    Calculator::ImageMap& image_list = c.getImageData();
    setWindowTitle(tr("Select images for formula"));
    QWidget *mainLayoutWidget = new QWidget(this);
    mainLayoutWidget->setGeometry(QRect(margin, margin, base_width,
                                        layout_height + 2 * layout_height * image_list.size()));
    QVBoxLayout *verticalLayout = new QVBoxLayout(mainLayoutWidget);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    QGroupBox *formulaBox = new QGroupBox(mainLayoutWidget);
    QLabel *formulaLabel = new QLabel(formulaBox);
    formulaBox->setTitle(QString(tr("Formula")));
    formulaLabel->setGeometry(QRect(margin, 3 * margin, base_width - 2 * margin, 20));
    formulaLabel->setAutoFillBackground(true);
    formulaLabel->setFrameShape(QFrame::Box);
    formulaLabel->setFrameShadow(QFrame::Raised);
    formulaLabel->setLineWidth(2);
    formulaLabel->setTextFormat(Qt::PlainText);
    formulaLabel->setText(c.getFormula());

    verticalLayout->addWidget(formulaBox);

    QList<int>keys = image_list.keys();
    QList<int>::const_iterator lnum;
    DataSelector list(w);
    for (lnum = keys.begin(); lnum != keys.end(); ++lnum) {
        ImageLayout *box = new ImageLayout(mainLayoutWidget, *lnum);
        imageGroups[*lnum] = box;
        verticalLayout->addWidget(box);
        box->fileCombo->installEventFilter(this);
        connect(box->fileCombo, SIGNAL(activated(QString)), this, SLOT(comboSelection()));
    }
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    buttonLayout->addItem(horizontalSpacer);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), mainLayoutWidget);
    buttonLayout->addWidget(cancelButton);
    okButton = new QPushButton(tr("OK"), mainLayoutWidget);
    buttonLayout->addWidget(okButton);
    verticalLayout->addLayout(buttonLayout);
    okButton->setDefault(true);
    okButton->setEnabled(false);
    comboSelection(); // This won't do anything except test whether
                       // okButton can be re-enabled

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void GetImageDialog::comboSelection()
{
    QMap<int, ImageLayout *>::iterator i;
    if (currentCombo && currentCombo->currentText() == DataSelector::chooseFileText()) {
        QString filename = EigenbroetlerWindow::getFileName(dynamic_cast<QWidget *>(parent()));
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
            for (i = imageGroups.begin(); i != imageGroups.end(); ++i)
                i.value()->fileCombo->insertItem(n - 1, name, filename);
            currentCombo->setCurrentIndex(n - 1);
        }
    }
    for (i = imageGroups.begin(); i != imageGroups.end(); ++i)
        if (i.value()->fileCombo->currentText() == DataSelector::chooseFileText())
            return;
    okButton->setEnabled(true);
}

bool GetImageDialog::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::FocusIn)
        currentCombo = dynamic_cast<DataSelector *>(obj);
    return QDialog::eventFilter(obj, evt);
}

void GetImageDialog::accept()
{
    Calculator::ImageMap& image_list = c.getImageData();
    Calculator::ImageMap::iterator m;
    err.clear();
    for (m = image_list.begin(); m != image_list.end(); ++m) {
        QMap<int, ImageLayout *>::const_iterator i = imageGroups.find(m.key());
        if (i == imageGroups.end()) {
            err += QString(tr("Internal error: no match for key #%1\n")).arg(m.key());
            return; // This shouldn't happen
        }
        ImageLayout const *l = i.value();
        Calculator::ImageData *d = m.value();
        QVariant v = l->fileCombo->itemData(l->fileCombo->currentIndex());
        d->ax = (l->truncate->isChecked() || l->wrapY->isChecked()) ?
            Calculator::ImageData::TRUNCATE : Calculator::ImageData::WRAP;
        d->ay = (l->truncate->isChecked() || l->wrapX->isChecked()) ?
            Calculator::ImageData::TRUNCATE : Calculator::ImageData::WRAP;
        if (v.canConvert(QVariant::String)) {
            // file name
            QString errStr;
            d->c << ComplexArray::readFileData(v.toString(), errStr, true)[0];
            if (!d->c.last()->isValid())
                err += errStr + "\n";
            d->owned = true;
            d->curr = 0;
        }
        else {
            // window data
            ArrayWindow const *w = dynamic_cast<ArrayWindow const *>(v.value<QWidget*>());
            d->c = w->getData();
            d->curr = w->currentIndex();
            d->owned = false;
        }
    }
    QDialog::accept();
}
