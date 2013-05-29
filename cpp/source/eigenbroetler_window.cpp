#include <eigenbroetler_window.h>
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QImageReader>
#include <QInputDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QSignalMapper>
#include <QStatusBar>
#include <QToolBar>
#include <about_dialog.h>
#include <array_window_2d.h>
#include <complex_array.h>
#include <complex_operations.h>
#include <formula_dialog.h>

QString const EigenbroetlerWindow::app_owner("magister-ludi");
QString const EigenbroetlerWindow::app_name(QString::fromUtf8("eigenbroetler"));
QString const EigenbroetlerWindow::win_name(QString::fromUtf8("eigenbrötler"));

QString const EigenbroetlerWindow::window_size("size");
QString const EigenbroetlerWindow::last_save("LastSave");
QString const EigenbroetlerWindow::last_read("LastRead");

EigenbroetlerWindow *EigenbroetlerWindow::eb = NULL;

EigenbroetlerWindow::EigenbroetlerWindow()
{
    setWindowIcon(QIcon(QPixmap(":/resources/eigen_icon.png")));

    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubwindow(QWidget*)));
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
    resize(settings.value(window_size, QSize(800, 600)).toSize());
    resetGUI();
    statusBar()->showMessage(tr("Ready"));
}

void EigenbroetlerWindow::resetGUI()
{
    QMdiSubWindow const *w = mdiArea->activeSubWindow();
    ArrayWindow const *a = getArrayWindow(w);
    bool enable = (a != NULL);
    QList<QAction *>::iterator op;
    for (op = disabledActions.begin(); op != disabledActions.end(); ++op)
        (*op)->setEnabled(enable);
    QList<QWidget *>::iterator menu;
    for (menu = disabledWidgets.begin(); menu != disabledWidgets.end(); ++menu)
        (*menu)->setEnabled(enable);

    if (a) {
        bool disable = a->getComponent() == DisplayInfo::REAL;
        if (disable)
            riAction->setChecked(true);
        else
            mpAction->setChecked(true);
        if (disable)
            linAction->setChecked(true);
        linAction->setEnabled(!disable);
        logAction->setEnabled(!disable);
        powAction->setEnabled(!disable);
        int dummy;
        switch(a->getScale(dummy)) {
        case DisplayInfo::LIN:
            linAction->setChecked(true);
            break;
        case DisplayInfo::LOG:
            logAction->setChecked(true);
            break;
        case DisplayInfo::POW:
            powAction->setChecked(true);
            break;
        }
        setWindowTitle(a->windowTitle() + " - " + win_name);
    }
    else if (w)
        setWindowTitle(w->windowTitle() + " - " + win_name);
    else
        setWindowTitle(win_name);
}

void EigenbroetlerWindow::closeEvent(QCloseEvent *evt)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow())
        evt->ignore();
    else
        evt->accept();
}

void EigenbroetlerWindow::constructActions()
{
    // File actions
    openAction = new QAction(QIcon(":/resources/open.png"), tr("&Open..."), this);
    openAction->setStatusTip(tr("Open an image file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(readData()));

    saveAsAction = new QAction(QIcon(":/resources/save.png"), tr("&Save as..."), this);
    saveAsAction->setShortcut(tr("Ctrl+S"));
    saveAsAction->setStatusTip(tr("Save complex array to disk"));
    disabledActions << saveAsAction;
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveData()));

    exportAction = new QAction(QIcon(":/resources/export.png"), tr("&Export..."), this);
    exportAction->setStatusTip(tr("Save display images to disk"));
    disabledActions << exportAction;
    connect(exportAction, SIGNAL(triggered()), this, SLOT(exportImages()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(QString(tr("Close %1")).arg(win_name));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
    // Basic actions
    newAction = new QAction(QIcon(":/resources/new.png"), tr("&New..."), this);
    newAction->setShortcuts(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new complex array"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newWindow()));
    padAction = new QAction(tr("&Pad/Crop..."), this);
    padAction->setStatusTip(tr("Pad or crop complex array"));
    connect(padAction, SIGNAL(triggered()), this, SLOT(padData()));
    disabledActions << padAction;
    arithAction = new QAction(QIcon(":/resources/arith.png"), tr("&Arithmetic..."), this);
    arithAction->setStatusTip(tr("Arithmetic operations on complex arrays"));
    connect(arithAction, SIGNAL(triggered()), this, SLOT(arithmetic()));

    // Advanced actions
    dislocationAction = new QAction(tr("&Spiral dislocation..."), this);
    dislocationAction->setStatusTip(tr("Insert spiral phase dislocation"));
    connect(dislocationAction, SIGNAL(triggered()), this, SLOT(dislocation()));
    constPhaseAction = new QAction(tr("Add &constant phase..."), this);
    constPhaseAction->setStatusTip(tr("Add a constant phase"));
    connect(constPhaseAction, SIGNAL(triggered()), this, SLOT(addConstPhase()));
    linearPhaseAction = new QAction(tr("Add &linear phase..."), this);
    linearPhaseAction->setStatusTip(tr("Add a linear phase"));
    connect(linearPhaseAction, SIGNAL(triggered()), this, SLOT(addLinearPhase()));
    quadPhaseAction = new QAction(tr("Add &quadratic phase..."), this);
    quadPhaseAction->setStatusTip(tr("Add a quadratic phase"));
    connect(quadPhaseAction, SIGNAL(triggered()), this, SLOT(addQuadPhase()));

    // Fourier actions
    fftAction = new QAction(QIcon(":/resources/fft.png"), tr("2D &FFT"), this);
    fftAction->setShortcut(tr("Ctrl+F"));
    fftAction->setStatusTip(tr("2D discrete Fourier transform"));
    connect(fftAction, SIGNAL(triggered()), this, SLOT(fft()));
    disabledActions << fftAction;
    fftxAction = new QAction(tr("1D FFT (&X direction)"), this);
    fftxAction->setStatusTip(tr("1D discrete Fourier transform in X direction"));
    connect(fftxAction, SIGNAL(triggered()), this, SLOT(fftx()));
    disabledActions << fftxAction;
    fftyAction = new QAction(tr("1D FFT (&Y direction)"), this);
    fftyAction->setStatusTip(tr("1D discrete Fourier transform in Y direction"));
    connect(fftyAction, SIGNAL(triggered()), this, SLOT(ffty()));
    disabledActions << fftyAction;

    // Settings actions
    // 1. components
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

    toggleAction = new QAction(QIcon(":/resources/toggle.png"), tr("&Toggle components"), this);
    disabledActions << toggleAction;
    toggleAction->setStatusTip(tr("Toggle component display"));
    connect(toggleAction, SIGNAL(triggered()), this, SLOT(toggleComponents()));
    // 2. scale
    scaleGroup = new QActionGroup(this);
    scaleGroup->setExclusive(true);
    linAction = new QAction(QIcon(":/resources/lin.png"), tr("Li&near"), this);
    scaleGroup->addAction(linAction);
    linAction->setCheckable(true);
    linAction->setStatusTip(tr("Linear scaling"));
    connect(linAction, SIGNAL(triggered()), this, SLOT(setScale()));
    disabledActions << linAction;
    logAction = new QAction(QIcon(":/resources/log.png"), tr("Lo&garithmic"), this);
    scaleGroup->addAction(logAction);
    logAction->setCheckable(true);
    logAction->setStatusTip(tr("Logarithmic scaling"));
    connect(logAction, SIGNAL(triggered()), this, SLOT(setScale()));
    disabledActions << logAction;
    powAction = new QAction(QIcon(":/resources/root.png"), tr("R&oot"), this);
    scaleGroup->addAction(powAction);
    powAction->setCheckable(true);
    powAction->setStatusTip(tr("Root scaling"));
    connect(powAction, SIGNAL(triggered()), this, SLOT(setScale()));
    disabledActions << powAction;
    // 3. Colours
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
    }
    colourmapAction = new QAction(QIcon(":/resources/colours.png"), tr("&Colours"), this);
    colourmapAction->setStatusTip(tr("Choose colour map"));
    connect(colourmapAction, SIGNAL(triggered()), this, SLOT(setColourMap()));
    disabledActions << colourmapAction;

    // Window actions
    closeAction = new QAction(tr("&Close window"), this);
    closeAction->setStatusTip(tr("Close active window"));
    connect(closeAction, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));
    closeAllAction = new QAction(tr("Close &All windows"), this);
    closeAllAction->setStatusTip(tr("Close all windows"));
    connect(closeAllAction, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
    tileAction = new QAction(tr("&Tile windows"), this);
    tileAction->setStatusTip(tr("Tile windows"));
    connect(tileAction, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
    cascadeAction = new QAction(tr("C&ascade windows"), this);
    cascadeAction->setStatusTip(tr("Cascade windows"));
    connect(cascadeAction, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));
    nextAction = new QAction(tr("&Next window"), this);
    nextAction->setShortcuts(QKeySequence::NextChild);
    connect(nextAction, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));
    previousAction = new QAction(tr("&Previous window"), this);
    previousAction->setShortcuts(QKeySequence::PreviousChild);
    connect(previousAction, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));
    separatorAction = new QAction(this);
    separatorAction->setSeparator(true);

    // help actions
    helpAction = new QAction(tr("&Help"), this);
    helpAction->setEnabled(false);
    aboutAction = new QAction(tr("A&bout ") + win_name, this);
    aboutAction->setStatusTip(aboutAction->text());
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void EigenbroetlerWindow::constructMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(exportAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    basicOpsMenu = menuBar()->addMenu(tr("&Basic"));
    basicOpsMenu->addAction(newAction);
    basicOpsMenu->addAction(padAction);
    basicOpsMenu->addAction(arithAction);

    advancedOpsMenu = menuBar()->addMenu(tr("&Advanced"));
    phaseOpsMenu = advancedOpsMenu->addMenu(tr("&Phase"));
    phaseOpsMenu->addAction(dislocationAction);
    phaseOpsMenu->addAction(constPhaseAction);
    phaseOpsMenu->addAction(linearPhaseAction);
    phaseOpsMenu->addAction(quadPhaseAction);
    disabledWidgets << advancedOpsMenu;

    fourierMenu = menuBar()->addMenu(tr("&Fourier"));
    fourierMenu->addAction(fftAction);
    fourierMenu->addAction(fftxAction);
    fourierMenu->addAction(fftyAction);
    disabledWidgets << fourierMenu;

    displayMenu = menuBar()->addMenu(tr("&Settings"));
    disabledWidgets << displayMenu;
    displayMenu->addAction(colourmapAction);
    colourMenu = new QMenu(tr("Colour maps"), this);
    QList<QAction *>::const_iterator a;
    for (a = colourActions.begin(); a != colourActions.end(); ++a)
        colourMenu->addAction(*a);
    displayMenu->addSeparator()->setText(tr("Components"));
    displayMenu->addAction(riAction);
    displayMenu->addAction(mpAction);
    displayMenu->addSeparator()->setText(tr("Scale"));
    displayMenu->addAction(linAction);
    displayMenu->addAction(logAction);
    displayMenu->addAction(powAction);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    disabledWidgets << windowMenu;

    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);
}

void EigenbroetlerWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAction);
    windowMenu->addAction(closeAllAction);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAction);
    windowMenu->addAction(cascadeAction);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAction);
    windowMenu->addAction(previousAction);
    windowMenu->addAction(separatorAction);

    QList<QMdiSubWindow *> window_list = mdiArea->subWindowList();
    separatorAction->setVisible(!window_list.isEmpty());

    QList<QMdiSubWindow *>::iterator w;
    for (w = window_list.begin(); w != window_list.end(); ++w) {
        QWidget const *wd = dynamic_cast<QWidget const *>((*w)->widget());
        int i = w - window_list.begin();
        QString text;
        if (i < 9)
            text = tr("&%1 %2").arg(i + 1).arg(wd->windowTitle());
        else
            text = tr("%1 %2").arg(i + 1).arg(wd->windowTitle());
        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(*w == mdiArea->activeSubWindow());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, *w);
    }
}

void EigenbroetlerWindow::constructToolbars()
{
    fileToolbar = addToolBar("File");
    fileToolbar->addAction(openAction);
    fileToolbar->addAction(saveAsAction);
    fileToolbar->addAction(exportAction);

    opsToolbar = addToolBar("Operations");
    opsToolbar->addAction(newAction);
    opsToolbar->addAction(fftAction);
    opsToolbar->addAction(arithAction);

    displayToolbar = addToolBar("Display");
    displayToolbar->addAction(linAction);
    displayToolbar->addAction(logAction);
    displayToolbar->addAction(powAction);
    displayToolbar->addAction(toggleAction);
    displayToolbar->addAction(colourmapAction);
}

void EigenbroetlerWindow::newWindow()
{
    bool stack;
    QList<ComplexArray *> a = FormulaDialog::create_image(this, stack);
    newWindow(a, stack);
}

void EigenbroetlerWindow::newWindow(QList<ComplexArray *>& a, bool stack)
{
    if (!a.isEmpty()) {
        DisplayInfo::ComplexComponent cmp = a.at(0)->isFFT() ? DisplayInfo::MAGN : DisplayInfo::REAL;
        DisplayInfo::Scale scl = a.at(0)->isFFT() ? DisplayInfo::LOG : DisplayInfo::LIN;
        QString p = colourGroup->checkedAction()->text();
        if (stack && a.size() > 1) {
            ArrayWindow *w = ArrayWindow::createWindow(a, cmp, scl,
                                                       DisplayInfo::instance().getColourMap(p));
            if (w) {
                mdiArea->addSubWindow(w);
                w->show();
            }
        }
        else {
            while (!a.isEmpty()) {
                QList<ComplexArray *> arg;
                arg << a.at(0);
                a.pop_front();
                ArrayWindow *w = ArrayWindow::createWindow(arg, cmp, scl,
                                                           DisplayInfo::instance().getColourMap(p));
                if (w) {
                    mdiArea->addSubWindow(w);
                    w->show();
                }
            }
        }
    }
}

QString EigenbroetlerWindow::getFileName(QWidget *p)
{
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    QList<QByteArray>::iterator fmt;
    QString fileTypes(tr("Image Files (*.fits *.fit"));
    for (fmt = formats.begin(); fmt != formats.end(); ++fmt)
        fileTypes += QString(" *.") + *fmt;
    fileTypes += tr(";;All files (*.*)");

    QSettings settings(app_owner, app_name);
    QString dir = QFile::decodeName(settings.value(last_read, QString()).toString().toAscii());
    QString fileName = QFileDialog::getOpenFileName(p, tr("Read file"),
                                                    dir, fileTypes, 0, QFileDialog::DontUseNativeDialog);
    if (!fileName.isEmpty()) {
#if defined(Q_OS_WIN)
        fileName = fileName.toUtf8().toLower();
#else
        fileName = fileName.toUtf8();
#endif
        int slash = fileName.lastIndexOf('/');
        settings.setValue(last_read, fileName.left(slash + 1));
    }
    return fileName;
}

void EigenbroetlerWindow::readData()
{
    QString fileName = getFileName(this);
    if (!fileName.isEmpty())
        loadImage(fileName);
}

void EigenbroetlerWindow::loadImage(QString const& filename)
{
    QString err;
    QList<ComplexArray *> cdata = ComplexArray::readFileData(filename, err);
    if (!err.isEmpty()) {
        QMessageBox::warning(this, "File load failed", err);
        QList<ComplexArray *>::iterator c;
        for (c = cdata.begin(); c != cdata.end(); ++c)
            delete *c;
    }
    else
        newWindow(cdata, true);
}

ArrayWindow *EigenbroetlerWindow::getArrayWindow(QMdiSubWindow *w)
{
    ArrayWindow *a = NULL;
    if (w)
        a = dynamic_cast<ArrayWindow *>(w->widget());
    return a;
}

ArrayWindow const *EigenbroetlerWindow::getArrayWindow(QMdiSubWindow const *w) const
{
    ArrayWindow *a = NULL;
    if (w)
        a = dynamic_cast<ArrayWindow *>(w->widget());
    return a;
}

void EigenbroetlerWindow::exportImages()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a)
        a->exportComponents();
}

void EigenbroetlerWindow::saveData()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        a->saveData();
        resetGUI();
    }
}

void EigenbroetlerWindow::setComponent()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        if (componentGroup->checkedAction() == riAction)
            a->setComponent(DisplayInfo::REAL);
        else
            a->setComponent(DisplayInfo::MAGN);
        resetGUI();
    }
}

void EigenbroetlerWindow::toggleComponents()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        if (a->getComponent() == DisplayInfo::REAL)
            a->setComponent(DisplayInfo::MAGN);
        else
            a->setComponent(DisplayInfo::REAL);
        resetGUI();
    }
}

void EigenbroetlerWindow::setScale()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        if (scaleGroup->checkedAction() == linAction)
            a->setScale(DisplayInfo::LIN);
        else if (scaleGroup->checkedAction() == logAction)
            a->setScale(DisplayInfo::LOG);
        else if (scaleGroup->checkedAction() == powAction) {
            int	root = QInputDialog::getInt(this, tr("Select root to use"),
                                            tr("Root:"), 2, 2);
            a->setScale(DisplayInfo::POW, root);
        }
        resetGUI();
    }
}

void EigenbroetlerWindow::setColourMap()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        if (colourMenu->exec(QCursor::pos()))
            a->setColourMap(DisplayInfo::instance().getColourMap(colourGroup->checkedAction()->text()));
    }
}

void EigenbroetlerWindow::resizeEvent(QResizeEvent *evt)
{
    QSettings settings(app_owner, app_name);
    settings.setValue(window_size, evt->size());
    QMainWindow::resizeEvent(evt);
}

void EigenbroetlerWindow::windowActivated(QMdiSubWindow *)
{
    resetGUI();
}

void EigenbroetlerWindow::setActiveSubwindow(QWidget *w)
{
    if (w)
        mdiArea->setActiveSubWindow(dynamic_cast<QMdiSubWindow *>(w));
}

void EigenbroetlerWindow::fft()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        for (int i = 0; i < a->numDataSets(); ++i)
            da << a->getData()[i]->dft(true);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::fftx()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        for (int i = 0; i < a->numDataSets(); ++i)
            da << a->getData()[i]->xdft(true);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::ffty()
{
    ArrayWindow *a = getArrayWindow(mdiArea->activeSubWindow());
    if (a) {
        QList<ComplexArray *> da;
        for (int i = 0; i < a->numDataSets(); ++i)
            da << a->getData()[i]->ydft(true);
        newWindow(da, true);
    }
}

void EigenbroetlerWindow::about()
{
    AboutDialog dlg(this);
    dlg.exec();
}
