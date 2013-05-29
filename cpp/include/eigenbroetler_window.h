// -*- c++ -*-
#ifndef EIGENBROT_WINDOW_INCLUDE
#define EIGENBROT_WINDOW_INCLUDE

#include <global_defs.h>
#include <QMainWindow>
#include <display_info.h>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QCloseEvent;
template<typename T>class QList;
class QMdiArea;
class QMdiSubWindow;
class QMenu;
class QSignalMapper;
QT_END_NAMESPACE

class ArrayWindow;
class ComplexArray;

class EigenbroetlerWindow: public QMainWindow {
public:
    static QString const app_owner;
    static QString const app_name;
    static QString const win_name;
    static QString const window_size;
    static QString const last_save;
    static QString const last_read;

    static EigenbroetlerWindow *instance();
    ~EigenbroetlerWindow();
    static QString getFileName(QWidget *p);
    void loadImage(QString const& filename);
protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
private slots:
    void newWindow();
    void readData();
    void saveData();
    void exportImages();
    void setComponent();
    void toggleComponents();
    void setScale();
    void setColourMap();
    void fft();
    void fftx();
    void ffty();
    void about();
    void windowActivated(QMdiSubWindow *w);
    void updateWindowMenu();
    void setActiveSubwindow(QWidget *w);
    void dislocation();
    void addConstPhase();
    void addLinearPhase();
    void addQuadPhase();
    void padData();
    void arithmetic();
private:
    EigenbroetlerWindow();
    EigenbroetlerWindow(EigenbroetlerWindow const&); // not implemented
    EigenbroetlerWindow& operator=(EigenbroetlerWindow const&); // not implemented
    friend class DataSelector;
    void newWindow(QList<ComplexArray *>& a, bool stack);
    void loadSettings();
    void constructActions();
    void constructMenu();
    void constructToolbars();
    void resetGUI();
    ArrayWindow *getArrayWindow(QMdiSubWindow *w);
    ArrayWindow const *getArrayWindow(QMdiSubWindow const *w) const;

    static EigenbroetlerWindow *eb;
    QMdiArea *mdiArea;
    // Actions for file menu
    QAction *openAction;
    QAction *saveAsAction;
    QAction *exportAction;
    QAction *exitAction;
    // Actions for basic operations
    QAction *newAction;
    QAction *padAction;
    QAction *arithAction;
    // Actions for advanced operations
    QAction *dislocationAction;
    QAction *constPhaseAction;
    QAction *linearPhaseAction;
    QAction *quadPhaseAction;
    // Actions for Fourier menu
    QAction *fftAction;
    QAction *fftxAction;
    QAction *fftyAction;
    // Actions for display menu
    QActionGroup *componentGroup;
    QAction *riAction;
    QAction *mpAction;
    QAction *toggleAction;
    QActionGroup *scaleGroup;
    QAction *linAction;
    QAction *logAction;
    QAction *powAction;
    // Actions for setting colour map
    QActionGroup *colourGroup;
    QList<QAction *> colourActions;
    QAction *colourmapAction;
    // Actions for window menu
    QAction *closeAction;
    QAction *closeAllAction;
    QAction *tileAction;
    QAction *cascadeAction;
    QAction *nextAction;
    QAction *previousAction;
    QAction *separatorAction;
    QSignalMapper *windowMapper;
    // Actions for help menu
    QAction *helpAction;
    QAction *aboutAction;

    QMenu *fileMenu;
    QMenu *basicOpsMenu;
    QMenu *advancedOpsMenu;
    QMenu *phaseOpsMenu;
    QMenu *fourierMenu;
    QMenu *displayMenu;
    QMenu *colourMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QToolBar *fileToolbar;
    QToolBar *opsToolbar;
    QToolBar *displayToolbar;
    QList<QAction *> disabledActions;
    QList<QWidget *> disabledWidgets;

    Q_OBJECT
};

inline EigenbroetlerWindow *EigenbroetlerWindow::instance()
{
    if (eb == NULL)
        eb = new EigenbroetlerWindow;
    return eb;
}

inline EigenbroetlerWindow::~EigenbroetlerWindow()
{
    std::cout << "Bye bye, I'm off" << std::endl;
}

#endif /* EIGENBROT_WINDOW_INCLUDE */
