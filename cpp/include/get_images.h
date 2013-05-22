// -*- c++ -*-
#ifndef GET_IMAGES_INCLUDE
#define GET_IMAGES_INCLUDE

#include <global_defs.h>
#include <QDialog>
#include <QGroupBox>
#include <QMap>
#include <calculator.h>

QT_BEGIN_NAMESPACE
class QButtonGroup;
class QRadioButton;
QT_END_NAMESPACE

class EigenbroetlerWindow;
class DataSelector;

class ImageLayout: public QGroupBox {
public:
    ImageLayout(QWidget *p, int id);
    DataSelector *fileCombo;
    QButtonGroup *buttonGroup;
    QRadioButton *truncate;
    QRadioButton *wrapX;
    QRadioButton *wrapY;
    QRadioButton *wrapBoth;
private:
    ImageLayout(); // not implemented
    ImageLayout(ImageLayout const&); // not implemented
    ImageLayout& operator=(ImageLayout const&); // not implemented
    Q_OBJECT
};

class GetImageDialog: public QDialog {
public:
    GetImageDialog(EigenbroetlerWindow *w, Calculator& calc);
    QString const& errors() const;
protected slots:
    void comboSelection();
    void accept();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    GetImageDialog(); // not implemented
    GetImageDialog(GetImageDialog const&); // not implemented
    GetImageDialog& operator=(GetImageDialog const&); // not implemented
    QMap<int, ImageLayout *> imageGroups;
    Calculator& c;
    DataSelector *currentCombo;
    QPushButton *okButton;
    QString err;
    Q_OBJECT
};

inline QString const& GetImageDialog::errors() const
{
    return err;
}

#endif // GET_IMAGES_INCLUDE
