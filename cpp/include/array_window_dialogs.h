#ifndef ARRAY_WINDOW_DIALOGS_INCLUDE
#define ARRAY_WINDOW_DIALOGS_INCLUDE

#include <global_defs.h>
#include <display_info.h>
#include <ui_close_subwindow_dialog.h>
#include <ui_export_components_dialog.h>

class CloseSubwindowDialog: public QDialog {
public:
    static QString const ask_on_unsaved;
    CloseSubwindowDialog(QWidget *p, QString const& name);
    ~CloseSubwindowDialog();
    bool saveRequested() const;
 private slots:
    void saveData();
private:
    CloseSubwindowDialog(); // not implemented
    CloseSubwindowDialog(CloseSubwindowDialog const&); // not implemented
    CloseSubwindowDialog&operator=(CloseSubwindowDialog const&); // not implemented
    Ui_closeSubwindowDialog ui;
    bool save_requested;
    Q_OBJECT
};

class ExportDialog: public QDialog {
public:
    ExportDialog(QWidget *p, DisplayInfo::ComplexComponent cmp);
    int result() const;
protected slots:
    void accept();
private:
    ExportDialog(); // not implemented
    ExportDialog(ExportDialog const&); // not implemented
    ExportDialog&operator=(ExportDialog const&); // not implemented
    Ui_exportDialog ui;
    QString left_name;
    QString right_name;
    QString both_name;
    Q_OBJECT
};

inline void CloseSubwindowDialog::saveData()
{
    save_requested = true;
    accept();
}

inline bool CloseSubwindowDialog::saveRequested() const
{
    return save_requested;
}

inline int ExportDialog::result() const
{
    int rval = 0;
    if (ui.leftCheckBox->isChecked())
        rval += 1;
    if (ui.rightCheckBox->isChecked())
        rval += 2;
    if (ui.bothCheckBox->isChecked())
        rval += 4;
    return rval;
}

#endif /* ARRAY_WINDOW_DIALOGS_INCLUDE */
