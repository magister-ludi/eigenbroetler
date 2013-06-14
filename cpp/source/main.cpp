
#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QTranslator>
#include <eigenbroetler_window.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(eigenbrot);
    QApplication app(argc, argv);
    QTranslator translator;
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    translator.load(QFile::decodeName(QString("eigenbroetler_de").toAscii()));
    app.installTranslator(&translator);

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
