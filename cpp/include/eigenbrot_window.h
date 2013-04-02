#ifndef EIGENBROT_WINDOW_INCLUDE
#define EIGENBROT_WINDOW_INCLUDE

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QCloseEvent;
QT_END_NAMESPACE

class EigenbrotWindow: public QMainWindow {
    Q_OBJECT;
 public:
    static EigenbrotWindow *instance();
    ~EigenbrotWindow();
protected:
    void closeEvent(QCloseEvent *event);
 private:
    EigenbrotWindow();
    EigenbrotWindow(EigenbrotWindow const&); // not implemented
    EigenbrotWindow& operator=(EigenbrotWindow const&); // not implemented
    static EigenbrotWindow *win;
};

inline EigenbrotWindow *EigenbrotWindow::instance()
{
    if (!win)
        win = new EigenbrotWindow();
    return win;
}

inline EigenbrotWindow::~EigenbrotWindow()
{
    win = NULL;
}

#endif /* EIGENBROT_WINDOW_INCLUDE */
