#include <eigenbrot_window.h>
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
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
QString const EigenbrotWindow::app_name(QString::fromUtf8("eigenbroetler"));
QString const EigenbrotWindow::win_name(QString::fromUtf8("eigenbrötler"));

QString const EigenbrotWindow::window_pos("pos");
QString const EigenbrotWindow::window_size("size");
QString const EigenbrotWindow::last_save("LastSave");
QString const EigenbrotWindow::last_read("LastRead");

EigenbrotWindow::EigenbrotWindow()
{
    setWindowTitle(win_name);

    // 1. Set up the basic container
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(windowActivated(QMdiSubWindow *)));
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
    saveAsAction->setShortcut(tr("Ctrl+S"));
    saveAsAction->setStatusTip(tr("Save complex array to disk"));
    saveAsAction->setEnabled(false);
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(QString(tr("Close %1")).arg(win_name));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    componentGroup = new QActionGroup(this);
    componentGroup->setExclusive(true);
    riAction = new QAction(tr("&Real/imaginary"), this);
    componentGroup->addAction(riAction);
    riAction->setCheckable(true);
    riAction->setShortcut(tr("Ctrl+R"));
    riAction->setStatusTip(tr("Display real and imaginary components"));
    connect(riAction, SIGNAL(triggered()), this, SLOT(setComponent()));
    mpAction = new QAction(tr("&Magnitude/Phase"), this);
    componentGroup->addAction(mpAction);
    mpAction->setCheckable(true);
    mpAction->setShortcut(tr("Ctrl+M"));
    mpAction->setStatusTip(tr("Display magnitude and phase"));
    connect(mpAction, SIGNAL(triggered()), this, SLOT(setComponent()));

    //powAction = new QAction(tr("&Power law..."), this);
    //componentGroup->addAction(powAction);
    //powAction->setShortcuts();
    //powAction->setStatusTip(tr(""));
    //connect(powAction, SIGNAL(triggered()), this, SLOT(setComponents()));

    colourGroup = new QActionGroup(this);
    colourGroup->setExclusive(true);
    QList<QString> map_names = DisplayInfo::instance().getColourMapNames();
    qSort(map_names.begin(), map_names.end());
    QList<QString>::const_iterator pal;
    QString const checkName = DisplayInfo::instance().getDefaultColourMapName();
    for (pal = map_names.begin(); pal != map_names.end(); ++pal) {
        QAction *act = new QAction(*pal, this);
        colourActions << act;
        act->setCheckable(true);
        if (*pal == checkName)
            act->setChecked(true);
        colourGroup->addAction(act);
        connect(act, SIGNAL(triggered()), this, SLOT(setColourMap()));
    }
}

void EigenbrotWindow::constructMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    displayMenu = menuBar()->addMenu(tr("&Display"));
    displayMenu->addSeparator()->setText(tr("Components"));
    displayMenu->addAction(riAction);
    displayMenu->addAction(mpAction);
    displayMenu->setEnabled(false);
    displayMenu->addSeparator();
    QMenu *coloursMenu = displayMenu->addMenu(tr("&Colours"));
    QList<QAction *>::const_iterator a;
    for (a = colourActions.begin(); a != colourActions.end(); ++a)
        coloursMenu->addAction(*a);
    //displayMenu->addAction(powAction);
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
        QString p = colourGroup->checkedAction()->text();
        ArrayWindow *w = ArrayWindow::createWindow(a, DisplayInfo::REAL, DisplayInfo::LIN,
                                                   DisplayInfo::instance().getColourMap(p));
        if (w) {
            mdiArea->addSubWindow(w);
            w->show();
        }
        // TODO: report errors
    }
}

void EigenbrotWindow::readData()
{
    QString fileTypes(tr("Image Files (*.fits *.fit *.png *.tif"
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
        QString p = colourGroup->checkedAction()->text();
        ArrayWindow *w = ArrayWindow::createWindow(cdata, DisplayInfo::REAL, DisplayInfo::LIN,
                                                   DisplayInfo::instance().getColourMap(p));
            mdiArea->addSubWindow(w);
            w->show();
            displayMenu->setEnabled(true);
            riAction->setChecked(true);
       }
    }
}

void EigenbrotWindow::setComponent()
{
    QMdiSubWindow *w = mdiArea->activeSubWindow();
    if (w) {
        ArrayWindow *a = dynamic_cast<ArrayWindow *>(w->widget());
        if (a) {
            if (componentGroup->checkedAction() == riAction)
                a->setComponent(DisplayInfo::REAL);
            else
                a->setComponent(DisplayInfo::MAGN);
        }
    }
}

void EigenbrotWindow::setColourMap()
{
    QMdiSubWindow *w = mdiArea->activeSubWindow();
    if (w) {
        ArrayWindow *a = dynamic_cast<ArrayWindow *>(w->widget());
        if (a)
            a->setColourMap(DisplayInfo::instance().getColourMap(colourGroup->checkedAction()->text()));
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

void EigenbrotWindow::windowActivated(QMdiSubWindow *w)
{
    if (w) {
        setWindowTitle(QString(w->widget()->windowTitle()) + " - " + win_name);
        ArrayWindow *a = dynamic_cast<ArrayWindow *>(w->widget());
        if (a) {
            //if (a->getScale() == DisplayInfo::LIN)
            if (a->getComponent() == DisplayInfo::REAL)
                riAction->setChecked(true);
            else
                mpAction->setChecked(true);
            displayMenu->setEnabled(true);
        }
    }
    else {
        setWindowTitle(win_name);
        displayMenu->setEnabled(false);
    }
}
