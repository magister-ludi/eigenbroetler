#include <eigenbrot_window.h>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QMdiArea>
#include <QMenuBar>
#include <QPixmap>
#include <array_window_2d.h>
#include <iostream>

EigenbrotWindow::EigenbrotWindow()
{
    //setTitle(QString::fromUtf8("eigenbrötler"));
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
    resize(800, 600);
    move (80, 80);
}

void EigenbrotWindow::closeEvent(QCloseEvent *evt)
{
    mdiArea->closeAllSubWindows();
    // if (mdiArea->currentSubWindow()) {
    //     evt->ignore();
    // } else {
    //     writeSettings();
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

    saveAsAction = new QAction(QIcon(":/resources/save.png"), tr("&Save as..."), this);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
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

void EigenbrotWindow::newWindow()
{
    // Placeholder code, until we have a formula parser...
    ArrayWindow2D *w = new ArrayWindow2D;
    mdiArea->addSubWindow(w);
    w->show();
    //return child;
}
