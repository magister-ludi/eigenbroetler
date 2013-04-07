
#include <array_window_2d.h>
#include <scaled_plotter.h>

#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>
#include <iostream>

ArrayWindow2D::ArrayWindow2D():
    ArrayWindow()
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

#define TESTSIZE 80

   QWidget *plotLayout = new QWidget;
   left_plot = new ScaledPlotter(TESTSIZE, TESTSIZE);
   left_plot->setParent(plotLayout);
   left_plot->move(0, 0);
   right_plot = new ScaledPlotter(TESTSIZE, TESTSIZE);
   right_plot->setParent(plotLayout);
   right_plot->move(left_plot->width(), 0);

   plotLayout->setFixedSize(left_plot->width() + right_plot->width(), left_plot->height());
   QScrollArea *scrollArea = new QScrollArea;
   scrollArea->setWidget(plotLayout);
   horizontalLayout->addWidget(scrollArea);
   scrollArea->setFrameStyle(QFrame::NoFrame);

   QPalette status_palette;
   QBrush brush(QColor(192, 255, 192, 255));
   brush.setStyle(Qt::SolidPattern);
   status_palette.setBrush(QPalette::Active, QPalette::Base, brush);
   QLineEdit *status = new QLineEdit;
   verticalLayout->addWidget(status);
   status->setText(QString::fromUtf8("eigenbrötler up and running"));
   status->setReadOnly(true);
   status->setPalette(status_palette);
   //status->setFrame(false);
}

ArrayWindow2D::~ArrayWindow2D()
{
}

