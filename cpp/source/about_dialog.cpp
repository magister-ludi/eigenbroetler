
#include <about_dialog.h>
#include <eigenbrot_window.h>
#include <eigenbrot_version.h>

AboutDialog::AboutDialog(QWidget *p):
    QDialog(p)
{
    ui.setupUi(this);
    setWindowTitle(QString(tr("About %1")).arg(EigenbrotWindow::win_name));
    setWindowIcon(QIcon(QPixmap(":/resources/eigen_icon.png")));

    // About tab
    QPixmap aboutLeft(":/resources/icon_49.png");
    QPixmap aboutRight(":/resources/lacrymshift.png");
    QSize sz(std::max(aboutLeft.width(), aboutRight.width()),
             std::max(aboutLeft.height(), aboutRight.height()));
    ui.aboutLeftLabel->setFixedSize(sz);
    ui.aboutLeftLabel->setPixmap(aboutLeft);
    ui.aboutRightLabel->setFixedSize(sz);
    ui.aboutRightLabel->setPixmap(aboutRight);

    QString info;
    info += QString("<b>%1</b> -- <font color=\"#0000ff\">").arg(EigenbrotWindow::win_name);
    info += QString(tr("complex data manipulation</font><br />"));
    info += QString("<br />");
    info += QString(tr(" - Fourier image processing<br />"));
    info += QString(tr(" - Fourier optics<br />"));
    info += QString(tr(" - and more...<br />"));
    info += QString("<br />");
    info += QString(tr("Copyright &copy; 2013, Michael Oldfield and Kieran Larkin<br />"));
    ui.aboutCentreLabel->setWordWrap(true);
    ui.aboutCentreLabel->setText(info);

    // Version tab
    QPixmap versionLeft(":/resources/egz4.png");
    QPixmap versionRight(":/resources/um2.png");
    sz = QSize(std::max(versionLeft.width(), versionRight.width()),
               std::max(versionLeft.height(), versionRight.height()));
    ui.versionLeftLabel->setFixedSize(sz);
    ui.versionLeftLabel->setPixmap(versionLeft);
    ui.versionRightLabel->setFixedSize(sz);
    ui.versionRightLabel->setPixmap(versionRight);

    info = QString(tr("<font color=\"#0000ff\">%1</font> Version %2.%3.%4<br />")).arg(EigenbrotWindow::win_name).
        arg(EIGENBROT_VERSION_MAJOR).arg(EIGENBROT_VERSION_MINOR).arg(EIGENBROT_VERSION_PATCH);
    info += QString("<br />");
    info += QString(tr("Git ID: %1<br />")).arg(QString(EIGENBROT_GIT_ID).left(8));
    info += QString("");
    ui.versionCentreLabel->setWordWrap(true);
    ui.versionCentreLabel->setText(info);

    ui.tabWidget->setCurrentIndex(0);
}
