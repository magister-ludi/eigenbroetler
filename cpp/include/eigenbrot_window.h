#ifndef EIGENBROT_WINDOW_INCLUDE
#define EIGENBROT_WINDOW_INCLUDE

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QCloseEvent;
class QMdiArea;
class QMenu;
QT_END_NAMESPACE

class EigenbrotWindow: public QMainWindow {
 public:
    EigenbrotWindow();
    ~EigenbrotWindow();
 protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void newWindow();
 private:
    EigenbrotWindow(EigenbrotWindow const&); // not implemented
    EigenbrotWindow& operator=(EigenbrotWindow const&); // not implemented

    void constructActions();
    void constructMenu();

    QMdiArea *mdiArea;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAsAction;
    QAction *exitAction;

    QMenu *fileMenu;

    Q_OBJECT
};

inline EigenbrotWindow::~EigenbrotWindow()
{
}

#endif /* EIGENBROT_WINDOW_INCLUDE */
