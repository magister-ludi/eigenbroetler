#include <data_selector.h>
#include <array_window.h>
#include <eigenbroetler_window.h>
#include <QMdiArea>

QString const DataSelector::select_text(QObject::tr("Choose file..."));

DataSelector::DataSelector(QWidget *p):
    QComboBox(p)
{
    EigenbroetlerWindow const *win = EigenbroetlerWindow::instance();
    if (win) {
        QList<QMdiSubWindow *> window_list = win->mdiArea->subWindowList();
        QList<QMdiSubWindow *>::const_iterator w;
        for (w = window_list.begin(); w != window_list.end(); ++w) {
            ArrayWindow const *a = win->getArrayWindow(*w);
            if (a) {
                QString label = a->windowTitle();
                addItem(label, QVariant::fromValue((QWidget *) a));
            }
        }
    }
    addItem(select_text, 0);
    setCurrentIndex(0);
}
