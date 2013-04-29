
#include <array_window.h>
#include <QLineEdit>
#include <QMouseEvent>
#include <QScrollArea>
#include <QVBoxLayout>

#include <array_window_2d.h>
#include <complex_array.h>
#include <scaled_plotter.h>

uint ArrayWindow::anon_count = 0;

ArrayWindow *ArrayWindow::createWindow(ComplexArray *data)
{
    ArrayWindow *w = new ArrayWindow2D(data);
    if (w->getData()->isValid()) {
        return w;
    }
    else {
        delete w;
        return NULL;
    }
}

ArrayWindow::ArrayWindow(ComplexArray *cdata):
    QWidget(),
    d(cdata)
{
    setContentsMargins (0, 0, 0, 0);
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins (0, 0, 0, 0);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->setContentsMargins (0, 0, 0, 0);
    verticalLayout->addLayout(horizontalLayout);

    QVBoxLayout *pholder = new QVBoxLayout;
    pholder->setAlignment(Qt::AlignTop | Qt::AlignRight);
    horizontalLayout->addLayout(pholder);
    palette_plot = new Plotter(40, 256);
    pholder->addWidget(palette_plot);

    plotLayout = new QWidget;
    left_plot = new ScaledPlotter(cdata->width(), cdata->height(), this);
    left_plot->setParent(plotLayout);
    left_plot->move(0, 0);
    left_plot->setCursor(Qt::CrossCursor);
    right_plot = new ScaledPlotter(cdata->width(), cdata->height(), this);
    right_plot->setParent(plotLayout);
    right_plot->move(left_plot->width(), 0);
    right_plot->setCursor(Qt::CrossCursor);

    plotLayout->setFixedSize(left_plot->width() + right_plot->width(), left_plot->height());
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(plotLayout);
    horizontalLayout->addWidget(scrollArea);
    scrollArea->setFrameStyle(QFrame::NoFrame);

    QPalette status_palette;
    QBrush brush(QColor(192, 255, 192, 255));
    brush.setStyle(Qt::SolidPattern);
    status_palette.setBrush(QPalette::Active, QPalette::Base, brush);
    status = new QLineEdit;
    verticalLayout->addWidget(status);
    status->setText("");
    status->setReadOnly(true);
    status->setPalette(status_palette);
    //status->setFrame(false);

    QString title = cdata->source();
    if (!title.isEmpty()) {
        int slash = title.lastIndexOf('/');
        if (slash >= 0)
            title = title.mid(slash + 1);
    }
    else {
        setWindowTitle(title.sprintf("eigenbrot_%04u", ++anon_count));
    }
    setWindowTitle(title);
}

ArrayWindow::~ArrayWindow()
{
    delete d;
}
