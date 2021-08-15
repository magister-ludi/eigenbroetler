
#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <eigenbroetler_window.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(eigenbrot);
    QApplication app(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    EigenbroetlerWindow *mainWin = EigenbroetlerWindow::instance();
#if defined(Q_OS_SYMBIAN)
    mainWin->showMaximized();
#else
    mainWin->show();
#endif
    for (char **ch = argv + 1; *ch; ++ch)
        mainWin->loadImage(*ch);
    return app.exec();
}
