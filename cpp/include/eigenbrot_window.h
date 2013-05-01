#ifndef EIGENBROT_WINDOW_INCLUDE
#define EIGENBROT_WINDOW_INCLUDE

#include <global_defs.h>
#include <QMainWindow>
#include <display_info.h>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QCloseEvent;
class QMdiArea;
class QMdiSubWindow;
class QMenu;
QT_END_NAMESPACE

class ArrayWindow;

class EigenbrotWindow: public QMainWindow {
 public:
    static QString const app_owner;
    static QString const app_name;
    static QString const win_name;
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
        void setComponent();
        void setColourMap();
        void windowActivated(QMdiSubWindow *w);
private:
    EigenbrotWindow(EigenbrotWindow const&); // not implemented
    EigenbrotWindow& operator=(EigenbrotWindow const&); // not implemented

    void loadSettings();
    void constructActions();
    void constructMenu();
    void constructToolbars();

    QMdiArea *mdiArea;
    // Actins for file menu
    QAction *newAction;
    QAction *openAction;
    QAction *saveAsAction;
    QAction *exitAction;
    // Actions for display menu
    QActionGroup *componentGroup;
    QAction *riAction;
    QAction *mpAction;
    //QAction *powAction;
    QActionGroup *colourGroup;
    QList<QAction *> colourActions;

    QMenu *fileMenu;
    QMenu *displayMenu;
    QToolBar *fileToolbar;
    //QToolBar *editToolBar;

    Q_OBJECT
};

inline EigenbrotWindow::~EigenbrotWindow()
{
}

#endif /* EIGENBROT_WINDOW_INCLUDE */
