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
class ComplexArray;

class EigenbrotWindow: public QMainWindow {
 public:
    static QString const app_owner;
    static QString const app_name;
    static QString const win_name;
    static QString const window_size;
    static QString const last_save;
    static QString const last_read;

    EigenbrotWindow();
    ~EigenbrotWindow();
 protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
 private slots:
        void newWindow();
        void readData();
        void saveData();
        void setComponent();
        void setColourMap();
        void fft();
        void about();
        void windowActivated(QMdiSubWindow *w);
private:
    EigenbrotWindow(EigenbrotWindow const&); // not implemented
    EigenbrotWindow& operator=(EigenbrotWindow const&); // not implemented

    void newWindow(ComplexArray *a);
    void loadSettings();
    void constructActions();
    void constructMenu();
    void constructToolbars();
    void enableOperations(bool enable);

    QMdiArea *mdiArea;
    // Actions for file menu
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
    // Actions for Fourier menu
    QAction *fftAction;
    // Actions for help menu
    QAction *helpAction;
    QAction *aboutAction;

    QMenu *fileMenu;
    QMenu *displayMenu;
    QMenu *fourierMenu;
    QMenu *helpMenu;
    QToolBar *fileToolbar;
    QToolBar *opsToolbar;
    QList<QAction *> complexOps;
    QList<QMenu *> complexMenus;

    Q_OBJECT
};

inline EigenbrotWindow::~EigenbrotWindow()
{
}

#endif /* EIGENBROT_WINDOW_INCLUDE */
