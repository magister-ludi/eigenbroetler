#ifndef FORMULA_DIALOG_INCLUDE
#define FORMULA_DIALOG_INCLUDE

#include <global_defs.h>
#include <QDialog>
#include <ui_creation_dialog.h>

class ComplexArray;

class FormulaDialog: public QDialog {
public:
    static const QString sq_name;
    static QString const dim_name;
    static QString const width_name;
    static QString const height_name;
    static QString const form_name;
    static QString const multi_name;
    static QString const start_name;
    static QString const stop_name;
    static QString const incr_name;
    static ComplexArray *create_image(QWidget *p);
public slots:
    void accept();
protected slots:
    void updateControls();
    void formulaChanged();
private:
    FormulaDialog(QWidget *p);
    ComplexArray *construct();
    FormulaDialog(); // not implemented
    FormulaDialog(FormulaDialog const&); // not implemented
    FormulaDialog& operator=(FormulaDialog const&); // not implemented
    Ui_formulaDialog ui;
    bool stacked;
    Q_OBJECT
};

#endif /* FORMULA_DIALOG_INCLUDE */
