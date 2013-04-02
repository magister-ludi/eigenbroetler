
#include <QApplication>
#include <eigenbrot_window.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(eigenbrot);

    QApplication app(argc, argv);
    EigenbrotWindow *mainWin = EigenbrotWindow::instance();
#if defined(Q_OS_SYMBIAN)
    mainWin->showMaximized();
#else
    mainWin->show();
#endif
    int result = app.exec();
    delete mainWin;
    return result;
}
