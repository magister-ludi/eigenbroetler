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
    static QString const app_owner;
    static QString const app_name;
    static QString const window_pos;
    static QString const window_size;
    static QString const last_save;
    static QString const last_read;

    EigenbrotWindow();
    ~EigenbrotWindow();
 protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
 private slots:
        void newWindow();
        void readData();
private:
    EigenbrotWindow(EigenbrotWindow const&); // not implemented
    EigenbrotWindow& operator=(EigenbrotWindow const&); // not implemented

    void loadSettings();
    void constructActions();
    void constructMenu();
    void constructToolbars();

    QMdiArea *mdiArea;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAsAction;
    QAction *exitAction;

    QMenu *fileMenu;
    QToolBar *fileToolbar;
    //QToolBar *editToolBar;

    Q_OBJECT
};

inline EigenbrotWindow::~EigenbrotWindow()
{
}

#endif /* EIGENBROT_WINDOW_INCLUDE */
