#include <eigenbrot_window.h>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMdiArea>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>
#include <array_window_2d.h>
#include <complex_array.h>
#include <formula_dialog.h>

QString const EigenbrotWindow::app_owner("mao");
QString const EigenbrotWindow::app_name(QString::fromUtf8("eigenbrötler"));

QString const EigenbrotWindow::window_pos("pos");
QString const EigenbrotWindow::window_size("size");
QString const EigenbrotWindow::last_save("LastSave");
QString const EigenbrotWindow::last_read("LastRead");

EigenbrotWindow::EigenbrotWindow()
{
    setWindowTitle(app_name);

    // 1. Set up the basic container
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // TODO: should the following line be QImage or QPixmap? Both work!
    mdiArea->setBackground(QBrush(QPixmap(":/resources/eigenbg.png")));
    setCentralWidget(mdiArea);

    // 2. Set up actions, menu, toolbar
    constructActions();
    constructMenu();
    constructToolbars();

    QSettings const settings(app_owner, app_name);
    move(settings.value(window_pos, QPoint(80, 80)).toPoint());
    resize(settings.value(window_size, QSize(800, 600)).toSize());
    statusBar()->showMessage(tr("Ready"));
}

void EigenbrotWindow::closeEvent(QCloseEvent *evt)
{
    mdiArea->closeAllSubWindows();
    // if (mdiArea->currentSubWindow()) {
    //     evt->ignore();
    // } else {
    evt->accept();
    // }
}

void EigenbrotWindow::constructActions()
{
    newAction = new QAction(QIcon(":/resources/new.png"), tr("&New..."), this);
    newAction->setShortcuts(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new complex array"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newWindow()));

    openAction = new QAction(QIcon(":/resources/open.png"), tr("&Open..."), this);
    openAction->setStatusTip(tr("Open an image file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(readData()));

    saveAsAction = new QAction(QIcon(":/resources/save.png"), tr("&Save as..."), this);
    saveAsAction->setStatusTip(tr("Save complex array to disk"));
    saveAsAction->setEnabled(false);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(QString(tr("Close %1")).arg(app_name));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

void EigenbrotWindow::constructMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
}

void EigenbrotWindow::constructToolbars()
{
    fileToolbar = addToolBar(tr("File"));
    fileToolbar->addAction(newAction);
    fileToolbar->addAction(openAction);
    fileToolbar->addAction(saveAsAction);
}

void EigenbrotWindow::newWindow()
{
    ComplexArray *a = FormulaDialog::create_image(this);
    if (a) {
        ArrayWindow *w = ArrayWindow::createWindow(a);
        if (w) {
            mdiArea->addSubWindow(w);
            w->show();
        }
        // TODO: report errors
    }
}

void EigenbrotWindow::readData()
{
    static QString const fileTypes(QObject::tr("Image Files (*.fits *.fit *.png *.tif"
                                               " *.jpg *.bmp *.gif);;All files (*.*)"));
    QSettings settings(app_owner, app_name);
    QString dir = QFile::decodeName(settings.value(last_read, QString()).toString().toAscii());
    QFileDialog fd(this, tr("Read file"), dir, fileTypes);
    fd.setFileMode(QFileDialog::ExistingFile);
    if (fd.exec()) {
        QStringList fileNames = fd.selectedFiles();
        QString fileName = fileNames.at(0).toUtf8();
        int slash = fileName.lastIndexOf('/');
        settings.setValue(last_read, fileName.left(slash + 1));
        ComplexArray *cdata = new ComplexArray(fileName);
        if (!cdata->isValid()) {
            QMessageBox::warning(this, "File load failed", cdata->errorString());
            delete cdata;
        }
        else {
            ArrayWindow *w = ArrayWindow::createWindow(cdata);
            //ArrayWindow2D *w = new ArrayWindow2D;
            mdiArea->addSubWindow(w);
            w->show();
        }
    }
}

void EigenbrotWindow::resizeEvent(QResizeEvent *evt)
{
    QSettings settings(app_owner, app_name);
    settings.setValue(window_size, evt->size());
    QMainWindow::resizeEvent(evt);
}

void EigenbrotWindow::moveEvent(QMoveEvent *evt)
{
    QSettings settings(app_owner, app_name);
    settings.setValue(window_pos, evt->pos());
    QMainWindow::moveEvent(evt);
}
