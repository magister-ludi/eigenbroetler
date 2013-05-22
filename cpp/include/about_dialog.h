// -*- c++ -*-
#ifndef ABOUT_DIALOG_INCLUDE
#define ABOUT_DIALOG_INCLUDE

#include <global_defs.h>
#include <ui_about_dialog.h>

class AboutDialog: public QDialog {
public:
    AboutDialog(QWidget *p);
private:
    AboutDialog(); // not implemented
    AboutDialog(AboutDialog const&); // not implemented
    AboutDialog& operator=(AboutDialog const&); // not implemented
    Ui_aboutDialog ui;

    Q_OBJECT
};

#endif /* ABOUT_DIALOG_INCLUDE */
